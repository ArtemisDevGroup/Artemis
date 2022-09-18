#include <dxgi.h>
#include <d3d11.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include "FunctionDecl.h"
#include "WindowDecl.h"

// WndProc
WNDPROC oWndProc;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) return true;
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

// Present hook.
typedef HRESULT(APIENTRY* tPresent)(IDXGISwapChain*, UINT, UINT);
tPresent oPresent;
void SetOriginalPresentPtr(void* ptr) { oPresent = (tPresent)ptr; }

// Globals.
HWND hWnd;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* pMainRenderTargetView = nullptr;

const ImVec4 LightGray(0.16f, 0.16f, 0.16f, 1.0f); // #2A2A2A
const ImVec4 DarkGray(0.1f, 0.1f, 0.1f, 1.0f); // #1A1A1A
const ImVec4 DarkGreen(0.00f, 0.25f, 0.05f, 0.40f);
const ImVec4 Green(0.00f, 0.48f, 0.17f, 0.67f);

bool bInitialized = false;
HRESULT APIENTRY hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (!bInitialized) {
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			hWnd = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pMainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

			ImGuiStyle& style = ImGui::GetStyle();

			style.WindowPadding								= ImVec2(6, 4);
			style.FramePadding								= ImVec2(4, 2);
			style.ItemSpacing								= ImVec2(4, 3);

			style.WindowBorderSize							= 0.0f;
			style.ChildBorderSize							= 0.0f;
			style.PopupBorderSize							= 0.0f;

			style.Colors[ImGuiCol_WindowBg]					= LightGray;
			style.Colors[ImGuiCol_FrameBg]					= DarkGray;
			style.Colors[ImGuiCol_FrameBgHovered]			= DarkGreen;
			style.Colors[ImGuiCol_FrameBgActive]			= Green;
			style.Colors[ImGuiCol_TitleBg]					= DarkGray;
			style.Colors[ImGuiCol_TitleBgActive]			= DarkGray;
			style.Colors[ImGuiCol_ScrollbarBg]				= DarkGray;
			style.Colors[ImGuiCol_ScrollbarGrab]			= LightGray;
			style.Colors[ImGuiCol_ScrollbarGrabHovered]		= DarkGreen;
			style.Colors[ImGuiCol_ScrollbarGrabActive]		= Green;
			style.Colors[ImGuiCol_CheckMark]				= Green;
			style.Colors[ImGuiCol_SliderGrab]				= LightGray;
			style.Colors[ImGuiCol_SliderGrabActive]			= Green;
			style.Colors[ImGuiCol_Button]					= DarkGray;
			style.Colors[ImGuiCol_ButtonHovered]			= DarkGreen;
			style.Colors[ImGuiCol_ButtonActive]				= Green;
			style.Colors[ImGuiCol_Header]					= DarkGray;
			style.Colors[ImGuiCol_HeaderHovered]			= DarkGreen;
			style.Colors[ImGuiCol_HeaderActive]				= Green;
			style.Colors[ImGuiCol_Separator]				= DarkGray;
			style.Colors[ImGuiCol_SeparatorHovered]			= DarkGreen;
			style.Colors[ImGuiCol_SeparatorActive]			= Green;
			style.Colors[ImGuiCol_ResizeGrip]				= DarkGray;
			style.Colors[ImGuiCol_ResizeGripHovered]		= DarkGreen;
			style.Colors[ImGuiCol_ResizeGripActive]			= Green;
			style.Colors[ImGuiCol_Tab]						= DarkGray;
			style.Colors[ImGuiCol_TabHovered]				= DarkGreen;
			style.Colors[ImGuiCol_TabActive]				= Green;
			style.Colors[ImGuiCol_TabUnfocused]				= DarkGreen;
			style.Colors[ImGuiCol_TabUnfocusedActive]		= Green;
			style.Colors[ImGuiCol_TextSelectedBg]			= Green;
			style.Colors[ImGuiCol_NavHighlight]				= Green;

			ImGui_ImplWin32_Init(hWnd);
			ImGui_ImplDX11_Init(pDevice, pContext);
			bInitialized = true;
		}
		else return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	MainWindow();

	ImGui::EndFrame();
	ImGui::Render();
	pContext->OMSetRenderTargets(1, &pMainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (!env::IsRunning()) {
		oWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)(oWndProc));
		return 0;
	}

	return oPresent(pSwapChain, SyncInterval, Flags);
}