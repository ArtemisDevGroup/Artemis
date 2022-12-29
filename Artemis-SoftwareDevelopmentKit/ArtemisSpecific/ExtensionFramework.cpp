#include "ExtensionFramework.h"

#include <Windows.h>

namespace Artemis {
	Extension::Extension(_In_z_ LPCSTR lpDllName) {
		CONTEXT_BEGIN;

		CHAR szDllPath[MAX_PATH] = "Artemis-Extensions\\";
		strcat_s(szDllPath, lpDllName);

		hModule = LoadLibraryA(szDllPath);
		if (!hModule) throw WindowsApiException("LoadLibraryA");

		GetExtensionDescriptor = (GetExtensionDescriptorFunction)GetProcAddress(hModule, "GetExtensionDescriptor");
		if (!GetExtensionDescriptor) throw WindowsApiException("GetProcAddress");

		GetExtensionDescriptor(&Descriptor);

		CONTEXT_END;
	}

	_Check_return_ _Ret_z_ LPCSTR Extension::GetExtensionName() const { return Descriptor.szExtensionName; }
	_Check_return_ _Ret_z_ LPCSTR Extension::GetAuthorName() const { return Descriptor.szAuthorName; }

	_Check_return_ BOOL Extension::Initialize() const { return Descriptor.Initialize(hModule); }
	
	void Extension::Release() {
		CONTEXT_BEGIN;

		if (hModule) {
			if (Descriptor.Release) Descriptor.Release();

			if (FreeLibrary(hModule))
				hModule = nullptr;
			else throw WindowsApiException("FreeLibrary");
		}

		CONTEXT_END;
	}

	Extension::~Extension() { Release(); }

	ExtensionManager::ExtensionManager() : nExtensionCount(0) { memset(lpszExtensions, 0x00, sizeof(lpszExtensions)); }

	BOOL PathIsDll(_In_z_ LPCSTR lpDllName) {
		SIZE_T uSize = strlen(lpDllName);

		if (lpDllName[uSize - 4] == '.' &&
			lpDllName[uSize - 3] == 'd' &&
			lpDllName[uSize - 2] == 'l' &&
			lpDllName[uSize - 1] == 'l'
			) return TRUE;
		else return FALSE;
	}

	void ExtensionManager::LoadAll() {
		CONTEXT_BEGIN;

		WIN32_FIND_DATAA FindData;
		HANDLE hFindFile = FindFirstFileA(
			"Artemis-Extensions\\*",
			&FindData
		);

		if (hFindFile == INVALID_HANDLE_VALUE) throw WindowsApiException("FindFirstFileA");

		do {
			if (PathIsDll(FindData.cFileName)) {
				lpszExtensions[nExtensionCount] = new Extension(FindData.cFileName);
				if (lpszExtensions[nExtensionCount]->Initialize()) {
					nExtensionCount++;
				}
				else {
					lpszExtensions[nExtensionCount]->Release();
					delete lpszExtensions[nExtensionCount];
					lpszExtensions[nExtensionCount] = nullptr;
				}
			}
		} while (FindNextFileA(hFindFile, &FindData));

		FindClose(hFindFile);

		CONTEXT_END;
	}

	_Check_return_ _Ret_maybenull_ Extension* ExtensionManager::GetExtensionByName(_In_z_ LPCSTR lpExtensionName) {
		CONTEXT_BEGIN;

		Extension* pReturn = nullptr;

		for (INT i = 0; i < nExtensionCount; i++) {
			if (!strcmp(lpExtensionName, lpszExtensions[i]->GetExtensionName())) {
				pReturn = lpszExtensions[i];
				break;
			}
		}

		if (!pReturn) throw ObjectNotFoundException(lpExtensionName, "Extension object");

		CONTEXT_END;
		return pReturn;
	}

	void ExtensionManager::Release() {
		for (INT i = 0; i < nExtensionCount; i++) {
			lpszExtensions[i]->Release();
			delete lpszExtensions[i];
			lpszExtensions[i] = nullptr;
		}
	}

	ExtensionManager::~ExtensionManager() { Release(); }
}