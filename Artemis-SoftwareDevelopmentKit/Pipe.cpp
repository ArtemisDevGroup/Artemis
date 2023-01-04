//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Astrea0014.												|
// @Astrea0014: https://github.com/Astrea0014											|
//-------------------------------------------------------------------------------------->

#include "Pipe.h"
#include "Exceptions.h"

#include <Windows.h>

namespace Artemis {
	IPipeObject::IPipeObject() : hPipe(nullptr) { szName[0] = '\0'; }

	void IPipeObject::Open(
		_In_z_ LPCSTR lpPipeName,
		_In_ PipeAccess nPipeAccess
	) {
		strcpy_s(szName, lpPipeName);
		hPipe = _Open(lpPipeName, nPipeAccess);
	}

	void IPipeObject::Read(
		_Out_writes_bytes_(dwSize) LPVOID lpBuffer,
		_In_range_(0, MAX_MESSAGE) DWORD dwSize
	) const {
		CONTEXT_BEGIN;

		if (!ReadFile(
			hPipe,
			lpBuffer,
			dwSize,
			nullptr,
			nullptr
		)) throw WindowsApiException("ReadFile");

		CONTEXT_END;
	}

	void IPipeObject::Write(
		_In_reads_bytes_(dwSize) LPCVOID lpBuffer, 
		_In_range_(0, MAX_MESSAGE) DWORD dwSize
	) const {
		CONTEXT_BEGIN;

		if (!WriteFile(
			hPipe,
			lpBuffer,
			dwSize,
			nullptr,
			nullptr
		)) throw WindowsApiException("WriteFile");

		CONTEXT_END;
	}

	_Check_return_ _Ret_z_ LPCSTR IPipeObject::GetPipeName() const { return szName; }
	_Check_return_ _Ret_maybenull_ HANDLE IPipeObject::GetPipeHandle() const { return hPipe; }

	void IPipeObject::Release() { if (hPipe && hPipe != INVALID_HANDLE_VALUE) CloseHandle(hPipe); }
	IPipeObject::~IPipeObject() { Release(); }

	PipeServer::PipeServer() : IPipeObject() {}

	HANDLE PipeServer::_Open(
		_In_z_ LPCSTR lpPipeName,
		_In_ PipeAccess nPipeAccess
	) {
		CONTEXT_BEGIN;

		DWORD dwPipeAccess = 0;
		switch (nPipeAccess) {
		case PipeAccess::Inbound:
			dwPipeAccess = PIPE_ACCESS_INBOUND;
			break;

		case PipeAccess::Outbound:
			dwPipeAccess = PIPE_ACCESS_OUTBOUND;
			break;

		case PipeAccess::Duplex:
			dwPipeAccess = PIPE_ACCESS_DUPLEX;
			break;

		default:
			throw ParameterException("nPipeAccess");
		}

		HANDLE hPipe = CreateNamedPipeA(
			lpPipeName,
			dwPipeAccess,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_ACCEPT_REMOTE_CLIENTS,
			PIPE_UNLIMITED_INSTANCES,
			MAX_MESSAGE,
			MAX_MESSAGE,
			INFINITE,
			nullptr
		);

		if (hPipe == INVALID_HANDLE_VALUE) throw WindowsApiException("CreateNamedPipeA");

		CONTEXT_END;
		return hPipe;
	}

	void PipeServer::WaitForConnection() {
		CONTEXT_BEGIN;

		if (!ConnectNamedPipe(
			GetPipeHandle(),
			nullptr
		) && GetLastError() != ERROR_PIPE_CONNECTED)
			throw WindowsApiException("ConnectNamedPipe");

		CONTEXT_END;
	}

	PipeClient::PipeClient() : IPipeObject() {}

	HANDLE PipeClient::_Open(
		_In_z_ LPCSTR lpPipeName,
		_In_ PipeAccess nPipeAccess
	) {
		CONTEXT_BEGIN;

		HANDLE hPipe = CreateFileA(
			lpPipeName,
			nPipeAccess == PipeAccess::Outbound ? GENERIC_WRITE :
			(nPipeAccess == PipeAccess::Inbound ? GENERIC_READ :
				(nPipeAccess == PipeAccess::Duplex ? GENERIC_READ | GENERIC_WRITE : 0)),
			0,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);

		if (hPipe == INVALID_HANDLE_VALUE) throw WindowsApiException("CreateFileA");

		CONTEXT_END;
		return hPipe;
	}
}