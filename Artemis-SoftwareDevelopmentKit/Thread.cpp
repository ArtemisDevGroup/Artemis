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

	BOOL Thread::Wait(_In_ DWORD dwMilliseconds) {
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

	BOOL Thread::IsRunning() {
		CONTEXT_BEGIN;

		BOOL bRet = !Wait(1);

		CONTEXT_END;
		return bRet;
	}

	void Thread::Suspend() {
		CONTEXT_BEGIN;

		if (SuspendThread(hThread) == (DWORD)-1) throw WindowsApiException("SuspendThread");

		CONTEXT_END;
	}

	void Thread::Resume() {
		CONTEXT_BEGIN;

		if (ResumeThread(hThread) == (DWORD)-1) throw WindowsApiException("ResumeThread");

		CONTEXT_END;
	}

	void Thread::Terminate() {
		CONTEXT_BEGIN;

#pragma warning(push)
#pragma warning(disable:6258)	// Using TerminateThread does not allow proper thread clean up.
								// Disregarded due to it being irrelevant in this use case.
		if (!TerminateThread(hThread, -1)) throw WindowsApiException("TerminateThread");
#pragma warning(pop)

		CONTEXT_END;
	}

	_Check_return_ DWORD Thread::GetExitCode() {
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