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

#ifndef __AURORA_MEMORY_WIN32_H__
#define __AURORA_MEMORY_WIN32_H__

#include "Definitions.h"
#include "ProcessInfo.h"
#include "SharedHandle.h"
#include "MemoryTypes.h"

#include <Windows.h>

namespace Aurora {
	/// <summary>
	/// Defines the type of virtual memory allocation.
	/// </summary>
	struct AllocationTypeFlags {
		A_DWORD dwValue;

		enum Enumeration : A_DWORD {
			/// <summary>
			/// Allocates memory charges (from the overall size of memory and the paging files on disk) for the specified reserved memory pages.
			/// The function also guarantees that when the caller later initially accesses the memory, the contents will be zero.
			/// </summary>
			Commit = MEM_COMMIT,
			/// <summary>
			/// Reserves a range of the process's virtual address space without allocating any actual physical storage in memory or in the paging file on disk.
			/// </summary>
			Reserve = MEM_RESERVE,
			/// <summary>
			/// Allocates memory at the highest possible address. This can be slower than regular allocations, especially when there are many allocations.
			/// </summary>
			TopDown = MEM_TOP_DOWN,
			/// <summary>
			/// Allocates memory using large page support.
			/// The size and alignment must be a multiple of the large-page minimum.
			/// To obtain this value, use the 'GetLargePageMinimum' function.
			/// If you specify this value, you must also specify 'Reserve' and 'Commit'.
			/// </summary>
			LargePages = MEM_LARGE_PAGES,
			/// <summary>
			/// Reserves an address range that can be used to map Address Windowing Extensions (AWE) pages.
			/// This value must be used with 'Reserve' and no other values.
			/// </summary>
			Physical = MEM_PHYSICAL,
			/// <summary>
			/// Causes the system to track pages that are written to in the allocated region. If you specify this value, you must also specify 'Reserve'.
			/// To retrieve the addresses of the pages that have been written to since the region was allocated or the write-tracking state was reset, call the 'GetWriteWatch' function.
			/// To reset the write-tracking state, call 'GetWriteWatch' or 'ResetWriteWatch'. The write-tracking feature remains enabled for the memory region until the region is freed.
			/// </summary>
			WriteWatch = MEM_WRITE_WATCH
		};

		constexpr AllocationTypeFlags() noexcept : dwValue(0) {}
		constexpr AllocationTypeFlags(A_DWORD dwValue) noexcept : dwValue(dwValue) {}

		/// <summary>
		/// Validates that the value follows the VirtualAlloc specifications.
		/// </summary>
		AURORA_NDWR_PURE("Validate") constexpr A_BOOL Validate() noexcept {
			if (!!(dwValue & MEM_LARGE_PAGES) && !(!!(dwValue & MEM_RESERVE) && !!(dwValue & MEM_COMMIT))) return false;
			if (!!(dwValue & MEM_PHYSICAL) && (dwValue ^ MEM_PHYSICAL) != MEM_RESERVE) return false;
			if (!!(dwValue & MEM_WRITE_WATCH) && !(dwValue & MEM_RESERVE)) return false;
			return true;
		}

		operator A_DWORD& () noexcept { return dwValue; }
		constexpr operator const A_DWORD& () const noexcept { return dwValue; }
	};

	/// <summary>
	/// Defines the protection level of a memory page.
	/// </summary>
	enum class MemoryProtection : A_DWORD {
		Execute = PAGE_EXECUTE,						// Enables execute access to the committed region of pages. An attempt to write to the committed region results in an access violation.
		Execute_Readonly = PAGE_EXECUTE_READ,		// Enables execute or read-only access to the committed region of pages. An attempt to write to the committed region results in an access violation.
		Execute_ReadWrite = PAGE_EXECUTE_READWRITE,	// Enables execute, read-only, or read/write access to the committed region of pages.
		/// <summary>
		/// Enables execute, read-only, or copy-on-write access to a mapped view of a file mapping object.
		/// An attempt to write to a committed copy-on-write page results in a private copy of the page being made for the process.
		/// The private page is marked as 'Execute_ReadWrite', and the change is written to the new page.
		/// </summary>
		Execute_WriteCopy = PAGE_EXECUTE_WRITECOPY,
		NoAccess = PAGE_NOACCESS,					// Disables all access to the committed region of pages. An attempt to read from, write to, or execute the committed region results in an access violation.
		Readonly = PAGE_READONLY,					// Enables read-only access to the committed region of pages. An attempt to write to the committed region results in an access violation. If Data Execution Prevention is enabled, an attempt to execute code in the committed region results in an access violation.
		ReadWrite = PAGE_READWRITE,					// Enables read-only or read/write access to the committed region of pages. If Data Execution Prevention is enabled, attempting to execute code in the committed region results in an access violation.
		/// <summary>
		/// Enables read-only or copy-on-write access to a mapped view of a file mapping object.
		/// An attempt to write to a committed copy-on-write page results in a private copy of the page being made for the process.
		/// The private page is marked as 'ReadWrite', and the change is written to the new page.
		/// If Data Execution Prevention is enabled, attempting to execute code in the committed region results in an access violation.
		/// </summary>
		WriteCopy = PAGE_WRITECOPY
	};

	/// <summary>
	/// Defines the state of an allocation.
	/// </summary>
	enum class AllocationState {
		Unknown = -1,		// The state of the allocation is unknown.
		None,				// The state of the allocation is free.
		Reserved,			// The state of the allocation is reserved.
		Committed			// The state of the allocation is committed.
	};

	/// <summary>
	/// Manages a virtual memory allocation.
	/// </summary>
	class AURORA_API VirtualAllocation32 {
		A_ADDR32 uRequestedAddress;
		A_ADDR32 uAllocatedAddress;
		A_DWORD dwAllocatedSize;
		AllocationTypeFlags dwAllocationType;
		MemoryProtection dwMemoryProtection;

		HANDLE hProcess;

		AllocationState nAllocationState;

	public:
		VirtualAllocation32();

		/// <summary>
		/// Allocates memory in the current process.
		/// </summary>
		/// <param name="uAddress">- The address to allocate, if 0, the function will decide where to allocate.</param>
		/// <param name="dwSize">- The number of bytes to allocate.</param>
		/// <param name="dwAllocationType">- The allocation type. See 'AllocationTypeFlags' for more.</param>
		/// <param name="dwMemoryProtection">- The memory page protection. See 'MemoryProtection' for more.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="WindowsApiException"/>
		VirtualAllocation32(
			_In_opt_a32_(1) A_ADDR32 uAddress,
			_In_ A_DWORD dwSize,
			_In_ AllocationTypeFlags dwAllocationType,
			_In_ MemoryProtection dwMemoryProtection
		);

		/// <summary>
		/// Allocates memory in a remote process.
		/// </summary>
		/// <param name="refTargetProcess">- A reference to the process to allocate memory in.</param>
		/// <param name="uAddress">- The address to allocate, if 0, the function will decide where to allocate.</param>
		/// <param name="dwSize">- The number of bytes to allocate.</param>
		/// <param name="dwAllocationType">- The allocation type. See 'AllocationTypeFlags' for more.</param>
		/// <param name="dwMemoryProtection">- The memory page protection. See 'MemoryProtection' for more.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="WindowsApiException"/>
		VirtualAllocation32(
			_In_ const ProcessInfo32& refTargetProcess,
			_In_opt_a32_(2) A_ADDR32 uAddress,
			_In_ A_DWORD dwSize,
			_In_ AllocationTypeFlags dwAllocationType,
			_In_ MemoryProtection dwMemoryProtection
		);

		VirtualAllocation32(const VirtualAllocation32&) = delete;
		~VirtualAllocation32();

		/// <summary>
		/// Gets the state of the allocation.
		/// </summary>
		/// <returns>The allocation state.</returns>
		AURORA_NDWR_GET("GetAllocationState") constexpr AllocationState GetAllocationState() const noexcept;

		/// <summary>
		/// Gets the current memory page protection constant.
		/// </summary>
		/// <returns>The current memory page protection.</returns>
		AURORA_NDWR_GET("GetMemoryProtection") constexpr MemoryProtection GetMemoryProtection() const noexcept;

		/// <summary>
		/// Gets the requested address.
		/// </summary>
		/// <returns>The requested address.</returns>
		AURORA_NDWR_GET("GetRequestedAddress") constexpr A_ADDR32 GetRequestedAddress() const noexcept;

		/// <summary>
		/// Gets the allocated address.
		/// </summary>
		/// <returns>The allocated address.</returns>
		AURORA_NDWR_GET("GetAllocatedAddress") constexpr A_ADDR32 GetAllocatedAddress() const noexcept;

		/// <summary>
		/// Gets the allocated size in bytes.
		/// </summary>
		/// <returns>The allocated size.</returns>
		AURORA_NDWR_GET("GetAllocatedSize") constexpr A_DWORD GetAllocatedSize() const noexcept;

		/// <summary>
		/// Sets the memory page protection for the allocation.
		/// </summary>
		/// <param name="dwNewProtection">- The new protection constant.</param>
		/// <returns>The previous protection constant.</returns>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="WindowsApiException"/>
		MemoryProtection SetMemoryProtection(_In_ MemoryProtection dwNewProtection);

		/// <summary>
		/// Commits the currently reserved memory.
		/// </summary>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="WindowsApiException"/>
		A_VOID Commit();

		/// <summary>
		/// Decommits the currently committed memory.
		/// </summary>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="WindowsApiException"/>
		A_VOID Decommit();

		/// <summary>
		/// Reads memory from the allocation.
		/// </summary>
		/// <param name="uIndex">- The index relative to the allocation base to read from.</param>
		/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
		/// <param name="dwSize">- The size of the buffer in bytes.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="Read64Exception"/>
		A_VOID Read(
			_In_ A_U32 uIndex,
			_Out_writes_bytes_(dwSize) A_LPVOID lpBuffer,
			_In_ A_DWORD dwSize
		) const;

		/// <summary>
		/// Reads memory from the allocation.
		/// </summary>
		/// <typeparam name="ReturnType">- The type to read.</typeparam>
		/// <<param name="uIndex">- The index relative to the allocation base to read from.</param>
		/// <returns>The read element.</returns>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="Read64Exception"/>
		template<ReadReturnType ReturnType>
		AURORA_NDWR_PURE("Read") ReturnType Read(_In_ A_U32 uIndex) const {
			AuroraContextStart();
			ReturnType ret;
			Read(uIndex, &ret, sizeof(ReturnType));
			AuroraContextEnd();
			return ret;
		}

		/// <summary>
		/// Reads a an array of elements from the allocation.
		/// </summary>
		/// <typeparam name="ReturnType">- The type to read.</typeparam>
		/// <typeparam name="nSize">- The size of the buffer in elements.</typeparam>
		/// <param name="uIndex">- The index relative to the allocation base to read from.</param>
		/// <param name="lpBuffer">- A reference to a buffer to receive the read data.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="Read64Exception"/>
		template<ReadReturnType ReturnType, A_I32 nSize>
		A_VOID Read(
			_In_ A_U32 uIndex,
			_Out_writes_(nSize) ReturnType(&lpBuffer)[nSize]
		) {
			AuroraContextStart();
			Read(uIndex, lpBuffer, nSize * sizeof(ReturnType));
			AuroraContextEnd();
		}

		/// <summary>
		/// Writes memory to the allocation.
		/// </summary>
		/// <param name="uIndex">- The index relative to the allocation base to write to.</param>
		/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
		/// <param name="dwSize">- The size of the buffer in bytes.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="Write64Exception"/>
		A_VOID Write(
			_In_ A_U32 uIndex,
			_In_reads_bytes_(dwSize) A_LPCVOID lpBuffer,
			_In_ A_DWORD dwSize
		);

		/// <summary>
		/// Writes a single element to the allocation.
		/// </summary>
		/// <typeparam name="DataType">- The type to write.</typeparam>
		/// <param name="uIndex">- The index relative to the allocation base to write to.</param>
		/// <param name="refData">- A reference to the element to write.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="Write64Exception"/>
		template<WriteDataType DataType>
		A_VOID Write(
			_In_ A_U32 uIndex,
			_In_ const DataType& refData
		) {
			AuroraContextStart();
			Write(uIndex, &refData, sizeof(DataType));
			AuroraContextEnd();
		}

		/// <summary>
		/// Writes an array of elements to the allocation.
		/// </summary>
		/// <typeparam name="DataType">- The type to write.</typeparam>
		/// <typeparam name="nSize">- The size of the buffer in elements.</typeparam>
		/// <param name="uIndex">- The index relative to the allocation base to write to.</param>
		/// <param name="lpBuffer">- A reference to a buffer containing the data to write.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="Write64Exception"/>
		template<WriteDataType DataType, A_I32 nSize>
		A_VOID Write(
			_In_ A_U32 uIndex,
			_In_reads_(nSize) const DataType(&lpBuffer)[nSize]
		) {
			AuroraContextStart();
			Write(uIndex, lpBuffer, nSize * sizeof(DataType));
			AuroraContextEnd();
		}
	};

	/// <summary>
	/// Manages a virtual memory allocation.
	/// </summary>
	class AURORA_API VirtualAllocation64 {
		A_ADDR64 uRequestedAddress;
		A_ADDR64 uAllocatedAddress;
		A_DWORD dwAllocatedSize;
		AllocationTypeFlags dwAllocationType;
		MemoryProtection dwMemoryProtection;

		HANDLE hProcess;

		AllocationState nAllocationState;

	public:
		VirtualAllocation64();

		/// <summary>
		/// Allocates memory in the current process.
		/// </summary>
		/// <param name="uAddress">- The address to allocate, if 0, the function will decide where to allocate.</param>
		/// <param name="dwSize">- The number of bytes to allocate.</param>
		/// <param name="dwAllocationType">- The allocation type. See 'AllocationTypeFlags' for more.</param>
		/// <param name="dwMemoryProtection">- The memory page protection. See 'MemoryProtection' for more.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="WindowsApiException"/>
		VirtualAllocation64(
			_In_opt_a64_(1) A_ADDR64 uAddress,
			_In_ A_DWORD dwSize,
			_In_ AllocationTypeFlags dwAllocationType,
			_In_ MemoryProtection dwMemoryProtection
		);

		/// <summary>
		/// Allocates memory in a remote process.
		/// </summary>
		/// <param name="refTargetProcess">- A reference to the process to allocate memory in.</param>
		/// <param name="uAddress">- The address to allocate, if 0, the function will decide where to allocate.</param>
		/// <param name="dwSize">- The number of bytes to allocate.</param>
		/// <param name="dwAllocationType">- The allocation type. See 'AllocationTypeFlags' for more.</param>
		/// <param name="dwMemoryProtection">- The memory page protection. See 'MemoryProtection' for more.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="WindowsApiException"/>
		VirtualAllocation64(
			_In_ const ProcessInfo64& refTargetProcess,
			_In_opt_a64_(2) A_ADDR64 uAddress,
			_In_ A_DWORD dwSize,
			_In_ AllocationTypeFlags dwAllocationType,
			_In_ MemoryProtection dwMemoryProtection
		);

		VirtualAllocation64(const VirtualAllocation64&) = delete;
		~VirtualAllocation64();

		/// <summary>
		/// Gets the state of the allocation.
		/// </summary>
		/// <returns>The allocation state.</returns>
		AURORA_NDWR_GET("GetAllocationState") constexpr AllocationState GetAllocationState() const noexcept;

		/// <summary>
		/// Gets the current memory page protection constant.
		/// </summary>
		/// <returns>The current memory page protection.</returns>
		AURORA_NDWR_GET("GetMemoryProtection") constexpr MemoryProtection GetMemoryProtection() const noexcept;

		/// <summary>
		/// Gets the requested address.
		/// </summary>
		/// <returns>The requested address.</returns>
		AURORA_NDWR_GET("GetRequestedAddress") constexpr A_ADDR64 GetRequestedAddress() const noexcept;

		/// <summary>
		/// Gets the allocated address.
		/// </summary>
		/// <returns>The allocated address.</returns>
		AURORA_NDWR_GET("GetAllocatedAddress") constexpr A_ADDR64 GetAllocatedAddress() const noexcept;

		/// <summary>
		/// Gets the allocated size in bytes.
		/// </summary>
		/// <returns>The allocated size.</returns>
		AURORA_NDWR_GET("GetAllocatedSize") constexpr A_DWORD GetAllocatedSize() const noexcept;

		/// <summary>
		/// Sets the memory page protection for the allocation.
		/// </summary>
		/// <param name="dwNewProtection">- The new protection constant.</param>
		/// <returns>The previous protection constant.</returns>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="WindowsApiException"/>
		MemoryProtection SetMemoryProtection(_In_ MemoryProtection dwNewProtection);

		/// <summary>
		/// Commits the currently reserved memory.
		/// </summary>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="WindowsApiException"/>
		A_VOID Commit();

		/// <summary>
		/// Decommits the currently committed memory.
		/// </summary>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="WindowsApiException"/>
		A_VOID Decommit();

		/// <summary>
		/// Reads memory from the allocation.
		/// </summary>
		/// <param name="uIndex">- The index relative to the allocation base to read from.</param>
		/// <param name="lpBuffer">- A pointer to a buffer to receive the read data.</param>
		/// <param name="dwSize">- The size of the buffer in bytes.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="Read64Exception"/>
		A_VOID Read(
			_In_ A_U32 uIndex,
			_Out_writes_bytes_(dwSize) A_LPVOID lpBuffer,
			_In_ A_DWORD dwSize
		) const;

		/// <summary>
		/// Reads memory from the allocation.
		/// </summary>
		/// <typeparam name="ReturnType">- The type to read.</typeparam>
		/// <<param name="uIndex">- The index relative to the allocation base to read from.</param>
		/// <returns>The read element.</returns>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="Read64Exception"/>
		template<ReadReturnType ReturnType>
		AURORA_NDWR_PURE("Read") ReturnType Read(_In_ A_U32 uIndex) const {
			AuroraContextStart();
			ReturnType ret;
			Read(uIndex, &ret, sizeof(ReturnType));
			AuroraContextEnd();
			return ret;
		}

		/// <summary>
		/// Reads a an array of elements from the allocation.
		/// </summary>
		/// <typeparam name="ReturnType">- The type to read.</typeparam>
		/// <typeparam name="nSize">- The size of the buffer in elements.</typeparam>
		/// <param name="uIndex">- The index relative to the allocation base to read from.</param>
		/// <param name="lpBuffer">- A reference to a buffer to receive the read data.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="Read64Exception"/>
		template<ReadReturnType ReturnType, A_I32 nSize>
		A_VOID Read(
			_In_ A_U32 uIndex,
			_Out_writes_(nSize) ReturnType(&lpBuffer)[nSize]
		) {
			AuroraContextStart();
			Read(uIndex, lpBuffer, nSize * sizeof(ReturnType));
			AuroraContextEnd();
		}

		/// <summary>
		/// Writes memory to the allocation.
		/// </summary>
		/// <param name="uIndex">- The index relative to the allocation base to write to.</param>
		/// <param name="lpBuffer">- A pointer to a buffer containing the data to write.</param>
		/// <param name="dwSize">- The size of the buffer in bytes.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="Write64Exception"/>
		A_VOID Write(
			_In_ A_U32 uIndex,
			_In_reads_bytes_(dwSize) A_LPCVOID lpBuffer,
			_In_ A_DWORD dwSize
		);

		/// <summary>
		/// Writes a single element to the allocation.
		/// </summary>
		/// <typeparam name="DataType">- The type to write.</typeparam>
		/// <param name="uIndex">- The index relative to the allocation base to write to.</param>
		/// <param name="refData">- A reference to the element to write.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="Write64Exception"/>
		template<WriteDataType DataType>
		A_VOID Write(
			_In_ A_U32 uIndex,
			_In_ const DataType& refData
		) {
			AuroraContextStart();
			Write(uIndex, &refData, sizeof(DataType));
			AuroraContextEnd();
		}

		/// <summary>
		/// Writes an array of elements to the allocation.
		/// </summary>
		/// <typeparam name="DataType">- The type to write.</typeparam>
		/// <typeparam name="nSize">- The size of the buffer in elements.</typeparam>
		/// <param name="uIndex">- The index relative to the allocation base to write to.</param>
		/// <param name="lpBuffer">- A reference to a buffer containing the data to write.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="Write64Exception"/>
		template<WriteDataType DataType, A_I32 nSize>
		A_VOID Write(
			_In_ A_U32 uIndex,
			_In_reads_(nSize) const DataType(&lpBuffer)[nSize]
		) {
			AuroraContextStart();
			Write(uIndex, lpBuffer, nSize * sizeof(DataType));
			AuroraContextEnd();
		}
	};

#ifdef _WIN64
	/// <summary>
	/// Manages a virtual memory allocation.
	/// </summary>
	using VirtualAllocation = VirtualAllocation64;
#else
	/// <summary>
	/// Manages a virtual memory allocation.
	/// </summary>
	using VirtualAllocation = VirtualAllocation32;
#endif // _WIN64s

	/// <summary>
	/// <para>Sets the protection of one or more virtual memory pages.</para>
	/// <para>This function wraps a Windows API function, see VirtualProtect for more.</para>
	/// </summary>
	/// <param name="uAddress">- The address to change the protection at.</param>
	/// <param name="dwSize">- The number of bytes to change the protection for.</param>
	/// <param name="dwProtect">- The protection to set.</param>
	/// <returns>The previous page protection constant.</returns>
	/// <exception cref="WindowsApiException"/>
	AURORA_API MemoryProtection ProtectMemory(
		_In_a_ A_ADDR uAddress,
		_In_ A_DWORD dwSize,
		_In_ MemoryProtection dwProtect
	);

	/// <summary>
	/// <para>Sets the protection of one or more virtual memory pages.</para>
	/// <para>This function wraps a Windows API function, see VirtualProtectEx for more.</para>
	/// </summary>
	/// <param name="refTargetProcess">- A reference to the target process.</param>
	/// <param name="uAddress">- The address to change the protection at.</param>
	/// <param name="dwSize">- The number of bytes to change the protection for.</param>
	/// <param name="dwProtect">- The protection to set.</param>
	/// <returns>The previous page protection constant.</returns>
	/// <exception cref="WindowsApiException"/>
	AURORA_API MemoryProtection ProtectMemory(
		_In_ const ProcessInfo32& refTargetProcess,
		_In_a32_ A_ADDR32 uAddress,
		_In_ A_DWORD dwSize,
		_In_ MemoryProtection dwProtect
	);

	/// <summary>
	/// <para>Sets the protection of one or more virtual memory pages.</para>
	/// <para>This function wraps a Windows API function, see VirtualProtectEx for more.</para>
	/// </summary>
	/// <param name="refTargetProcess">- A reference to the target process.</param>
	/// <param name="uAddress">- The address to change the protection at.</param>
	/// <param name="dwSize">- The number of bytes to change the protection for.</param>
	/// <param name="dwProtect">- The protection to set.</param>
	/// <returns>The previous page protection constant.</returns>
	/// <exception cref="WindowsApiException"/>
	AURORA_API MemoryProtection ProtectMemory(
		_In_ const ProcessInfo64& refTargetProcess,
		_In_a64_ A_ADDR64 uAddress,
		_In_ A_DWORD dwSize,
		_In_ MemoryProtection dwProtect
	);

	/// <summary>
	/// Queries information about one or more memory pages.
	/// </summary>
	/// <param name="uAddress">- The address to query information from.</param>
	/// <param name="lpMbi">- A pointer to a MEMORY_BASIC_INFORMATION structure to receive the queried information.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WindowsApiException"/>
	AURORA_API A_VOID QueryMemory(
		_In_a_ A_ADDR uAddress,
		_Out_ PMEMORY_BASIC_INFORMATION lpMbi
	);

	/// <summary>
	/// Queries information about one or more memory pages.
	/// </summary>
	/// <param name="refTargetProcess">- A reference to the target process.</param>
	/// <param name="uAddress">- The address to query information from.</param>
	/// <param name="lpMbi">- A pointer to a MEMORY_BASIC_INFORMATION structure to receive the queried information.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WindowsApiException"/>
	AURORA_API A_VOID QueryMemory(
		_In_ const ProcessInfo32& refTargetProcess,
		_In_a32_ A_ADDR32 uAddress,
		_Out_ PMEMORY_BASIC_INFORMATION lpMbi
	);

	/// <summary>
	/// Queries information about one or more memory pages.
	/// </summary>
	/// <param name="refTargetProcess">- A reference to the target process.</param>
	/// <param name="uAddress">- The address to query information from.</param>
	/// <param name="lpMbi">- A pointer to a MEMORY_BASIC_INFORMATION structure to receive the queried information.</param>
	/// <exception cref="ParameterInvalidException"/>
	/// <exception cref="WindowsApiException"/>
	AURORA_API A_VOID QueryMemory(
		_In_ const ProcessInfo64& refTargetProcess,
		_In_a64_ A_ADDR64 uAddress,
		_Out_ PMEMORY_BASIC_INFORMATION lpMbi
	);
}

#endif // !__AURORA_MEMORY_WIN32_H__