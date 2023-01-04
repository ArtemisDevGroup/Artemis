//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Astrea0014.												|
// @Astrea0014: https://github.com/Astrea0014											|
//-------------------------------------------------------------------------------------->

#include "MemoryInjection.h"
#include "MemoryAllocation.h"
#include "Exceptions.h"

#undef UNICODE

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <DbgHelp.h>

#pragma comment(lib, "DbgHelp.lib")

namespace Artemis {
	//---------------------------------------------------------------------->
	// Private fields:
	void DllInjection::GetRealPath(
		_In_ LPCSTR lpDllPath,
		_In_ PathRelativity pr
	) {
		switch (pr) {
		case Absolute:
			strcpy_s(szPath, lpDllPath);
			break;
		case ToCaller:
		case ToTarget:
			if (!K32GetModuleFileNameExA(pr == ToCaller ? GetCurrentProcess() : hProcess, nullptr, szPath, MAX_PATH)) throw WindowsApiException("K32GetModuleFileNameExA");
			for (SIZE_T i = strlen(szPath); szPath[i - 1] != '\\'; szPath[i--] = '\0');
			break;
		default:
			throw ParameterException("pr");
		}
	}
	//-------------------------------------//
	void DllInjection::CheckArchitecture() {
		HANDLE hFile = CreateFileA(
			szPath,
			FILE_READ_ACCESS,
			FILE_SHARE_READ,
			nullptr,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);

		if (!hFile) throw WindowsApiException("CreateFileA");

		HANDLE hMap = CreateFileMappingA(
			hFile,
			nullptr,
			PAGE_READONLY,
			0, 0, nullptr
		);

		if (!hMap) {
			WindowsApiException e("CreateFileA");
			CloseHandle(hFile);
			throw e;
		}

		LPVOID lpView = MapViewOfFileEx(
			hMap,
			FILE_MAP_READ,
			0, 0, 0, nullptr
		);

		if (!lpView) {
			WindowsApiException e("MapViewOfFileEx");
			CloseHandle(hFile);
			CloseHandle(hMap);
			throw e;
		}

		PIMAGE_NT_HEADERS lpHeaders = ImageNtHeader(lpView);

		if (!lpHeaders) {
			WindowsApiException e("ImageNtHeader");
			CloseHandle(hFile);
			CloseHandle(hMap);
			throw e;
		}

		WORD wDllMachine = lpHeaders->FileHeader.Machine; // IMAGE_FILE_MACHINE_I386 | IMAGE_FILE_MACHINE_AMD64

		CloseHandle(hMap);
		CloseHandle(hFile);

		WORD wProcessMachine;

		if (!IsWow64Process2(
			this->hProcess,
			&wProcessMachine,
			nullptr
		)) throw WindowsApiException("IsWow64Process2");

		if ((wProcessMachine ? wProcessMachine : IMAGE_FILE_MACHINE_AMD64) != wDllMachine) throw CompatibilityException();
	}
	//---------------------------------------------------------------------->
	// Constructors and destructor:
	DllInjection::DllInjection(
		_In_z_ LPCSTR lpTargetProcess,
		_In_z_ LPCSTR lpDllPath,
		_In_ PathRelativity pr
	) : bCloseHandle(FALSE) {
		CONTEXT_BEGIN;

		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnap == INVALID_HANDLE_VALUE) throw WindowsApiException("CreateToolhelp32Snapshot");

		PROCESSENTRY32 pe32 = { NULL };
		pe32.dwSize = sizeof(pe32);

		if (!Process32First(hSnap, &pe32)) {
			WindowsApiException e("CreateToolhelp32Snapshot");
			CloseHandle(hSnap);
			throw e;
		}

		DWORD dwProcessId = 0;
		do {
			if (!strcmp(lpTargetProcess, pe32.szExeFile)) {
				dwProcessId = pe32.th32ProcessID;
				break;
			}
		} while (Process32Next(hSnap, &pe32));
		CloseHandle(hSnap);

		if (!dwProcessId) throw ObjectNotFoundException(lpTargetProcess, "Running process.");

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, FALSE, dwProcessId);
		if (!hProcess) throw WindowsApiException("OpenProcess");

		bCloseHandle = TRUE;

		GetRealPath(lpDllPath, pr);
		CheckArchitecture();

		CONTEXT_END;
	}
	//-------------------------------------//
	DllInjection::DllInjection(
		_In_ DWORD dwTargetProcessId,
		_In_z_ LPCSTR lpDllPath,
		_In_ PathRelativity pr
	) : bCloseHandle(FALSE) {
		CONTEXT_BEGIN;

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, FALSE, dwTargetProcessId);
		if (!hProcess) throw WindowsApiException("OpenProcess");

		bCloseHandle = TRUE;

		GetRealPath(lpDllPath, pr);
		CheckArchitecture();

		CONTEXT_END;
	}
	//-------------------------------------//
	DllInjection::DllInjection(
		_In_ HANDLE hTargetProcess,
		_In_z_ LPCSTR lpDllPath,
		_In_ PathRelativity pr
	) : bCloseHandle(FALSE), hProcess(hTargetProcess) {
		CONTEXT_BEGIN;

		GetRealPath(lpDllPath, pr);
		CheckArchitecture();

		CONTEXT_END;
	}
	//-------------------------------------//
	DllInjection::~DllInjection() { Release(); }
	//---------------------------------------------------------------------->
	// Public methods:
	void DllInjection::Inject() const {
		CONTEXT_BEGIN;

		VirtualAllocation* lpAlloc = new VirtualAllocation(0, strlen(szPath) + 1);
		lpAlloc->SetExternalHandle(hProcess);
		lpAlloc->Allocate();
		lpAlloc->Write(0, szPath, (DWORD)strlen(szPath) + 1);

		HMODULE hK32 = GetModuleHandleA("kernel32.dll");
		if (!hK32) {
			WindowsApiException e("GetModuleHandleA");
			lpAlloc->Release();
			delete lpAlloc;
			throw e;
		}

		LPTHREAD_START_ROUTINE lpLoadLibraryA = (LPTHREAD_START_ROUTINE)GetProcAddress(hK32, "LoadLibraryA");
		if (!lpLoadLibraryA) {
			WindowsApiException e("GetProcAddress");
			lpAlloc->Release();
			delete lpAlloc;
			throw e;
		}

		HANDLE hThread = CreateRemoteThread(
			hProcess,
			nullptr, 0,
			lpLoadLibraryA,
			lpAlloc->GetAllocatedAddress<void>(),
			0, nullptr
		);

		if (!hThread) {
			WindowsApiException e("CreateRemoteThread");
			lpAlloc->Release();
			delete lpAlloc;
			throw e;
		}

		Sleep(200);

		lpAlloc->Release();
		delete lpAlloc;

		WaitForSingleObject(hThread, INFINITE);

		DWORD dwExitCode;
		GetExitCodeThread(hThread, &dwExitCode);

		CloseHandle(hThread);

		if (!dwExitCode) {
			LPTHREAD_START_ROUTINE lpGetLastError = (LPTHREAD_START_ROUTINE)GetProcAddress(hK32, "GetLastError");

			HANDLE hErrorThread = CreateRemoteThread(
				this->hProcess,
				nullptr, 0,
				lpGetLastError,
				nullptr, 0, nullptr
			);

			if (!hErrorThread) throw WindowsApiException("CreateRemoteThread");

			WaitForSingleObject(hErrorThread, INFINITE);

			DWORD dwWinCode;
			GetExitCodeThread(hErrorThread, &dwWinCode);

			CloseHandle(hErrorThread);

			throw WindowsApiException("LoadLibraryA", dwWinCode);
		}

		CONTEXT_END;
	}
	//-------------------------------------//
	void DllInjection::Release() {
		if (bCloseHandle) {
			CloseHandle(hProcess);
			bCloseHandle = FALSE;
		}
	}
}