#include "PipeApi.h"

#include <Windows.h>
#include <stdio.h>

BOOL XECreatePipe(
	_Out_ LPPIPE lpPipe,
	_In_ DWORD dwPipeSize
) {
	if (!lpPipe) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (lpPipe->bActive == TRUE) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	lpPipe->bActive = FALSE;
	lpPipe->dwSize = dwPipeSize;

	if (!CreatePipe(
		&lpPipe->hRead,
		&lpPipe->hWrite,
		NULL,
		lpPipe->dwSize
	)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	lpPipe->bActive = TRUE;
	return XEResetLastError();
}

BOOL XEClosePipe(
	_In_ LPPIPE lpPipe
) {
	if (!lpPipe) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	if (lpPipe->bActive) {
		CloseHandle(lpPipe->hRead);
		CloseHandle(lpPipe->hWrite);
	}

	return XEResetLastError();
}

BOOL XEReadPipe(
	_In_ LPPIPE lpPipe,
	_Out_writes_(dwSize) LPVOID lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpPipe) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (!lpPipe->bActive) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	if (!ReadFile(
		lpPipe->hRead,
		lpBuffer,
		dwSize,
		NULL,
		NULL
	)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	return XEResetLastError();
}

BOOL XEWritePipe(
	_In_ LPPIPE lpPipe,
	_In_ LPVOID lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpPipe) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (!lpPipe->bActive) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	if (!WriteFile(
		lpPipe->hRead,
		lpBuffer,
		dwSize,
		NULL,
		NULL
	)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	return XEResetLastError();
}

BOOL XECreateNamedPipe(
	_Out_ LPNAMED_PIPE lpPipe,
	_In_z_ LPCSTR lpName,
	_In_ PIPE_ACCESS PipeAccess,
	_In_ PIPE_TYPE PipeType,
	_In_ DWORD dwBufferSize
) {
	if (!lpPipe || !lpName) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (lpPipe->bActive == TRUE) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	lpPipe->bActive = FALSE;
	lpPipe->dwSize = dwBufferSize;

	sprintf_s(lpPipe->szPipeName, MAX_PATH, "\\\\.\\pipe\\%s", lpName);

	DWORD dwOpenMode;
	switch (PipeAccess) {
	case PA_INBOUND:
		dwOpenMode = PIPE_ACCESS_INBOUND;
		break;
	case PA_OUTBOUND:
		dwOpenMode = PIPE_ACCESS_OUTBOUND;
		break;
	case PA_DUPLEX:
		dwOpenMode = PIPE_ACCESS_DUPLEX;
		break;
	default:
		return XESetLastError(XE_ERROR_PARAMETER_INVALID);
	}

	DWORD dwPipeMode;
	switch (PipeType) {
	case PT_BYTE:
		dwPipeMode = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT;
		break;
	case PT_MESSAGE:
		dwPipeMode = PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT;
		break;
	default:
		return XESetLastError(XE_ERROR_PARAMETER_INVALID);
	}

	lpPipe->hPipe = CreateNamedPipeA(
		lpPipe->szPipeName,
		dwOpenMode,
		dwPipeMode,
		PIPE_UNLIMITED_INSTANCES,
		dwBufferSize,
		dwBufferSize,
		INFINITE,
		NULL
	);

	if (lpPipe->hPipe == INVALID_HANDLE_VALUE) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	lpPipe->bActive = TRUE;
	return XEResetLastError();
}

BOOL XEOpenNamedPipe(
	_Out_ LPNAMED_PIPE lpPipe,
	_In_z_ LPCSTR lpName,
	_In_ PIPE_ACCESS PipeAccess,
	_In_ PIPE_TYPE PipeType
) {
	if (!lpPipe || !lpName) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (lpPipe->bActive == TRUE) return XESetLastError(XE_ERROR_PARAMETER_INVALID);
	
	lpPipe->bActive = FALSE;
	lpPipe->dwSize = -1;

	sprintf_s(lpPipe->szPipeName, MAX_PATH, "\\\\.\\pipe\\%s", lpName);

	DWORD dwDesiredAccess;
	switch (PipeAccess) {
	case PA_INBOUND:
		dwDesiredAccess = GENERIC_READ;
		break;
	case PA_OUTBOUND:
		dwDesiredAccess = GENERIC_WRITE;
		break;
	case PA_DUPLEX:
		dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
		break;
	default:
		return XESetLastError(XE_ERROR_PARAMETER_INVALID);
	}

	if (dwDesiredAccess == GENERIC_READ && PipeType == PT_MESSAGE) dwDesiredAccess |= FILE_WRITE_ATTRIBUTES;

	lpPipe->hPipe = CreateFileA(
		lpPipe->szPipeName,
		dwDesiredAccess,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (lpPipe->hPipe == INVALID_HANDLE_VALUE) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	return XEResetLastError();
}

BOOL XECloseNamedPipe(
	_In_ LPNAMED_PIPE lpPipe
) {
	if (!lpPipe) return XESetLastError(XE_ERROR_PARAMETER_NULL);

	if (lpPipe->bActive) {
		CloseHandle(lpPipe->hPipe);
		lpPipe->bActive = FALSE;
	}

	return XEResetLastError();
}

BOOL XEReadNamedPipe(
	_In_ LPNAMED_PIPE lpPipe,
	_Out_writes_(dwSize) LPVOID lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpPipe) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (!lpPipe->bActive) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	if (!ReadFile(
		lpPipe->hPipe,
		lpBuffer,
		dwSize,
		NULL,
		NULL
	)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	return XEResetLastError();
}

BOOL XEWriteNamedPipe(
	_In_ LPNAMED_PIPE lpPipe,
	_In_ LPCSTR lpBuffer,
	_In_ DWORD dwSize
) {
	if (!lpPipe) return XESetLastError(XE_ERROR_PARAMETER_NULL);
	if (!lpPipe->bActive) return XESetLastError(XE_ERROR_PARAMETER_INVALID);

	if (!WriteFile(
		lpPipe->hPipe,
		lpBuffer,
		dwSize,
		NULL,
		NULL
	)) return XESetLastError(XE_ERROR_WINAPI_FAIL);

	return XEResetLastError();
}