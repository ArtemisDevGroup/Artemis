#include "pch.h"
#include "PresentHook.h"
#include "External.h"

#include "Events.h"

#pragma comment(lib, "d3d11.lib")

typedef struct _ENUMWINDOWSPARAM {
	DWORD dwProcessId;
	HWND hWnd;
} ENUMWINDOWSPARAM, * PENUMWINDOWSPARAM;

BOOL WINAPI EnumWindowsCallback(HWND hWnd, LPARAM lParam) {
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

LPVOID GetPresentFnPtr(_In_ HWND hGameWnd) {
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

void ImGui_SetStyle() {
	ImGuiStyle& Style = ImGui::GetStyle();

	Style.WindowPadding = ImVec2(8.00f, 8.00f);
	Style.FramePadding = ImVec2(5.00f, 2.00f);
	Style.ItemSpacing = ImVec2(6.00f, 6.00f);
	Style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
	Style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	Style.IndentSpacing = 25;
	Style.ScrollbarSize = 15;
	Style.GrabMinSize = 10;
	Style.WindowBorderSize = 1;
	Style.ChildBorderSize = 1;
	Style.PopupBorderSize = 1;
	Style.FrameBorderSize = 1;
	Style.TabBorderSize = 1;
	Style.WindowRounding = 7;
	Style.ChildRounding = 4;
	Style.FrameRounding = 3;
	Style.PopupRounding = 4;
	Style.ScrollbarRounding = 9;
	Style.GrabRounding = 3;
	Style.TabRounding = 4;
	
	ImVec4(&Colors)[48] = Style.Colors;

	Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	Colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
	Colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
	Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	Colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	Colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	Colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	Colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	Colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	Colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	Colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
	Colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
	Colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	Colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	Colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	Colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	Colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	Colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
	Colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	Colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	Colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
	Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
	Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);
}

typedef HRESULT(APIENTRY* tPresent)(IDXGISwapChain*, UINT, UINT);
tPresent oPresent;
WNDPROC oWndProc;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) return TRUE;
	return CallWindowProcW(oWndProc, hWnd, uMsg, wParam, lParam);
}

ID3D11DeviceContext* pDeviceContext = nullptr;
ID3D11RenderTargetView* pRenderTargetView = nullptr;

HWND hWnd;

bool bInitialized = false;

HRESULT APIENTRY hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (!bInitialized) {
		ID3D11Device* pDevice = nullptr;

		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
			pDevice->GetImmediateContext(&pDeviceContext);

			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			hWnd = sd.OutputWindow;

			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			if (!pBackBuffer) return oPresent(pSwapChain, SyncInterval, Flags);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
			pBackBuffer->Release();

			oWndProc = (WNDPROC)SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			io.WantCaptureMouse = true;
			io.WantCaptureKeyboard = true;

			ImGui_SetStyle();

			ImGui_ImplWin32_Init(hWnd);
			ImGui_ImplDX11_Init(pDevice, pDeviceContext);

			bInitialized = true;
		}
		else return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Artemis::Engine::Events::OnNewFrameEventArgs e;
	Artemis::Engine::Events::OnNewFrameEvent.Invoke(nullptr, &e);

	Artemis::DrawManagers.PresentAll(ImGui::GetForegroundDrawList(), ImGui::GetBackgroundDrawList());
	Artemis::EventEntries.Invoke();
	Artemis::Windows.PresentAll();

	ImGui::ShowStyleEditor();
	ImGui::ShowDemoWindow();

	ImGui::EndFrame();
	ImGui::Render();
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return oPresent(pSwapChain, SyncInterval, Flags);
}

PresentHook::PresentHook() {
	HWND hWnd = GetGameWindow();
	pTarget = GetPresentFnPtr(hWnd);

	MH_STATUS status = MH_CreateHook(pTarget, hkPresent, (void**)&oPresent);
	if (status != MH_OK) Artemis::Log.LogError(__FUNCTION__, "Failed to create present hook: %s", MH_StatusToString(status));
	else Artemis::Log.LogSuccess(__FUNCTION__, "Successfully created present hook.");
}

PresentHook::~PresentHook() {
	SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);

	MH_STATUS status = MH_RemoveHook(pTarget);
	if (status != MH_OK) Artemis::Log.LogError(__FUNCTION__, "Failed to remove present hook: %s", MH_StatusToString(status));
	else Artemis::Log.LogSuccess(__FUNCTION__, "Successfully removed present hook.");
}

PresentHook* PresentHook::Create() { return new PresentHook(); }

void PresentHook::Enable() {
	MH_STATUS status = MH_EnableHook(pTarget);
	if (status != MH_OK) Artemis::Log.LogError(__FUNCTION__, "Failed to enable present hook: %s", MH_StatusToString(status));
	else Artemis::Log.LogSuccess(__FUNCTION__, "Successfully enabled present hook.");
}

void PresentHook::Disable() {
	MH_STATUS status = MH_DisableHook(pTarget);
	if (status != MH_OK) Artemis::Log.LogError(__FUNCTION__, "Failed to disable present hook: %s", MH_StatusToString(status));
	else Artemis::Log.LogSuccess(__FUNCTION__, "Successfully disabled present hook.");
}

void PresentHook::Release() { delete this; }