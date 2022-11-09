#include "KeybindManager.h"

#include <stdio.h>
#include <Windows.h>

namespace Artemis {
	IKeybind::IKeybind(_In_ WORD wKey) : wKey(wKey) {}

	void IKeybind::Invoke() { if (GetAsyncKeyState(wKey) & 1) WhenPressed(); }

	KeybindManager::KeybindManager() { memset(lpszKeybindArray, 0x00, sizeof(lpszKeybindArray)); }

	void KeybindManager::RegisterKeybind(_In_ IKeybind* lpKeybindInst) {
		CONTEXT_BEGIN;

		if (!lpKeybindInst) throw ParameterException("lpKeybindInst");

		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (lpszKeybindArray[i])
				if (lpszKeybindArray[i]->wKey == lpKeybindInst->wKey)
					throw AttributeException("dwDrawId");
		}

		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (!lpszKeybindArray[i]) {
				lpszKeybindArray[i] = lpKeybindInst;
				break;
			}
		}

		CONTEXT_END;
	}

	void KeybindManager::UnregisterKeybind(_In_ IKeybind* lpKeybindInst) {
		CONTEXT_BEGIN;

		BOOL bUnregistered = FALSE;

		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (lpszKeybindArray[i] == lpKeybindInst) {
				lpszKeybindArray[i] = nullptr;
				bUnregistered = TRUE;
				break;
			}
		}

		if (!bUnregistered) throw ObjectNotFoundException("lpKeybindInst", "IKeybind*");

		CONTEXT_END;
	}

	void KeybindManager::Release() {
		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (lpszKeybindArray[i]) {
				delete lpszKeybindArray[i];
				lpszKeybindArray[i] = nullptr;
			}
		}
	}

	void KeybindManager::InvokeKeybinds() {
		for (INT i = 0; i < MAX_INVOKE; i++)
			if (lpszKeybindArray[i])
				lpszKeybindArray[i]->Invoke();
	}

	_Check_return_ _Ret_maybenull_ IKeybind* KeybindManager::GetKeybindByVirtualKey(_In_ WORD wKey) {
		CONTEXT_BEGIN;

		IKeybind* lpReturn = nullptr;

		for (INT i = 0; i < MAX_INVOKE; i++)
			if (lpszKeybindArray[i])
				if (lpszKeybindArray[i]->wKey == wKey)
					lpReturn = lpszKeybindArray[i];

		if (!lpReturn) {
			CHAR szBuffer[MAX_NAME];
			sprintf_s(szBuffer, "Draw instance belonging to virtual key %hu", wKey);
			throw ObjectNotFoundException(szBuffer, "IDraw*");
		}

		CONTEXT_END;

		return lpReturn;
	}
}