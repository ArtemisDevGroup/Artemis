#include "pch.h"
#include "Console.hxx"
#include "Error.hxx"

namespace Artemis::API {
	BOOL g_bIsConsoleWindowOpen = FALSE;

	FILE* g_pStdIn = nullptr;
	FILE* g_pStdOut = nullptr;
	FILE* g_pStdErr = nullptr;

	BOOL SetConsoleWindowTitle(_In_z_ LPCSTR lpConsoleWindowTitle) {
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

	BOOL OpenConsoleWindow(_In_opt_z_ LPCSTR lpConsoleWindowTitle) {
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

	BOOL OpenConsoleWindow() {
		OpenConsoleWindow(nullptr);
	}

	BOOL CloseConsoleWindow() {
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

	BOOL OpenConsoleStream(_In_ ConsoleStream nConsoleStream) {
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

		if (*ppFile) {
			SetLastArtemisError(__FUNCTION__, ErrorCode::StateInvalid);
			return FALSE;
		}

		// ADD ERRNO ERROR HANDLING:
		freopen_s(ppFile, pFileName, pMode, pOldStream);

		return TRUE;
	}

	BOOL OpenAllConsoleStreams() {
		if (!g_pStdIn)
			if (!OpenConsoleStream(ConsoleStream::In))
				return FALSE;
		if (!g_pStdOut)
			if (!OpenConsoleStream(ConsoleStream::Out))
				return FALSE;
		if (!g_pStdErr)
			if (!OpenConsoleStream(ConsoleStream::Error))
				return FALSE;
		return TRUE;
	}

	BOOL CloseConsoleStream(_In_ ConsoleStream nConsoleStream) {
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

		if (!*ppFile) {
			SetLastArtemisError(__FUNCTION__, ErrorCode::StateInvalid);
			return FALSE;
		}

		// ADD ERRNO ERROR HANDLING:
		fclose(*ppFile);

		return TRUE;
	}

	BOOL CloseAllConsoleStreams() {
		if (g_pStdIn)
			if (!CloseConsoleStream(ConsoleStream::In))
				return FALSE;
		if (g_pStdOut)
			if (!CloseConsoleStream(ConsoleStream::Out))
				return FALSE;
		if (g_pStdErr)
			if (!CloseConsoleStream(ConsoleStream::Error))
				return FALSE;
		return TRUE;
	}
}