//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#include "Midnight.h"

#include <Windows.h>

namespace Artemis {
	Midnight g_CoreInst;
	Midnight* Midnight::GetInst() { return &g_CoreInst; }

	Midnight::Midnight() : Mem(Memory(nullptr)), ImGuiWndManager(), Log(Logger(TRUE, TRUE)), ConInst(), bRun(FALSE), hModule(nullptr), hWnd(nullptr), lpPresent(nullptr), oWndProc(nullptr) {}

	void Midnight::Initialize(_In_ HMODULE hModule) {
		this->hModule = hModule;

		Log.LogInfo(__FUNCTION__, "Attempting to hook Present...");

		HWND hWnd = GetTopLevelWnd();
		if (hWnd) Log.LogSuccess(__FUNCTION__, "Obtained hWnd: %p", hWnd);
		else {
			Log.LogError(__FUNCTION__, "Failed to obtain hWnd!");
			return;
		}

		LPVOID lpfnPresent = GetPresentFnPtr(hWnd);
		if (lpfnPresent) Log.LogSuccess(__FUNCTION__, "Obtained Present ptr: %p", lpfnPresent);
		else {
			Log.LogError(__FUNCTION__, "Failed to obtain Present ptr!");
			return;
		}

		Log.LogInfo(__FUNCTION__, "Initializing hooking library...");
		try {
			Hook::InitializeLibrary();
			Log.LogSuccess(__FUNCTION__, "Successfully initialized hooking library.");
		}
		catch (HookException& e) {
			Log.LogError(__FUNCTION__, "Failed to initialize hooking library. Message: %s", e.GetExceptionMessage());
			return;
		}

		LPVOID lpOriginal = nullptr;
		try {
			PresentHook = Hook(lpfnPresent, hkPresent, &lpOriginal);
			Log.LogSuccess(__FUNCTION__, "Successfully created hook on present.");
			lpPresent = lpOriginal;
		}
		catch (HookException& e) {
			Log.LogError(__FUNCTION__, "Constructing Hook object failed. Message: %s\n", e.GetExceptionMessage());
			return;
		}

		Log.LogInfo(__FUNCTION__, "Enabling hook...");

		try {
			PresentHook.Enable();
			Log.LogSuccess(__FUNCTION__, "Successfully enabled present hook.");
		}
		catch (HookException& e) {
			Log.LogError(__FUNCTION__, "Failed to enable present hook. Message: %s", e.GetExceptionMessage());
			return;
		}

		Log.LogSuccess(__FUNCTION__, "Initialization successful!");
		bRun = TRUE;
	}

	void Midnight::Release() {
		Log.LogInfo(__FUNCTION__, "Attempting to release hook...");

		try {
			PresentHook.Disable();
			Log.LogSuccess(__FUNCTION__, "Successfully disabled hook.");
		}
		catch (HookException& e) {
			Log.LogError(__FUNCTION__, "Failed to disable hook. Message: %s", e.GetExceptionMessage());
			return;
		}

		try {
			PresentHook.Release();
			Log.LogSuccess(__FUNCTION__, "Successfully released hook.");
		}
		catch (HookException& e) {
			Log.LogError(__FUNCTION__, "Failed to release hook. Message: %s", e.GetExceptionMessage());
			return;
		}

		Log.LogInfo(__FUNCTION__, "Attempting to restore the old window procedure.");

		try {
			if (!SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc)) throw WindowsApiException("SetWindowLongPtrW");
			Log.LogSuccess(__FUNCTION__, "Successfully restored the window procedure.");
		}
		catch (WindowsApiException& e) {
			Log.LogError("Failed to restore the old window procedure. Message: %s", e.GetWindowsMessage());
			return;
		}

		Log.LogInfo(__FUNCTION__, "Releasing hooking library...");
		try {
			Hook::UninitializeLibrary();
			Log.LogSuccess(__FUNCTION__, "Successfully released hooking library.");
		}
		catch (HookException& e) {
			Log.LogError(__FUNCTION__, "Failed to release hooking library. Message: %s", e.GetExceptionMessage());
			return;
		}
	}
}