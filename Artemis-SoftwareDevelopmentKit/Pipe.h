//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __PIPE_H__
#define __PIPE_H__

#include "Definitions.h"
#include "Interfaces.h"

namespace Artemis {
	/// <summary>
	/// An enum representing different pipe access levels.
	/// </summary>
	enum class PipeAccess {
		/// <summary>
		/// Inbound means that the pipe only has read access from the current instance.
		/// </summary>
		Inbound,

		/// <summary>
		/// Outbound means that the pipe only has write access from the current instance.
		/// </summary>
		Outbound,

		/// <summary>
		/// Duplex means that the pipe has both read and write access from the current instance.
		/// </summary>
		Duplex
	};

	/// <summary>
	/// An interface for generic pipe objects.
	/// </summary>
	class ARTEMIS_API IPipeObject : public IDisposable {
		HANDLE hPipe;
		CHAR szName[MAX_NAME];

		virtual HANDLE _Open(
			_In_z_ LPCSTR lpPipeName,
			_In_ PipeAccess nPipeAccess
		) = 0;

	public:
		IPipeObject();

		/// <summary>
		/// Opens a new pipe instance.
		/// </summary>
		/// <param name="lpPipeName">
		/// - The name of the pipe to open.
		/// <para>The pipe name must be formatted as such: \\.\PipeName. (With escape sequences it would be: \\\\.\\PipeName)</para>
		/// </param>
		/// <param name="nPipeAccess">- The access level of the pipe instance.</param>
		void Open(
			_In_z_ LPCSTR lpPipeName,
			_In_ PipeAccess nPipeAccess
		);

		/// <summary>
		/// Reads pending data from the pipe.
		/// </summary>
		/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
		/// <param name="dwSize">- The size of the buffer.</param>
		void Read(
			_Out_writes_bytes_(dwSize) LPVOID lpBuffer,
			_In_range_(0, MAX_MESSAGE) DWORD dwSize
		) const;

		/// <summary>
		/// Writes pending data to the pipe.
		/// </summary>
		/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
		/// <param name="dwSize">- The size of the buffer.</param>
		void Write(
			_In_reads_bytes_(dwSize) LPCVOID lpBuffer,
			_In_range_(0, MAX_MESSAGE) DWORD dwSize
		) const;

		/// <summary>
		/// Gets the name of the pipe associated with this instance.
		/// </summary>
		/// <returns>The pipe name.</returns>
		_Check_return_ _Ret_z_ LPCSTR GetPipeName() const;

		/// <summary>
		/// Gets a handle to the pipe associated with this instance.
		/// </summary>
		/// <returns>A handle to the pipe.</returns>
		_Check_return_ _Ret_maybenull_ HANDLE GetPipeHandle() const;

		/// <summary>
		/// Releases the pipe and all associated resources.
		/// </summary>
		void Release();

		~IPipeObject();
	};

	/// <summary>
	/// A class for creating and hosting a system data pipe.
	/// </summary>
	class ARTEMIS_API PipeServer : public IPipeObject {
		HANDLE _Open(
			_In_z_ LPCSTR lpPipeName,
			_In_ PipeAccess nPipeAccess
		);

	public:
		PipeServer();

		/// <summary>
		/// Waits for a client to connect to the server instance.
		/// </summary>
		void WaitForConnection();
	};

	/// <summary>
	/// A class for connecting to pipes.
	/// </summary>
	class ARTEMIS_API PipeClient : public IPipeObject {
		HANDLE _Open(
			_In_z_ LPCSTR lpPipeName,
			_In_ PipeAccess nPipeAccess
		);

	public:
		PipeClient();
	};
}

#endif // !__PIPE_H__