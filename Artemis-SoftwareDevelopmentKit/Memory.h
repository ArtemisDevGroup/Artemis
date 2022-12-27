//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "Definitions.h"
#include "MemoryDefinitions.h"
#include "Exceptions.h"
#include "Array.h"

#include "MemoryScanner.h"
#include "MemoryAllocation.h"
#include "MemoryHook.h"
#include "MemoryInjection.h"

#undef GetModuleHandle

namespace Artemis {
	/// <summary>
	/// A class for interacting with the local or a remote process' virtual memory.
	/// </summary>
	class ARTEMIS_API Memory : public IDisposable {
	public:
		/// <summary>
		/// The object target type.
		/// </summary>
		enum Type {
			/// <summary>
			/// Indicates that the virtual memory space targetted is the one of the local process.
			/// </summary>
			Internal,
	
			/// <summary>
			/// Indicates that the virtual memory space targetted is the one of a remote process.
			/// </summary>
			External
		};
	private:
		Type TargetType;
	
		CHAR szProcessName[MAX_NAME];
		CHAR szProcessPath[MAX_PATH];
		CHAR szModuleName[MAX_NAME];
		CHAR szModulePath[MAX_PATH];
	
		HANDLE hProcess;
		HMODULE hModule;
		DWORD dwProcessId;
		ADDRESS uModuleBaseAddress;
		DWORD dwModuleSize;
	
		void _Read(
			_In_ ADDRESS uAddress,
			_Out_ LPVOID lpBuffer,
			_In_ DWORD dwSize
		) const;
	
		void _Write(
			_In_ ADDRESS uAddress,
			_In_ LPCVOID lpBuffer,
			_In_ DWORD dwSize
		) const;
	
	public:
		Memory();
		Memory(const Memory& cpy);
	
		/// <summary>
		/// Constructs a Memory object targetting a module in the local process' virtual memory region.
		/// </summary>
		/// <param name="lpModuleName">- The name of the module to base off of with it's extension. This parameter is optional, nullptr will target the main module of the process.</param>
		/// <exception cref="ObjectNotFoundException"/>
		/// <exception cref="WindowsApiException"/>
		Memory(_In_opt_z_ LPCSTR lpModuleName);
	
		/// <summary>
		/// Constructs a Memory object targetting a module in a remote process' virtual memory region.
		/// </summary>
		/// <param name="lpProcessName">- The name of the process along with it's extension.</param>
		/// <param name="lpModuleName">- The name of the module to base off of with it's extension. This parameter is optional, nullptr will target the main module of the process.</param>
		/// <exception cref="ObjectNotFoundException"/>
		/// <exception cref="WindowsApiException"/>
		Memory(_In_z_ LPCSTR lpProcessName, _In_opt_z_ LPCSTR lpModuleName);

		~Memory();
	
		/// <summary>
		/// Gets the name of the target process along with it's file extension.
		/// </summary>
		/// <returns>The name of the process.</returns>
		_Check_return_ _Ret_z_ LPCSTR GetProcessName() const;
	
		/// <summary>
		/// Gets the full path of the target process.
		/// </summary>
		/// <returns>The path of the process.</returns>
		_Check_return_ _Ret_z_ LPCSTR GetProcessPath() const;
	
		/// <summary>
		/// Gets the name of the target module along with itäs file extension.
		/// </summary>
		/// <returns>The name of the module.</returns>
		_Check_return_ _Ret_z_ LPCSTR GetModuleName() const;
	
		/// <summary>
		/// Gets the path of the target module.
		/// </summary>
		/// <returns>The path of the module.</returns>
		_Check_return_ _Ret_z_ LPCSTR GetModulePath() const;
	
		/// <summary>
		/// Gets the target process' type.
		/// </summary>
		/// <returns>The process type.</returns>
		_Check_return_ Type GetTargetType() const;
	
		/// <summary>
		/// Gets a handle to the target process.
		/// </summary>
		/// <returns>A process handle.</returns>
		_Check_return_ _Ret_z_ HANDLE GetProcessHandle() const;
	
		/// <summary>
		/// Gets the unique system identifier of the target process.
		/// </summary>
		/// <returns>The process id.</returns>
		_Check_return_ DWORD GetProcessId() const;
	
		/// <summary>
		/// Gets a handle to the target module.
		/// </summary>
		/// <returns>A module handle.</returns>
		_Check_return_ _Ret_notnull_ HMODULE GetModuleHandle() const;
	
		/// <summary>
		/// Gets the base address of the target module's executable allocation.
		/// </summary>
		/// <returns>The module base address.</returns>
		_Check_return_ ADDRESS GetModuleBase() const;
	
		/// <summary>
		/// Gets the size of the target module's executable allocation in bytes.
		/// </summary>
		/// <returns>The module size.</returns>
		_Check_return_ DWORD GetModuleSize() const;
	
		/// <summary>
		/// Reads memory from an address.
		/// </summary>
		/// <typeparam name="T">- The datatype to read.</typeparam>
		/// <param name="uAddress">- The address to read.</param>
		/// <returns>The read value.</returns>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		template<typename T>
		inline _Check_return_ T Read(_In_ ADDRESS uAddress) const {
			CONTEXT_BEGIN;

			T n = T();
			_Read(uAddress, &n, sizeof(T));

			CONTEXT_END;
			return n;
		}
	
		/// <summary>
		/// Reads an array from an address.
		/// </summary>
		/// <typeparam name="T">- The datatype to read.</typeparam>
		/// <typeparam name="uSize">- The number of elements to read.</typeparam>
		/// <param name="uAddress">- The address to read.</param>
		/// <param name="lpBuffer">- A pointer to the buffer to receive the read data.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		template<typename T, UINT uSize>
		inline void ReadArray(
			_In_ ADDRESS uAddress,
			_Out_writes_(uSize) T(&lpBuffer)[uSize]
		) const {
			CONTEXT_BEGIN;

			_Read(uAddress, lpBuffer, uSize * sizeof(T));

			CONTEXT_END;
		}
	
		/// <summary>
		/// Reads an array from an address.
		/// </summary>
		/// <typeparam name="T">- The datatype to read.</typeparam>
		/// <param name="uAddress">- The address to read.</param>
		/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
		/// <param name="uSize">- The size of the buffer in bytes.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		template<typename T>
		inline void ReadArray(
			_In_ ADDRESS uAddress,
			_Out_writes_bytes_(uSize) T* lpBuffer,
			_In_ UINT uSize
		) const {
			CONTEXT_BEGIN;

			_Read(uAddress, lpBuffer, uSize);

			CONTEXT_END;
		}
	
		/// <summary>
		/// Reads the address from the end of a pointer.
		/// </summary>
		/// <param name="uAddress">- The pointer base address.</param>
		/// <param name="lpPointer">- A pointer to an object containing the pointer.</param>
		/// <returns>The address at the end of the pointer.</returns>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		_Check_return_ ADDRESS ReadPtrAddress(
			_In_ ADDRESS uAddress,
			_In_ LPCPOINTER lpPointer
		) const;
	
		/// <summary>
		/// Reads the address from the end of a pointer.
		/// </summary>
		/// <param name="lpPointer">- A pointer to an object containing the base offset and the pointer.</param>
		/// <returns>The address at the end of the pointer.</returns>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		_Check_return_ ADDRESS ReadPtrAddress(_In_ LPCBASE_POINTER lpPointer) const;

		/// <summary>
		/// Reads the address from the end of a pointer.
		/// </summary>
		/// <param name="uAddress">- The pointer base address.</param>
		/// <param name="lpPointer">- A List containing the pointer.</param>
		/// <returns>The address at the end of the pointer.</returns>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		_Check_return_ ADDRESS ReadPtrAddress(
			_In_ ADDRESS uAddress,
			_In_ const List<ADDRESS>& Offsets
		) const;
	
		/// <summary>
		/// Reads data from a pointer.
		/// </summary>
		/// <typeparam name="T">- The datatype to read.</typeparam>
		/// <param name="uAddress">- The pointer base address.</param>
		/// <param name="lpPointer">- A pointer to an object containing the pointer.</param>
		/// <returns>The read data.</returns>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		template<typename T>
		inline _Check_return_ T ReadPtr(
			_In_ ADDRESS uAddress,
			_In_ LPCPOINTER lpPointer
		) const {
			CONTEXT_BEGIN;

			T ret = Read<T>(ReadPtrAddress(uAddress, lpPointer));

			CONTEXT_END;
			return ret;
		}
	
		/// <summary>
		/// Reads data from a pointer.
		/// </summary>
		/// <typeparam name="T">- The datatype to read.</typeparam>
		/// <param name="lpPointer">- A pointer to an object containing the base offset and the pointer.</param>
		/// <returns>The read data.</returns>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		template<typename T>
		inline _Check_return_ T ReadPtr(_In_ LPCBASE_POINTER lpPointer) const {
			CONTEXT_BEGIN;

			T ret = Read<T>(ReadPtrAddress(lpPointer));

			CONTEXT_END;
			return ret;
		}

		/// <summary>
		/// Reads data from a pointer.
		/// </summary>
		/// <typeparam name="T">- The datatype to read.</typeparam>
		/// <param name="uAddress">- The pointer base address.</param>
		/// <param name="lpPointer">- A List object containig the pointer.</param>
		/// <returns>The read data.</returns>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		template<typename T>
		inline _Check_return_ T ReadPtr(
			_In_ ADDRESS uAddress,
			_In_ const List<ADDRESS>& Offsets
		) const {
			CONTEXT_BEGIN;

			T ret = Read<T>(ReadPtrAddress(uAddress, Offsets));

			CONTEXT_END;
			return ret;
		}
	
		/// <summary>
		/// Reads a string from an address.
		/// </summary>
		/// <param name="uAddress">- The address to read.</param>
		/// <param name="lpBuffer">- A pointer to a buffer to receive the read string.</param>
		/// <param name="uCount">- The number of characters to read.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		void ReadStringA(
			_In_ ADDRESS uAddress,
			_Out_writes_z_(uCount) LPSTR lpBuffer,
			_In_ UINT uCount
		) const ;
	
		/// <summary>
		/// Reads a string from an address.
		/// </summary>
		/// <typeparam name="uCount">- The number of characters to read.</typeparam>
		/// <param name="uAddress">- The address to read.</param>
		/// <param name="lpBuffer">- A pointer to a buffer to receive the read string.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		template<UINT uCount>
		inline void ReadStringA(
			_In_ ADDRESS uAddress,
			_Out_writes_z_(uCount) CHAR(&lpBuffer)[uCount]
		) const {
			ReadStringA(uAddress, lpBuffer, uCount);
		}
	
		/// <summary>
		/// Reads a string from an address.
		/// </summary>
		/// <param name="uAddress">- The address to read.</param>
		/// <param name="lpBuffer">- A pointer to a buffer to receive the read string.</param>
		/// <param name="uCount">- The number of characters to read.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		void ReadStringW(
			_In_ ADDRESS uAddress,
			_Out_writes_z_(uCount) LPWSTR lpBuffer,
			_In_ UINT uCount
		) const;
	
		/// <summary>
		/// Reads a string from an address.
		/// </summary>
		/// <typeparam name="uCount">- The number of characters to read.</typeparam>
		/// <param name="uAddress">- The address to read.</param>
		/// <param name="lpBuffer">- A pointer to a buffer to receive the read string.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		template<UINT uCount>
		inline void ReadStringW(
			_In_ ADDRESS uAddress,
			_Out_writes_z_(uCount) WCHAR(&lpBuffer)[uCount]
		) const {
			ReadStringW(uAddress, lpBuffer, uCount);
		}
	
		/// <summary>
		/// Writes data to an address.
		/// </summary>
		/// <typeparam name="T">- The datatype to write.</typeparam>
		/// <param name="uAddress">- The address to write to.</param>
		/// <param name="Data">- The data to write.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		template<typename T>
		inline void Write(
			_In_ ADDRESS uAddress,
			_In_ const T Data
		) const {
			BOOL b = ExceptionContext::HasContext();
			if (!b) ExceptionContext::SetContext(__FUNCTION__);
			_Write(uAddress, &Data, sizeof(T));
			if (!b) ExceptionContext::ResetContext();
		}
	
		/// <summary>
		/// Writes an array of data to an address.
		/// </summary>
		/// <typeparam name="T">- The datatype to write.</typeparam>
		/// <typeparam name="uSize">- The number of elements to write.</typeparam>
		/// <param name="uAddress">- The address to write to.</param>
		/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		template<typename T, UINT uSize>
		inline void WriteArray(
			_In_ ADDRESS uAddress,
			_In_ const T(&lpBuffer)[uSize]
		) const {
			CONTEXT_BEGIN;

			_Write(uAddress, lpBuffer, uSize * sizeof(T));

			CONTEXT_END;
		}

		/// <summary>
		/// Writes an array of data to an address.
		/// </summary>
		/// <typeparam name="T">- The datatype to write.</typeparam>
		/// <param name="uAddress">- The address to write to.</param>
		/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
		/// <param name="uSize">- The size of the buffer in bytes.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		template<typename T>
		inline void WriteArray(
			_In_ ADDRESS uAddress,
			_In_ const T* lpBuffer,
			_In_ UINT uSize
		) const {
			CONTEXT_BEGIN;

			_Write(uAddress, lpBuffer, uSize);

			CONTEXT_END;
		}
	
		/// <summary>
		/// Writes data to a pointer.
		/// </summary>
		/// <typeparam name="T">- The datatype to write.</typeparam>
		/// <param name="uAddress">- The pointer base address.</param>
		/// <param name="lpPointer">- A pointer to an object containing the pointer.</param>
		/// <param name="Data">- The data to write.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		template<typename T>
		inline void WritePtr(
			_In_ ADDRESS uAddress,
			_In_ LPCPOINTER lpPointer,
			_In_ const T Data
		) const {
			CONTEXT_BEGIN;

			Write<T>(ReadPtrAddress(uAddress, lpPointer), Data);

			CONTEXT_END;
		}
	
		/// <summary>
		/// Writes data to a pointer.
		/// </summary>
		/// <typeparam name="T">- The datatype to write.</typeparam>
		/// <param name="lpPointer">- A pointer to an object containing the base offset and the pointer.</param>
		/// <param name="Data">- The data to write.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		template<typename T>
		inline void WritePtr(
			_In_ LPCBASE_POINTER lpPointer,
			_In_ const T Data
		) const {
			CONTEXT_BEGIN;

			Write<T>(ReadPtrAddress(lpPointer), Data);

			CONTEXT_END;
		}
	
		/// <summary>
		/// Writes a string to an address.
		/// </summary>
		/// <param name="uAddress">- The address to write to.</param>
		/// <param name="lpString">- The string to write.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		void WriteStringA(
			_In_ ADDRESS uAddress,
			_In_z_ LPCSTR lpString
		) const;
	
		/// <summary>
		/// Writes a string to an address.
		/// </summary>
		/// <param name="uAddress">- The address to write to.</param>
		/// <param name="lpString">- The string to write.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		void WriteStringW(
			_In_ ADDRESS uAddress,
			_In_z_ LPCWSTR lpString
		) const;
	
		/// <summary>
		/// Defines which operation to run.
		/// </summary>
		enum AssemblyAction {
			/// <summary>
			/// Writes the enable code.
			/// </summary>
			Enable,
	
			/// <summary>
			/// Writes the disable code.
			/// </summary>
			Disable
		};
	
		/// <summary>
		/// Patches assembly code.
		/// </summary>
		/// <param name="uAddress">- The address to patch.</param>
		/// <param name="lpPatch">- A pointer to an object containing the assembly code to patch.</param>
		/// <param name="Action">- The action to take.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		void AssemblyPatch(
			_In_ ADDRESS uAddress,
			_In_ LPCASM_PATCH lpPatch,
			_In_ AssemblyAction Action
		) const;
	
		/// <summary>
		/// Patches assembly code.
		/// </summary>
		/// <param name="lpPatch">- A pointer to an object containing the assembly code to patch aswell as the base offset.</param>
		/// <param name="Action">- The action to take.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="Exception"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="InstanceInvalidException"/>
		void AssemblyPatch(
			_In_ LPCBASE_ASM_PATCH lpPatch,
			_In_ AssemblyAction Action
		) const;
	
		/// <summary>
		/// Creates a memory scanner associated to the current target process. Scans a code style pattern.
		/// </summary>
		/// <param name="lpPattern">- The pattern to scan.</param>
		/// <param name="lpMask">- The associated pattern mask.</param>
		/// <param name="bScanModule">- Defines wether the scanner should only scan the process allocation or not.</param>
		/// <returns>An initialized MemoryScanner object.</returns>
		/// <exception cref="ParameterException"/>
		_Check_return_ MemoryScanner CreateScanner(
			_In_ LPCSTR lpPattern,
			_In_z_ LPCSTR lpMask,
			_In_ BOOL bScanModule = TRUE
		) const;
	
		/// <summary>
		/// Creates a memory scanner associared with the current target process. Scans an IDA style pattern.
		/// </summary>
		/// <param name="lpPattern">- The pattern to scan.</param>
		/// <param name="bScanModule">- Defines wether the scanner should only scan the process allocation or not.</param>
		/// <returns>An initialized MemoryScanner object.</returns>
		/// <exception cref="ParameterException"/>
		_Check_return_ MemoryScanner CreateScanner(
			_In_z_ LPCSTR lpPattern,
			_In_ BOOL bScanModule = TRUE
		) const;
	
		/// <summary>
		/// Creates a memory scanner associated with the current target process. Scans a string.
		/// </summary>
		/// <param name="lpString">- The string to scan.</param>
		/// <param name="bCaseSensitive"><- Defines wether the scan should be case sensitive or not.s/param>
		/// <param name="bScanModule">- Defines wether the scanner should only scan the process allocation or not.</param>
		/// <returns>An initialized MemoryScanner object.</returns>
		/// <exception cref="ParameterException"/>
		_Check_return_ MemoryScanner CreateScanner(
			_In_z_ LPCSTR lpString,
			_In_ BOOL bCaseSensitive,
			_In_ BOOL bScanModule = TRUE
		) const;
	
		/// <summary>
		/// Creates a memory scanner associated with the current target process. Scans a string.
		/// </summary>
		/// <param name="lpString">- The string to scan.</param>
		/// <param name="bCaseSensitive"><- Defines wether the scan should be case sensitive or not.s/param>
		/// <param name="bScanModule">- Defines wether the scanner should only scan the process allocation or not.</param>
		/// <returns>An initialized MemoryScanner object.</returns>
		/// <exception cref="ParameterException"/>
		_Check_return_ MemoryScanner CreateScanner(
			_In_z_ LPCWSTR lpString,
			_In_ BOOL bCaseSensitive,
			_In_ BOOL bScanModule = TRUE
		) const;
	
		/// <summary>
		/// Creates a virtual allocation object with the provided parameters.
		/// </summary>
		/// <param name="uAddress">- The address to allocate, if set to 0, the function itself will determine where to allocate.</param>
		/// <param name="uSize">- The number of bytes to allocate.</param>
		/// <param name="Type">- The allocation type/method.</param>
		/// <param name="Protection">- The allocation memory page protection.</param>
		/// <returns>An initialized VirtualAllocation object.</returns>
		_Check_return_ VirtualAllocation CreateVirtualAllocation(
			_In_opt_ ADDRESS uAddress = 0,
			_In_ SIZE_T uSize = 1024,
			_In_ AllocationType Type = AllocationType::Commit | AllocationType::Reserve,
			_In_ MemoryProtection Protection = MemoryProtection::ReadWrite
		) const;
	
		/// <summary>
		/// Changes the protection of one or more memory pages.
		/// </summary>
		/// <param name="uAddress">- The address to change the protection of.</param>
		/// <param name="uSize">- The number of bytes to change the protection of.</param>
		/// <param name="NewProtection">- The new protection of the page.</param>
		/// <param name="OldProtection">- A pointer to a buffer to receive the previous memory protection constant.</param>
		/// <exception cref="WindowsApiException"/>
		void VirtualProtect(
			_In_ ADDRESS uAddress,
			_In_ SIZE_T uSize,
			_In_ MemoryProtection NewProtection,
			_Out_opt_ MemoryProtection* lpOldProtection = nullptr
		) const;
	
		/// <summary>
		/// Queries information about a memory page.
		/// </summary>
		/// <param name="uAddress">- The address of the memory page.</param>
		/// <param name="lpMbi">- A pointer to a buffer to receive information about the memory page.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="WindowsApiException"/>
		void VirtualQuery(
			_In_ ADDRESS uAddress,
			_Out_ LPMBI lpMbi
		) const;
	
		/// <summary>
		/// Creates a hook on the API function provided.
		/// </summary>
		/// <param name="lpFunction">- The module where the function is contained.</param>
		/// <param name="lpDetour">- The name of the function to fetch from the export table.</param>
		/// <param name="lpOriginal">- A pointer to call for the original function.</param>
		/// <returns>An initialized hook object.</returns>
		/// <exception cref="NotImplementedException (Type == External)"/>
		/// <exception cref="HookException"/>
		_Check_return_ Hook CreateHook(
			_In_z_ LPCSTR lpFunction,
			_In_ LPVOID lpDetour,
			_Out_opt_ LPVOID* lpOriginal = nullptr
		) const;
	
		/// <summary>
		/// Creates a hook on the target function.
		/// </summary>
		/// <param name="lpTarget">- The target function.</param>
		/// <param name="lpDetour">- The function to replace with.</param>
		/// <param name="lpOriginal">- A pointer to call for the original function.</param>
		/// <returns>An initialized hook object.</returns>
		/// <exception cref="NotImplementedException (Type == External)"/>
		/// <exception cref="HookException"/>
		_Check_return_ Hook CreateHook(
			_In_ LPVOID lpTarget,
			_In_ LPVOID lpDetour,
			_Out_opt_ LPVOID* lpOriginal = nullptr
		) const;
	
		/// <summary>
		/// Creates a hook on the function at the target address.
		/// </summary>
		/// <param name="uTarget">- The address of the function.</param>
		/// <param name="lpDetour">- The function to replace with.</param>
		/// <param name="lpOriginal">- A pointer to call for the original function.</param>
		/// <returns>An initialized hook object.</returns>
		/// <exception cref="NotImplementedException (Type == External)"/>
		/// <exception cref="HookException"/>
		_Check_return_ Hook CreateHook(
			_In_ ADDRESS uTarget,
			_In_ LPVOID lpDetour,
			_Out_opt_ LPVOID* lpOriginal = nullptr
		) const;

		/// <summary>
		/// Releases the current instance.
		/// </summary>
		void Release();
	};
}

#endif // !__MEMORY_H__