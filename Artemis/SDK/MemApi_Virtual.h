#ifndef __MEMAPI_VIRTUAL_H__
#define __MEMAPI_VIRTUAL_H__

#include "Def.h"
#include "ErrorApi.h"
#include "MemShApi.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/// <summary>
	/// Changes the protection of one or more memory pages.
	/// </summary>
	/// <param name="uAddress">- The address to change the protections of.</param>
	/// <param name="dwSize">- The number of bytes to change.</param>
	/// <param name="MemoryProtection">- The memory protection to apply.</param>
	/// <param name="lpOldProtection">- The previous memory protection.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XEVirtualProtect(
		_In_ ADDRESS uAddress,
		_In_ DWORD dwSize,
		_In_ MEMORY_PROTECTION MemoryProtection,
		_Out_opt_ LPMEMORY_PROTECTION lpOldProtection
	);

	/// <summary>
	/// Allocates a chunk of memory on the heap.
	/// </summary>
	/// <param name="lpVirtualAlloc">- A pointer to a VIRTUAL_ALLOC object to receive information about the allocation.</param>
	/// <param name="uAddress">- The address to attempt to allocate at.</param>
	/// <param name="dwSize">- The number of bytes to allocate.</param>
	/// <param name="AllocationType">- The type of allocation to allocate.</param>
	/// <param name="MemoryProtection">- The memory page protection to apply to the allocation.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	BOOL XEVirtualAlloc(
		_Out_ LPVIRTUAL_ALLOC lpVirtualAlloc,
		_In_opt_ ADDRESS uAddress,
		_In_ DWORD dwSize,
		_In_ ALLOCATION_TYPE AllocationType,
		_In_ MEMORY_PROTECTION MemoryProtection
	);

	/// <summary>
	/// Frees an allocation previously allocated by XEVirtualAlloc.
	/// </summary>
	/// <param name="lpVirtualAlloc">- A pointer to a VIRTUAL_ALLOC object containing the allocation information.</param>
	/// <param name="FreeType">- The free type that determines how to free the allocation.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	BOOL XEVirtualFree(
		_In_ LPVIRTUAL_ALLOC lpVirtualAlloc,
		_In_ FREE_TYPE FreeType
	);

	/// <summary>
	/// Queries information about one or more pages of memory.
	/// </summary>
	/// <param name="uAddress">- The address of the page(s) to query.</param>
	/// <param name="lpMbi">- A pointer to a MBI object to receive the retrieved information.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	BOOL XEVirtualQuery(
		_In_ ADDRESS uAddress,
		_Out_ LPMBI lpMbi
	);

#ifdef __cplusplus
}

namespace api {
	class VirtualAllocation {
		VIRTUAL_ALLOC valloc;
	public:
		inline VirtualAllocation() : valloc() {}

		~VirtualAllocation() { XEVirtualFree(&valloc, FT_RELEASE); }

		/// <summary>
		/// Gets a const pointer to the current allocation instance.
		/// </summary>
		/// <returns>A pointer to the allocation instance.</returns>
		const LPVIRTUAL_ALLOC GetAllocationInstance() const { return (const LPVIRTUAL_ALLOC)&valloc; }

		/// <summary>
		/// Gets the address requested in the allocation call.
		/// </summary>
		/// <returns>The address requested when making the allocation.</returns>
		ADDRESS GetRequestedAddress() const { return valloc.uRequestedAddress; }

		/// <summary>
		/// Gets the address of the current allocation.
		/// </summary>
		/// <returns>The address of the allocation.</returns>
		ADDRESS GetAllocationAddress() const { return valloc.uAllocatedAddress; }

		/// <summary>
		/// Gets the size of the current allocation in bytes.
		/// </summary>
		/// <returns>The size of the allocation.</returns>
		DWORD GetAllocationSize() const { return valloc.dwAllocatedSize; }

		/// <summary>
		/// Gets the type of the allocation.
		/// </summary>
		/// <returns>The allocation type.</returns>
		ALLOCATION_TYPE GetAllocationType() const { valloc.AllocationType; }

		/// <summary>
		/// Gets the memory protection of the allocation.
		/// </summary>
		/// <returns>The allocation protection.</returns>
		MEMORY_PROTECTION GetMemoryProtection() const { valloc.MemoryProtection; }

		/// <summary>
		/// Allocates memory at or near the provided address.
		/// </summary>
		/// <param name="uAddress">- The address to target. If this parameter is 0 the system will determine where to place the allocation.</param>
		/// <param name="dwSize">- The number of bytes to allocate.</param>
		/// <param name="AllocationType">- The type of allocation to allocate.</param>
		/// <param name="MemoryProtection">- The protection of the allocated memory.</param>
		/// <exception cref="WinApiException"/>
		void Alloc(
			_In_opt_ ADDRESS uAddress,
			_In_ DWORD dwSize,
			_In_ ALLOCATION_TYPE AllocationType = (ALLOCATION_TYPE)(AT_COMMIT | AT_RESERVE),
			_In_ MEMORY_PROTECTION MemoryProtection = MP_READWRITE
		) {
			if (!XEVirtualAlloc(
				&valloc,
				uAddress,
				dwSize,
				AllocationType,
				MemoryProtection
			)) throw WinApiException("VirtualAlloc");
		}

		/// <summary>
		/// Frees the previously allocated memory.
		/// </summary>
		/// <param name="FreeType">- The type of way to free the memory.</param>
		/// <exception cref="WinApiException"/>
		void Free(_In_ FREE_TYPE FreeType = FT_RELEASE) {
			if (!XEVirtualFree(
				&valloc,
				FreeType
			)) throw WinApiException("VirtualFree");
		}
	};

	/// <summary>
	/// Changes the protection of one or more memory pages.
	/// </summary>
	/// <param name="uAddress">- The address to change the protections of.</param>
	/// <param name="dwSize">- The number of bytes to change.</param>
	/// <param name="MemoryProtection">- The memory protection to apply.</param>
	/// <param name="lpOldProtection">- The previous memory protection.</param>
	/// <exception cref="WinApiException"/>
	BOOL VirtualProtect(
		_In_ ADDRESS uAddress,
		_In_ DWORD dwSize,
		_In_ MEMORY_PROTECTION MemoryProtection,
		_Out_opt_ LPMEMORY_PROTECTION lpOldProtection = nullptr
	) {
		if (!XEVirtualProtect(
			uAddress,
			dwSize,
			MemoryProtection,
			lpOldProtection
		)) throw WinApiException("VirtualProtect");
	}

	/// <summary>
	/// Allocates a chunk of memory on the heap.
	/// </summary>
	/// <param name="lpVirtualAlloc">- A pointer to a VIRTUAL_ALLOC object to receive information about the allocation.</param>
	/// <param name="uAddress">- The address to attempt to allocate at.</param>
	/// <param name="dwSize">- The number of bytes to allocate.</param>
	/// <param name="AllocationType">- The type of allocation to allocate.</param>
	/// <param name="MemoryProtection">- The memory page protection to apply to the allocation.</param>
	/// <exception cref="WinApiException"/>
	/// <exception cref="ParameterNullException"/>
	void VirtualAlloc(
		_Out_ LPVIRTUAL_ALLOC lpVirtualAlloc,
		_In_opt_ ADDRESS uAddress,
		_In_ DWORD dwSize,
		_In_ ALLOCATION_TYPE AllocationType = (ALLOCATION_TYPE)(AT_COMMIT | AT_RESERVE),
		_In_ MEMORY_PROTECTION MemoryProtection = MP_READWRITE
	) {
		if (!lpVirtualAlloc) throw ParameterNullException("lpVirtualAlloc");

		if (!XEVirtualAlloc(
			lpVirtualAlloc,
			uAddress,
			dwSize,
			AllocationType,
			MemoryProtection
		)) throw WinApiException("VirtualAlloc");
	}

	/// <summary>
	/// Frees an allocation previously allocated by XEVirtualAlloc.
	/// </summary>
	/// <param name="lpVirtualAlloc">- A pointer to a VIRTUAL_ALLOC object containing the allocation information.</param>
	/// <param name="FreeType">- The free type that determines how to free the allocation.</param>
	/// <exception cref="WinApiException"/>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="ParameterInvalidException"/>
	void VirtualFree(
		_In_ LPVIRTUAL_ALLOC lpVirtualAlloc,
		_In_ FREE_TYPE FreeType = FT_RELEASE
	) {
		if (!lpVirtualAlloc) throw ParameterNullException("lpVirtualAlloc");
		if (FreeType != FT_RELEASE && FreeType != FT_DECOMMIT) throw ParameterInvalidException("FreeType");

		if (!XEVirtualFree(
			lpVirtualAlloc,
			FreeType
		)) throw WinApiException("VirtualFree");
	}

	/// <summary>
	/// Queries information about one or more pages of memory.
	/// </summary>
	/// <param name="uAddress">- The address of the page(s) to query.</param>
	/// <param name="lpMbi">- A pointer to a MBI object to receive the retrieved information.</param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="WinApiException"/>
	void VirtualQuery(
		_In_ ADDRESS uAddress,
		_Out_ LPMBI lpMbi
	) {
		if (!lpMbi) throw ParameterNullException("lpMbi");

		if (!XEVirtualQuery(
			uAddress,
			lpMbi
		)) throw WinApiException("VirtualQuery");
	}
}
#endif // __cplusplus

#endif // !__MEMAPI_VIRTUAL_H__