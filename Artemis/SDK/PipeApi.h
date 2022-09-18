#ifndef __PIPEAPI_H__
#define __PIPEAPI_H__

#include "Def.h"
#include "ErrorApi.h"

#include <Windows.h>

/// <summary>
/// Represents an anonymous data pipe.
/// </summary>
typedef struct _PIPE {
	/// <summary>
	/// A handle to the read end of the pipe.
	/// </summary>
	HANDLE hRead;

	/// <summary>
	/// A handle to the write end of the pipe.
	/// </summary>
	HANDLE hWrite;

	/// <summary>
	/// The buffer size of the pipe.
	/// </summary>
	DWORD dwSize;

	/// <summary>
	/// A bool indicating whether the handles are open or not.
	/// </summary>
	BOOL bActive;
} PIPE, * LPPIPE;

/// <summary>
/// Represents a named pipe.
/// </summary>
typedef struct _NAMED_PIPE {
	/// <summary>
	/// The full name/path of the pipe.
	/// </summary>
	CHAR szPipeName[MAX_PATH];

	/// <summary>
	/// The pipe handle.
	/// </summary>
	HANDLE hPipe;

	/// <summary>
	/// The buffer size of the pipe.
	/// </summary>
	DWORD dwSize;

	/// <summary>
	/// A bool indicating whether the handle is open or not.
	/// </summary>
	BOOL bActive;
} NAMED_PIPE, * LPNAMED_PIPE;

/// <summary>
/// Indicates which permissions the pipe has.
/// </summary>
typedef enum _PIPE_ACCESS {
	/// <summary>
	/// Gives the pipe handle read access.
	/// </summary>
	PA_INBOUND = 1,

	/// <summary>
	/// Gives the pipe handle write access.
	/// </summary>
	PA_OUTBOUND = 2,

	/// <summary>
	/// Gives the pipe handle read and write access.
	/// </summary>
	PA_DUPLEX = 3
} PIPE_ACCESS;

/// <summary>
/// Indicates which type the pipe should be.
/// </summary>
typedef enum _PIPE_TYPE {
	/// <summary>
	/// Opens the pipe in byte mode. This means that the pipe data is read and written as a stream of bytes.
	/// </summary>
	PT_BYTE = 1,

	/// <summary>
	/// Opens the pipe in message mode. This means that the pipe data is read and written as a message unit. This means that different write operations produce different message units.
	/// </summary>
	PT_MESSAGE = 2
} PIPE_TYPE;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/// <summary>
	/// Creates an anonymous pipe.
	/// </summary>
	/// <param name="lpPipe">- A pointer to a pipe object to receive information about the pipe.</param>
	/// <param name="dwPipeSize">- The size of the pipe buffer.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XECreatePipe(
		_Out_ LPPIPE lpPipe,
		_In_ DWORD dwPipeSize
	);

	/// <summary>
	/// Closes an anonymous pipe.
	/// </summary>
	/// <param name="lpPipe">- A pointer to a pipe object containing the information about the pipe to close.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	BOOL XEClosePipe(
		_In_ LPPIPE lpPipe
	);

	/// <summary>
	/// Reads from an anonymous pipe.
	/// </summary>
	/// <param name="lpPipe">- A pointer to a pipe object containing the information about the pipe to read.</param>
	/// <param name="lpBuffer">- A pointer to a buffer receiving the read data.</param>
	/// <param name="dwSize">- The size of the buffer.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XEReadPipe(
		_In_ LPPIPE lpPipe,
		_Out_writes_(dwSize) LPVOID lpBuffer,
		_In_ DWORD dwSize
	);

	/// <summary>
	/// Writes to an anonymous pipe.
	/// </summary>
	/// <param name="lpPipe">- A pointer to a pipe object containing the information about the pipe to write to.</param>
	/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
	/// <param name="dwSize">- The size of the buffer.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XEWritePipe(
		_In_ LPPIPE lpPipe,
		_In_ LPVOID lpBuffer,
		_In_ DWORD dwSize
	);

	/// <summary>
	/// Creates a named pipe.
	/// </summary>
	/// <param name="lpPipe">- A pointer to a NAMED_PIPE object to receive information about the pipe.</param>
	/// <param name="lpName">- The name of the pipe to be created.</param>
	/// <param name="PipeAccess">- The access mode of the pipe to be created.</param>
	/// <param name="PipeType">- The type of the pipe to be created.</param>
	/// <param name="dwBufferSize">- The size of the pipe read and write buffers.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XECreateNamedPipe(
		_Out_ LPNAMED_PIPE lpPipe,
		_In_z_ LPCSTR lpName,
		_In_ PIPE_ACCESS PipeAccess,
		_In_ PIPE_TYPE PipeType,
		_In_ DWORD dwBufferSize
	);

	/// <summary>
	/// Opens an existing named pipe.
	/// </summary>
	/// <param name="lpPipe">- A pointer to a NAMED_PIPE object to receive information about the pipe.</param>
	/// <param name="lpName">- The name of the pipe to be created.</param>
	/// <param name="PipeAccess">- The access mode of the pipe to be created.</param>
	/// <param name="PipeType">- The type of the pipe to be created.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XEOpenNamedPipe(
		_Out_ LPNAMED_PIPE lpPipe,
		_In_z_ LPCSTR lpName,
		_In_ PIPE_ACCESS PipeAccess,
		_In_ PIPE_TYPE PipeType
	);

	/// <summary>
	/// Closes an open named pipe.
	/// </summary>
	/// <param name="lpPipe">- A pointer to a NAMED_PIPE object containing the information about the pipe to close.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	BOOL XECloseNamedPipe(
		_In_ LPNAMED_PIPE lpPipe
	);

	/// <summary>
	/// Reads data from a named pipe.
	/// </summary>
	/// <param name="lpPipe">- A pointer to a NAMED_PIPE object containing the information about the pipe to read from.</param>
	/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XEReadNamedPipe(
		_In_ LPNAMED_PIPE lpPipe,
		_Out_writes_(dwSize) LPVOID lpBuffer,
		_In_ DWORD dwSize
	);

	/// <summary>
	/// Reads data from a named pipe.
	/// </summary>
	/// <param name="lpPipe">- A pointer to a NAMED_PIPE object containing the information about the pipe to write to.</param>
	/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
	/// <param name="dwSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XEWriteNamedPipe(
		_In_ LPNAMED_PIPE lpPipe,
		_In_ LPCSTR lpBuffer,
		_In_ DWORD dwSize
	);

#ifdef __cplusplus
}

namespace api {
	/// <summary>
	/// A class for creating a named pipe instance.
	/// </summary>
	class PipeServer {
		NAMED_PIPE pipe;

	public:
		PipeServer() {
			pipe.bActive = FALSE;
		}

		/// <summary>
		/// Creates an active pipe server object with the following properties.
		/// </summary>
		/// <param name="lpName">- The name of the pipe to be created.</param>
		/// <param name="PipeAccess">- The access mode of the pipe to be created.</param>
		/// <param name="PipeType">- The type of the pipe to be created.</param>
		/// <param name="dwBufferSize">- The size of the pipe read and write buffers.</param>
		/// <exception cref="ParameterNullException"/>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="WinApiException"/>
		PipeServer(
			_In_ LPCSTR lpName,
			_In_ PIPE_ACCESS PipeAccess,
			_In_ PIPE_TYPE PipeType,
			_In_ DWORD dwBufferSize
		) {
			if (!lpName) throw ParameterNullException("lpName");

			if (!IN_RANGE(PipeAccess, PA_INBOUND, PA_DUPLEX)) throw ParameterInvalidException("PipeAccess");
			if (PipeType != PT_BYTE && PipeType != PT_MESSAGE) throw ParameterInvalidException("PipeType");

			if (!XECreateNamedPipe(
				&pipe,
				lpName,
				PipeAccess,
				PipeType,
				dwBufferSize
			)) throw WinApiException("CreateNamedPipe");
		}

		~PipeServer() { XECloseNamedPipe(&pipe); }

		/// <summary>
		/// Reads data from the pipe.
		/// </summary>
		/// <typeparam name="T">The buffer type.</typeparam>
		/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
		/// <exception cref="ParameterNullException"/>
		/// <exception cref="WinApiException"/>
		template<typename T>
		void Read(T* lpBuffer) {
			if (!lpBuffer) throw ParameterNullException("lpBuffer");

			if (!XEReadNamedPipe(&pipe, lpBuffer, sizeof(T))) throw WinApiException("ReadFile");
		}

		/// <summary>
		/// Reads data from the pipe.
		/// </summary>
		/// <param name="lpBuffer">- A reference to a buffer to receive the read data.</param>
		/// <exception cref="ParameterNullException"/>
		/// <exception cref="WinApiException"/>
		template<SIZE_T uSize>
		void Read(BYTE(&lpBuffer)[uSize]) {
			if (!lpBuffer) throw ParameterNullException("lpBuffer");

			if (!XEReadNamedPipe(&pipe, lpBuffer, sizeof(T))) throw WinApiException("ReadFile");
		}

		/// <summary>
		/// Writes data to the pipe.
		/// </summary>
		/// <typeparam name="T">The buffer type.</typeparam>
		/// <param name="Buffer">- A buffer containing the data to write.</param>
		/// <exception cref="WinApiException"/>
		template<typename T>
		void Write(T Buffer) {
			if (!XEWriteNamedPipe(&pipe, &Buffer, sizeof(T))) throw WinApiException("WriteFile");
		}

		/// <summary>
		/// Writes data to the pipe.
		/// </summary>
		/// <typeparam name="T">The buffer type.</typeparam>
		/// <param name="lpBuffer">- A pointer to the buffer containing the data to write.</param>
		/// <param name="dwSize">- The size of the buffer in bytes.</param>
		/// <exception cref="ParameterNullException"/>
		/// <exception cref="WinApiException"/>
		template<typename T>
		void Write(T* lpBuffer, DWORD dwSize) {
			if (!lpBuffer) throw ParameterNullException("lpBuffer");

			if (!XEWriteNamedPipe(&pipe, lpBuffer, dwSize)) throw WinApiException("WriteFile");
		}

		/// <summary>
		/// Writes data to the pipe.
		/// </summary>
		/// <typeparam name="T">The buffer type.</typeparam>
		/// <param name="lpBuffer">- A reference to the buffer containing the data to write.</param>
		/// <exception cref="ParameterNullException"/>
		/// <exception cref="WinApiException"/>
		template<typename T, SIZE_T uSize>
		void Write(T(&lpBuffer)[uSize]) {
			if (!lpBuffer) throw ParameterNullException("lpBuffer");

			if (!XEWriteNamedPipe(&pipe, lpBuffer, dwSize)) throw WinApiException("WriteFile");
		}

		/// <summary>
		/// Waits for a pipe client to connect to the pipe.
		/// </summary>
		/// <exception cref="WinApiException"/>
		void Connect() {
			if (!ConnectNamedPipe(pipe.hPipe, nullptr)) throw WinApiException("ConnectNamedPipe");
		}

		/// <summary>
		/// Closes the pipe.
		/// </summary>
		void Close() { XECloseNamedPipe(&pipe); }
	};

	/// <summary>
	/// A class for interacting with an already created named pipe instance.
	/// </summary>
	class PipeClient {
		NAMED_PIPE pipe;

	public:
		PipeClient() {
			pipe.bActive = FALSE;
		}

		/// <summary>
		/// Connects to a named pipe.
		/// </summary>
		/// <param name="lpName">- The name of the pipe to be created.</param>
		/// <param name="PipeAccess">- The access mode of the pipe to be created.</param>
		/// <param name="PipeType">- The type of the pipe to be created.</param>
		/// <exception cref="ParameterNullException"/>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="WinApiException"/>
		PipeClient(
			_In_z_ LPCSTR lpName,
			_In_ PIPE_ACCESS PipeAccess,
			_In_ PIPE_TYPE PipeType
		) {
			if (!lpName) throw ParameterNullException("lpName");

			if (!IN_RANGE(PipeAccess, PA_INBOUND, PA_DUPLEX)) throw ParameterInvalidException("PipeAccess");
			if (PipeType != PT_BYTE && PipeType != PT_MESSAGE) throw ParameterInvalidException("PipeType");

			if (!XEOpenNamedPipe(&pipe, lpName, PipeAccess, PipeType)) throw WinApiException("CreateFile");
		}

		/// <summary>
		/// Reads data from the pipe.
		/// </summary>
		/// <typeparam name="T">The buffer type.</typeparam>
		/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
		/// <exception cref="ParameterNullException"/>
		/// <exception cref="WinApiException"/>
		template<typename T>
		void Read(T* lpBuffer) {
			if (!lpBuffer) throw ParameterNullException("lpBuffer");

			if (!XEReadNamedPipe(&pipe, lpBuffer, sizeof(T))) throw WinApiException("ReadFile");
		}

		/// <summary>
		/// Reads data from the pipe.
		/// </summary>
		/// <param name="lpBuffer">- A reference to a buffer to receive the read data.</param>
		/// <exception cref="ParameterNullException"/>
		/// <exception cref="WinApiException"/>
		template<SIZE_T uSize>
		void Read(BYTE(&lpBuffer)[uSize]) {
			if (!lpBuffer) throw ParameterNullException("lpBuffer");

			if (!XEReadNamedPipe(&pipe, lpBuffer, sizeof(T))) throw WinApiException("ReadFile");
		}

		/// <summary>
		/// Writes data to the pipe.
		/// </summary>
		/// <typeparam name="T">The buffer type.</typeparam>
		/// <param name="Buffer">- A buffer containing the data to write.</param>
		/// <exception cref="WinApiException"/>
		template<typename T>
		void Write(T Buffer) {
			if (!XEWriteNamedPipe(&pipe, &Buffer, sizeof(T))) throw WinApiException("WriteFile");
		}

		/// <summary>
		/// Writes data to the pipe.
		/// </summary>
		/// <typeparam name="T">The buffer type.</typeparam>
		/// <param name="lpBuffer">- A pointer to the buffer containing the data to write.</param>
		/// <param name="dwSize">- The size of the buffer in bytes.</param>
		/// <exception cref="ParameterNullException"/>
		/// <exception cref="WinApiException"/>
		template<typename T>
		void Write(T* lpBuffer, DWORD dwSize) {
			if (!lpBuffer) throw ParameterNullException("lpBuffer");

			if (!XEWriteNamedPipe(&pipe, lpBuffer, dwSize)) throw WinApiException("WriteFile");
		}

		/// <summary>
		/// Writes data to the pipe.
		/// </summary>
		/// <typeparam name="T">The buffer type.</typeparam>
		/// <param name="lpBuffer">- A reference to the buffer containing the data to write.</param>
		/// <exception cref="ParameterNullException"/>
		/// <exception cref="WinApiException"/>
		template<typename T, SIZE_T uSize>
		void Write(T(&lpBuffer)[uSize]) {
			if (!lpBuffer) throw ParameterNullException("lpBuffer");

			if (!XEWriteNamedPipe(&pipe, lpBuffer, dwSize)) throw WinApiException("WriteFile");
		}

		/// <summary>
		/// Closes the pipe.
		/// </summary>
		void Close() { XECloseNamedPipe(&pipe); }
	};
}
#endif // __cplusplus

#endif // !__PIPEAPI_H__