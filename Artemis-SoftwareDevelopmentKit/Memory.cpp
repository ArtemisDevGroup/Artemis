//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Astrea0014.												|
// @Astrea0014: https://github.com/Astrea0014											|
//-------------------------------------------------------------------------------------->

#undef UNICODE

#pragma warning(disable:6101)

#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>

#include "Memory.h"

namespace Artemis {
	//---------------------------------------------------------------------->
	// Private methods:
	void Memory::_Read(
		_In_ ADDRESS uAddress,
		_Out_ LPVOID lpBuffer,
		_In_ DWORD dwSize
	) const {
		if (!ExceptionContext::HasContext()) throw Exception("No context set.", ExceptionCode::Unknown, FALSE);
		if (!lpBuffer) throw ParameterException("lpBuffer");

		if (this->TargetType == Type::Internal) {
			__try {
				for (UINT i = 0; i < dwSize; i++) {
					*((BYTE*)((ADDRESS)lpBuffer + i)) = *(BYTE*)(uAddress + i);
				}
			}
			__except (EXCEPTION_EXECUTE_HANDLER) { throw MemoryAccessViolationException(uAddress, dwSize, OperationType::Read); }
		}
		else if (this->TargetType == Type::External) {
			if (!ReadProcessMemory(
				this->hProcess,
				(LPCVOID)(uAddress),
				lpBuffer,
				dwSize,
				nullptr
			)) throw WindowsApiException("ReadProcessMemory");
		}
		else throw InstanceInvalidException();
	}
	//-------------------------------------//
	void Memory::_Write(
		_In_ ADDRESS uAddress,
		_In_ LPCVOID lpBuffer,
		_In_ DWORD dwSize
	) const {
		if (!ExceptionContext::HasContext()) throw Exception("No context set.", ExceptionCode::Unknown, FALSE);
		if (!lpBuffer) throw ParameterException("lpBuffer");

		if (this->TargetType == Type::Internal) {
			__try {
				for (UINT i = 0; i < dwSize; i++) {
					*(BYTE*)(uAddress + i) = *((BYTE*)((ADDRESS)lpBuffer + i));
				}
			}
			__except (EXCEPTION_EXECUTE_HANDLER) { throw MemoryAccessViolationException(uAddress, dwSize, OperationType::Write); }
		}
		else if (this->TargetType == Type::External) {
			if (!WriteProcessMemory(
				this->hProcess,
				(LPVOID)(uAddress),
				lpBuffer,
				dwSize,
				nullptr
			)) throw WindowsApiException("WriteProcessMemory");
		}
		else throw InstanceInvalidException();
	}
	//---------------------------------------------------------------------->
	// Constructors and destructor:
	Memory::Memory() : hProcess(nullptr), hModule(nullptr), dwProcessId(0), uModuleBaseAddress(0), dwModuleSize(0), TargetType() {
		this->szProcessName[0] = { '\0' };
		this->szProcessPath[0] = { '\0' };
		this->szModuleName[0] = { '\0' };
		this->szModulePath[0] = { '\0' };
	}
	//-------------------------------------//
	Memory::Memory(const Memory& cpy) {
		memcpy(this, &cpy, sizeof(Memory));

		if (TargetType == External) {
			if (!DuplicateHandle(
				GetCurrentProcess(),
				hProcess,
				GetCurrentProcess(),
				&hProcess,
				0,
				FALSE,
				DUPLICATE_SAME_ACCESS
			)) throw WindowsApiException("DuplicateHandle");
		}
	}
	//-------------------------------------//
	Memory::Memory(_In_opt_z_ LPCSTR lpModuleName) : TargetType(Internal) {
		CONTEXT_BEGIN;

		this->szModuleName[0] = '\0';
		this->szModulePath[0] = '\0';
		this->szProcessName[0] = '\0';
		this->szProcessPath[0] = '\0';

		this->hModule = GetModuleHandleA(lpModuleName);
		if (!this->hModule) throw ObjectNotFoundException(lpModuleName, "Loaded module.");

		this->hProcess = GetCurrentProcess();
		this->dwProcessId = GetCurrentProcessId();

		this->uModuleBaseAddress = (ADDRESS)this->hModule;

		MODULEINFO mnfo = { NULL };
		if (!K32GetModuleInformation(
			this->hProcess,
			this->hModule,
			&mnfo,
			sizeof(mnfo)
		)) throw WindowsApiException("K32GetModuleInformation");

		this->dwModuleSize = mnfo.SizeOfImage;

		if (!K32GetModuleBaseNameA(
			this->hProcess,
			nullptr,
			this->szProcessName,
			sizeof(this->szProcessName)
		)) throw WindowsApiException("K32GetModuleBaseNameA");

		if (!K32GetModuleFileNameExA(
			this->hProcess,
			nullptr,
			this->szProcessPath,
			sizeof(this->szProcessPath)
		)) throw WindowsApiException("K32GetModuleFileNameExA");

		if (!K32GetModuleBaseNameA(
			this->hProcess,
			this->hModule,
			this->szModuleName,
			sizeof(this->szModuleName)
		)) throw WindowsApiException("K32GetModuleBaseNameA");

		if (!K32GetModuleFileNameExA(
			this->hProcess,
			this->hModule,
			this->szModulePath,
			sizeof(this->szModulePath)
		)) throw WindowsApiException("K32GetModuleFileNameExA");

		CONTEXT_END;
	}
	//-------------------------------------//
	Memory::Memory(_In_z_ LPCSTR lpProcessName, _In_opt_z_ LPCSTR lpModuleName) : TargetType(External), dwProcessId(0), uModuleBaseAddress(0), dwModuleSize(0), hModule(nullptr) {
		CONTEXT_BEGIN;

		this->szModuleName[0] = '\0';
		this->szModulePath[0] = '\0';
		this->szProcessName[0] = '\0';
		this->szProcessPath[0] = '\0';

		PROCESSENTRY32 pe32 = { NULL };
		pe32.dwSize = sizeof(pe32);

		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (hSnap == INVALID_HANDLE_VALUE) throw WindowsApiException("CreateToolhelp32Snapshot");

		if (!Process32First(hSnap, &pe32)) {
			WindowsApiException e = WindowsApiException("Process32First");
			CloseHandle(hSnap);
			throw e;
		}

		do {
			if (!strcmp(lpProcessName, pe32.szExeFile)) {
				this->dwProcessId = pe32.th32ProcessID;
				break;
			}
		} while (Process32Next(hSnap, &pe32));
		CloseHandle(hSnap);

		if (!this->dwProcessId) throw ObjectNotFoundException(lpProcessName, "Running process.");

		MODULEENTRY32 me32 = { NULL };
		me32.dwSize = sizeof(me32);

		hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->dwProcessId);
		if (hSnap == INVALID_HANDLE_VALUE) throw WindowsApiException("CreateToolhelp32Snapshot");

		if (!Module32First(hSnap, &me32)) {
			WindowsApiException e = WindowsApiException("Module32First");
			CloseHandle(hSnap);
			throw e;
		}

		do {
			if (!strcmp(lpModuleName ? lpModuleName : lpProcessName, me32.szModule)) {
				this->hModule = me32.hModule;
				this->uModuleBaseAddress = (ADDRESS)me32.modBaseAddr;
				this->dwModuleSize = me32.modBaseSize;
			}
		} while (Module32Next(hSnap, &me32));
		CloseHandle(hSnap);

		if (!this->uModuleBaseAddress) throw ObjectNotFoundException(lpModuleName ? lpModuleName : lpProcessName, "Loaded module in target process.");

		this->hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_DUP_HANDLE, FALSE, this->dwProcessId);
		if (!this->hProcess) throw WindowsApiException("OpenProcess");

		if (!K32GetModuleBaseNameA(
			this->hProcess,
			nullptr,
			this->szProcessName,
			sizeof(this->szProcessName)
		)) throw WindowsApiException("K32GetModuleBaseNameA");

		if (!K32GetModuleFileNameExA(
			this->hProcess,
			nullptr,
			this->szProcessPath,
			sizeof(this->szProcessPath)
		)) throw WindowsApiException("K32GetModuleFileNameExA");

		if (!K32GetModuleBaseNameA(
			this->hProcess,
			this->hModule,
			this->szModuleName,
			sizeof(this->szModuleName)
		)) throw WindowsApiException("K32GetModuleBaseNameA");

		if (!K32GetModuleFileNameExA(
			this->hProcess,
			this->hModule,
			this->szModulePath,
			sizeof(this->szModulePath)
		)) throw WindowsApiException("K32GetModuleFileNameExA");

		CONTEXT_END;
	}
	//-------------------------------------//
	Memory::~Memory() { Release(); }
	//---------------------------------------------------------------------->
	// Getters:
	_Check_return_ _Ret_z_ LPCSTR Memory::GetProcessName() const { return this->szProcessName; }
	_Check_return_ _Ret_z_ LPCSTR Memory::GetProcessPath() const { return this->szProcessPath; }
	_Check_return_ _Ret_z_ LPCSTR Memory::GetModuleName() const { return this->szModuleName; }
	_Check_return_ _Ret_z_ LPCSTR Memory::GetModulePath() const { return this->szModulePath; }

	_Check_return_ Memory::Type Memory::GetTargetType() const { return this->TargetType; }
	_Check_return_ _Ret_z_ HANDLE Memory::GetProcessHandle() const { return this->hProcess; }
	_Check_return_ DWORD Memory::GetProcessId() const { return this->dwProcessId; }
	_Check_return_ _Ret_notnull_ HMODULE Memory::GetModuleHandle() const { return this->hModule; }
	_Check_return_ ADDRESS Memory::GetModuleBase() const { return this->uModuleBaseAddress; }
	_Check_return_ DWORD Memory::GetModuleSize() const { return this->dwModuleSize; }
	//---------------------------------------------------------------------->
	// Public methods:
	void Memory::ReadStringA(
		_In_ ADDRESS uAddress,
		_Out_writes_z_(uCount) LPSTR lpBuffer,
		_In_ UINT uCount
	) const {
		CONTEXT_BEGIN;

		_Read(uAddress, lpBuffer, uCount);
		lpBuffer[uCount - 1] = '\0';
		
		CONTEXT_END;
	}
	//-------------------------------------//
	void Memory::ReadStringW(
		_In_ ADDRESS uAddress,
		_Out_writes_z_(uCount) LPWSTR lpBuffer,
		_In_ UINT uCount
	) const {
		CONTEXT_BEGIN;

		_Read(uAddress, lpBuffer, uCount * sizeof(WCHAR));
		lpBuffer[uCount - 1] = L'\0';
		
		CONTEXT_END;
	}
	//-------------------------------------//
	void Memory::WriteStringA(
		_In_ ADDRESS uAddress,
		_In_z_ LPCSTR lpString
	) const {
		CONTEXT_BEGIN;

		_Write(uAddress, lpString, (DWORD)strlen(lpString) + 1);
		
		CONTEXT_END;
	}
	//-------------------------------------//
	void Memory::WriteStringW(
		_In_ ADDRESS uAddress,
		_In_z_ LPCWSTR lpString
	) const {
		CONTEXT_BEGIN;

		_Write(uAddress, lpString, ((DWORD)wcslen(lpString) + 1) * 2);

		CONTEXT_END;
	}
	//-------------------------------------//
	void Memory::AssemblyPatch(
		_In_ ADDRESS uAddress,
		_In_ LPCASM_PATCH lpPatch,
		_In_ AssemblyAction Action
	) const {
		CONTEXT_BEGIN;

		if (Action == Enable) _Write(uAddress, lpPatch->szEnable, lpPatch->dwCount);
		else if (Action == Disable) _Write(uAddress, lpPatch->szDisable, lpPatch->dwCount);
		else throw ParameterException("Action");

		CONTEXT_END;
	}
	//-------------------------------------//
	void Memory::AssemblyPatch(
		_In_ LPCBASE_ASM_PATCH lpPatch,
		_In_ AssemblyAction Action
	) const {
		CONTEXT_BEGIN;

		if (Action == Enable) _Write(GetModuleBase() + lpPatch->uBaseOffset, lpPatch->szEnable, lpPatch->dwCount);
		else if (Action == Disable) _Write(GetModuleBase() + lpPatch->uBaseOffset, lpPatch->szDisable, lpPatch->dwCount);
		else throw ParameterException("Action");

		CONTEXT_END;
	}
	//-------------------------------------//
	_Check_return_ MemoryScanner Memory::CreateScanner(_In_ LPCSTR lpPattern, _In_z_ LPCSTR lpMask, _In_ BOOL bScanModule) const {
		CONTEXT_BEGIN;

		MemoryScanner ms(lpPattern, lpMask);
		if (this->TargetType == External) { ms.SetExternalHandle(this->hProcess); }

		if (bScanModule) {
			ms.SetStartAddress(this->GetModuleBase());
			ms.SetScanSize(this->GetModuleSize());
		}

		CONTEXT_END;
		return ms;
	}
	//-------------------------------------//
	_Check_return_ MemoryScanner Memory::CreateScanner(_In_z_ LPCSTR lpPattern, _In_ BOOL bScanModule) const {
		CONTEXT_BEGIN;

		MemoryScanner ms(lpPattern);
		if (this->TargetType == External) { ms.SetExternalHandle(this->hProcess); }

		if (bScanModule) {
			ms.SetStartAddress(this->GetModuleBase());
			ms.SetScanSize(this->GetModuleSize());
		}

		CONTEXT_END;
		return ms;
	}
	//-------------------------------------//
	_Check_return_ MemoryScanner Memory::CreateScanner(_In_z_ LPCSTR lpString, _In_ BOOL bCaseSensitive, _In_ BOOL bScanModule) const {
		CONTEXT_BEGIN;

		MemoryScanner ms(lpString, bCaseSensitive);
		if (this->TargetType == External) { ms.SetExternalHandle(this->hProcess); }

		if (bScanModule) {
			ms.SetStartAddress(this->GetModuleBase());
			ms.SetScanSize(this->GetModuleSize());
		}

		CONTEXT_END;
		return ms;
	}
	//-------------------------------------//
	_Check_return_ MemoryScanner Memory::CreateScanner(_In_z_ LPCWSTR lpString, _In_ BOOL bCaseSensitive, _In_ BOOL bScanModule) const {
		CONTEXT_BEGIN;

		MemoryScanner ms(lpString, bCaseSensitive);
		if (this->TargetType == External) { ms.SetExternalHandle(this->hProcess); }

		if (bScanModule) {
			ms.SetStartAddress(this->GetModuleBase());
			ms.SetScanSize(this->GetModuleSize());
		}

		CONTEXT_END;
		return ms;
	}
	//-------------------------------------//
	_Check_return_ VirtualAllocation Memory::CreateVirtualAllocation(
		_In_opt_ ADDRESS uAddress,
		_In_ SIZE_T uSize,
		_In_ AllocationType Type,
		_In_ MemoryProtection Protection
	) const {
		CONTEXT_BEGIN;

		VirtualAllocation va(uAddress, uSize, Type, Protection);
		if (this->TargetType == External) { va.SetExternalHandle(this->hProcess); }

		CONTEXT_END;
		return va;
	}
	//-------------------------------------//
	void Memory::VirtualProtect(
		_In_ ADDRESS uAddress,
		_In_ SIZE_T uSize,
		_In_ MemoryProtection NewProtection,
		_Out_opt_ MemoryProtection* lpOldProtection
	) const {
		CONTEXT_BEGIN;

		DWORD dwOld = 0;

		if (this->TargetType == External) {
			if (!VirtualProtectEx(
				this->hProcess,
				(LPVOID)uAddress,
				uSize,
				(DWORD)NewProtection,
				&dwOld
			)) throw WindowsApiException("VirtualProtectEx");
		}
		else if (this->TargetType == Internal) {
			if (!::VirtualProtect(
				(LPVOID)uAddress,
				uSize,
				(DWORD)NewProtection,
				&dwOld
			)) throw WindowsApiException("VirtualProtect");
		}
		else throw InstanceInvalidException();

		if (lpOldProtection) *lpOldProtection = (MemoryProtection)dwOld;

		CONTEXT_END;
	}
	//-------------------------------------//
	void Memory::VirtualQuery(
		_In_ ADDRESS uAddress,
		_Out_ LPMBI lpMbi
	) const {
		CONTEXT_BEGIN;

		DWORD dwOld = 0;

		if (this->TargetType == External) {
			if (!VirtualQueryEx(
				this->hProcess,
				(LPCVOID)uAddress,
				lpMbi,
				sizeof(MBI)
			)) throw WindowsApiException("VirtualProtectEx");
		}
		else if (this->TargetType == Internal) {
			if (!::VirtualQuery(
				(LPCVOID)uAddress,
				lpMbi,
				sizeof(MBI)
			)) throw WindowsApiException("VirtualProtectEx");
		}
		else throw InstanceInvalidException();

		CONTEXT_END;
	}
	//-------------------------------------//
	_Check_return_ Hook Memory::CreateHook(
		_In_z_ LPCSTR lpFunction,
		_In_ LPVOID lpDetour,
		_Out_opt_ LPVOID* lpOriginal
	) const {
		CONTEXT_BEGIN;

		if (this->TargetType == External) { throw NotImplementedException(); }
		else if (this->TargetType == Internal) {
			Hook hook(hModule, lpFunction, lpDetour, lpOriginal);
			return hook;
		}
		else throw InstanceInvalidException();

		CONTEXT_END;
	}
	//-------------------------------------//
	_Check_return_ Hook Memory::CreateHook(
		_In_ LPVOID lpTarget,
		_In_ LPVOID lpDetour,
		_Out_opt_ LPVOID* lpOriginal
	) const {
		CONTEXT_BEGIN;

		if (this->TargetType == External) { throw NotImplementedException(); }
		else if (this->TargetType == Internal) {
			Hook hook(lpTarget, lpDetour, lpOriginal);
			return hook;
		}
		else throw InstanceInvalidException();

		CONTEXT_END;
	}
	//-------------------------------------//
	_Check_return_ Hook Memory::CreateHook(
		_In_ ADDRESS uTarget,
		_In_ LPVOID lpDetour,
		_Out_opt_ LPVOID* lpOriginal
	) const {
		CONTEXT_BEGIN;

		if (this->TargetType == External) { throw NotImplementedException(); }
		else if (this->TargetType == Internal) {
			Hook hook(uTarget, lpDetour, lpOriginal);
			return hook;
		}
		else throw InstanceInvalidException();

		CONTEXT_END;
	}
	//-------------------------------------//
	void Memory::Release() {
		CONTEXT_BEGIN;

		if (TargetType == External && hProcess)
			if (CloseHandle(hProcess)) hProcess = nullptr;
			else throw WindowsApiException("CloseHandle");

		CONTEXT_END;
	}
}