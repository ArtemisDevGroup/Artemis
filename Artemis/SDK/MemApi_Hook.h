#ifndef __MEMAPI_HOOK_H__
#define __MEMAPI_HOOK_H__

#include "Def.h"
#include "ErrorApi.h"

/// <summary>
/// Represents a function hook.
/// </summary>
typedef struct _HOOK {
	/// <summary>
	/// The name of the hooked API function. If the hooked function isn't an API function, this parameter will be set to NULL.
	/// </summary>
	CHAR szApiFunction[64];

	/// <summary>
	/// A pointer to the hooked function.
	/// </summary>
	LPVOID lpFunction;

	/// <summary>
	/// A pointer to the detour function.
	/// </summary>
	LPVOID lpDetour;

	/// <summary>
	/// A pointer to the original function.
	/// </summary>
	LPVOID lpOriginal;

	/// <summary>
	/// Shows whether the hook is enabled or disabled.
	/// </summary>
	BOOL bEnabled;

	/// <summary>
	/// Shows whether the hook is created or not.
	/// </summary>
	BOOL bSet;
} HOOK, * LPHOOK;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/// <summary>
	/// Initializes the hooking library. Is required in order to use the hook functions.
	/// </summary>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_MEMORY_FULL"/>
	BOOL XEInitHook();

	/// <summary>
	/// Uninitializes the hooking library. Is required in order to not leak memory before exiting the process.
	/// </summary>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	BOOL XEUninitHook();

	/// <summary>
	/// Creates a hook on a function.
	/// </summary>
	/// <param name="lpHook">- A pointer to a HOOK object to receive the hook information.</param>
	/// <param name="lpTarget">- A pointer to the target function to hook.</param>
	/// <param name="lpDetour">- A pointer to the detour/replacement function.</param>
	/// <param name="bEnable">- A bool to indicate whether the function should enable the hook directly after it's creation or not.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_HOOK_ALREADY_CREATED"/>
	/// <exception cref="XE_ERROR_MEMORY_FULL"/>
	/// <exception cref="XE_ERROR_HOOK_UNSUPPORTED"/>
	/// <exception cref="XE_ERROR_HOOK_NOT_EXECUTABLE"/>
	/// <exception cref="XE_ERROR_HOOK_NOT_INIT"/>
	BOOL XECreateHook(
		_Out_ LPHOOK lpHook,
		_In_ LPVOID lpTarget,
		_In_ LPVOID lpDetour,
		_In_ BOOL bEnable
	);

	/// <summary>
	/// Creates a hook on a dynamically imported function. (Ex Kernel32, User32, etc functions.)
	/// </summary>
	/// <param name="lpHook">- A pointer to a HOOK object to receive the hook information.</param>
	/// <param name="lpModule">- The name of the module containing the function to hook.</param>
	/// <param name="lpFunction">- The exact name of the function to hook.</param>
	/// <param name="lpDetour">- A pointer to the detour/replacement function.</param>
	/// <param name="bEnable">- A bool to indicate whether the function should enable the hook directly after it's creation or not.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_HOOK_ALREADY_CREATED"/>
	/// <exception cref="XE_ERROR_MEMORY_FULL"/>
	/// <exception cref="XE_ERROR_HOOK_UNSUPPORTED"/>
	/// <exception cref="XE_ERROR_HOOK_NOT_EXECUTABLE"/>
	/// <exception cref="XE_ERROR_HOOK_NOT_INIT"/>
	/// <exception cref="XE_ERROR_ENTRY_NOT_FOUND"/>
	BOOL XECreateApiHook(
		_Out_ LPHOOK lpHook,
		_In_ LPCSTR lpModule,
		_In_ LPCSTR lpFunction,
		_In_ LPVOID lpDetour,
		_In_ BOOL bEnable
	);

	/// <summary>
	/// Removes a hook from a function.
	/// </summary>
	/// <param name="lpHook">- A pointer to a HOOK object containing the hook information.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_HOOK_NOT_CREATED"/>
	/// <exception cref="XE_ERROR_HOOK_NOT_INIT"/>
	BOOL XERemoveHook(
		_In_ LPHOOK lpHook
	);

	/// <summary>
	/// Enables a hook.
	/// </summary>
	/// <param name="lpHook">- A pointer to a HOOK object containing the hook information.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_HOOK_NOT_CREATED"/>
	/// <exception cref="XE_ERROR_HOOK_NOT_INIT"/>
	BOOL XEEnableHook(
		_In_ LPHOOK lpHook
	);

	/// <summary>
	/// Disables a hook.
	/// </summary>
	/// <param name="lpHook">- A pointer to a HOOK object containing the hook information.</param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_HOOK_NOT_CREATED"/>
	/// <exception cref="XE_ERROR_HOOK_NOT_INIT"/>
	BOOL XEDisableHook(
		_In_ LPHOOK lpHook
	);

#ifdef __cplusplus
}

namespace api {
	/// <summary>
	/// A class for creating a function hook.
	/// </summary>
	class Hook {
		HOOK hk;
	public:
		/// <summary>
		/// Creates a hook on a function.
		/// </summary>
		/// <param name="lpTarget">- A pointer to the target function to hook.</param>
		/// <param name="lpDetour">- A pointer to the detour/replacement function.</param>
		/// <param name="bEnable">- A bool to indicate whether the function should enable the hook directly after it's creation or not.</param>
		/// <exception cref="ParameterNullException"/>
		/// <exception cref="HookAlreadyCreatedException"/>
		/// <exception cref="MemoryFullException"/>
		/// <exception cref="HookUnsupportedException"/>
		/// <exception cref="HookNotExecutableException"/>
		/// <exception cref="HookNotInitializedException"/>
		inline Hook(
			_In_ LPVOID lpTarget,
			_In_ LPVOID lpDetour,
			_In_ BOOL bEnable = TRUE
		) {
			if (!lpTarget) throw ParameterNullException("lpTarget");
			if (!lpDetour) throw ParameterNullException("lpDetour");

			if (!XECreateHook(
				&hk,
				lpTarget,
				lpDetour,
				bEnable
			)) {
				switch (XEGetLastError()) {
				case XE_ERROR_HOOK_ALREADY_CREATED:
					throw HookAlreadyCreatedException();
				case XE_ERROR_MEMORY_FULL:
					throw MemoryFullException();
				case XE_ERROR_HOOK_UNSUPPORTED:
					throw HookUnsupportedException();
				case XE_ERROR_HOOK_NOT_EXECUTABLE:
					throw HookNotExecutableException();
				case XE_ERROR_HOOK_NOT_INIT:
					throw HookNotInitializedException();
				}
			}
		}

		/// <summary>
		/// Creates a hook on a dynamically imported function. (Ex Kernel32, User32, etc functions.)
		/// </summary>
		/// <param name="lpModule">- The name of the module containing the function to hook.</param>
		/// <param name="lpFunction">- The exact name of the function to hook.</param>
		/// <param name="lpDetour">- A pointer to the detour/replacement function.</param>
		/// <param name="bEnable">- A bool to indicate whether the function should enable the hook directly after it's creation or not.</param>
		/// <exception cref="ParameterNullException"/>
		/// <exception cref="HookAlreadyCreatedException"/>
		/// <exception cref="MemoryFullException"/>
		/// <exception cref="HookUnsupportedException"/>
		/// <exception cref="HookNotExecutableException"/>
		/// <exception cref="HookNotInitializedException"/>
		/// <exception cref="EntryNotFoundException"/>
		inline Hook(
			_In_z_ LPCSTR lpModule,
			_In_z_ LPCSTR lpFunction,
			_In_ LPVOID lpDetour,
			_In_ BOOL bEnable = TRUE
		) {
			if (!lpModule) throw ParameterNullException("lpModule");
			if (!lpFunction) throw ParameterNullException("lpFunction");
			if (!lpDetour) throw ParameterNullException("lpDetour");

			if (!XECreateApiHook(
				&hk,
				lpModule,
				lpFunction,
				lpDetour,
				bEnable
			)) {
				switch (XEGetLastError()) {
				case XE_ERROR_ENTRY_NOT_FOUND:
					if (!GetModuleHandleA(lpModule)) throw EntryNotFoundException(lpModule);
					else throw EntryNotFoundException(lpFunction);
				case XE_ERROR_HOOK_ALREADY_CREATED:
					throw HookAlreadyCreatedException();
				case XE_ERROR_MEMORY_FULL:
					throw MemoryFullException();
				case XE_ERROR_HOOK_UNSUPPORTED:
					throw HookUnsupportedException();
				case XE_ERROR_HOOK_NOT_EXECUTABLE:
					throw HookNotExecutableException();
				case XE_ERROR_HOOK_NOT_INIT:
					throw HookNotInitializedException();
				}
			}
		}

		~Hook() { XERemoveHook(&hk); }

		/// <summary>
		/// Gets a const pointer to the current hook instance.
		/// </summary>
		/// <returns>A pointer to the hook instance.</returns>
		inline const LPHOOK GetHookInstance() const { return (const LPHOOK)&hk; }

		/// <summary>
		/// Gets the name of the hooked API function. If the hooked function is not an API function, this function will return nullptr.
		/// </summary>
		/// <returns>The API function name, or nullptr.</returns>
		inline LPCSTR GetApiFunctionName() const { return hk.szApiFunction[0] != '\0' ? hk.szApiFunction : nullptr; }

		/// <summary>
		/// Gets a pointer to the target function.
		/// </summary>
		/// <returns>A pointer to the target.</returns>
		inline LPVOID GetFunctionPtr() const { return hk.lpFunction; }

		/// <summary>
		/// Gets a pointer to the detour function.
		/// </summary>
		/// <returns>A pointer to the detour.</returns>
		inline LPVOID GetDetourPtr() const { return hk.lpDetour; }

		/// <summary>
		/// Gets a pointer to the original function. Call this address to call the original function.
		/// </summary>
		/// <returns>A pointer to the original.</returns>
		inline LPVOID GetOriginalFunctionPtr() const { return hk.lpOriginal; }

		/// <summary>
		/// Returns a bool that indicates whether the hook is enabled or not.
		/// </summary>
		/// <returns>TRUE if enabled, FALSE if not.</returns>
		inline BOOL IsEnabled() const { return hk.bEnabled; }

		/// <summary>
		/// Returns a bool that indicates whether the hook is set/created or not.
		/// </summary>
		/// <returns>TRUE if set, FALSE if not.</returns>
		inline BOOL IsSet() const { return hk.bSet; }

		/// <summary>
		/// Enables the hook.
		/// </summary>
		/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
		/// <exception cref="HookNotCreatedException"/>
		/// <exception cref="HookNotInitializedException"/>
		inline void Enable() {
			if (!XEEnableHook(&hk)) {
				switch (XEGetLastError()) {
				case XE_ERROR_HOOK_NOT_CREATED:
					throw HookNotCreatedException();
				case XE_ERROR_HOOK_NOT_INIT:
					throw HookNotInitializedException();
				}
			};
		}

		/// <summary>
		/// Disables the hook.
		/// </summary>
		/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
		/// <exception cref="HookNotCreatedException"/>
		/// <exception cref="HookNotInitializedException"/>
		inline void Disable() {
			if (!XEDisableHook(&hk)) {
				switch (XEGetLastError()) {
				case XE_ERROR_HOOK_NOT_CREATED:
					throw HookNotCreatedException();
				case XE_ERROR_HOOK_NOT_INIT:
					throw HookNotInitializedException();
				}
			}
		}

		/// <summary>
		/// Removes the hook from the target function.
		/// </summary>
		/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
		/// <exception cref="HookNotCreatedException"/>
		/// <exception cref="HookNotInitializedException"/>
		inline void Remove() {
			if (!XERemoveHook(&hk)) {
				switch (XEGetLastError()) {
				case XE_ERROR_HOOK_NOT_CREATED:
					throw HookNotCreatedException();
				case XE_ERROR_HOOK_NOT_INIT:
					throw HookNotInitializedException();
				}
			}
		}
	};
}
#endif // __cplusplus

#endif // !__MEMAPI_HOOK_H__