//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Astrea0014.												|
// @Astrea0014: https://github.com/Astrea0014											|
//-------------------------------------------------------------------------------------->

#include "Thread.h"
#include "Exceptions.h"

namespace Artemis {
	Thread::Thread() : hThread(nullptr), lpThreadFunction(nullptr), lpThreadParameter(nullptr) {}

	void Thread::Start() {
		CONTEXT_BEGIN;

		hThread = CreateThread(
			nullptr,
			0,
			lpThreadFunction,
			lpThreadParameter,
			0,
			nullptr
		);

		if (!hThread) throw WindowsApiException("CreateThread");

		CONTEXT_END;
	}

	BOOL Thread::Wait(_In_ DWORD dwMilliseconds) const {
		CONTEXT_BEGIN;

		BOOL bReturn = FALSE;

		switch (WaitForSingleObject(hThread, dwMilliseconds)) {
		case WAIT_OBJECT_0:
			bReturn = TRUE;
		case WAIT_TIMEOUT:
			break;
		case WAIT_FAILED:
			throw WindowsApiException("WaitForSingleObject");
		}

		CONTEXT_END;
		return bReturn;
	}

	BOOL Thread::IsRunning() const {
		CONTEXT_BEGIN;

		BOOL bRet = !Wait(1);

		CONTEXT_END;
		return bRet;
	}

	void Thread::Suspend() const {
		CONTEXT_BEGIN;

		if (SuspendThread(hThread) == (DWORD)-1) throw WindowsApiException("SuspendThread");

		CONTEXT_END;
	}

	void Thread::Resume() const {
		CONTEXT_BEGIN;

		if (ResumeThread(hThread) == (DWORD)-1) throw WindowsApiException("ResumeThread");

		CONTEXT_END;
	}

	void Thread::Terminate() const {
		CONTEXT_BEGIN;

#pragma warning(push)
#pragma warning(disable:6258)	// Using TerminateThread does not allow proper thread clean up.
								// Disregarded due to it being irrelevant in this use case.
		if (!TerminateThread(hThread, -1)) throw WindowsApiException("TerminateThread");
#pragma warning(pop)

		CONTEXT_END;
	}

	_Check_return_ DWORD Thread::GetExitCode() const {
		CONTEXT_BEGIN;

		DWORD dwRet;
		if (!GetExitCodeThread(hThread, &dwRet)) throw WindowsApiException("GetExitCodeThread");

		CONTEXT_END;
		return dwRet;
	}

	void Thread::Release() {
		if (hThread)
			if (CloseHandle(hThread))
				hThread = nullptr;
	}

	Thread::~Thread() { Release(); }

	DWORD WINAPI LoopThread::ThreadFunction(LoopThreadFunction lpThreadFunction) {
		while (lpThreadFunction());
		return 0;
	}

	LoopThread::LoopThread() : Thread() {}

	LoopThread::LoopThread(_In_ LoopThreadFunction lpThreadFunction) : Thread(ThreadFunction, lpThreadFunction) {}
}