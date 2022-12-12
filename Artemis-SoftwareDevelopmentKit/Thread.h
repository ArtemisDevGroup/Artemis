//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __THREAD_H__
#define __THREAD_H__

#include "Definitions.h"
#include "Interfaces.h"

#include <Windows.h>

namespace Artemis {
	/// <summary>
	/// A template type for a function to run in a separate thread.
	/// </summary>
	/// <typeparam name="T">- The parameter base type.</typeparam>
	template<typename T>
	using ThreadFunction = DWORD(WINAPI*)(T* lpThreadParameter);

	/// <summary>
	/// A template type for a function to be looped in a thread.
	/// <para>The return value of this function determines whether the thread shall keep running or exit.</para>
	/// <para>Returning TRUE will signal to the thread to go another iteration, while returning FALSE will have the opposite effect and immediately exit the thread, stopping the loop.</para>
	/// </summary>
	typedef BOOL(WINAPI* LoopThreadFunction)();

	/// <summary>
	/// A class for creating threads inside the current process.
	/// </summary>
	class ARTEMIS_API Thread : public IDisposable {
		HANDLE hThread;
		LPTHREAD_START_ROUTINE lpThreadFunction;
		LPVOID lpThreadParameter;

	public:
		Thread();

		/// <summary>
		/// Consructs a Thread object.
		/// </summary>
		/// <param name="lpThreadFunction">- A pointer to the function to run as a thread.</param>
		/// <param name="lpThreadParameter">- A pointer to the thread parameter.</param>
		template<typename T>
		inline Thread(
			_In_ ThreadFunction<T> lpThreadFunction,
			_In_opt_ T* lpThreadParameter
		) : hThread(nullptr), lpThreadFunction((LPTHREAD_START_ROUTINE)lpThreadFunction), lpThreadParameter((LPVOID)lpThreadParameter) {}

		/// <summary>
		/// Starts the thread.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Start();

		/// <summary>
		/// Waits for the thread to finish execution.
		/// </summary>
		/// <param name="dwMilliseconds">- The number of milliseconds to wait. Default is INFINITE.</param>
		/// <returns>A BOOL defining wether the thread exited (TRUE) or the timer ran out (FALSE).</returns>
		/// <exception cref="WindowsApiException"/>
		BOOL Wait(_In_ DWORD dwMilliseconds = INFINITE) const;

		/// <summary>
		/// Checks if the thread is still running or if it has exited.
		/// </summary>
		/// <returns>TRUE if the thread is running. FALSE if it has exited.</returns>
		/// <exception cref="WindowsApiException"/>
		BOOL IsRunning() const;

		/// <summary>
		/// Suspends the thread.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Suspend() const;

		/// <summary>
		/// Resumes the previously suspended thread.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Resume() const;

		/// <summary>
		/// Terminates the thread.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Terminate() const;

		/// <summary>
		/// Gets the exit code of the thread. If the thread was terminated, the return value will be (DWORD)-1;
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		_Check_return_ DWORD GetExitCode() const;

		/// <summary>
		/// Releases the thread instance by closing its handle.
		/// </summary>
		void Release();

		~Thread();
	};

	/// <summary>
	/// A class for creating a thread that loops a single function until it is terminated.
	/// </summary>
	class ARTEMIS_API LoopThread : public Thread {
		static DWORD WINAPI ThreadFunction(LoopThreadFunction lpThreadFunction);

	public:
		LoopThread();

		/// <summary>
		/// Constructs a LoopThread object.
		/// </summary>
		/// <param name="lpThreadFunction">- The function to loop.</param>
		LoopThread(_In_ LoopThreadFunction lpThreadFunction);
	};
}

#endif // !__THREAD_H__