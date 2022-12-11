//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#include "WindowManager.h"
#include "..\SafeMemory.h"

#include <stdio.h>

namespace Artemis {
	//---------------------------------------------------------------------->
	// Globals
	//---------------------------------------------------------------------->
	BOOL bShowAll = TRUE;
	//---------------------------------------------------------------------->
	// IWindow
	//---------------------------------------------------------------------->
	void IWindow::Present() {
		if (bShowAll && bShow) {
			if (ImGui::Begin(lpWndName)) Window();
			ImGui::End();
		}
	}
	//---------------------------------------------------------------------->
	// WindowManager
	//---------------------------------------------------------------------->
	WindowManager::WindowManager() { memset(lpszWndArray, 0x00, sizeof(lpszWndArray)); }
	//-------------------------------------//
	void WindowManager::RegisterWnd(_In_ IWindow* lpWndInst) {
		CONTEXT_BEGIN;

		if (!lpWndInst) throw ParameterException("lpWndInst");

		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (lpszWndArray[i])
				if (lpszWndArray[i]->dwWndId == lpWndInst->dwWndId)
					throw AttributeException("dwWndId");
		}

		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (!lpszWndArray[i]) {
				lpszWndArray[i] = lpWndInst;
				lpWndInst->dwRegisteredCount++;
				break;
			}
		}

		CONTEXT_END;
	}
	//-------------------------------------//
	void WindowManager::UnregisterWnd(_In_ IWindow* lpWndInst) {
		CONTEXT_BEGIN;

		BOOL bUnregistered = FALSE;

		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (lpszWndArray[i] == lpWndInst) {
				lpszWndArray[i]->dwRegisteredCount--;
				lpszWndArray[i] = nullptr;
				bUnregistered = TRUE;
				break;
			}
		}

		if (!bUnregistered) throw ObjectNotFoundException("lpWndInst", "IWindow*");

		CONTEXT_END;
	}
	//-------------------------------------//
	void WindowManager::Release() {
		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (lpszWndArray[i]) {
				if (lpszWndArray[i]->dwRegisteredCount <= 1) SafeDelete(lpszWndArray[i]);
				else lpszWndArray[i]->dwRegisteredCount--;
				lpszWndArray[i] = nullptr;
			}
		}
	}
	//-------------------------------------//
	void WindowManager::InvokeWnd() {
		for (INT i = 0; i < MAX_INVOKE; i++)
			if (lpszWndArray[i])
				lpszWndArray[i]->Present();
	}
	//-------------------------------------//
	_Check_return_ _When_(IN_RANGE(dwWndId, 0, 19), _Ret_notnull_) _When_(!IN_RANGE(dwWndId, 0, 19), _Ret_maybenull_) IWindow* WindowManager::GetWndById(_In_ DWORD dwWndId) {
		CONTEXT_BEGIN;

		IWindow* lpReturn = nullptr;

		for (INT i = 0; i < MAX_INVOKE; i++)
			if (lpszWndArray[i])
				if (lpszWndArray[i]->dwWndId == dwWndId)
					lpReturn =  lpszWndArray[i];

		if (!lpReturn) {
			CHAR szBuffer[MAX_NAME];
			sprintf_s(szBuffer, "Window instance belonging to id %lu", dwWndId);
			throw ObjectNotFoundException(szBuffer, "IWindow*");
		}

		CONTEXT_END;

		return lpReturn;
	}
	//-------------------------------------//
	void WindowManager::ShowWnd(_In_ BOOL bShow) { bShowAll = bShow; }
	//-------------------------------------//
	_Check_return_ LPBOOL WindowManager::GetWndVisibilityPtr() { return &bShowAll; }
	//---------------------------------------------------------------------->
}