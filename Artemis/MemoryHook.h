#ifndef __MEMORY_HOOK_H__
#define __MEMORY_HOOK_H__

#include "Definitions.h"
#include "MemoryDefinitions.h"
#include "Interfaces.h"

namespace Artemis {
	/// <summary>
	/// Á class for hooking functions. This class wraps around the MinHook library.
	/// </summary>
	class Hook : public IDisposable {
		LPVOID lpTarget;

	public:
		/// <summary>
		/// Initializes the hooking library.
		/// </summary>
		/// <exception cref="HookException"/>
		static void InitializeLibrary();

		/// <summary>
		/// Uninitializes the hooking library.
		/// </summary>
		/// <exception cref="HookException"/>
		static void UninitializeLibrary();

		Hook();

		/// <summary>
		/// Creates a hook on the API function provided.
		/// </summary>
		/// <param name="lpModule">- The module where the function is contained.</param>
		/// <param name="lpFunction">- The name of the function to fetch from the export table.</param>
		/// <param name="lpDetour">- The function to replace with.</param>
		/// <param name="lpOriginal">- A pointer to call for the original function.</param>
		/// <exception cref="HookException"/>
		Hook(
			_In_z_ LPCSTR lpModule,
			_In_z_ LPCSTR lpFunction,
			_In_ LPVOID lpDetour,
			_Out_opt_ LPVOID* lpOriginal = nullptr
		);

		/// <summary>
		/// Creates a hook on the API function provided.
		/// </summary>
		/// <param name="lpModule">- A handle to the module where the function is contained.</param>
		/// <param name="lpFunction">- The name of the function to fetch from the export table.</param>
		/// <param name="lpDetour">- The function to replace with.</param>
		/// <param name="lpOriginal">- A pointer to call for the original function.</param>
		/// <exception cref="HookException"/>
		Hook(
			_In_ HMODULE lpModule,
			_In_z_ LPCSTR lpFunction,
			_In_ LPVOID lpDetour,
			_Out_opt_ LPVOID* lpOriginal = nullptr
		);

		/// <summary>
		/// Creates a hook on the target function.
		/// </summary>
		/// <param name="lpTarget">- The target function.</param>
		/// <param name="lpDetour">- The function to replace with.</param>
		/// <param name="lpOriginal">- A pointer to call for the original function.</param>
		/// <exception cref="HookException"/>
		Hook(
			_In_ LPVOID lpTarget,
			_In_ LPVOID lpDetour,
			_Out_opt_ LPVOID* lpOriginal = nullptr
		);

		/// <summary>
		/// Creates a hook on the function at the target address.
		/// </summary>
		/// <param name="uTarget">- The address of the function.</param>
		/// <param name="lpDetour">- The function to replace with.</param>
		/// <param name="lpOriginal">- A pointer to call for the original function.</param>
		/// <exception cref="HookException"/>
		Hook(
			_In_ ADDRESS uTarget,
			_In_ LPVOID lpDetour,
			_Out_opt_ LPVOID* lpOriginal = nullptr
		);

		/// <summary>
		/// Enables the hook.
		/// </summary>
		/// <exception cref="HookException"/>
		void Enable();

		/// <summary>
		/// Disables the hook.
		/// </summary>
		/// <exception cref="HookException"/>
		void Disable();

		/// <summary>
		/// Releases the hook instance by removing the hook.
		/// </summary>
		/// <exception cref="HookException"/>
		void Release();
	};

}

#endif // !__MEMORY_HOOK_H__