#include "ErrorApi.h"

#include <Windows.h>

QWORD qwCode;

QWORD XEGetLastError() { return qwCode; }

BOOL XESetLastError(_In_ DWORD dwLibCode) {
	*(DWORD*)(&qwCode) = dwLibCode;
	if (dwLibCode == XE_ERROR_WINAPI_FAIL) *(DWORD*)((uintptr_t)&qwCode + 4) = GetLastError();
	else *(DWORD*)((uintptr_t)&qwCode + 4) = 0;

	return FALSE;
}

BOOL XEResetLastError() {
	qwCode = 0;
	SetLastError(0);

	return TRUE;
}

ERROR_ENTRY XEGetLastErrorEntry() {
	ERROR_ENTRY e;

	e.dwLibCode = *(DWORD*)(&qwCode);
	XEGetErrorMessage(e.dwLibCode, e.szLibMessage, sizeof(e.szLibMessage));

	e.dwWinCode = *(DWORD*)((uintptr_t)&qwCode + 4);
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, e.dwWinCode, LANG_SYSTEM_DEFAULT, e.szWinMessage, sizeof(e.szWinMessage), NULL);

	return e;
}

#pragma warning(push)
#pragma warning(disable:6054)
BOOL XEGetErrorMessage(
	_In_ DWORD dwLibCode,
	_Out_writes_z_(dwSize) LPSTR lpBuffer,
	_In_ DWORD dwSize
) {
#define STR(code, s) case code: if (strcpy_s(lpBuffer, dwSize, s)) return FALSE; break

	switch (dwLibCode) {
		STR(XE_ERROR_SUCCESS, "Function call succeeded.");
		STR(XE_ERROR_BUFFER_SMALL, "A return buffer used by this function is too small for the operation to succeed.");
		STR(XE_ERROR_WINAPI_FAIL, "A WinApi function has failed, see the WinApi error for further details.");
		STR(XE_ERROR_PARAMETER_INVALID, "A parameter has an invalid value.");
		STR(XE_ERROR_PARAMETER_NULL, "A crucial parameter is NULL.");
		STR(XE_ERROR_MEMORY_FULL, "Unable to allocate memory.");
		STR(XE_ERROR_FUNCTION_FAIL, "A function that has been called has threw an exception.");
		STR(XE_ERROR_READ_ACCESS_VIOLATION, "Memory access violation reading address. Address is either invalid or inaccessible.");
		STR(XE_ERROR_WRITE_ACCESS_VIOLATION, "Memory access violation writing address. Address is either invalid or inaccessible.");
		STR(XE_ERROR_SCAN_NOT_FOUND, "Scanner could not locate a matching entry.");
		STR(XE_ERROR_ENTRY_NOT_FOUND, "The entry searched for could not be found.");
		STR(XE_ERROR_HOOK_UNSUPPORTED, "The specified target function cannot be hooked.");
		STR(XE_ERROR_HOOK_ALREADY_CREATED, "The hook for the specified target function is already created.");
		STR(XE_ERROR_HOOK_NOT_EXECUTABLE, "The specified pointer is invalid. It points the address of non-allocated and/or non-executable region.");
		STR(XE_ERROR_HOOK_NOT_INIT, "The hooking library is not initialized yet, or already uninitialized.");
		STR(XE_ERROR_HOOK_NOT_CREATED, "The hook for the specified target function is not created yet.");
		STR(XE_ERROR_INDEX_OUT_OF_RANGE, "The index was out of range.");
	default:
		return FALSE;
	}

	return TRUE;
}
#pragma warning(pop)