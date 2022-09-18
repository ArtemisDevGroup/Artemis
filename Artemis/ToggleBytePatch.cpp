#include "ToggleBytePatch.h"

#include "SDK/MemApi_Write.h"

namespace api {
	/// <summary>
	/// Toggles an assembly patch.
	/// </summary>
	/// <param name="lpModule">- A pointer to a MODULE object containing information about the base module.</param>
	/// <param name="lpAssemblyPatch">- A pointer to an ASSEMBLY_PATCH object containing information about the patch.</param>
	/// <param name="bEnable">- A BOOL indicating whether to patch the enable or disable code.</param>
	void ToggleAssemblyPatch(
		_In_ LPMODULE lpModule,
		_In_ LPCASSEMBLY_PATCH lpAssemblyPatch,
		_In_ BOOL bEnable
	) {
		if (!lpModule) throw ParameterNullException("lpModule");
		if (!lpAssemblyPatch) throw ParameterNullException("lpAssemblyPatch");

		if (bEnable)
			WriteBytes(
				lpModule->uModuleBase + lpAssemblyPatch->uOffset,
				lpAssemblyPatch->szEnable,
				lpAssemblyPatch->dwCount
			);
		else
			WriteBytes(
				lpModule->uModuleBase + lpAssemblyPatch->uOffset,
				lpAssemblyPatch->szDisable,
				lpAssemblyPatch->dwCount
			);
	}
}