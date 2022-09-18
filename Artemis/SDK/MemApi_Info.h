#ifndef __MEMAPI_INFO_H__
#define __MEMAPI_INFO_H__

#include "Def.h"
#include "ErrorApi.h"
#include "MemShApi.h"

/// <summary>
/// Contains information about a specific module and it's owning process.
/// </summary>
typedef struct _MODULE {
	/// <summary>
	/// The name of the loaded module.
	/// </summary>
	CHAR szModuleName[64];

	/// <summary>
	/// The path to the loaded module.
	/// </summary>
	CHAR szModulePath[MAX_PATH];

	/// <summary>
	/// A handle to the loaded module.
	/// </summary>
	HMODULE hModule;

	/// <summary>
	/// The base address of the module allocation.
	/// </summary>
	ADDRESS uModuleBase;

	/// <summary>
	/// The size of the module allocation.
	/// </summary>
	DWORD dwModuleSize;

	struct {
		/// <summary>
		/// The name of the process owning the represented module.
		/// </summary>
		CHAR szProcessName[64];

		/// <summary>
		/// The path to the process owning the represented module.
		/// </summary>
		CHAR szProcessPath[MAX_PATH];

		/// <summary>
		/// A handle to the process owning the represented module.
		/// </summary>
		HANDLE hProcess;

		/// <summary>
		/// The process id of the process owning the represented module.
		/// </summary>
		DWORD dwOwnerProcessId;
	} OwningProcess;
} MODULE, * LPMODULE;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/// <summary>
	/// Retrieves information about a module and it's owning process.
	/// </summary>
	/// <param name="lpModule">- A pointer to a MODULE object to receive the retrieved information.</param>
	/// <param name="lpModuleName">- The name of the module to retrieve information from.
	/// <para>If this param is set to NULL, the function will retrieve information from the main module of the process instead.</para></param>
	/// <returns>TRUE or FALSE to indcate whether the function succeeded or failed.</returns>
	/// <exception cref="XE_ERROR_PARAMETER_NULL"/>
	/// <exception cref="XE_ERROR_WINAPI_FAIL"/>
	BOOL XEGetModuleInformation(
		_Out_ LPMODULE lpModule,
		_In_opt_z_ LPCSTR lpModuleName
	);

#ifdef __cplusplus
}

#include <Windows.h>
#include <Psapi.h>
#undef GetModuleInformation

namespace api {

	/// <summary>
	/// Retrieves information about a module and it's owning process.
	/// </summary>
	/// <param name="lpModule">- A pointer to a MODULE object to receive the retrieved information.</param>
	/// <param name="lpModuleName">- The name of the module to retrieve information from.
	/// <para>If this param is set to NULL, the function will retrieve information from the main module of the process instead.</para></param>
	/// <exception cref="ParameterNullException"/>
	/// <exception cref="EntryNotFoundException"/>
	/// <exception cref="WinApiException"/>
	inline void GetModuleInformation(
		_Out_ LPMODULE lpModule,
		_In_opt_z_ LPCSTR lpModuleName
	) {
		if (!lpModule) throw ParameterNullException("lpModule");

		lpModule->OwningProcess.hProcess = GetCurrentProcess();
		lpModule->OwningProcess.dwOwnerProcessId = GetCurrentProcessId();

		lpModule->hModule = GetModuleHandleA(lpModuleName);
		if (!lpModule->hModule) {
			if (lpModuleName) throw EntryNotFoundException(lpModuleName);
			else throw WinApiException("GetModuleHandleA");
		}

		MODULEINFO mnfo;
		if (!K32GetModuleInformation(
			lpModule->OwningProcess.hProcess,
			lpModule->hModule,
			&mnfo,
			sizeof(MODULEINFO)
		)) throw WinApiException("K32GetModuleInformation");

		lpModule->uModuleBase = (ADDRESS)lpModule->hModule;
		lpModule->dwModuleSize = mnfo.SizeOfImage;

		if (!K32GetModuleBaseNameA(
			lpModule->OwningProcess.hProcess,
			lpModule->hModule,
			lpModule->szModuleName,
			sizeof(lpModule->szModuleName)
		)) throw WinApiException("K32GetModuleBaseNameA");

		if (!K32GetModuleFileNameExA(
			lpModule->OwningProcess.hProcess,
			lpModule->hModule,
			lpModule->szModulePath,
			sizeof(lpModule->szModulePath)
		)) throw WinApiException("K32GetModuleFileNameExA");

		if (!K32GetModuleBaseNameA(
			lpModule->OwningProcess.hProcess,
			NULL,
			lpModule->OwningProcess.szProcessName,
			sizeof(lpModule->OwningProcess.szProcessName)
		)) throw WinApiException("K32GetModuleBaseNameA");

		if (!K32GetModuleFileNameExA(
			lpModule->OwningProcess.hProcess,
			NULL,
			lpModule->OwningProcess.szProcessPath,
			sizeof(lpModule->OwningProcess.szProcessPath)
		)) throw WinApiException("K32GetModuleFileNameExA");
	}
}
#endif // __cplusplus

#endif // !__MEMAPI_INFO_H__