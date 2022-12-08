//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __MEMORY_SHARED_DEFINITIONS_H__
#define __MEMORY_SHARED_DEFINITIONS_H__

#include "Definitions.h"

#define MAX_POINTER_LENGTH				64							// The maximum number of pointer offsets.
#define MAX_ASSEMBLY_PATCH_SIZE			64							// The maximum number of bytes in an assembly patch.
#define MIN_ADDRESS						0x10000						// The lowest application address.
#define MAX_ADDRESS						0x7FFFFFFFFFFF				// The highest application address.
#define MAX_SCAN_RESULT_COUNT			256							// The maximum number of results in a memory scan.
#define MAX_PATTERN_LENGTH				64							// The maximum pattern character length.
#define MAX_STRING_LENGTH				256							// The maximum string length.

#undef ADDRESS
#undef LPADDRESS
#undef LPCADDRESS

namespace Artemis {

#ifdef _WIN64
	typedef unsigned __int64		ADDRESS,		* LPADDRESS;		// Represents a memory address.
#else
	typedef unsigned __int32		ADDRESS,		* LPADDRESS;		// Represents a memory address.
#endif
	typedef const ADDRESS*			LPCADDRESS;							// Represents a memory address.

	typedef UINT					BASE_OFFSET,	* LPBASE_OFFSET;	// Represents an offset off of the base address.
	typedef const BASE_OFFSET*		LPCBASE_OFFSET;						// Represents an offset off of the base address.

	typedef ADDRESS					POINTER_OFFSET, * LPPOINTER_OFFSET; // Represents a pointer offset.
	typedef const POINTER_OFFSET* LPCPOINTER_OFFSET;					// Represents a pointer offset.

	typedef MEMORY_BASIC_INFORMATION MBI,			* LPMBI;			// An alias to the MEMORY_BASIC_INFORMATION structure.

	// Represents a pointer off of a base offset.
	typedef struct _BASE_POINTER {
		BASE_OFFSET uBaseOffset;										// The offset off of the base address.
		POINTER_OFFSET szPointer[MAX_POINTER_LENGTH];					// An array containing the pointer offsets.
		DWORD dwCount;													// The number of pointer offsets set in the array.
	} BASE_POINTER, * LPBASE_POINTER;
	typedef const BASE_POINTER* LPCBASE_POINTER;						// Represents a pointer off of a base offset.
	// Represents a pointer.
	typedef struct _POINTER {
		POINTER_OFFSET szPointer[MAX_POINTER_LENGTH];					// An array containing the pointer offsets.
		DWORD dwCount;													// The number of pointer offsets set in the array.
	} POINTER, * LPPOINTER;
	typedef const POINTER* LPCPOINTER;									// Represents a pointer.
	// Represents an assembly patch off of a base offset.
	typedef struct _BASE_ASM_PATCH {
		BASE_OFFSET uBaseOffset;										// The offset off of the base address.
		BYTE szEnable[MAX_ASSEMBLY_PATCH_SIZE];							// An array of bytes to be written to "enable" the patch.
		BYTE szDisable[MAX_ASSEMBLY_PATCH_SIZE];						// An array of bytes to be written to "disable" the patch.
		DWORD dwCount;													// The number of bytes set in the array.
	} BASE_ASM_PATCH, * LPBASE_ASM_PATCH;
	typedef const BASE_ASM_PATCH* LPCBASE_ASM_PATCH;					// Represents an assembly patch off of a base offset.
	// Represents an assembly patch.
	typedef struct _ASM_PATCH {
		BYTE szEnable[MAX_ASSEMBLY_PATCH_SIZE];							// An array of bytes to be written to "enable" the patch.
		BYTE szDisable[MAX_ASSEMBLY_PATCH_SIZE];						// An array of bytes to be written to "disable" the patch.
		DWORD dwCount;
	} ASM_PATCH, * LPASM_PATCH;
	typedef const ASM_PATCH* LPCASM_PATCH;								// Represents an assembly patch.
	// Represents a memory allocation.
	typedef struct _ALLOCATION {
		ADDRESS uAddress;												// The address of the allocation.
		DWORD dwSize;													// The size of the allocation in bytes.
	} ALLOCATION, * LPALLOCATION;
	typedef const ALLOCATION* LPCALLOCATION;							// Represents a memory allocation.
	// Represents a memory allocation made using the Virtual function set. (WinApi)
	typedef struct _VIRTUAL_ALLOCATION {
		ALLOCATION Allocation;											// The basic allocation information.
		DWORD dwProtect;												// The memory page protection constant.
		DWORD dwAllocationType;											// The allocation type.
	} VIRTUAL_ALLOCATION, * LPVIRTUAL_ALLOCATION;
	typedef const VIRTUAL_ALLOCATION* LPCVIRTUAL_ALLOCATION;			// Represents a memory allocation made using the Virtual function set. (WinApi)
	// Represents a process running on the system.
	typedef struct _PROCESS {
		CHAR szName[MAX_NAME];											// The name of the process executable.
		CHAR szPath[MAX_PATH];											// A full path to the process executable.
		DWORD dwProcessId;												// The unique process identifier.
		HANDLE hProcess;												// A handle to the process.
	} PROCESS, * LPPROCESS;
	typedef const PROCESS* LPCPROCESS;									// Represents a process running on the system.
	// Represents a module loaded in a process.
	typedef struct _MODULE {
		CHAR szName[MAX_NAME];											// The name of the module binary.
		CHAR szPath[MAX_PATH];											// A full path to the module binary.
		HMODULE hModule;												// A handle to the module.
		ALLOCATION Allocation;											// Information about the module base allocation.
		PROCESS Owner;													// Information about the host process.
	} MODULE, * LPMODULE;
	typedef const MODULE* LPCMODULE;									// Represents a module loaded in a process.
	// Represents a thread running in a process.
	typedef struct _THREAD {
		DWORD dwThreadId;												// The unique thread identifier.
		HANDLE hThread;													// A handle to the thread.
		PROCESS Owner;													// Information about the host process.
	} THREAD, * LPTHREAD;
	typedef const THREAD* LPCTHREAD;									// Represents a thread running in a process.
	// Represents a function hook.
	typedef struct _HOOK {
		CHAR szHostModule[MAX_NAME];									// The name of the module that hosts the function. Can be null.
		CHAR szFunctionName[MAX_NAME];									// The name of the function in the export table. Can be null.
		LPVOID lpTarget;												// A pointer to the target function.
		LPVOID lpDetour;												// A pointer to the detour or hook function.
		LPVOID lpOriginal;												// A pointer to the original function.
	} HOOK, * LPHOOK;
	typedef const HOOK* LPCHOOK;										// Represents a function hook.

}

#endif // !__MEMORY_SHARED_DEFINITIONS_H__