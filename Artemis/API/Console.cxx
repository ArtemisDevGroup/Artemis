#include "pch.h"
#include "Console.hxx"
#include "Error.hxx"

namespace Artemis::API {
	BOOL g_bIsConsoleWindowOpen = FALSE;

	FILE* g_pStdIn = nullptr;
	FILE* g_pStdOut = nullptr;
	FILE* g_pStdErr = nullptr;

	ARTEMIS_API BOOL IsConsoleWindowOpen() noexcept {
		return g_bIsConsoleWindowOpen;
	}

	ARTEMIS_API BOOL SetConsoleWindowTitle(_In_z_ LPCSTR lpConsoleWindowTitle) {
		if (!g_bIsConsoleWindowOpen) {
			SetLastArtemisError(__FUNCTION__, ErrorCode::StateInvalid);
			return FALSE;
		}
		
		if (!SetWindowTextA(GetConsoleWindow(), lpConsoleWindowTitle)) {
			SetLastArtemisError(__FUNCTION__, ErrorCode::Windows, WindowsError::Create("SetWindowTextA", GetLastError(), "Window handle given to function was handle returned by GetConsoleWindow."));
			return FALSE;
		}

		return TRUE;
	}

	ARTEMIS_API BOOL OpenConsoleWindow(_In_opt_z_ LPCSTR lpConsoleWindowTitle) {
		if (g_bIsConsoleWindowOpen) {
			SetLastArtemisError(__FUNCTION__, ErrorCode::StateInvalid);
			return FALSE;
		}

		if (!AllocConsole()) {
			SetLastArtemisError(__FUNCTION__, ErrorCode::Windows, WindowsError::Create("AllocConsole", GetLastError(), nullptr));
			return FALSE;
		}

		g_bIsConsoleWindowOpen = TRUE;

		if (lpConsoleWindowTitle)
			SetConsoleWindowTitle(lpConsoleWindowTitle);

		return TRUE;
	}

	ARTEMIS_API BOOL OpenConsoleWindow() {
		OpenConsoleWindow(nullptr);
	}

	ARTEMIS_API BOOL CloseConsoleWindow() {
		if (!g_bIsConsoleWindowOpen) {
			SetLastArtemisError(__FUNCTION__, ErrorCode::StateInvalid);
			return FALSE;
		}

		if (!FreeConsole()) {
			SetLastArtemisError(__FUNCTION__, ErrorCode::Windows, WindowsError::Create("FreeConsole", GetLastError(), nullptr));
			return FALSE;
		}

		g_bIsConsoleWindowOpen = FALSE;

		return TRUE;
	}

	ARTEMIS_API BOOL IsConsoleStreamOpen(_In_ ConsoleStream nConsoleStream) noexcept {
		switch (nConsoleStream) {
		case ConsoleStream::In:
			return !!g_pStdIn;
		case ConsoleStream::Out:
			return !!g_pStdOut;
		case ConsoleStream::Error:
			return !!g_pStdErr;
		default:
			return FALSE;
		}
	}

	ARTEMIS_API BOOL OpenConsoleStream(_In_ ConsoleStream nConsoleStream) {
		if (IsConsoleStreamOpen(nConsoleStream)) {
			SetLastArtemisError(__FUNCTION__, ErrorCode::StateInvalid);
			return FALSE;
		}

		FILE** ppFile;
		const char* pFileName;
		const char* pMode;
		FILE* pOldStream;

		switch (nConsoleStream) {
		case ConsoleStream::In:
			ppFile = &g_pStdIn;
			pFileName = "$CONIN";
			pMode = "r";
			pOldStream = stdin;
			break;
		case ConsoleStream::Out:
			ppFile = &g_pStdOut;
			pFileName = "$CONOUT";
			pMode = "w";
			pOldStream = stdout;
			break;
		case ConsoleStream::Error:
			ppFile = &g_pStdErr;
			pFileName = "$CONERR";
			pMode = "w";
			pOldStream = stderr;
			break;
		default:
			SetLastArtemisError(__FUNCTION__, ErrorCode::ParameterInvalid, ParameterError::Create(AA_PEA(nConsoleStream)));
			return FALSE;
		}

		// ADD ERRNO ERROR HANDLING:
		freopen_s(ppFile, pFileName, pMode, pOldStream);

		return TRUE;
	}

	ARTEMIS_API BOOL OpenAllConsoleStreams() {
		if (!IsConsoleStreamOpen(ConsoleStream::In))
			if (!OpenConsoleStream(ConsoleStream::In))
				return FALSE;
		if (!IsConsoleStreamOpen(ConsoleStream::Out))
			if (!OpenConsoleStream(ConsoleStream::Out))
				return FALSE;
		if (!IsConsoleStreamOpen(ConsoleStream::Error))
			if (!OpenConsoleStream(ConsoleStream::Error))
				return FALSE;
		return TRUE;
	}

	ARTEMIS_API BOOL CloseConsoleStream(_In_ ConsoleStream nConsoleStream) {
		if (!IsConsoleStreamOpen(nConsoleStream)) {
			SetLastArtemisError(__FUNCTION__, ErrorCode::StateInvalid);
			return FALSE;
		}
		
		FILE** ppFile;

		switch (nConsoleStream) {
		case ConsoleStream::In:
			ppFile = &g_pStdIn;
			break;
		case ConsoleStream::Out:
			ppFile = &g_pStdOut;
			break;
		case ConsoleStream::Error:
			ppFile = &g_pStdErr;
			break;
		default:
			SetLastArtemisError(__FUNCTION__, ErrorCode::ParameterInvalid, ParameterError::Create(AA_PEA(nConsoleStream)));
			return FALSE;
		}

		// ADD ERRNO ERROR HANDLING:
		fclose(*ppFile);

		return TRUE;
	}

	ARTEMIS_API BOOL CloseAllConsoleStreams() {
		if (IsConsoleStreamOpen(ConsoleStream::In))
			if (!CloseConsoleStream(ConsoleStream::In))
				return FALSE;
		if (IsConsoleStreamOpen(ConsoleStream::Out))
			if (!CloseConsoleStream(ConsoleStream::Out))
				return FALSE;
		if (IsConsoleStreamOpen(ConsoleStream::Error))
			if (!CloseConsoleStream(ConsoleStream::Error))
				return FALSE;
		return TRUE;
	}
}