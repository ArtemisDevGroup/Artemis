#ifndef __THREADAPI_H__
#define __THREADAPI_H__

#ifdef __cplusplus

#include "Def.h"
#include "ErrorApi.h"

#include <Windows.h>

namespace api {
	/// <summary>
	/// Represents a thread.
	/// </summary>
	/// <typeparam name="PaTy">- The type of the parameter.</typeparam>
	template<typename PaTy>
	class Thread {
		typedef DWORD(APIENTRY* FnTy)(PaTy*);
		FnTy lpFn;
		PaTy* lpParam;
		HANDLE hThread;
	public:
		Thread() : lpFn(nullptr), lpParam(nullptr), hThread(nullptr) {}

		/// <summary>
		/// Creates a Thread object with the specified function pointer.
		/// </summary>
		/// <param name="lpFunction">- The function pointer to set.</param>
		Thread(_In_ FnTy lpFunction) : lpFn(lpFunction), lpParam(nullptr), hThread(nullptr) {}

		/// <summary>
		/// Creates a Thread object with the specified function and parameter pointer.
		/// </summary>
		/// <param name="lpFunction">- The function pointer to set.</param>
		/// <param name="lpParameter">- The parameter pointer to set.</param>
		Thread(_In_ FnTy lpFunction, _In_ PaTy* lpParameter) : lpFn(lpFunction), lpParam(lpParameter), hThread(nullptr) {}

		/// <summary>
		/// Sets the function pointer to the one specified.
		/// </summary>
		/// <param name="lpFunction">- The function pointer to set.</param>
		void SetFunctionPtr(_In_ FnTy lpFunction) { lpFn = lpFunction; }

		/// <summary>
		/// Sets the parameter pointer to the one specified.
		/// </summary>
		/// <param name="lpParameter">- The parameter pointer to set.</param>
		void SetParameterPtr(_In_ PaTy* lpParameter) { lpParam = lpParameter; }

		/// <summary>
		/// Starts the thread.
		/// </summary>
		/// <exception cref="ParameterNullException"/>
		/// <exception cref="WinApiException"/>
		void Start() {
			if (!lpFn) throw api::ParameterNullException("lpFunction");

			hThread = CreateThread(
				nullptr,
				0,
				(LPTHREAD_START_ROUTINE)lpFn,
				(LPVOID)lpParam,
				0,
				nullptr
			);

			if (!hThread) throw api::WinApiException("CreateThread");
		}

		/// <summary>
		/// Waits the specified number of milliseconds for the thread to finish executing.
		/// </summary>
		/// <param name="dwMilliseconds">- The number of milliseconds to wait.</param>
		/// <exception cref="WinApiException"/>
		/// <exception cref="Exception"/>
		void Wait(_In_ DWORD dwMilliseconds = INFINITE) {
			DWORD dwReturn = WaitForSingleObject(
				hThread,
				dwMilliseconds
			);

			switch (dwReturn) {
			case WAIT_FAILED:
				throw api::WinApiException("WaitForSingleObject");
			case WAIT_TIMEOUT:
				throw api::Exception("The time-out interval elapsed without the thread object returning.");
			case WAIT_OBJECT_0:
				return;
			}
		}

		/// <summary>
		/// Gets the return value of the thread function when it has finished.
		/// <para>A generic exception is thrown when an attempt to call this function has been made before the thread has finished execution.</para>
		/// </summary>
		/// <returns>The thread return value.</returns>
		/// <exception cref="WinApiException"/>
		/// <exception cref="Exception"/>
		DWORD GetReturnValue() {
			DWORD dwCode;

			if (!GetExitCodeThread(
				hThread,
				&dwCode
			)) throw api::WinApiException("GetExitCodeThread");

			if (dwCode == STILL_ACTIVE) throw api::Exception("Thread still running. Wait for object before calling this function.");

			return dwCode;
		}

		/// <summary>
		/// Terminates the thread.
		/// </summary>
		/// <param name="dwExitCode">- The exit code of the thread.</param>
		/// <exception cref="WinApiException"/>
		void Terminate(DWORD dwExitCode = 0) {
			if (!TerminateThread(hThread, dwExitCode)) throw api::WinApiException("TerminateThread");
		}
	};
}
#endif // __cplusplus

#endif // !__THREADAPI_H__