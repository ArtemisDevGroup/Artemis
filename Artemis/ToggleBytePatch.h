#ifndef __TOGGLE_BYTE_PATCH_H__
#define __TOGGLE_BYTE_PATCH_H__

#include "SDK/Def.h"
#include "SDK/MemApi_Info.h"

typedef UINT BASE_OFFSET;
typedef UINT PTR_OFFSET;

typedef struct _ASSEMBLY_PATCH {
	BASE_OFFSET uOffset;
	BYTE szEnable[16];
	BYTE szDisable[16];
	DWORD dwCount;
} ASSEMBLY_PATCH, * LPASSEMBLY_PATCH;

typedef const LPASSEMBLY_PATCH LPCASSEMBLY_PATCH;

#ifdef __cplusplus

namespace api {
	void ToggleAssemblyPatch(
		_In_ LPMODULE lpModule,
		_In_ LPCASSEMBLY_PATCH lpAssemblyPatch,
		_In_ BOOL bEnable
	);
}

#endif // __cplusplus

#endif // !__TOGGLE_BYTE_PATCH_H__