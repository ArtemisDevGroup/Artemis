#ifndef __PIPE_H__
#define __PIPE_H__

#include "Definitions.h"
#include "Interfaces.h"

namespace Artemis {
	enum class PipeAccess { Inbound, Outbound, Duplex };

	class IPipeObject : public IDisposable {
		HANDLE hPipe;
		CHAR szName[MAX_NAME];

	public:
		IPipeObject();
		IPipeObject(
			_In_z_ LPCSTR lpPipeName,
			_In_ HANDLE hPipe,
			_In_ BOOL bIsServer
		);

		void Read(
			_Out_writes_bytes_(dwSize) LPVOID lpBuffer,
			_In_range_(0, MAX_MESSAGE) DWORD dwSize
		) const;

		void Write(
			_In_reads_bytes_(dwSize) LPCVOID lpBuffer,
			_In_range_(0, MAX_MESSAGE) DWORD dwSize
		) const;

		_Check_return_ _Ret_z_ LPCSTR GetPipeName() const;
		_Check_return_ _Ret_maybenull_ HANDLE GetPipeHandle() const;

		void Release();

		~IPipeObject();
	};

	class PipeServer : public IPipeObject {
	public:
		PipeServer();
		PipeServer(
			_In_z_ LPCSTR lpPipeName,
			_In_ PipeAccess nPipeAccess
		);

		void WaitForConnection();
	};

	class PipeClient : public IPipeObject {
	public:
		PipeClient();
		PipeClient(
			_In_z_ LPCSTR lpPipeName,
			_In_ PipeAccess nPipeAccess
		);
	};
}

#endif // !__PIPE_H__