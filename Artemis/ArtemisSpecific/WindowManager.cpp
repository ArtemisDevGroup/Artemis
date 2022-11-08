#include "WindowManager.h"

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
				lpszWndArray[i] = nullptr;
				bUnregistered = TRUE;
				break;
			}
		}

		if (!bUnregistered) throw ObjectNotFoundException("lpWndInst", "IWindow*");

		CONTEXT_END;
	}
	//-------------------------------------//
	void WindowManager::FreeWndBuffer() {
		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (lpszWndArray[i]) {
				delete lpszWndArray[i];
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
	_Check_return_ _Ret_maybenull_ IWindow* WindowManager::GetWndById(_In_ DWORD dwWndId) {
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