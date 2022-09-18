#ifndef __MEMSHAPI_H__
#define __MEMSHAPI_H__

#include "Def.h"
#include "ErrorApi.h"

#define CE_MIN_ADDR						0				// The default minimum scan address in Cheat Engine.
#define CE_MAX_ADDR						0x7fffffffffff  // The default maximum scan address in Cheat Engine.
#define MAX_PATTERN_LENGTH				256				// The maximum length of a code style pattern and mask.
#define MAX_POINTER_LENGTH				64				// The maximum number of pointer offsets.

#ifdef _WIN64
/// <summary>
/// Represents a virtual memory address.
/// </summary>
typedef unsigned __int64 ADDRESS, * LPADDRESS;
#else
/// <summary>
/// Represents a virtual memory address.
/// </summary>
typedef unsigned __int32 ADDRESS, * LPADDRESS;
#endif

/// <summary>
/// Represents a set of pointer offsets.
/// </summary>
typedef struct _OFFSET {
	/// <summary>
	/// An array of pointer offsets.
	/// </summary>
	ADDRESS szOffsets[MAX_POINTER_LENGTH];

	/// <summary>
	/// The number of pointer offsets set in the array.
	/// </summary>
	DWORD dwCount;
} OFFSET, * LPOFFSET;

#ifdef __cplusplus

#include <initializer_list> // std::intializer_list

#define NEGATIVE_OFFSET(n) (ADDRESS)(-n) // Represents a negative offset.

/// <summary>
/// Represents a set of pointer offsets.
/// </summary>
class OFFSETS {
	std::initializer_list<ADDRESS> list;
public:

	/// <summary>
	/// Constructs an object of the offsets class.
	/// </summary>
	/// <param name="List">- An array of pointer offsets.</param>
	inline OFFSETS(std::initializer_list<ADDRESS> List) : list(List) {}

	/// <summary>
	/// Enables the usage of the [] operator on the class object to access the stored values.
	/// </summary>
	/// <param name="i">- The index of the offset to access.</param>
	/// <returns></returns>
	inline ADDRESS operator[](INT i) const { return list.begin()[i]; }

	/// <summary>
	/// Gets the number of offsets in the array.
	/// </summary>
	/// <returns>The number of offsets in the array.</returns>
	inline SIZE_T Size() const { return list.size(); }

	/// <summary>
	/// Gets a pointer to the first item in the array.
	/// </summary>
	/// <returns>A pointer to the first item in the array.</returns>
	inline const LPADDRESS Ptr() const { return (const LPADDRESS)list.begin(); }
};

#endif // __cplusplus

/// <summary>
/// Represents a set of memory protection levels. Used by the memory scanners.
/// </summary>
typedef enum _SCAN_PROTECTION {
	/// <summary>
	/// Scans exclusively executable readwrite memory.
	/// </summary>
	SP_EXECUTE_READWRITE,

	/// <summary>
	/// Scans exclusively executable readonly memory.
	/// </summary>
	SP_EXECUTE_READONLY,

	/// <summary>
	/// Scans both readwrite and readonly executable memory.
	/// </summary>
	SP_EXECUTE_READ,

	/// <summary>
	/// Scans exclusively non-executable readwrite memory.
	/// </summary>
	SP_NOEXECUTE_READWRITE,

	/// <summary>
	/// Scans exclusively non-executable readonly memory.
	/// </summary>
	SP_NOEXECUTE_READONLY,

	/// <summary>
	/// Scans both readwrite and readonly non-executable memory.
	/// </summary>
	SP_NOEXECUTE_READ,

	/// <summary>
	/// Scans all readwrite memory.
	/// </summary>
	SP_READWRITE,

	/// <summary>
	/// Scans all readonly memory.
	/// </summary>
	SP_READONLY,

	/// <summary>
	/// Scans all readable memory.
	/// </summary>
	SP_READ
} SCAN_PROTECTION;

typedef MEMORY_BASIC_INFORMATION MBI, * LPMBI;

/// <summary>
/// Represents a memory page protection.
/// </summary>
typedef enum _MEMORY_PROTECTION {
	/// <summary>
	/// Represents a readonly memory page.
	/// </summary>
	MP_READONLY = PAGE_READONLY,

	/// <summary>
	/// Represents a read and writable memory page.
	/// </summary>
	MP_READWRITE = PAGE_READWRITE,

	/// <summary>
	/// Represents a readonly executable memory page.
	/// </summary>
	MP_EXECUTE_READONLY = PAGE_EXECUTE_READ,

	/// <summary>
	/// Represents a read and writable executable memory page.
	/// </summary>
	MP_EXECUTE_READWRITE = PAGE_EXECUTE_READWRITE
} MEMORY_PROTECTION, * LPMEMORY_PROTECTION;

/// <summary>
/// Represents an allocation type.
/// </summary>
typedef enum _ALLOCATION_TYPE {
	/// <summary>
	/// Commits reserved memory to make it available for usage.
	/// </summary>
	AT_COMMIT = MEM_COMMIT,

	/// <summary>
	/// Reserves memory for later use.
	/// </summary>
	AT_RESERVE = MEM_RESERVE,

	/// <summary>
	/// Reserves and commits memory in the same action. In other words immediately creates a usable allocation.
	/// </summary>
	AT_COMMIT_RESERVE = MEM_COMMIT | MEM_RESERVE,

	/// <summary>
	/// Allocates as high of an address as possible.
	/// </summary>
	AT_TOPDOWN = MEM_TOP_DOWN
} ALLOCATION_TYPE;

/// <summary>
/// Represents a free type.
/// </summary>
typedef enum _FREE_TYPE {
	/// <summary>
	/// Releases the allocation, freeing the memory.
	/// </summary>
	FT_RELEASE = MEM_RELEASE,

	/// <summary>
	/// Decommits the allocation, putting the allocation in the reserved state.
	/// </summary>
	FT_DECOMMIT = MEM_DECOMMIT
} FREE_TYPE;

/// <summary>
/// Contains information about a memory allocation made using XEVirtualAlloc.
/// </summary>
typedef struct _VIRTUAL_ALLOC {
	/// <summary>
	/// The address requested to be allocated.
	/// </summary>
	ADDRESS uRequestedAddress;

	/// <summary>
	/// The address allocated.
	/// </summary>
	ADDRESS uAllocatedAddress;

	/// <summary>
	/// The size of the allocation.
	/// </summary>
	DWORD dwAllocatedSize;

	/// <summary>
	/// The type of the allocation.
	/// </summary>
	ALLOCATION_TYPE AllocationType;

	/// <summary>
	/// The memory page protection of the allocation.
	/// </summary>
	MEMORY_PROTECTION MemoryProtection;
} VIRTUAL_ALLOC, * LPVIRTUAL_ALLOC;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/// <summary>
	/// Checks a Vector mask to see if it is valid or not.
	/// </summary>
	/// <param name="lpMask">- The mask to check.</param>
	/// <param name="uCount">- The vector count. Ex: Vec4 is 4, Vec3 is 3 etc.</param>
	/// <returns>A bool that indicates whether the mask is valid or not.</returns>
	BOOL XECheckVecMask(_In_z_ LPCSTR lpMask, _In_ SIZE_T uCount);

	/// <summary>
	/// Parses an IDA style pattern into a code style pattern and mask.
	/// </summary>
	/// <param name="lpComboPattern">- The IDA pattern to parse.</param>
	/// <param name="lpPattern">- A pointer to a buffer receiving the parsed pattern.</param>
	/// <param name="uPatternSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <param name="lpMask">- A pointer to a buffer receiving the parsed mask.</param>
	/// <param name="uMaskSize">- The size of the buffer pointed to by the previous pointer.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_PARAMETER_INVALID"/>
	/// <exception cref="XE_ERROR_BUFFER_SMALL"/>
	BOOL XEParsePattern(
		_In_z_ LPCSTR lpComboPattern,
		_Out_writes_(uPatternSize) LPSTR lpPattern,
		_In_ SIZE_T uPatternSize,
		_Out_writes_z_(uMaskSize) LPSTR lpMask,
		_In_ SIZE_T uMaskSize
	);

	/// <summary>
	/// Checks if the passed SCAN_PROTECTION is a valid protection.
	/// </summary>
	/// <param name="nProtection">- The protection value to check.</param>
	/// <returns>A bool that indicates whether the value is valid or not.</returns>
	BOOL XEIsProtectionValid(_In_ SCAN_PROTECTION nProtection);

	/// <summary>
	/// Resolves whether the current memory page protection matches up agains the desired protection.
	/// </summary>
	/// <param name="nDesiredProtection">- The desired protection.</param>
	/// <param name="dwCurrentProtection">- The current page protection.</param>
	/// <returns>A bool that indicates whether the current page protection matches the desired protection.</returns>
	BOOL XEResolveProtection(_In_ SCAN_PROTECTION nDesiredProtection, _In_ DWORD dwCurrentProtection);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !__MEMSHAPI_H__