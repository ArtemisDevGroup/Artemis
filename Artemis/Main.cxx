#include "pch.h"

#include "Midnight.hxx"

namespace Artemis { midnight* g_DataInstance = nullptr; }

using Artemis::midnight;
using Artemis::present_function;
using Artemis::API::hook;

// Forwarded for initialization of present hook.
HWND GetGameWindow();
LPVOID GetPresentFunctionPtr(HWND hGameWnd);
HRESULT APIENTRY hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

DWORD APIENTRY ArtemisMain(_In_ HMODULE hModule) {
	// ----- Initialization -----

	// Hook Present.

	Artemis::API::global_hook_alloc();

	HWND hWnd = GetGameWindow();
	LPVOID lpPresentFunction = GetPresentFunctionPtr(hWnd);

	Artemis::g_DataInstance = new midnight{ { hook<present_function>(lpPresentFunction, hkPresent) } };

	auto& presentHook = Artemis::g_DataInstance->_PresentHook;

	presentHook.oPresent = presentHook.hkInstance.original();
	presentHook.hkInstance.enable();

	// ----- Main loop -----

	bool run = true;
	while (run) {
		if (GetAsyncKeyState(VK_DELETE) & 1)
			break;
	}

	// ----- Exit -----

	// Swap WndProc for original.

	SetWindowLongPtrW(presentHook.hWnd, GWLP_WNDPROC, (LONG_PTR)presentHook.oWndProc);

	Artemis::API::global_hook_release();

	delete Artemis::g_DataInstance;
	Artemis::g_DataInstance = nullptr;

	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}