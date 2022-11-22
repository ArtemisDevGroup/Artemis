#include "OnFrameManager.h"

#include <stdio.h>
#include <string.h>

namespace Artemis {
	OnFrameManager::OnFrameManager() { memset(lpszOnFrameArray, 0x00, sizeof(lpszOnFrameArray)); }

	void OnFrameManager::RegisterOnFrameAction(_In_ IOnFrame* lpOnFrameAction) {
		CONTEXT_BEGIN;

		if (!lpOnFrameAction) throw ParameterException("lpOnFrameAction");

		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (lpszOnFrameArray[i])
				if (lpszOnFrameArray[i]->dwOnFrameId == lpOnFrameAction->dwOnFrameId)
					throw AttributeException("dwOnFrameId");
		}

		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (!lpszOnFrameArray[i]) {
				lpszOnFrameArray[i] = lpOnFrameAction;
				break;
			}
		}

		CONTEXT_END;
	}

	void OnFrameManager::UnregisterOnFrameAction(_In_ IOnFrame* lpOnFrameAction) {
		CONTEXT_BEGIN;

		BOOL bUnregistered = FALSE;

		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (lpszOnFrameArray[i] == lpOnFrameAction) {
				lpszOnFrameArray[i] = nullptr;
				bUnregistered = TRUE;
				break;
			}
		}

		if (!bUnregistered) throw ObjectNotFoundException("lpOnFrameAction", "IOnFrame*");

		CONTEXT_END;
	}

	void OnFrameManager::Release() {
		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (lpszOnFrameArray[i]) {
				delete lpszOnFrameArray[i];
				lpszOnFrameArray[i] = nullptr;
			}
		}
	}

	void OnFrameManager::InvokeOnFrameActions() {
		for (INT i = 0; i < MAX_INVOKE; i++)
			if (lpszOnFrameArray[i])
				lpszOnFrameArray[i]->OnFrame();
	}

	_Check_return_ _Ret_maybenull_ IOnFrame* OnFrameManager::GetOnFrameActionById(_In_ DWORD dwOnFrameId) {
		CONTEXT_BEGIN;

		IOnFrame* lpReturn = nullptr;

		for (INT i = 0; i < MAX_INVOKE; i++)
			if (lpszOnFrameArray[i])
				if (lpszOnFrameArray[i]->dwOnFrameId == dwOnFrameId)
					lpReturn = lpszOnFrameArray[i];

		if (!lpReturn) {
			CHAR szBuffer[MAX_NAME];
			sprintf_s(szBuffer, "ONFrame instance belonging to id %lu", dwOnFrameId);
			throw ObjectNotFoundException(szBuffer, "IOnFrame*");
		}

		CONTEXT_END;

		return lpReturn;
	}
}