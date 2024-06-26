#include "pch.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#pragma comment(lib, "d3d11.lib")

#include "Midnight.hxx"

// hWnd

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

HWND GetGameWindow() {
	ENUMWINDOWSPARAM EnumWindowsParam;
	EnumWindowsParam.dwProcessId = GetCurrentProcessId();
	EnumWindowsParam.hWnd = nullptr;

	EnumWindows(EnumWindowsCallback, (LPARAM)&EnumWindowsParam);
	return EnumWindowsParam.hWnd;
}

// oPresent*

LPVOID GetPresentFunctionPtr(HWND hGameWnd) {
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

	const D3D_FEATURE_LEVEL szFeatureLevels[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
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

// oWndProc

WNDPROC oWndProc;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) return TRUE;
	return CallWindowProcW(oWndProc, hWnd, uMsg, wParam, lParam);
}

// hkPresent

ID3D11DeviceContext* pDeviceContext = nullptr;
ID3D11RenderTargetView* pRenderTargetView = nullptr;

Artemis::midnight* pThis;

bool bInitialized = false;
HRESULT APIENTRY hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (!bInitialized) {
		pThis = athis;

		ID3D11Device* pDevice = nullptr;

		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
			pDevice->GetImmediateContext(&pDeviceContext);

			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			pThis->_PresentHook.hWnd = sd.OutputWindow;

			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			if (!pBackBuffer)
				return pThis->_PresentHook.oPresent(pSwapChain, SyncInterval, Flags);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
			pBackBuffer->Release();

			oWndProc = (WNDPROC)SetWindowLongPtrW(athis->_PresentHook.hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			io.WantCaptureMouse = true;
			io.WantCaptureKeyboard = true;

			ImGui_ImplWin32_Init(pThis->_PresentHook.hWnd);
			ImGui_ImplDX11_Init(pDevice, pDeviceContext);

			bInitialized = true;
		}
		else return pThis->_PresentHook.oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// <--APPEND HERE-->

	ImGui::ShowStyleEditor();
	ImGui::ShowDemoWindow();

	//!<--APPEND HERE-->

	ImGui::EndFrame();
	ImGui::Render();
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return pThis->_PresentHook.oPresent(pSwapChain, SyncInterval, Flags);
}