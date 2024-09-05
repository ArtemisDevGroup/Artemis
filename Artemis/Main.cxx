#include "pch.h"

#include "Midnight.hxx"

#include "API/Console.hxx"

// Namespaces:
//	Artemis::		is the namespace for basic interactions with the client. It contains things like for example IWindow which is used for creating an ImGui window.
//	Artemis::Game:: is the namespace for interactable implementations of different mods.
//	Artemis::API::	is the namespace for API features like Read, Hook and so on.
//	scimitar::		contains everything we have reversed written as close to the original source code as possible.

// To-Do:
// >> Drawing.cxx: ...
// 
// - Main.cxx:
//  * Create Extension load and unload logic.
//		¤ Will require heavy lifting in Communications.cxx && Extensions.cxx && Artemis-Loader.
// 
// - Whole Project:
//  * Inline document.
//  * Finish some API modules.
//  * Add color structs.
//  * Tidy up files by making sure regions and other structure is clear.
//  * Double-check move semantics.
//  * Double-check move constructors and assignment operators.

namespace Artemis {
	void alloc_midnight();
	void release_midnight();
}

#pragma region Present hook functionality.

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#pragma comment(lib, "d3d11.lib")

#pragma region Functions for retrieving the game window handle.

typedef struct _ENUMWINDOWSPARAM {
	DWORD dwProcessId;
	HWND hWnd;
} ENUMWINDOWSPARAM, * PENUMWINDOWSPARAM;

static BOOL WINAPI EnumWindowsCallback(HWND hWnd, LPARAM lParam) {
	PENUMWINDOWSPARAM pEnumWindowsParam = (PENUMWINDOWSPARAM)lParam;

	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);

	if (pEnumWindowsParam->dwProcessId != dwProcessId || !IsWindowVisible(hWnd))
		return TRUE;
	pEnumWindowsParam->hWnd = hWnd;
	return FALSE;
}

static HWND GetGameWindow() {
	ENUMWINDOWSPARAM EnumWindowsParam;
	EnumWindowsParam.dwProcessId = GetCurrentProcessId();
	EnumWindowsParam.hWnd = nullptr;

	EnumWindows(EnumWindowsCallback, (LPARAM)&EnumWindowsParam);
	return EnumWindowsParam.hWnd;
}

#pragma endregion

#pragma region Function for retrieving the Present function pointer.

static LPVOID GetPresentFunctionPtr(HWND hGameWnd) {
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hGameWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGISwapChain* pSwapChain;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

	const D3D_FEATURE_LEVEL szFeatureLevels[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
	if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		szFeatureLevels,
		2,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pDeviceContext
	))) {
		void** ppVTable = *(void***)pSwapChain;
		pSwapChain->Release();
		pDevice->Release();
		pDeviceContext->Release();

		return ppVTable[8];
	}
	else return nullptr;
}

#pragma endregion

#pragma region Window Procedure.

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) return TRUE;
	
	athis->KeyActions->invoke(uMsg, (Artemis::key)wParam);

	return CallWindowProcW(athis->_PresentHook.oWndProc, hWnd, uMsg, wParam, lParam);
}

#pragma endregion

#pragma region Present detour.

ID3D11DeviceContext* pDeviceContext = nullptr;
ID3D11RenderTargetView* pRenderTargetView = nullptr;

bool bInitialized = false;
static HRESULT APIENTRY hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (!bInitialized) {
		ID3D11Device* pDevice = nullptr;

		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
			pDevice->GetImmediateContext(&pDeviceContext);

			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			athis->_PresentHook.hWnd = sd.OutputWindow;

			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			if (!pBackBuffer)
				return athis->_PresentHook.oPresent(pSwapChain, SyncInterval, Flags);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
			pBackBuffer->Release();

			athis->_PresentHook.oWndProc = (WNDPROC)SetWindowLongPtrW(athis->_PresentHook.hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			io.WantCaptureMouse = true;
			io.WantCaptureKeyboard = true;

			ImGui_ImplWin32_Init(athis->_PresentHook.hWnd);
			ImGui_ImplDX11_Init(pDevice, pDeviceContext);

			bInitialized = true;
		}
		else return athis->_PresentHook.oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// <--APPEND HERE-->

	ImGui::ShowStyleEditor();
	ImGui::ShowDemoWindow();

	athis->Windows->present_all();

	//!<--APPEND HERE-->

	ImGui::EndFrame();
	ImGui::Render();
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return athis->_PresentHook.oPresent(pSwapChain, SyncInterval, Flags);
}

#pragma endregion

#pragma endregion

DWORD APIENTRY ArtemisMain(_In_ HMODULE hModule) {
#pragma region ----- Initialization -----

	// Initialization order:
	// 1. Allocate midnight.
	// 2. Allocate and initalize console.
	// 3. Initialize communication pipelines.
	// 4. Initialize keybind manager and keybinds.
	// 5. Hook present.
	// 6. Initialize window manager and windows.
	// 7. Load extensions.

	Artemis::alloc_midnight();
	athis->Logger = new Artemis::API::logger();

	Artemis::API::open_console();
	Artemis::API::open_console_io();

	// Creates the necessary server side of the message pipelines.
	// In the end, this is a task the Loader will have the responsibility of doing.
	// FOR DEBUG PURPOSES ONLY
	Artemis::_::__debug_message_host* dmh = new Artemis::_::__debug_message_host();
	athis->ClientMessageRecipent = new Artemis::message_recipent(Artemis::ClientInboundPipeName);
	athis->MainRemoteDispatcher = new Artemis::message_dispatcher("Artemis Client Primary Remote Dispatcher", Artemis::ClientOutboundPipeName);
	athis->MainInternalDispatcher = new Artemis::message_dispatcher("Artemis Client Primary Internal Dispatcher", Artemis::ClientInboundPipeName);

	athis->KeyActions = new Artemis::key_action_manager();
	athis->KeyActions->set_instance_logger(athis->Logger);

	athis->KeyActions->register_action(Artemis::key::f1, []() {
		Artemis::message exitMessage(Artemis::message_type::exit);
		athis->MainInternalDispatcher->dispatch_message(&exitMessage);
		});

	Artemis::API::global_hook_alloc();

	auto presentHook = &athis->_PresentHook;

	HWND hWnd = GetGameWindow();
	LPVOID lpPresentFunction = GetPresentFunctionPtr(hWnd);

	presentHook->hkInstance = new Artemis::API::hook<Artemis::TPRESENT>(lpPresentFunction, hkPresent);
	presentHook->oPresent = presentHook->hkInstance->original();
	presentHook->hkInstance->enable();

	athis->Windows = new Artemis::window_manager();
	athis->Windows->set_instance_logger(athis->Logger);

	// Append windows here.

	athis->Extensions = new Artemis::extension_manager();
	athis->Extensions->set_instance_logger(athis->Logger);

#pragma endregion

#pragma region ----- Main loop -----

	while (athis->ClientMessageRecipent->await_message()) {
		Artemis::message* msg = athis->ClientMessageRecipent->get_message_body();

		switch (msg->type()) {
			// Set up message handler for extension messages.
		}
	}

	athis->Logger->info(std::format("Communications pipeline received 'exit' message from dispatcher '{}'.", athis->ClientMessageRecipent->get_message_body()->dispatcher_name()));
	athis->Logger->info("Starting uninitialization procedure...");

#pragma endregion

#pragma region ----- Uninitialization -----

	// Uninitialization order:
	// 1. Release extensions.
	// 2. Release windows and window manager.
	// 3. Restore present to its original state.
	// 4. Release keybinds and keybind manager.
	// 5. Release communication pipelines.
	// 6. Release console.
	// 7. Release midnight and exit.

	delete athis->Windows;

	SetWindowLongPtrW(presentHook->hWnd, GWLP_WNDPROC, (LONG_PTR)presentHook->oWndProc); // Swap WndProc for original.

	delete presentHook->hkInstance;

	Artemis::API::global_hook_release();

	delete athis->KeyActions;

	delete athis->ClientMessageRecipent;
	delete athis->MainRemoteDispatcher;
	delete athis->MainInternalDispatcher;
	delete dmh; // SEE ABOVE REMARKS

	Artemis::API::close_console_io();
	Artemis::API::close_console();

	delete athis->Logger;
	Artemis::release_midnight();

	FreeLibraryAndExitThread(hModule, 0);
	return 0;

#pragma endregion
}