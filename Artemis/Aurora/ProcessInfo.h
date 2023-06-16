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

#ifndef __AURORA_PROCESS_INFO_H__
#define __AURORA_PROCESS_INFO_H__

#include "Definitions.h"
#include "Exceptions.h"
#include "Array.h"
#include "SharedHandle.h"

#include <Windows.h>
#undef GetModuleHandle

namespace Aurora {
	class ProcessInfo32;
	class ProcessInfo64;

	/// <summary>
	/// Contains information about a module in a 32-bit process.
	/// </summary>
	class AURORA_API ModuleInfo32 {
		A_CHAR szModuleName[MAX_NAME];
		A_CHAR szModulePath[MAX_PATH];
		HMODULE hModule;
		A_ADDR32 uModuleBaseAddress;
		A_DWORD dwModuleSize;

		const ProcessInfo32* lpOwningProcess;

	public:
		ModuleInfo32();

		/// <summary>
		/// Gathers information about a module.
		/// </summary>
		/// <param name="lpOwningProcess">- A pointer to a process info instance belonging to the owning process.</param>
		/// <param name="hModule">- A handle to the module to gather information about</param>
		/// <exception cref="WindowsApiException"/>
		ModuleInfo32(_In_ const ProcessInfo32* lpOwningProcess, _In_ HMODULE hModule);

		ModuleInfo32(_In_ const ProcessInfo32* lpOwningProcess, _In_ const ModuleInfo32&);

		ModuleInfo32(const ModuleInfo32&);

		/// <summary>
		/// Gets the name of the module represented by the current instance.
		/// </summary>
		/// <returns>The name of the module.</returns>
		AURORA_NDWR_GET("GetModuleName") constexpr A_LPCSTR GetModuleName() const noexcept;

		/// <summary>
		/// Gets the absolute path of the module represented by the current instance.
		/// </summary>
		/// <returns>The path of the module.</returns>
		AURORA_NDWR_GET("GetModulePath") constexpr A_LPCSTR GetModulePath() const noexcept;

		/// <summary>
		/// Gets the handle of the module represented by the current instance.
		/// </summary>
		/// <returns>The handle of the module.</returns>
		AURORA_NDWR_GET("GetModuleHandle") constexpr HMODULE GetModuleHandle() const noexcept;

		/// <summary>
		/// Gets the base address of the allocation belonging to the module represented by the current instance.
		/// </summary>
		/// <returns>The base address of the module allocation.</returns>
		AURORA_NDWR_GET("GetModuleBaseAddress") constexpr A_ADDR32 GetModuleBaseAddress() const noexcept;

		/// <summary>
		/// Gets the size of the allocation belonging to the module represented by the current instance.
		/// </summary>
		/// <returns>The size of the module allocation.</returns>
		AURORA_NDWR_GET("GetModuleSize") constexpr A_DWORD GetModuleSize() const noexcept;

		/// <summary>
		/// Gets a reference to a process info object representing the process where the current instance of the module is loaded.
		/// </summary>
		/// <returns>A reference to the owning process.</returns>
		AURORA_NDWR_GET("GetOwningProcess") constexpr const ProcessInfo32& GetOwningProcess() const noexcept;
	};

	/// <summary>
	/// Contains information about a module in a 64-bit process.
	/// </summary>
	class AURORA_API ModuleInfo64 {
		A_CHAR szModuleName[MAX_NAME];
		A_CHAR szModulePath[MAX_PATH];
		HMODULE hModule;
		A_ADDR64 uModuleBaseAddress;
		A_DWORD dwModuleSize;

		const ProcessInfo64* lpOwningProcess;

	public:
		ModuleInfo64();

		/// <summary>
		/// Gathers information about a module.
		/// </summary>
		/// <param name="lpOwningProcess">- A pointer to a process info instance belonging to the owning process.</param>
		/// <param name="hModule">- A handle to the module to gather information about</param>
		/// <exception cref="WindowsApiException"/>
		ModuleInfo64(_In_ const ProcessInfo64* lpOwningProcess, _In_ HMODULE hModule);

		ModuleInfo64(_In_ const ProcessInfo64* lpOwningProcess, _In_ const ModuleInfo64&);

		ModuleInfo64(const ModuleInfo64&);

		/// <summary>
		/// Gets the name of the module represented by the current instance.
		/// </summary>
		/// <returns>The name of the module.</returns>
		AURORA_NDWR_GET("GetModuleName") constexpr A_LPCSTR GetModuleName() const noexcept;

		/// <summary>
		/// Gets the absolute path of the module represented by the current instance.
		/// </summary>
		/// <returns>The path of the module.</returns>
		AURORA_NDWR_GET("GetModulePath") constexpr A_LPCSTR GetModulePath() const noexcept;

		/// <summary>
		/// Gets the handle of the module represented by the current instance.
		/// </summary>
		/// <returns>The handle of the module.</returns>
		AURORA_NDWR_GET("GetModuleHandle") constexpr HMODULE GetModuleHandle() const noexcept;

		/// <summary>
		/// Gets the base address of the allocation belonging to the module represented by the current instance.
		/// </summary>
		/// <returns>The base address of the module allocation.</returns>
		AURORA_NDWR_GET("GetModuleBaseAddress") constexpr A_ADDR64 GetModuleBaseAddress() const noexcept;

		/// <summary>
		/// Gets the size of the allocation belonging to the module represented by the current instance.
		/// </summary>
		/// <returns>The size of the module allocation.</returns>
		AURORA_NDWR_GET("GetModuleSize") constexpr A_DWORD GetModuleSize() const noexcept;

		/// <summary>
		/// Gets a reference to a process info object representing the process where the current instance of the module is loaded.
		/// </summary>
		/// <returns>A reference to the owning process.</returns>
		AURORA_NDWR_GET("GetOwningProcess") constexpr const ProcessInfo64& GetOwningProcess() const noexcept;
	};

	/// <summary>
	/// Contains information about a 32-bit process.
	/// </summary>
	class AURORA_API ProcessInfo32 {
		A_CHAR szProcessName[MAX_NAME];
		A_CHAR szProcessPath[MAX_PATH];
		HANDLE hProcess;
		A_DWORD dwProcessId;
		List<ModuleInfo32> ModuleList;

	public:
		/// <summary>
		/// Fetches information about a 32-bit process.
		/// </summary>
		/// <param name="lpProcessName">
		/// <para>- The name of the process to fetch information from.</para>
		/// <para>'nullptr' (default) will fetch information about the current process instead.</para>
		/// </param>
		/// <exception cref="WindowsApiException"/>
		ProcessInfo32(_In_opt_z_ A_LPCSTR lpProcessName = nullptr);
		ProcessInfo32(const ProcessInfo32&);
		~ProcessInfo32();

		/// <summary>
		/// Gets the name of the process associated with the instance.
		/// </summary>
		/// <returns>The process name.</returns>
		AURORA_NDWR_GET("GetProcessName") constexpr A_LPCSTR GetProcessName() const noexcept;

		/// <summary>
		/// Gets a path to the process executable associated with the instance.
		/// </summary>
		/// <returns>A process executable path.</returns>
		AURORA_NDWR_GET("GetProcessPath") constexpr A_LPCSTR GetProcessPath() const noexcept;

		/// <summary>
		/// <para>Gets a handle to the process associated with the instance.</para>
		/// <para>The handle has the following access rights:</para>
		/// <para> - PROCESS_VM_READ</para>
		/// <para> - PROCESS_VM_WRITE</para>
		/// <para> - PROCESS_VM_OPERATION</para>
		/// <para> - PROCESS_CREATE_PROCESS</para>
		/// <para> - PROCESS_CREATE_THREAD</para>
		/// <para> - PROCESS_QUERY_INFORMATION</para>
		/// <para> - SYNCHRONIZE</para>
		/// </summary>
		/// <returns>A handle to the process.</returns>
		AURORA_NDWR_GET("GetProcessHandle") constexpr HANDLE GetProcessHandle() const noexcept;

		/// <summary>
		/// Gets the id of the process associated with the instance.
		/// </summary>
		/// <returns>The process id.</returns>
		AURORA_NDWR_GET("GetProcessId") constexpr A_DWORD GetProcessId() const noexcept;

		/// <summary>
		/// Gets a reference to the list of modules loaded in the process associated with the instance.
		/// </summary>
		/// <returns>The loaded module list.</returns>
		AURORA_NDWR_GET("GetModuleList") constexpr const List<ModuleInfo32>& GetModuleList() const noexcept;

		/// <summary>
		/// Gets a reference to the module with the provided name.
		/// </summary>
		/// <param name="lpModuleName">- The name of the module to find.</param>
		/// <returns>A reference to a ModuleInfo32 instance containing information about the module.</returns>
		/// <exception cref="ObjectNotFoundException"/>
		AURORA_NDWR_GET("GetModule") const ModuleInfo32& GetModule(_In_opt_z_ A_LPCSTR lpModuleName = nullptr) const;

		/// <summary>
		/// Checks whether the process info instance represents the current process.
		/// </summary>
		/// <returns>True if the instance represents the current process, otherwise false.</returns>
		AURORA_NDWR_GET("IsCurrentProcess") A_BOOL IsCurrentProcess() const noexcept;

		/// <summary>
		/// Refreshes the module list by fetching every module again.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		AURORA_NDWR_GET("RefreshModuleList") A_VOID RefreshModuleList();
	}; 

	/// <summary>
	/// Contains information about a 64-bit process.
	/// </summary>
	class AURORA_API ProcessInfo64 {
		A_CHAR szProcessName[MAX_NAME];
		A_CHAR szProcessPath[MAX_PATH];
		HANDLE hProcess;
		A_DWORD dwProcessId;
		List<ModuleInfo64> ModuleList;

	public:
		/// <summary>
		/// Fetches information about a 64-bit process.
		/// </summary>
		/// <param name="lpProcessName">
		/// <para>- The name of the process to fetch information from.</para>
		/// <para>'nullptr' (default) will fetch information about the current process instead.</para>
		/// </param>
		/// <exception cref="WindowsApiException"/>
		ProcessInfo64(_In_opt_z_ A_LPCSTR lpProcessName = nullptr);
		ProcessInfo64(const ProcessInfo64&);
		~ProcessInfo64();

		/// <summary>
		/// Gets the name of the process associated with the instance.
		/// </summary>
		/// <returns>The process name.</returns>
		AURORA_NDWR_GET("GetProcessName") constexpr A_LPCSTR GetProcessName() const noexcept;

		/// <summary>
		/// Gets a path to the process executable associated with the instance.
		/// </summary>
		/// <returns>A process executable path.</returns>
		AURORA_NDWR_GET("GetProcessPath") constexpr A_LPCSTR GetProcessPath() const noexcept;

		/// <summary>
		/// <para>Gets a handle to the process associated with the instance.</para>
		/// <para>The handle has the following access rights:</para>
		/// <para> - PROCESS_VM_READ</para>
		/// <para> - PROCESS_VM_WRITE</para>
		/// <para> - PROCESS_VM_OPERATION</para>
		/// <para> - PROCESS_CREATE_PROCESS</para>
		/// <para> - PROCESS_CREATE_THREAD</para>
		/// <para> - PROCESS_QUERY_INFORMATION</para>
		/// <para> - SYNCHRONIZE</para>
		/// </summary>
		/// <returns>A handle to the process.</returns>
		AURORA_NDWR_GET("GetProcessHandle") constexpr HANDLE GetProcessHandle() const noexcept;

		/// <summary>
		/// Gets the id of the process associated with the instance.
		/// </summary>
		/// <returns>The process id.</returns>
		AURORA_NDWR_GET("GetProcessId") constexpr A_DWORD GetProcessId() const noexcept;

		/// <summary>
		/// Gets a reference to the list of modules loaded in the process associated with the instance.
		/// </summary>
		/// <returns>The loaded module list.</returns>
		AURORA_NDWR_GET("GetModuleList") constexpr const List<ModuleInfo64>& GetModuleList() const noexcept;

		/// <summary>
		/// Gets a reference to the module with the provided name.
		/// </summary>
		/// <param name="lpModuleName">- The name of the module to find.</param>
		/// <returns>A reference to a ModuleInfo32 instance containing information about the module.</returns>
		/// <exception cref="ObjectNotFoundException"/>
		AURORA_NDWR_GET("GetModule") const ModuleInfo64& GetModule(_In_opt_z_ A_LPCSTR lpModuleName = nullptr) const;

		/// <summary>
		/// Checks whether the process info instance represents the current process.
		/// </summary>
		/// <returns>True if the instance represents the current process, otherwise false.</returns>
		AURORA_NDWR_GET("IsCurrentProcess") A_BOOL IsCurrentProcess() const noexcept;

		/// <summary>
		/// Refreshes the module list by fetching every module again.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		AURORA_NDWR_GET("RefreshModuleList") A_VOID RefreshModuleList();
	};

#ifdef _WIN64
	/// <summary>
	/// Contains information about a module in a 64-bit process.
	/// </summary>
	using ModuleInfo = ModuleInfo64;

	/// <summary>
	/// Contains information about a 64-bit process.
	/// </summary>
	using ProcessInfo = ProcessInfo64;
#else
	/// <summary>
	/// Contains information about a module in a 32-bit process.
	/// </summary>
	using ModuleInfo = ModuleInfo32;

	/// <summary>
	/// Contains information about a 32-bit process.
	/// </summary>
	using ProcessInfo = ProcessInfo32;
#endif // _WIN64

	/// <summary>
	/// Gets information about the current process.
	/// </summary>
	/// <returns>Information about the current process.</returns>
	AURORA_NDWR_GET("GetCurrentProcessInfo") AURORA_API ProcessInfo& GetCurrentProcessInfo() noexcept;
}

#endif // !__AURORA_PROCESS_INFO_H__