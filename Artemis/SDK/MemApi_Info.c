#include "MemApi_Info.h"

#include <Windows.h>
#include <Psapi.h>

BOOL XEGetModuleInformation(
	_Out_ LPMODULE lpModule,
	_In_opt_z_ LPCSTR lpModuleName
) {
	if (!lpModule) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	lpModule->OwningProcess.hProcess = GetCurrentProcess();
	lpModule->OwningProcess.dwOwnerProcessId = GetCurrentProcessId();

	lpModule->hModule = GetModuleHandleA(lpModuleName);
	if (!lpModule->hModule) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	MODULEINFO mnfo;
	if (!K32GetModuleInformation(
		lpModule->OwningProcess.hProcess,
		lpModule->hModule,
		&mnfo,
		sizeof(MODULEINFO)
	)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	lpModule->uModuleBase = (ADDRESS)lpModule->hModule;
	lpModule->dwModuleSize = mnfo.SizeOfImage;

	if (!K32GetModuleBaseNameA(
		lpModule->OwningProcess.hProcess,
		lpModule->hModule,
		lpModule->szModuleName,
		sizeof(lpModule->szModuleName)
	)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	if (!K32GetModuleFileNameExA(
		lpModule->OwningProcess.hProcess,
		lpModule->hModule,
		lpModule->szModulePath,
		sizeof(lpModule->szModulePath)
	)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	if (!K32GetModuleBaseNameA(
		lpModule->OwningProcess.hProcess,
		NULL,
		lpModule->OwningProcess.szProcessName,
		sizeof(lpModule->OwningProcess.szProcessName)
	)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	if (!K32GetModuleFileNameExA(
		lpModule->OwningProcess.hProcess,
		NULL,
		lpModule->OwningProcess.szProcessPath,
		sizeof(lpModule->OwningProcess.szProcessPath)
	)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	return XEResetLastError();
}