#include <Windows.h>
#include <Psapi.h>

#undef UNICODE
#include <TlHelp32.h>

#include "Memory.h"

// MemoryScanner

DWORD APIENTRY MemoryScanner::StartScan(LPSCAN_PARAMS p) {
	if (p->hProcess) {

	}
	else {

	}

	return 0;
}

MemoryScanner::MemoryScanner() : hThread(nullptr) { memset(&this->params, 0x00, sizeof(SCAN_PARAMS)); }
MemoryScanner::MemoryScanner(const MemoryScanner& cpy) {
	memcpy(this, &cpy, sizeof(MemoryScanner));

	if (cpy.params.lpAlloc) {
		this->params.lpAlloc = (LPADDRESS)calloc(cpy.params.dwMaxResultCount, sizeof(ADDRESS));
		memcpy(this->params.lpAlloc, cpy.params.lpAlloc, cpy.params.dwMaxResultCount * sizeof(ADDRESS));
	}
}
MemoryScanner::~MemoryScanner() { if (this->params.lpAlloc) free(this->params.lpAlloc); }

MemoryScanner::MemoryScanner(_In_ LPCSTR lpPattern, _In_z_ LPCSTR lpMask) : hThread(nullptr) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	memset(&this->params, 0x00, sizeof(SCAN_PARAMS));
	memcpy_s(params.CodePattern.szPattern, MAX_PATTERN_LENGTH, lpPattern, strlen(lpMask));
	if (strcpy_s(params.CodePattern.szMask, lpMask)) throw ParameterException("lpMask");

	SetMaxResultCount();
	SetStartAddress();
	SetScanSize();
	SetAlignment();
	SetScanProtection();
	if (!b) ExceptionContext::ResetContext();
}

MemoryScanner::MemoryScanner(_In_z_ LPCSTR lpPattern) : hThread(nullptr) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	memset(&this->params, 0x00, sizeof(SCAN_PARAMS));
	if (strcpy_s(params.IdaPattern.szPattern, lpPattern)) throw ParameterException("lpPattern");

	SetMaxResultCount();
	SetStartAddress();
	SetScanSize();
	SetAlignment();
	SetScanProtection();
	if (!b) ExceptionContext::ResetContext();
}

MemoryScanner::MemoryScanner(_In_z_ LPCSTR lpString, _In_ BOOL bCaseSensitive) : hThread(nullptr) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	memset(&this->params, 0x00, sizeof(SCAN_PARAMS));
	if (strcpy_s(params.StringA.szString, lpString)) throw ParameterException("lpString");
	params.StringA.bCaseSensitive;

	SetMaxResultCount();
	SetStartAddress();
	SetScanSize();
	SetAlignment();
	SetScanProtection();
	if (!b) ExceptionContext::ResetContext();
}
MemoryScanner::MemoryScanner(_In_z_ LPCWSTR lpString, _In_ BOOL bCaseSensitive) : hThread(nullptr) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	memset(&this->params, 0x00, sizeof(SCAN_PARAMS));
	if (wcscpy_s(params.StringW.szString, lpString)) throw ParameterException("lpString");
	params.StringA.bCaseSensitive;
	if (!b) ExceptionContext::ResetContext();
}

void MemoryScanner::SetExternalHandle(_In_ HANDLE hProcess) { this->params.hProcess = hProcess; }

void MemoryScanner::SetMaxResultCount(_In_ DWORD dwMaxResultCount) { this->params.dwMaxResultCount = dwMaxResultCount; }
void MemoryScanner::SetStartAddress(_In_ ADDRESS uStartAddress) { this->params.uStartAddress = uStartAddress; }
void MemoryScanner::SetScanSize(_In_ SIZE_T uScanSize) { this->params.uScanSize = uScanSize; }
void MemoryScanner::SetAlignment(_In_ DWORD dwAlignment) { this->params.dwAlignment = dwAlignment; }
void MemoryScanner::SetScanProtection(_In_ ScanProtection Protection) { this->params.Protection = Protection; }

void MemoryScanner::Start() {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	this->hThread = CreateThread(
		nullptr,
		NULL,
		(LPTHREAD_START_ROUTINE)StartScan,
		&this->params,
		NULL,
		nullptr
	);

	if (!this->hThread) throw WindowsApiException("CreateThread");
	if (!b) ExceptionContext::ResetContext();
}

void MemoryScanner::Wait() { WaitForSingleObject(this->hThread, INFINITE); }

LPADDRESS MemoryScanner::GetResult() { return params.lpAlloc; }
DWORD MemoryScanner::GetResultCount() { return params.dwResultCount; }

DWORD MemoryScanner::GetExitCode() {
	DWORD dw;
	GetExitCodeThread(this->hThread, &dw);
	return dw;
}

// VirtualAllocation

VirtualAllocation::VirtualAllocation() : hProcess(nullptr), uAddress(0), uSize(0), Type(), Protection(), uAllocatedAddress(0) {}

VirtualAllocation::~VirtualAllocation() { Free(); }

VirtualAllocation::VirtualAllocation(
	_In_ ADDRESS uAddress,
	_In_ SIZE_T uSize,
	_In_ AllocationType Type,
	_In_ MemoryProtection Protection
) : hProcess(nullptr), uAddress(uAddress), uSize(uSize), Type(Type), Protection(Protection), uAllocatedAddress(0) {}

void VirtualAllocation::SetExternalHandle(_In_ HANDLE hProcess) { this->hProcess = hProcess; }

void VirtualAllocation::Allocate() {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	if (!this->uAllocatedAddress) {
		if (this->hProcess) {
			if ((this->uAllocatedAddress = (ADDRESS)VirtualAllocEx(
				this->hProcess,
				(LPVOID)this->uAddress,
				this->uSize,
				(DWORD)this->Type,
				(DWORD)this->Protection
			)) == 0) throw WindowsApiException("VirtualAllocEx");
		}
		else {
			if ((this->uAllocatedAddress = (ADDRESS)VirtualAlloc(
				(LPVOID)this->uAddress,
				this->uSize,
				(DWORD)this->Type,
				(DWORD)this->Protection
			)) == 0) throw WindowsApiException("VirtualAlloc");
		}
	}
	if (!b) ExceptionContext::ResetContext();
}

void VirtualAllocation::Free(FreeType Type) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	if (this->uAllocatedAddress) {
		if (this->hProcess) {
			if (!VirtualFreeEx(
				this->hProcess,
				(LPVOID)uAllocatedAddress,
				Type == FreeType::Decommit ? uSize : 0,
				(DWORD)Type
			)) throw WindowsApiException("VirtualFreeEx");
		}
		else {
			if (!VirtualFree(
				(LPVOID)uAllocatedAddress,
				Type == FreeType::Decommit ? uSize : 0,
				(DWORD)Type
			)) throw WindowsApiException("VirtualFree");
		}
	}
	if (!b) ExceptionContext::ResetContext();
}

ADDRESS VirtualAllocation::GetAllocatedAddress() { return this->uAllocatedAddress; }

// Hook

// DllInjection

DllInjection::DllInjection(
	_In_z_ LPCSTR lpTargetProcess,
	_In_z_ LPCSTR lpDllPath,
	_In_ Architecture arch,
	_In_ PathRelativity pr
) : bCloseHandle(FALSE), arch(arch), pr(pr) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) throw WindowsApiException("CreateToolhelp32Snapshot");

	PROCESSENTRY32 pe32;
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

	// Path relativity

	if (!b) ExceptionContext::ResetContext();
}

DllInjection::DllInjection(
	_In_ DWORD dwTargetProcessId,
	_In_z_ LPCSTR lpDllPath,
	_In_ Architecture arch,
	_In_ PathRelativity pr
) : bCloseHandle(FALSE), arch(arch), pr(pr) {

}

DllInjection::DllInjection(
	_In_ HANDLE hTargetProcess,
	_In_z_ LPCSTR lpDllPath,
	_In_ Architecture arch,
	_In_ PathRelativity pr
) : bCloseHandle(FALSE), arch(arch), pr(pr) {

}

DllInjection::~DllInjection() { if (bCloseHandle) CloseHandle(hProcess); }

void DllInjection::Inject() {

}

// Memory

void Memory::_Read(
	_In_ ADDRESS uAddress,
	_Out_ LPVOID lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpBuffer) throw ParameterException("lpBuffer");
	if (!ExceptionContext::HasContext()) throw Exception("No context set.", ExceptionCode::Unknown, FALSE);

	if (this->TargetType == Type::Internal) {
		__try {
			for (INT i = 0; i < dwSize; i++) {
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

void Memory::_Write(
	_In_ ADDRESS uAddress,
	_In_ LPCVOID lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpBuffer) throw ParameterException("lpBuffer");
	if (!ExceptionContext::HasContext()) throw Exception("No context set.", ExceptionCode::Unknown, FALSE);

	if (this->TargetType == Type::Internal) {
		__try {
			for (INT i = 0; i < dwSize; i++) {
				*(BYTE*)(uAddress + i) = *((BYTE*)((ADDRESS)uAddress + i));
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

Memory::Memory() : hProcess(nullptr), hModule(nullptr), dwProcessId(0), uModuleBaseAddress(0), dwModuleSize(0) {
	this->szProcessName[0] = { '\0' };
	this->szProcessPath[0] = { '\0' };
	this->szModuleName[0] = { '\0' };
	this->szModulePath[0] = { '\0' };
}

Memory::Memory(_In_opt_ LPCSTR lpModuleName) : TargetType(Internal) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);

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

	if (!b) ExceptionContext::ResetContext();
}

Memory::Memory(_In_ LPCSTR lpProcessName, _In_opt_ LPCSTR lpModuleName) : TargetType(External), dwProcessId(0), uModuleBaseAddress(0), dwModuleSize(0) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	
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

	this->hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION, FALSE, this->dwProcessId);
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

	if (!b) ExceptionContext::ResetContext();
}

Memory::~Memory() { if (this->TargetType == External && this->hProcess) CloseHandle(this->hProcess); }

LPCSTR Memory::GetProcessName() const { return this->szProcessName; }
LPCSTR Memory::GetProcessPath() const { return this->szProcessPath; }
LPCSTR Memory::GetModuleName() const { return this->szModuleName; }
LPCSTR Memory::GetModulePath() const { return this->szModulePath; }

Memory::Type Memory::GetTargetType() const { return this->TargetType; }
HANDLE Memory::GetProcessHandle() const { return this->hProcess; }
DWORD Memory::GetProcessId() const { return this->dwProcessId; }
HMODULE Memory::GetModuleHandle() const { return this->hModule; }
ADDRESS Memory::GetModuleBase() const { return this->uModuleBaseAddress; }
DWORD Memory::GetModuleSize() const { return this->dwModuleSize; }

ADDRESS Memory::ReadPtrAddress(
	_In_ ADDRESS uAddress,
	_In_ LPCPOINTER lpPointer
) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	for (INT i = 0; i < lpPointer->dwCount; i++) {
		uAddress = Read<ADDRESS>(uAddress) + lpPointer->szPointer[i];
	}

	if (!b) ExceptionContext::ResetContext();
	return uAddress;
}

ADDRESS Memory::ReadPtrAddress(_In_ LPCBASE_POINTER lpPointer) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	ADDRESS uAddress = GetModuleBase() + lpPointer->uBaseOffset;
	for (INT i = 0; i < lpPointer->dwCount; i++) {
		uAddress = Read<ADDRESS>(uAddress) + lpPointer->szPointer[i];
	}

	if (!b) ExceptionContext::ResetContext();
	return uAddress;
}

void Memory::ReadStringA(
	_In_ ADDRESS uAddress,
	_Out_ LPSTR lpBuffer,
	_In_ UINT uCount
) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	_Read(uAddress, lpBuffer, uCount);
	lpBuffer[uCount - 1] = '\0';
	ExceptionContext::ResetContext();
}

void Memory::ReadStringW(
	_In_ ADDRESS uAddress,
	_Out_writes_z_(uCount) LPWSTR lpBuffer,
	_In_ UINT uCount
) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	_Read(uAddress, lpBuffer, uCount * sizeof(WCHAR));
	lpBuffer[uCount - 1] = L'\0';
	if (!b) ExceptionContext::ResetContext();
}

void Memory::WriteStringA(
	_In_ ADDRESS uAddress,
	_In_z_ LPCSTR lpString
) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	_Write(uAddress, lpString, strlen(lpString) + 1);
	if (!b) ExceptionContext::ResetContext();
}

void Memory::WriteStringW(
	_In_ ADDRESS uAddress,
	_In_z_ LPCWSTR lpString
) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	_Write(uAddress, lpString, (wcslen(lpString) + 1) * 2);
	if (!b) ExceptionContext::ResetContext();
}

void Memory::AssemblyPatch(
	ADDRESS uAddress,
	LPCASM_PATCH lpPatch,
	AssemblyAction Action
) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);

	if (Action == Enable) _Write(uAddress, lpPatch->szEnable, lpPatch->dwCount);
	else if (Action == Disable) _Write(uAddress, lpPatch->szDisable, lpPatch->dwCount);
	else throw ParameterException("Action");

	if (!b) ExceptionContext::ResetContext();
}

void Memory::AssemblyPatch(
	LPCBASE_ASM_PATCH lpPatch,
	AssemblyAction Action
) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);

	if (Action == Enable) _Write(GetModuleBase() + lpPatch->uBaseOffset, lpPatch->szEnable, lpPatch->dwCount);
	else if (Action == Disable) _Write(GetModuleBase() + lpPatch->uBaseOffset, lpPatch->szDisable, lpPatch->dwCount);
	else throw ParameterException("Action");

	if (!b) ExceptionContext::ResetContext();
}

MemoryScanner Memory::CreateScanner(_In_ LPCSTR lpPattern, _In_z_ LPCSTR lpMask, _In_ BOOL bScanModule) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	MemoryScanner ms(lpPattern, lpMask);
	if (this->TargetType == External) { ms.SetExternalHandle(this->hProcess); }

	if (bScanModule) {
		ms.SetStartAddress(this->GetModuleBase());
		ms.SetScanSize(this->GetModuleSize());
	}
	if (!b) ExceptionContext::ResetContext();
	return ms;
}

MemoryScanner Memory::CreateScanner(_In_z_ LPCSTR lpPattern, _In_ BOOL bScanModule) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	MemoryScanner ms(lpPattern);
	if (this->TargetType == External) { ms.SetExternalHandle(this->hProcess); }

	if (bScanModule) {
		ms.SetStartAddress(this->GetModuleBase());
		ms.SetScanSize(this->GetModuleSize());
	}
	if (!b) ExceptionContext::ResetContext();
	return ms;
}

MemoryScanner Memory::CreateScanner(_In_z_ LPCSTR lpString, _In_ BOOL bCaseSensitive, _In_ BOOL bScanModule) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	MemoryScanner ms(lpString, bCaseSensitive);
	if (this->TargetType == External) { ms.SetExternalHandle(this->hProcess); }

	if (bScanModule) {
		ms.SetStartAddress(this->GetModuleBase());
		ms.SetScanSize(this->GetModuleSize());
	}
	if (!b) ExceptionContext::ResetContext();
	return ms;
}

MemoryScanner Memory::CreateScanner(_In_z_ LPCWSTR lpString, _In_ BOOL bCaseSensitive, _In_ BOOL bScanModule) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	MemoryScanner ms(lpString, bCaseSensitive);
	if (this->TargetType == External) { ms.SetExternalHandle(this->hProcess); }

	if (bScanModule) {
		ms.SetStartAddress(this->GetModuleBase());
		ms.SetScanSize(this->GetModuleSize());
	}
	if (!b) ExceptionContext::ResetContext();
	return ms;
}

VirtualAllocation Memory::CreateVirtualAllocation(
	_In_opt_ ADDRESS uAddress,
	_In_ SIZE_T uSize,
	_In_ AllocationType Type,
	_In_ MemoryProtection Protection
) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);
	VirtualAllocation va(uAddress, uSize, Type, Protection);
	if (this->TargetType == External) { va.SetExternalHandle(this->hProcess); }
	if (!b) ExceptionContext::ResetContext();
	return va;
}

void Memory::VirtualProtect(
	_In_ ADDRESS uAddress,
	_In_ SIZE_T uSize,
	_In_ MemoryProtection NewProtection,
	_Out_opt_ MemoryProtection* OldProtection
) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);

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
		)) throw WindowsApiException("VirtualProtectEx");
	}
	else throw InstanceInvalidException();

	if (OldProtection) *OldProtection = (MemoryProtection)dwOld;

	if (!b) ExceptionContext::ResetContext();
}

void Memory::VirtualQuery(
	_In_ ADDRESS uAddress,
	_Out_ LPMBI lpMbi
) {
	BOOL b = ExceptionContext::HasContext();
	if (!b) ExceptionContext::SetContext(__FUNCTION__);

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

	if (!b) ExceptionContext::ResetContext();
}