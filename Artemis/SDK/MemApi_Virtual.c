#include "MemApi_Virtual.h"

#include <Windows.h>

BOOL XEVirtualProtect(
	_In_ ADDRESS uAddress,
	_In_ DWORD dwSize,
	_In_ MEMORY_PROTECTION MemoryProtection,
	_Out_opt_ LPMEMORY_PROTECTION lpOldProtection
) {
	DWORD dwOld;

	HANDLE hProc = OpenProcess(PROCESS_VM_OPERATION, FALSE, GetCurrentProcessId());

	if (!VirtualProtectEx(
		hProc,
		(LPVOID)uAddress,
		dwSize,
		(DWORD)MemoryProtection,
		&dwOld
	)) return XESetLastError(XE_ERROR_WINAPI_FAIL);
	CloseHandle(hProc);

	if (lpOldProtection) *lpOldProtection = (MEMORY_PROTECTION)dwOld;

	return XEResetLastError();
}

BOOL XEVirtualAlloc(
	_Out_ LPVIRTUAL_ALLOC lpVirtualAlloc,
	_In_opt_ ADDRESS uAddress,
	_In_ DWORD dwSize,
	_In_ ALLOCATION_TYPE AllocationType,
	_In_ MEMORY_PROTECTION MemoryProtection
) {
	if (!lpVirtualAlloc) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	LPVOID lpAlloc = VirtualAlloc(
		(LPVOID)uAddress,
		dwSize,
		(DWORD)AllocationType,
		(DWORD)MemoryProtection
	);

	if (!lpAlloc) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	lpVirtualAlloc->uRequestedAddress = uAddress;
	lpVirtualAlloc->uAllocatedAddress = (ADDRESS)lpAlloc;
	lpVirtualAlloc->dwAllocatedSize = dwSize;
	lpVirtualAlloc->AllocationType = AllocationType;
	lpVirtualAlloc->MemoryProtection = MemoryProtection;

	return XEResetLastError();
}

BOOL XEVirtualFree(
	_In_ LPVIRTUAL_ALLOC lpVirtualAlloc,
	_In_ FREE_TYPE FreeType
) {
	if (!lpVirtualAlloc) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	if (FreeType == FT_DECOMMIT) {
		if (!(lpVirtualAlloc->AllocationType & AT_COMMIT)) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

		if (!VirtualFree(
			(LPVOID)lpVirtualAlloc->uAllocatedAddress,
			lpVirtualAlloc->dwAllocatedSize,
			MEM_DECOMMIT
		)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

		lpVirtualAlloc->AllocationType ^= AT_COMMIT;
	}
	else if (FreeType == FT_RELEASE) {
		if (!VirtualFree(
			(LPVOID)lpVirtualAlloc->uAllocatedAddress,
			0,
			MEM_RELEASE
		)) return XESetLastError(XE_ERROR_WINAPI_FAIL);
	}
	else return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	return XEResetLastError();
}

BOOL XEVirtualQuery(
	_In_ ADDRESS uAddress,
	_Out_ LPMBI lpMbi
) {
	if (!lpMbi) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	MBI mbi;
	mbi.RegionSize = 4096;

	if (!VirtualQuery(
		(LPCVOID)uAddress,
		&mbi,
		sizeof(MBI)
	)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	*lpMbi = mbi;

	return XEResetLastError();
}