//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Astrea0014.												|
// @Astrea0014: https://github.com/Astrea0014											|
//-------------------------------------------------------------------------------------->

#include "MemoryScanner.h"
#include "Exceptions.h"

#include <Windows.h>

namespace Artemis {
	//---------------------------------------------------------------------->
	// MemoryScanner
	//---------------------------------------------------------------------->
	// Private methods:
	DWORD APIENTRY MemoryScanner::StartScan(LPSCAN_PARAMS p) {
		if (p->hProcess) {
#pragma message("Write logic for external scanning.")
		}
		else {
#pragma message("Write logic for internal scanning.")
		}

		return 0;
	}
	//---------------------------------------------------------------------->
	// Constructors and destructor:
	MemoryScanner::MemoryScanner() : hThread(nullptr) {
		memset(&this->params, 0x00, sizeof(SCAN_PARAMS));
	}
	//-------------------------------------//
	MemoryScanner::MemoryScanner(const MemoryScanner& cpy) {
		CONTEXT_BEGIN;

		this->hThread = nullptr;
		this->params = { NULL };

		memcpy(this, &cpy, sizeof(MemoryScanner));

		if (cpy.params.lpAlloc) {
			this->params.lpAlloc = (LPADDRESS)calloc(cpy.params.dwMaxResultCount, sizeof(ADDRESS));
			if (!this->params.lpAlloc) throw Exception("Failed to allocate memory.", ExceptionCode::Unknown, FALSE);

			memcpy(this->params.lpAlloc, cpy.params.lpAlloc, cpy.params.dwMaxResultCount * sizeof(ADDRESS));
		}

		CONTEXT_END;
	}
	//-------------------------------------//
	MemoryScanner::~MemoryScanner() {
		if (this->params.lpAlloc) free(this->params.lpAlloc);
	}
	//-------------------------------------//
	MemoryScanner::MemoryScanner(_In_ LPCSTR lpPattern, _In_z_ LPCSTR lpMask) : hThread(nullptr) {
		CONTEXT_BEGIN;

		memset(&this->params, 0x00, sizeof(SCAN_PARAMS));
		memcpy_s(params.CodePattern.szPattern, MAX_PATTERN_LENGTH, lpPattern, strlen(lpMask));
		if (strcpy_s(params.CodePattern.szMask, lpMask)) throw ParameterException("lpMask");

		SetMaxResultCount();
		SetStartAddress();
		SetScanSize();
		SetAlignment();
		SetScanProtection();

		CONTEXT_END;
	}
	//-------------------------------------//
	MemoryScanner::MemoryScanner(_In_z_ LPCSTR lpPattern) : hThread(nullptr) {
		CONTEXT_BEGIN;

		memset(&this->params, 0x00, sizeof(SCAN_PARAMS));
		if (strcpy_s(params.IdaPattern.szPattern, lpPattern)) throw ParameterException("lpPattern");

		SetMaxResultCount();
		SetStartAddress();
		SetScanSize();
		SetAlignment();
		SetScanProtection();

		CONTEXT_END;
	}
	//-------------------------------------//
	MemoryScanner::MemoryScanner(_In_z_ LPCSTR lpString, _In_ BOOL bCaseSensitive) : hThread(nullptr) {
		CONTEXT_BEGIN;

		memset(&this->params, 0x00, sizeof(SCAN_PARAMS));
		if (strcpy_s(params.StringA.szString, lpString)) throw ParameterException("lpString");
		params.StringA.bCaseSensitive = bCaseSensitive;

		SetMaxResultCount();
		SetStartAddress();
		SetScanSize();
		SetAlignment();
		SetScanProtection();

		CONTEXT_END;
	}
	//-------------------------------------//
	MemoryScanner::MemoryScanner(_In_z_ LPCWSTR lpString, _In_ BOOL bCaseSensitive) : hThread(nullptr) {
		CONTEXT_BEGIN;

		memset(&this->params, 0x00, sizeof(SCAN_PARAMS));
		if (wcscpy_s(params.StringW.szString, lpString)) throw ParameterException("lpString");
		params.StringW.bCaseSensitive = bCaseSensitive;

		SetMaxResultCount();
		SetStartAddress();
		SetScanSize();
		SetAlignment();
		SetScanProtection();

		CONTEXT_END;
	}
	//---------------------------------------------------------------------->
	// Setters:
	void MemoryScanner::SetExternalHandle(_In_ HANDLE hProcess) { this->params.hProcess = hProcess; }
	void MemoryScanner::SetMaxResultCount(_In_ DWORD dwMaxResultCount) { this->params.dwMaxResultCount = dwMaxResultCount; }
	void MemoryScanner::SetStartAddress(_In_ ADDRESS uStartAddress) { this->params.uStartAddress = uStartAddress; }
	void MemoryScanner::SetScanSize(_In_ SIZE_T uScanSize) { this->params.uScanSize = uScanSize; }
	void MemoryScanner::SetAlignment(_In_ DWORD dwAlignment) { this->params.dwAlignment = dwAlignment; }
	void MemoryScanner::SetScanProtection(_In_ ScanProtection Protection) { this->params.Protection = Protection; }
	//---------------------------------------------------------------------->
	// Public methods:
	void MemoryScanner::Start() {
		CONTEXT_BEGIN;

		this->hThread = CreateThread(
			nullptr,
			NULL,
			(LPTHREAD_START_ROUTINE)StartScan,
			&this->params,
			NULL,
			nullptr
		);

		if (!this->hThread) throw WindowsApiException("CreateThread");

		CONTEXT_END;
	}
	//-------------------------------------//
	void MemoryScanner::Wait() const {
		CONTEXT_BEGIN;

		if (WaitForSingleObject(this->hThread, INFINITE) == WAIT_FAILED) throw WindowsApiException("WaitForSingleObject");

		CONTEXT_END;
	}
	//---------------------------------------------------------------------->
	// Getters:
	_Check_return_ List<ADDRESS> MemoryScanner::GetResults() const { return List<ADDRESS>(this->params.lpAlloc, this->params.dwResultCount); }
	//-------------------------------------//
	_Check_return_ DWORD MemoryScanner::GetExitCode() const {
		CONTEXT_BEGIN;

		DWORD dw;
		if (!GetExitCodeThread(this->hThread, &dw)) throw WindowsApiException("GetExitCodeThread");

		CONTEXT_END;
		return dw;
	}
}