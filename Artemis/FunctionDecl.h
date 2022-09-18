#ifndef __FUNCTIONDECL_H__
#define __FUNCTIONDECL_H__

#include <Windows.h>

#include <dxgi.h>

#include "SDK/Logger.h"
#include "SDK/MemApi_Info.h"

namespace env {
	Logger& LogClass();
	bool IsRunning();
	void Stop();

	void InitMainModule();
	const LPMODULE GetMainModule();
}

void SetOriginalPresentPtr(void* ptr);
HRESULT APIENTRY hkPresent(IDXGISwapChain* pSwapChain, UINT SyncIntervals, UINT Flags);

DWORD APIENTRY Main(_In_ HMODULE hModule);
void Initialize();
void Uninitialize();

#endif // !__FUNCTIONDECL_H__