//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __PRESENT_HOOK_H__
#define __PRESENT_HOOK_H__

#include <dxgi.h>
#include <d3d11.h>

#pragma comment(lib,"d3d11.lib")

#include "..\Definitions.h"
#include "..\MemoryHook.h"
#include "..\Exceptions.h"

namespace Artemis {
	_Check_return_ _Ret_maybenull_ ARTEMIS_API HWND GetTopLevelWnd();
	_Check_return_ _Ret_maybenull_ ARTEMIS_API LPVOID GetPresentFnPtr(_In_ HWND hGameWnd);

	ARTEMIS_API HRESULT APIENTRY hkPresent(
		_In_ IDXGISwapChain* pSwapChain,
		_In_ UINT SyncInterval,
		_In_ UINT Flags
	);
}

#endif // !__PRESENT_HOOK_H__