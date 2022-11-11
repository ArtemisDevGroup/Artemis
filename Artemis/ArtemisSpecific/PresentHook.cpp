#include "PresentHook.h"
#include "Midnight.h"
#include "DrawManager.h"

#include "..\ImGui\imgui.h"
#include "..\ImGui\imgui_impl_dx11.h"
#include "..\ImGui\imgui_impl_win32.h"

namespace Artemis {
	typedef struct _HANDLEDATA {
		DWORD dwProcessId;
		HWND hWnd;
	} HANDLEDATA, * LPHANDLEDATA;

	BOOL CALLBACK EnumWindowsCallback(HWND hWnd, LPARAM lParam) {
		HANDLEDATA& data = *(LPHANDLEDATA)lParam;
		DWORD dwProcessId = 0;
		GetWindowThreadProcessId(hWnd, &dwProcessId);
		if (data.dwProcessId != dwProcessId || !IsWindowVisible(hWnd))
			return TRUE;
		data.hWnd = hWnd;
		return FALSE;
	}

	_Check_return_ _Ret_maybenull_ HWND GetTopLevelWnd() {
		HANDLEDATA data;
		data.dwProcessId = GetCurrentProcessId();
		data.hWnd = nullptr;
		EnumWindows(EnumWindowsCallback, (LPARAM)&data);
		return data.hWnd;
	}

	_Check_return_ _Ret_maybenull_ LPVOID GetPresentFnPtr(_In_ HWND hGameWnd) {
		DXGI_SWAP_CHAIN_DESC sd;
		memset(&sd, 0, sizeof(sd));
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

	const ImVec4 LightGray(0.16f, 0.16f, 0.16f, 1.0f); // #2A2A2A
	const ImVec4 DarkGray(0.1f, 0.1f, 0.1f, 1.0f); // #1A1A1A
	const ImVec4 DarkGreen(0.00f, 0.25f, 0.05f, 0.40f);
	const ImVec4 Green(0.00f, 0.48f, 0.17f, 0.67f);

	void ImGui_SetStyle() {
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding = ImVec2(6, 4);
		style.FramePadding = ImVec2(4, 2);
		style.ItemSpacing = ImVec2(4, 3);

		style.WindowBorderSize = 0.0f;
		style.ChildBorderSize = 0.0f;
		style.PopupBorderSize = 0.0f;

		style.Colors[ImGuiCol_WindowBg] = LightGray;
		style.Colors[ImGuiCol_FrameBg] = DarkGray;
		style.Colors[ImGuiCol_FrameBgHovered] = DarkGreen;
		style.Colors[ImGuiCol_FrameBgActive] = Green;
		style.Colors[ImGuiCol_TitleBg] = DarkGray;
		style.Colors[ImGuiCol_TitleBgActive] = DarkGray;
		style.Colors[ImGuiCol_ScrollbarBg] = DarkGray;
		style.Colors[ImGuiCol_ScrollbarGrab] = LightGray;
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = DarkGreen;
		style.Colors[ImGuiCol_ScrollbarGrabActive] = Green;
		style.Colors[ImGuiCol_CheckMark] = Green;
		style.Colors[ImGuiCol_SliderGrab] = LightGray;
		style.Colors[ImGuiCol_SliderGrabActive] = Green;
		style.Colors[ImGuiCol_Button] = DarkGray;
		style.Colors[ImGuiCol_ButtonHovered] = DarkGreen;
		style.Colors[ImGuiCol_ButtonActive] = Green;
		style.Colors[ImGuiCol_Header] = DarkGray;
		style.Colors[ImGuiCol_HeaderHovered] = DarkGreen;
		style.Colors[ImGuiCol_HeaderActive] = Green;
		style.Colors[ImGuiCol_Separator] = DarkGray;
		style.Colors[ImGuiCol_SeparatorHovered] = DarkGreen;
		style.Colors[ImGuiCol_SeparatorActive] = Green;
		style.Colors[ImGuiCol_ResizeGrip] = DarkGray;
		style.Colors[ImGuiCol_ResizeGripHovered] = DarkGreen;
		style.Colors[ImGuiCol_ResizeGripActive] = Green;
		style.Colors[ImGuiCol_Tab] = DarkGray;
		style.Colors[ImGuiCol_TabHovered] = DarkGreen;
		style.Colors[ImGuiCol_TabActive] = Green;
		style.Colors[ImGuiCol_TabUnfocused] = DarkGreen;
		style.Colors[ImGuiCol_TabUnfocusedActive] = Green;
		style.Colors[ImGuiCol_TextSelectedBg] = Green;
		style.Colors[ImGuiCol_NavHighlight] = Green;
	}
	
	typedef HRESULT(APIENTRY* tPresent)(IDXGISwapChain*, UINT, UINT);
	tPresent oPresent;
	BOOL bInit = FALSE;

	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pMainRenderTargetView = nullptr;

	Midnight* pInst;

	LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) return true;
		return CallWindowProc(Midnight::GetInst()->oWndProc, hWnd, uMsg, wParam, lParam);
	}

	HRESULT APIENTRY hkPresent(
		_In_ IDXGISwapChain* pSwapChain,
		_In_ UINT SyncInterval,
		_In_ UINT Flags
	) {
		if (!bInit) {
			pInst = Midnight::GetInst();
			oPresent = (tPresent)pInst->lpPresent;

			ID3D11Device* pDevice = nullptr;

			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
				pDevice->GetImmediateContext(&pContext);

				DXGI_SWAP_CHAIN_DESC sd;
				pSwapChain->GetDesc(&sd);
				pInst->hWnd = sd.OutputWindow;

				ID3D11Texture2D* pBackBuffer;
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
				pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pMainRenderTargetView);
				pBackBuffer->Release();

				pInst->oWndProc = (WNDPROC)SetWindowLongPtr(pInst->hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO();
				io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

				ImGui_SetStyle();

				ImGui_ImplWin32_Init(pInst->hWnd);
				ImGui_ImplDX11_Init(pDevice, pContext);

				bInit = TRUE;
			}
			else return oPresent(pSwapChain, SyncInterval, Flags);
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		pInst->ImGuiDrawManager.InvokeDraw(ImGui::GetForegroundDrawList());
		pInst->ImGuiWndManager.InvokeWnd();

		ImGui::EndFrame();
		ImGui::Render();
		pContext->OMSetRenderTargets(1, &pMainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (!pInst->bRun) return 0;
		return oPresent(pSwapChain, SyncInterval, Flags);
	}
}