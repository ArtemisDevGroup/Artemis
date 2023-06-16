//------------------------------------------------------------------------>
// MIT License
// 
// Copyright (c) 2023 Artemis Group
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------>
// Aurora: https://github.com/ArtemisDevGroup/Aurora
// This file was authored by:
// @Astrea0014: https://github.com/Astrea0014
//------------------------------------------------------------------------>

#ifndef __AURORA_SHARED_HANDLE_H__
#define __AURORA_SHARED_HANDLE_H__

#include "Definitions.h"
#include "Exceptions.h"

#include <Windows.h>
#undef CreateProcess

namespace Aurora {
	/// <summary>
	/// Desired access flags when opening a proces handle.
	/// </summary>
	struct ProcessAccessFlags {
		A_DWORD dwValue;

		enum Enumeration : A_DWORD {
			All = PROCESS_ALL_ACCESS,		// All possible access rights for a process object.
			Delete = DELETE,				// Required to delete the object.
			/// <summary>
			/// Required to read information in the security descriptor for the object, not including the information in the SACL.
			/// To read or write the SACL, you must request the ACCESS_SYSTEM_SECURITY access right.
			/// </summary>
			ReadControl = READ_CONTROL,
			Synchronize = SYNCHRONIZE,		// The right to use the object for synchronization. This enables a thread to wait until the object is in the signaled state.
			WriteDac = WRITE_DAC,			// Required to modify the DACL in the security descriptor for the object.
			WriteOwner = WRITE_OWNER,		// Required to change the owner in the security descriptor for the object.

			CreateProcess = PROCESS_CREATE_PROCESS,							// Required to use this process as the parent process with PROC_THREAD_ATTRIBUTE_PARENT_PROCESS.
			CreateThread = PROCESS_CREATE_THREAD,							// Required to create a thread in the process.
			DupHandle = PROCESS_DUP_HANDLE,									// Required to duplicate a handle using DuplicateHandle.
			QueryInformation = PROCESS_QUERY_INFORMATION,					// Required to retrieve certain information about a process, such as its token, exit code, and priority class.
			QueryLimitedInformation = PROCESS_QUERY_LIMITED_INFORMATION,	// Required to retrieve certain information about a process. A handle that has the PROCESS_QUERY_INFORMATION access right is automatically granted PROCESS_QUERY_LIMITED_INFORMATION.   
			SetInformation = PROCESS_SET_INFORMATION,						// Required to set certain information about a process, such as its priority class.
			SetQuota = PROCESS_SET_QUOTA,									// Required to set memory limits using SetProcessWorkingSetSize.
			SuspendResume = PROCESS_SUSPEND_RESUME,							// Required to suspend or resume a process.
			Terminate = PROCESS_TERMINATE,									// Required to terminate a process using TerminateProcess.
			VmOperation = PROCESS_VM_OPERATION,								// Required to perform an operation on the address space of a process (see VirtualProtectEx and WriteProcessMemory).
			VmRead = PROCESS_VM_READ,										// Required to read memory in a process using ReadProcessMemory.
			VmWrite = PROCESS_VM_WRITE,										// Required to write to memory in a process using WriteProcessMemory.
			Vm = PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE	// Includes access to all virtual memory operations.
		};

		constexpr ProcessAccessFlags() noexcept : dwValue(0) {}
		constexpr ProcessAccessFlags(A_DWORD dwValue) noexcept : dwValue(dwValue) {}

		operator A_DWORD& () noexcept { return dwValue; }
		constexpr operator const A_DWORD& () const noexcept { return dwValue; }
	};

	/// <summary>
	/// Opens a shared process handle.
	/// </summary>
	/// <param name="dwDesiredAccess">- The desired access to the process object.</param>
	/// <param name="bInheritHandle">- A bool that determines whether the handle is inheritable or not.</param>
	/// <param name="dwProcessId">- The id of the process.</param>
	/// <returns>A handle to the process.</returns>
	/// <exception cref="WindowsApiException"/>
	AURORA_NDWR_DISP("OpenSharedProcess") AURORA_API HANDLE OpenSharedProcess(_In_ ProcessAccessFlags dwDesiredAccess, _In_ A_BOOL bInheritHandle, _In_ A_DWORD dwProcessId);

	/// <summary>
	/// Closes a shared process handle.
	/// </summary>
	/// <param name="hProcess">- The shared process handle returned by 'OpenSharedProcess'.</param>
	AURORA_API A_VOID CloseSharedProcess(_In_ HANDLE hProcess);

	/// <summary>
	/// Duplicates the shared process handle.
	/// </summary>
	/// <param name="hProcess">-The shared process handle to duplicate.</param>
	/// <returns>The duplicated handle.</returns>
	AURORA_NDWR_DISP("DuplicateSharedProcess") AURORA_API HANDLE DuplicateSharedProcess(_In_ HANDLE hProcess);
}

#endif // !__AURORA_SHARED_HANDLE_H__