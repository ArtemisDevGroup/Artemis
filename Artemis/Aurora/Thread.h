//------------------------------------------------------------------------>
// MIT License
// 
// Copyright (c) 2023 Artemis Group
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------>
// Aurora: https://github.com/ArtemisDevGroup/Aurora
// This file was authored by:
// @Astrea0014: https://github.com/Astrea0014
//------------------------------------------------------------------------>

#ifndef __AURORA_THREAD_H__
#define __AURORA_THREAD_H__

#include "Definitions.h"
#include "Exceptions.h"

#include <Windows.h>

namespace Aurora {
	/// <summary>
	/// A thread exit code.
	/// </summary>
	enum class ExitCode : A_DWORD {
		Success = 0,					// The thread exited successfully.
		UnknownError = (A_DWORD)(-1),	// The thread exited due to one or more unknown errors.
		Terminated = (A_DWORD)(-2)		// The thread has been terminated.
	};

	/// <summary>
	/// The result of waiting for a thread..
	/// </summary>
	enum class WaitResult : A_DWORD {
		Succeeded = 0,		// The wait succeeded and the thread exited.
		Abandoned = 0x80,
		TimedOut = 258		// The wait timed out due to the thread not exiting before the wait timer expired.
	};

	/// <summary>
	/// A class for creating and managing a thread.
	/// </summary>
	/// <typeparam name="ArgumentType">- The type of the thread parameter.</typeparam>
	template<typename ArgumentType = void>
	class Thread {
	public:
		/// <summary>
		/// A function template for the function ran by the thread.
		/// </summary>
		using ThreadFunction = Function<ExitCode(ArgumentType Args)>;

	private:
		HANDLE hThread;
		A_DWORD dwThreadId;
		SECURITY_ATTRIBUTES SecurityAttributes;
		A_BOOL bSecurityAttributesActive;

		struct StaticThreadArgs {
			ThreadFunction lpfnThread;
			ArgumentType Args;
		} ArgStruct;

		static A_DWORD __stdcall StaticThread(_In_ StaticThreadArgs* pArgs) { return (A_DWORD)pArgs->lpfnThread(pArgs->Args); }

	public:
		/// <summary>
		/// Sets the fields necessary to start the thread.
		/// </summary>
		/// <param name="lpfnThread">- A pointer to the function to run in the thread.</param>
		/// <param name="lpSecurityAttributes">- A pointer to a security attributes structure specifying the security descriptor of the thread.</param>
		Thread(_In_ ThreadFunction lpfnThread, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr) noexcept : hThread(nullptr), dwThreadId(0) {
			ArgStruct.lpfnThread = lpfnThread;
			if (lpSecurityAttributes) {
				memcpy(&SecurityAttributes, lpSecurityAttributes, sizeof(SECURITY_ATTRIBUTES));
				bSecurityAttributesActive = true;
			}
			else bSecurityAttributesActive = false;
		}

		Thread(const Thread<ArgumentType>& cpy) {
			AuroraContextStart();

			ArgStruct.lpfnThread = cpy.ArgStruct.lpfnThread;
			ArgStruct.Args = cpy.ArgStruct.Args;
			SecurityAttributes = cpy.SecurityAttributes;

			if (!DuplicateHandle(
				GetCurrentProcess(),
				cpy.hThread,
				GetCurrentProcess(),
				&hThread,
				0,
				FALSE,
				DUPLICATE_SAME_ACCESS
			)) AuroraThrow(WindowsApiException, "DuplicateHandle");

			AuroraContextEnd();
		}

		~Thread() {
			if (hThread) {
				CloseHandle(hThread);
				hThread = nullptr;
			}
		}

		/// <summary>
		/// Starts the thread.
		/// </summary>
		/// <param name="Args">- The argument to pass to the thread.</param>
		/// <exception cref="WindowsApiException"/>
		A_VOID Start(_In_ ArgumentType Args) {
			AuroraContextStart();

			if (!hThread) {

				ArgStruct.Args = Args;

				hThread = CreateThread(
					bSecurityAttributesActive ? &SecurityAttributes : nullptr,
					0,
					(LPTHREAD_START_ROUTINE)StaticThread,
					&ArgStruct,
					0,
					&dwThreadId
				);

				if (!hThread) AuroraThrow(WindowsApiException, "CreateThread");
			}

			AuroraContextEnd();
		}

		/// <summary>
		/// Waits for the thread to finish executing.
		/// </summary>
		/// <param name="dwMilliseconds">- The number of milliseconds to wait before returning. Defaults to INFINITE.</param>
		/// <returns>The result of the wait operation.</returns>
		/// <exception cref="WindowsApiException"/>
		WaitResult Wait(_In_ A_DWORD dwMilliseconds = INFINITE) const {
			AuroraContextStart();

			A_DWORD dwResult = WaitForSingleObject(hThread, dwMilliseconds);
			if (dwResult == WAIT_FAILED) AuroraThrow(WindowsApiException, "WaitForSingleObject");

			AuroraContextEnd();
			return (WaitResult)dwResult;
		}

		/// <summary>
		/// Gets the exit code of the thread.
		/// </summary>
		/// <returns>The exit code of the thread.</returns>
		/// <exception cref="WindowsApiException"/>
		AURORA_NDWR_GET("GetExitCode") ExitCode GetExitCode() const {
			AuroraContextStart();

			A_DWORD dwResult;
			if (!GetExitCodeThread(hThread, &dwResult)) AuroraThrow(WindowsApiException, "GetExitCodeThread");

			AuroraContextEnd();
			return (ExitCode)dwResult;
		}

		/// <summary>
		/// Gets the id of the thread.
		/// </summary>
		/// <returns>The thread id.</returns>
		AURORA_NDWR_GET("GetThreadId") constexpr A_DWORD GetThreadId() const noexcept { return dwThreadId; }

		/// <summary>
		/// Terminates the thread.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		A_VOID Terminate() const {
			AuroraContextStart();

			if (!TerminateThread(hThread, (A_DWORD)ExitCode::Terminated)) AuroraThrow(WindowsApiException, "TerminateThread");

			AuroraContextEnd();
		}
	};

	/// <summary>
	/// A class for creating and managing a thread.
	/// </summary>
	/// <typeparam name="ArgumentType">- The type of the thread parameter.</typeparam>
	template<typename ArgumentType>
	class Thread<const ArgumentType&> {
	public:
		/// <summary>
		/// A function template for the function ran by the thread.
		/// </summary>
		using ThreadFunction = Function<ExitCode(const ArgumentType& refArgs)>;

	private:
		HANDLE hThread;
		A_DWORD dwThreadId;
		SECURITY_ATTRIBUTES SecurityAttributes;
		A_BOOL bSecurityAttributesActive;

		struct StaticThreadArgs {
			ThreadFunction lpfnThread;
			const ArgumentType* lpArgs;
		} ArgStruct;

		static A_DWORD __stdcall StaticThread(_In_ StaticThreadArgs* pArgs) { return (A_DWORD)pArgs->lpfnThread(*pArgs->lpArgs); }

	public:
		/// <summary>
		/// Sets the fields necessary to start the thread.
		/// </summary>
		/// <param name="lpfnThread">- A pointer to the function to run in the thread.</param>
		/// <param name="lpSecurityAttributes">- A pointer to a security attributes structure specifying the security descriptor of the thread.</param>
		Thread(_In_ ThreadFunction lpfnThread, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr) noexcept : hThread(nullptr), dwThreadId(0) {
			ArgStruct.lpfnThread = lpfnThread;
			if (lpSecurityAttributes) {
				memcpy(&SecurityAttributes, lpSecurityAttributes, sizeof(SECURITY_ATTRIBUTES));
				bSecurityAttributesActive = true;
			}
			else bSecurityAttributesActive = false;
		}

		Thread(const Thread<ArgumentType>& cpy) {
			AuroraContextStart();

			ArgStruct.lpfnThread = cpy.ArgStruct.lpfnThread;
			ArgStruct.lpArgs = cpy.ArgStruct.lpArgs;
			SecurityAttributes = cpy.SecurityAttributes;

			if (!DuplicateHandle(
				GetCurrentProcess(),
				cpy.hThread,
				GetCurrentProcess(),
				&hThread,
				0,
				FALSE,
				DUPLICATE_SAME_ACCESS
			)) AuroraThrow(WindowsApiException, "DuplicateHandle");

			AuroraContextEnd();
		}

		~Thread() {
			if (hThread) {
				CloseHandle(hThread);
				hThread = nullptr;
			}
		}

		/// <summary>
		/// Starts the thread.
		/// </summary>
		/// <param name="Args">- The argument to pass to the thread.</param>
		/// <exception cref="WindowsApiException"/>
		A_VOID Start(_In_ const ArgumentType& Args) {
			AuroraContextStart();

			if (!hThread) {

				ArgStruct.lpArgs = &Args;

				hThread = CreateThread(
					bSecurityAttributesActive ? &SecurityAttributes : nullptr,
					0,
					(LPTHREAD_START_ROUTINE)StaticThread,
					&ArgStruct,
					0,
					&dwThreadId
				);

				if (!hThread) AuroraThrow(WindowsApiException, "CreateThread");
			}

			AuroraContextEnd();
		}

		/// <summary>
		/// Waits for the thread to finish executing.
		/// </summary>
		/// <param name="dwMilliseconds">- The number of milliseconds to wait before returning. Defaults to INFINITE.</param>
		/// <returns>The result of the wait operation.</returns>
		/// <exception cref="WindowsApiException"/>
		WaitResult Wait(_In_ A_DWORD dwMilliseconds = INFINITE) const {
			AuroraContextStart();

			A_DWORD dwResult = WaitForSingleObject(hThread, dwMilliseconds);
			if (dwResult == WAIT_FAILED) AuroraThrow(WindowsApiException, "WaitForSingleObject");

			AuroraContextEnd();
			return (WaitResult)dwResult;
		}

		/// <summary>
		/// Gets the exit code of the thread.
		/// </summary>
		/// <returns>The exit code of the thread.</returns>
		/// <exception cref="WindowsApiException"/>
		AURORA_NDWR_GET("GetExitCode") ExitCode GetExitCode() const {
			AuroraContextStart();

			A_DWORD dwResult;
			if (!GetExitCodeThread(hThread, &dwResult)) AuroraThrow(WindowsApiException, "GetExitCodeThread");

			AuroraContextEnd();
			return (ExitCode)dwResult;
		}

		/// <summary>
		/// Gets the id of the thread.
		/// </summary>
		/// <returns>The thread id.</returns>
		AURORA_NDWR_GET("GetThreadId") constexpr A_DWORD GetThreadId() const noexcept { return dwThreadId; }

		/// <summary>
		/// Terminates the thread.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		A_VOID Terminate() const {
			AuroraContextStart();

			if (!TerminateThread(hThread, (A_DWORD)ExitCode::Terminated)) AuroraThrow(WindowsApiException, "TerminateThread");

			AuroraContextEnd();
		}
	};

	/// <summary>
	/// A class for creating and managing a thread.
	/// </summary>
	template<>
	class Thread<void> {
	public:
		/// <summary>
		/// A function template for the function ran by the thread.
		/// </summary>
		using ThreadFunction = Function<ExitCode()>;

	private:
		ThreadFunction lpfnThread;
		HANDLE hThread;
		A_DWORD dwThreadId;

		SECURITY_ATTRIBUTES SecurityAttributes;
		A_BOOL bSecurityAttributesActive;

		static A_DWORD __stdcall StaticThread(_In_ ThreadFunction* lpfnThread) { return (A_DWORD)((*lpfnThread)()); }

	public:
		/// <summary>
		/// Sets the fields necessary to start the thread.
		/// </summary>
		/// <param name="lpfnThread">- A pointer to the function to run in the thread.</param>
		/// <param name="lpSecurityAttributes">- A pointer to a security attributes structure specifying the security descriptor of the thread.</param>
		Thread(_In_ ThreadFunction lpfnThread, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr) noexcept : lpfnThread(lpfnThread), hThread(nullptr), dwThreadId(0), SecurityAttributes() {
			if (lpSecurityAttributes) {
				memcpy(&SecurityAttributes, lpSecurityAttributes, sizeof(SECURITY_ATTRIBUTES));
				bSecurityAttributesActive = true;
			}
			else bSecurityAttributesActive = false;
		}

		Thread(const Thread<>& cpy) {
			AuroraContextStart();

			lpfnThread = cpy.lpfnThread;
			SecurityAttributes = cpy.SecurityAttributes;

			if (!DuplicateHandle(
				GetCurrentProcess(),
				cpy.hThread,
				GetCurrentProcess(),
				&hThread,
				0,
				FALSE,
				DUPLICATE_SAME_ACCESS
			)) AuroraThrow(WindowsApiException, "DuplicateHandle");

			AuroraContextEnd();
		}

		~Thread() {
			if (hThread) {
				CloseHandle(hThread);
				hThread = nullptr;
			}
		}

		/// <summary>
		/// Starts the thread.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		A_VOID Start() {
			AuroraContextStart();

			if (!hThread) {
				hThread = CreateThread(
					nullptr,
					0,
					(LPTHREAD_START_ROUTINE)StaticThread,
					&lpfnThread,
					0,
					&dwThreadId
				);

				if (!hThread) AuroraThrow(WindowsApiException, "CreateThread");
			}

			AuroraContextEnd();
		}

		/// <summary>
		/// Waits for the thread to finish executing.
		/// </summary>
		/// <param name="dwMilliseconds">- The number of milliseconds to wait before returning. Defaults to INFINITE.</param>
		/// <returns>The result of the wait operation.</returns>
		/// <exception cref="WindowsApiException"/>
		WaitResult Wait(_In_ A_DWORD dwMilliseconds = INFINITE) const {
			AuroraContextStart();

			A_DWORD dwResult = WaitForSingleObject(hThread, dwMilliseconds);
			if (dwResult == WAIT_FAILED) AuroraThrow(WindowsApiException, "WaitForSingleObject");

			AuroraContextEnd();
			return (WaitResult)dwResult;
		}

		/// <summary>
		/// Gets the exit code of the thread.
		/// </summary>
		/// <returns>The exit code of the thread.</returns>
		/// <exception cref="WindowsApiException"/>
		AURORA_NDWR_GET("GetExitCode") ExitCode GetExitCode() const {
			AuroraContextStart();

			A_DWORD dwResult;
			if (!GetExitCodeThread(hThread, &dwResult)) AuroraThrow(WindowsApiException, "GetExitCodeThread");

			AuroraContextEnd();
			return (ExitCode)dwResult;
		}

		/// <summary>
		/// Gets the id of the thread.
		/// </summary>
		/// <returns>The thread id.</returns>
		AURORA_NDWR_GET("GetThreadId") constexpr A_DWORD GetThreadId() const noexcept { return dwThreadId; }

		/// <summary>
		/// Terminates the thread.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		A_VOID Terminate() const {
			AuroraContextStart();

#pragma warning(push)
#pragma warning(disable:6258)	// Using TerminateThread does not allow proper thread clean up.
								// Disregarded due to the use case of the function being in situations where no other options are available.
			if (!TerminateThread(hThread, (A_DWORD)ExitCode::Terminated)) AuroraThrow(WindowsApiException, "TerminateThread");
#pragma warning(pop)

			AuroraContextEnd();
		}
	};
}

#endif // !__AURORA_THREAD_H__