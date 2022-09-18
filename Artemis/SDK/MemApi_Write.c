#include "MemApi_Write.h"
#include "MemApi_Read.h"

BOOL XEWrite(
	_In_ ADDRESS uAddress,
	_In_ LPCVOID lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpBuffer) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	__try {
		for (DWORD i = 0; i < dwSize; i++) {
			*(LPBYTE)(uAddress + i) = *(LPCBYTE)((ADDRESS)lpBuffer + i);
		}

		return XEResetLastError();
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return XESetLastError(XE_ERROR_WRITE_ACCESS_VIOLATION);
	}
}

BOOL XEWriteU(
	_In_ ADDRESS uAddress,
	_In_ LPCVOID lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpBuffer) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	for (DWORD i = 0; i < dwSize; i++) {
		*(LPBYTE)(uAddress + i) = *(LPCBYTE)((ADDRESS)lpBuffer + i);
	}

	return XEResetLastError();
}

BOOL XEWritePtr(
	_In_ ADDRESS uAddress,
	_In_ LPOFFSET lpOffsets,
	_In_ LPCVOID lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpOffsets || !lpBuffer) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	if (!XEGetAddress(uAddress, lpOffsets, &uAddress)) return XESetLastError(XE_ERROR_READ_ACCESS_VIOLATION);

	if (!XEWrite(uAddress, lpBuffer, dwSize)) return XESetLastError(XE_ERROR_WRITE_ACCESS_VIOLATION);

	return XEResetLastError();
}

BOOL XEWriteStringA(
	_In_ ADDRESS uAddress,
	_In_z_ LPCSTR lpString,
	_In_ BOOL bWriteNullTerm
) {
	if (!lpString) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	if (!XEWrite(uAddress, lpString, strlen(lpString) + bWriteNullTerm ? 1 : 0)) return XESetLastError(XE_ERROR_WRITE_ACCESS_VIOLATION);

	return XEResetLastError();
}

BOOL XEWriteStringW(
	_In_ ADDRESS uAddress,
	_In_z_ LPCWSTR lpString,
	_In_ BOOL bWriteNullTerm
) {
	if (!lpString) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	if (!XEWrite(uAddress, lpString, (lstrlenW(lpString) + bWriteNullTerm ? 1 : 0) * 2)) return XESetLastError(XE_ERROR_WRITE_ACCESS_VIOLATION);

	return XEResetLastError();
}

BOOL XEWriteBytes(
	_In_ ADDRESS uAddress,
	_In_ LPCBYTE lpBuffer,
	_In_ DWORD dwSize,
	_In_ DWORD dwNop
) {
	if (!lpBuffer) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	if (!XEWrite(uAddress, lpBuffer, dwSize)) return XESetLastError(XE_ERROR_WRITE_ACCESS_VIOLATION);
	if (dwNop)
		if (!XESetBytes(uAddress + dwSize, 0x90, dwNop)) return XESetLastError(XE_ERROR_WRITE_ACCESS_VIOLATION);

	return XEResetLastError();
}

BOOL XESetBytes(
	_In_ ADDRESS uAddress,
	_In_ BYTE uValue,
	_In_ DWORD dwCount
) {
	__try {
		for (DWORD i = 0; i < dwCount; i++) {
			*((LPBYTE)uAddress + i) = uValue;
		}

		return XEResetLastError();
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return XESetLastError(XE_ERROR_WRITE_ACCESS_VIOLATION);
	}
}