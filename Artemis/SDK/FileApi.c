#include "FileApi.h"

#include <stdio.h>

BOOL XECreateFile(
	_Out_ LPFILE_ENTRY lpFileEntry,
	_In_z_ LPCSTR lpFilePath,
	_In_ FILE_ACCESS dwFileAccess,
	_In_ SHARE_MODE dwShareMode,
	_In_ CREATION_DISPOSITION dwCreationDisposition,
	_In_ FILE_ATTRIBUTE dwFileAttribute
) {
	if (!lpFileEntry || !lpFilePath) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	lpFileEntry->hFile = CreateFileA(
		lpFilePath,
		(DWORD)dwFileAccess,
		(DWORD)dwShareMode,
		NULL,
		(DWORD)dwCreationDisposition,
		(DWORD)dwFileAttribute,
		NULL
	);

	if (!lpFileEntry->hFile) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	strcpy_s(lpFileEntry->szFilePath, sizeof(lpFileEntry->szFilePath), lpFilePath);
	lpFileEntry->dwFileAccess = dwFileAccess;
	lpFileEntry->dwShareMode = dwShareMode;
	lpFileEntry->dwCreationDisposition = dwCreationDisposition;
	lpFileEntry->dwFileAttribute = dwFileAttribute;

	return XEResetLastError();
}

BOOL XEReadFile(
	_In_ LPFILE_ENTRY lpFileEntry,
	_Out_writes_(dwSize) LPVOID lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpFileEntry) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (!lpFileEntry->bActive) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	if (!ReadFile(
		lpFileEntry->hFile,
		lpBuffer,
		dwSize,
		NULL,
		NULL
	)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	return XEResetLastError();
}

BOOL XEWriteFile(
	_In_ LPFILE_ENTRY lpFileEntry,
	_In_ LPCVOID lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpFileEntry) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (!lpFileEntry->bActive) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	if (!WriteFile(
		lpFileEntry->hFile,
		lpBuffer,
		dwSize,
		NULL,
		NULL
	)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	return XEResetLastError();
}

BOOL XEPrintFile(
	_In_ LPFILE_ENTRY lpFileEntry,
	_In_z_ _Printf_format_string_ LPCSTR lpFormat,
	...
) {
	if (!lpFileEntry) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (!lpFileEntry->bActive) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	LPSTR lpString = calloc(strlen(lpFormat) + 0x64, sizeof(CHAR));
	if (!lpString) return XESetLastError(XE_ERROR_MEMORY_FULL);

	va_list va;
	va_start(va, lpFormat);
	vsprintf_s(lpString, strlen(lpFormat) + 0x64, lpFormat, va);
	va_end(va);
	
	BOOL b = WriteFile(lpFileEntry->hFile, lpString, (DWORD)strlen(lpString), NULL, NULL);

	free(lpString);

	if (!b) return XESetLastError(XE_ERROR_WINAPI_FAIL);
	else return XEResetLastError();
}

BOOL XEPrintFileV(
	_In_ LPFILE_ENTRY lpFileEntry,
	_In_z_ _Printf_format_string_ LPCSTR lpFormat,
	_In_ va_list lpArgs
) {
	if (!lpFileEntry) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (!lpFileEntry->bActive) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	LPSTR lpString = calloc(strlen(lpFormat) + 0x64, sizeof(CHAR));
	if (!lpString) return XESetLastError(XE_ERROR_MEMORY_FULL);

	vsprintf_s(lpString, strlen(lpFormat) + 0x64, lpFormat, lpArgs);

	BOOL b = WriteFile(lpFileEntry->hFile, lpString, (DWORD)strlen(lpString), NULL, NULL);

	free(lpString);

	if (!b) return XESetLastError(XE_ERROR_WINAPI_FAIL);
	else return XEResetLastError();
}

BOOL XEDeleteFile(
	_In_ LPFILE_ENTRY lpFileEntry
) {
	if (!lpFileEntry) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (!lpFileEntry->bActive) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	if (!DeleteFileA(lpFileEntry->szFilePath)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	return XEResetLastError();
}

BOOL XECloseFile(
	_In_ LPFILE_ENTRY lpFileEntry
) {
	if (!lpFileEntry) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (!lpFileEntry->bActive) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	if (!CloseHandle(lpFileEntry->hFile)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	return XEResetLastError();
}