#include "Pipe.h"
#include "Exceptions.h"

#include <Windows.h>

namespace Artemis {
	IPipeObject::IPipeObject() : hPipe(nullptr) { szName[0] = '\0'; }

	IPipeObject::IPipeObject(
		_In_z_ LPCSTR lpPipeName,
		_In_ HANDLE hPipe,
		_In_ BOOL bIsServer
	) : hPipe(hPipe) {
		if (hPipe == INVALID_HANDLE_VALUE) throw WindowsApiException(bIsServer ? "CreateNamedPipeA" : "CreateFileA");

		strcpy_s(szName, lpPipeName);
	}

	void IPipeObject::Read(
		_Out_writes_bytes_(dwSize) LPVOID lpBuffer,
		_In_range_(0, MAX_MESSAGE) DWORD dwSize
	) {
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
	) {
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
	
	PipeServer::PipeServer(
		_In_z_ LPCSTR lpPipeName,
		_In_ PipeAccess nPipeAccess
	) : IPipeObject(
		lpPipeName,
		CreateNamedPipeA(
			lpPipeName,
			nPipeAccess == PipeAccess::Outbound ? PIPE_ACCESS_OUTBOUND :
			(nPipeAccess == PipeAccess::Inbound ? PIPE_ACCESS_INBOUND :
			(nPipeAccess == PipeAccess::Duplex ? PIPE_ACCESS_DUPLEX : 0)),
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_ACCEPT_REMOTE_CLIENTS,
			PIPE_UNLIMITED_INSTANCES,
			MAX_MESSAGE,
			MAX_MESSAGE,
			INFINITE,
			nullptr),
		TRUE
	) {}

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

	PipeClient::PipeClient(
		_In_z_ LPCSTR lpPipeName,
		_In_ PipeAccess nPipeAccess
	) : IPipeObject(
		lpPipeName,
		CreateFileA(
			lpPipeName,
			nPipeAccess == PipeAccess::Outbound ? GENERIC_WRITE :
			(nPipeAccess == PipeAccess::Inbound ? GENERIC_READ :
			(nPipeAccess == PipeAccess::Duplex ? GENERIC_READ | GENERIC_WRITE : 0)),
			0,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		),
		FALSE
	) {}
}