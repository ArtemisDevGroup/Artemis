//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __MEMORY_INJECTION_H__
#define __MEMORY_INJECTION_H__

#include "Definitions.h"
#include "MemoryDefinitions.h"
#include "Interfaces.h"

namespace Artemis {
	/// <summary>
	/// A class for injecting dynamic link libraries into a process.
	/// </summary>
	class ARTEMIS_API DllInjection : public IDisposable {
	public:
		/// <summary>
		/// Defines what the dll path is relative to.
		/// </summary>
		enum PathRelativity {
			/// <summary>
			/// Absolute means that the path specified shall be complete. Ex: C:\Users\Me\Documents\MyDll.dll
			/// </summary>
			Absolute,

			/// <summary>
			/// Relative to caller means that the path specified shall be relative off of the current directory of the process that constructed this instance.
			/// <para>For example, if the process that constructed this instance is located at C:\Program Files\MyProcess.exe and the dll is located at C:\Program Files\bin\MyDll.dll then the relative path needs to be bin\MyDll.dll.</para>
			/// </summary>
			ToCaller,

			/// <summary>
			/// Relative to target means that the path specified shall be relative off of the current directory of the specified target process. 
			/// <para>For example, if the target process is located at C:\Program Files\MyProcess.exe and the dll is located at C:\Program Files\bin\MyDll.dll then the relative path needs to be bin\MyDll.dll.</para>
			/// </summary>
			ToTarget
		};
	private:
		HANDLE hProcess;
		BOOL bCloseHandle;
		CHAR szPath[MAX_PATH];

		void GetRealPath(
			_In_ LPCSTR lpDllPath,
			_In_ PathRelativity pr
		);

		void CheckArchitecture();
	public:
		/// <summary>
		/// Creates a DllInjection instance.
		/// </summary>
		/// <param name="lpTargetProcess">- The name of the target process.</param>
		/// <param name="lpDllPath">- The path of the dll to inject.</param>
		/// <param name="pr">- The relativity of the path specified as "lpDllPath".</param>
		/// <exception cref="WindowsApiException"/>
		/// <exception cref="ObjectNotFoundException"/>
		/// <exception cref="ParameterException"/>
		DllInjection(
			_In_z_ LPCSTR lpTargetProcess,
			_In_z_ LPCSTR lpDllPath,
			_In_ PathRelativity pr = ToCaller
		);

		/// <summary>
		/// Creates a DllInjection instance.
		/// </summary>
		/// <param name="dwTargetProcessId">- The unique identifier of the target process.</param>
		/// <param name="lpDllPath">- The path of the dll to inject.</param>
		/// <param name="pr">- The relativity of the path specified as "lpDllPath".</param>
		/// <exception cref="WindowsApiException"/>
		/// <exception cref="ParameterException"/>
		DllInjection(
			_In_ DWORD dwTargetProcessId,
			_In_z_ LPCSTR lpDllPath,
			_In_ PathRelativity pr = ToCaller
		);

		/// <summary>
		/// Creates a DllInjection instance.
		/// </summary>
		/// <param name="hTargetProcess">- A handle to the target process. Must have the following rights:
		/// <para>- PROCESS_QUERY_INFORMATION</para>
		/// <para>- PROCESS_VM_OPERATION</para>
		/// <para>- PROCESS_VM_READ</para>
		/// <para>- PROCESS_VM_WRITE</para>
		/// <para>- PROCESS_CREATE_THREAD</para>
		/// </param>
		/// <param name="lpDllPath">- The path of the dll to inject.</param>
		/// <param name="pr">- The relativity of the path specified as "lpDllPath".</param>
		/// <exception cref="WindowsApiException"/>
		/// <exception cref="ParameterException"/>
		DllInjection(
			_In_ HANDLE hTargetProcess,
			_In_z_ LPCSTR lpDllPath,
			_In_ PathRelativity pr = ToCaller
		);

		~DllInjection();

		/// <summary>
		/// Injects the library.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Inject() const;

		/// <summary>
		/// Releases the instance resources.
		/// </summary>
		void Release();
	};
}

#endif // !__MEMORY_INJECTION_H__