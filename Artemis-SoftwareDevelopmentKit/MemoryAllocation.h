//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __MEMORY_ALLOCATION_H__
#define __MEMORY_ALLOCATION_H__

#include "Definitions.h"
#include "MemoryDefinitions.h"
#include "Interfaces.h"

namespace Artemis {
	/// <summary>
	/// Represents a memory page protection constant.
	/// </summary>
	enum class MemoryProtection {
		/// <summary>
		/// Executable readonly memory.
		/// </summary>
		Execute_ReadOnly = PAGE_EXECUTE_READ,

		/// <summary>
		/// Executable read and writable memory.
		/// </summary>
		Execute_ReadWrite = PAGE_EXECUTE_READWRITE,

		/// <summary>
		/// Non-executable readonly memory.
		/// </summary>
		ReadOnly = PAGE_READONLY,

		/// <summary>
		/// Non-executable read and writable memory.
		/// </summary>
		ReadWrite = PAGE_READWRITE
	};

	/// <summary>
	/// Constants that determine how memory should be allocated.
	/// </summary>
	enum class AllocationType {
		/// <summary>
		/// Commits the memory in the region. This means it makes the memory usable. Cannot be used if the memory is not reserved.
		/// </summary>
		Commit = MEM_COMMIT,

		/// <summary>
		/// Reserves the memory region. This means the memory is on standby and cannot be allocated for something else.
		/// </summary>
		Reserve = MEM_RESERVE,

		/// <summary>
		/// Allocates the highest memory page possible. This means it'll give you a super high address.
		/// </summary>
		TopDown = MEM_TOP_DOWN
	};

	/// <summary>
	/// Constants that determine the way a memory allocation should be freed.
	/// </summary>
	enum class FreeType {
		/// <summary>
		/// Decommits the memory and puts it in a reserved state.
		/// </summary>
		Decommit = MEM_DECOMMIT,

		/// <summary>
		/// Releases the memory to be used by the process.
		/// </summary>
		Release = MEM_RELEASE
	};

	inline AllocationType operator|(AllocationType l, AllocationType r) { return (AllocationType)((DWORD)l | (DWORD)r); }

	/// <summary>
	/// A class for creating and freeing virtual memory allocations.
	/// </summary>
	class ARTEMIS_API VirtualAllocation : public IDisposable {
		HANDLE hProcess;
		ADDRESS uAddress;
		SIZE_T uSize;
		AllocationType Type;
		MemoryProtection Protection;
		FreeType DisposalType;
		ADDRESS uAllocatedAddress;

	public:
		VirtualAllocation();
		~VirtualAllocation();

		/// <summary>
		/// Constructs a VirtualAllocation object based on the provided parameters.
		/// </summary>
		/// <param name="uAddress">- The address to allocate at.</param>
		/// <param name="uSize">- The number of bytes to allocate.</param>
		/// <param name="Type">- The type of allocation to make.</param>
		/// <param name="Protection">- The memory protection of the allocation.</param>
		/// <param name="DisposalType">- The type of memory release to do when disposing of the allocation.</param>
		VirtualAllocation(
			_In_ ADDRESS uAddress,
			_In_ SIZE_T uSize,
			_In_ AllocationType Type = AllocationType::Commit | AllocationType::Reserve,
			_In_ MemoryProtection Protection = MemoryProtection::ReadWrite,
			_In_ FreeType DisposalType = FreeType::Release
		);

		/// <summary>
		/// Sets the handle to use in the event that an external process' virtual memory is being allocated.
		/// </summary>
		/// <param name="hProcess">- A handle to the process.</param>
		void SetExternalHandle(_In_ HANDLE hProcess);

		/// <summary>
		/// Allocates a memory region based on the parameters set in the constructor.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Allocate();

		/// <summary>
		/// Releases the memory allocated by the instance.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Release();

		/// <summary>
		/// Gets the real address of the allocated memory region.
		/// </summary>
		/// <returns>The address of the region.</returns>
		_Check_return_ ADDRESS GetAllocatedAddress() const;

		/// <summary>
		/// Gets the real address of the allocated memory region as a pointer to the templated type.
		/// </summary>
		/// <typeparam name="T">- The type to return a pointer of.</typeparam>
		/// <returns>A pointer to the region.</returns>
		template<typename T>
		inline _Check_return_ T* GetAllocatedAddress() const { return (T*)uAllocatedAddress; }

		/// <summary>
		/// Reads the specified number of bytes from the allocation.
		/// </summary>
		/// <param name="nIndex">- The index to start writing to in the allocation. 0 means it reads from the first byte and onwards in the allocation.
		/// <para>Ex: If you want to read bytes 10, 11 and 12 in the allocation, you pass 10 to nIndex and 3 to dwSize.</para></param>
		/// <param name="lpBuffer">- A pointer to a buffer to write data to.</param>
		/// <param name="dwSize">- The size of the buffer in bytes.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		void Read(
			_In_ INT nIndex,
			_Out_ LPVOID lpBuffer,
			_In_ DWORD dwSize
		) const;

		/// <summary>
		/// Writes the specified number of bytes from the allocation.
		/// </summary>
		/// <param name="nIndex">- The index to start writing to in the allocation. 0 means it writes to the first byte and onwards in the allocation.
		/// <para>Ex: If you want to write to bytes 10, 11 and 12 in the allocation, you pass 10 to nIndex and 3 to dwSize.</para></param>
		/// <param name="lpData">- A pointer to the buffer containing the data to write.</param>
		/// <param name="dwSize">- The size of the buffer in bytes.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="WindowsApiException (External)"/>
		/// <exception cref="MemoryAccessViolationException (Internal)"/>
		void Write(
			_In_ INT nIndex,
			_In_ LPCVOID lpData,
			_In_ DWORD dwSize
		) const;
	};
}

#endif // !__MEMORY_ALLOCATION_H__