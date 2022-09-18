#include "MemApi_Read.h"

BOOL XERead(
	_In_ ADDRESS uAddress,
	_Out_writes_(dwSize) LPVOID lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpBuffer) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	__try {
		for (SIZE_T i = 0; i < dwSize; i++) {
			*((LPBYTE)lpBuffer + i) = *((LPBYTE)(uAddress + i));
		}

		return XEResetLastError();
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return XESetLastError(XE_ERROR_READ_ACCESS_VIOLATION);
	}
}

BOOL XEReadU(
	_In_ ADDRESS uAddress,
	_Out_writes_(dwSize) LPVOID lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpBuffer) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	for (SIZE_T i = 0; i < dwSize; i++) {
		*((LPBYTE)lpBuffer + i) = *((LPBYTE)(uAddress + i));
	}

	return XEResetLastError();
}

BOOL XEGetAddress(
	_In_ ADDRESS uAddress,
	_In_ LPOFFSET lpOffsets,
	_Out_ LPADDRESS lpAddress
) {
	if (!lpOffsets || !lpAddress) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	for (DWORD i = 0; i < lpOffsets->dwCount; i++) {
		if (!XERead(
			uAddress,
			&uAddress,
			sizeof(uAddress)
		)) return XESetLastError(XE_ERROR_READ_ACCESS_VIOLATION);

		uAddress += lpOffsets->szOffsets[i];
	}

	*lpAddress = uAddress;
	return XEResetLastError();
}

BOOL XEReadPtr(
	_In_ ADDRESS uAddress,
	_In_ LPOFFSET lpOffsets,
	_Out_writes_(dwSize) LPVOID lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpOffsets || !lpBuffer) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	if (!XEGetAddress(
		uAddress,
		lpOffsets,
		&uAddress
	)) return XESetLastError(XE_ERROR_READ_ACCESS_VIOLATION);

	if (!XERead(
		uAddress,
		lpBuffer,
		dwSize
	)) return XESetLastError(XE_ERROR_READ_ACCESS_VIOLATION);

	return XEResetLastError();
}

BOOL XEReadStringA(
	_In_ ADDRESS uAddress,
	_Out_writes_z_(dwSize) LPSTR lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpBuffer) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	if (!XERead(
		uAddress,
		lpBuffer,
		dwSize - 1
	)) return XESetLastError(XE_ERROR_READ_ACCESS_VIOLATION);

	lpBuffer[dwSize - 1] = '\0';

	return XEResetLastError();
}

BOOL XEReadStringW(
	_In_ ADDRESS uAddress,
	_Out_writes_z_(dwSize) LPWSTR lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpBuffer) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	if (!XERead(
		uAddress,
		lpBuffer,
		(dwSize - 1) * 2
	)) return XESetLastError(XE_ERROR_READ_ACCESS_VIOLATION);

	lpBuffer[dwSize - 1] = L'\0';

	return XEResetLastError();
}