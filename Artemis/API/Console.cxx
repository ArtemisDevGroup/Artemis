#include "pch.h"
#include "Console.hxx"

#include "Exception.hxx"

namespace Artemis::API {
	bool	g_ConsoleIsOpen	= false;
	FILE*	g_StdIn			= nullptr;
	FILE*	g_StdOut		= nullptr;

	void open_console() {
		if (g_ConsoleIsOpen)
			throw invalid_state_exception("Console is already open.");

		if (!AllocConsole())
			throw win32_exception("AllocConsole");

		g_ConsoleIsOpen = true;
	}

	void close_console() {
		if (!g_ConsoleIsOpen)
			throw invalid_state_exception("Console is not open.");

		if (!FreeConsole())
			throw win32_exception("FreeConsole");

		g_ConsoleIsOpen = false;
	}

	bool is_console_open() noexcept { return g_ConsoleIsOpen; }

	HWND console_window_handle() {
		if (!g_ConsoleIsOpen)
			throw invalid_state_exception("Console is not open.");

		return GetConsoleWindow();
	}

	void set_console_window_title(const char* const _Title) {
		if (!g_ConsoleIsOpen)
			throw invalid_state_exception("Console is not open.");

		if (!SetConsoleTitleA(_Title))
			throw win32_exception("SetConsoleTitleA");
	}

	void open_console_io() {
		if (!g_ConsoleIsOpen)
			throw invalid_state_exception("Console is not open.");

		if (g_StdIn != nullptr || g_StdOut != nullptr)
			throw invalid_state_exception("Console IO streams are already open.");

		errno_t e = freopen_s(&g_StdOut, "CONOUT$", "w", stdout);
		if (e)
			throw errno_exception(e, "freopen_s#stdout");

		e = freopen_s(&g_StdIn, "CONIN$", "r", stdin);
		if (e)
			throw errno_exception(e, "freopen_s#stdin");
	}

	void close_console_io() {
		if (g_StdIn == nullptr || g_StdOut == nullptr)
			throw invalid_state_exception("Console IO streams are not open.");

		fclose(g_StdOut);
		fclose(g_StdIn);

		g_StdOut = nullptr;
		g_StdIn = nullptr;
	}

	bool is_console_io_open() noexcept { return g_StdOut && g_StdIn; }

	void set_console_color(console_color _Color, int _Shift) {
		if (!g_ConsoleIsOpen)
			throw invalid_state_exception("Console is not open.");

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		ZeroMemory(&csbi, sizeof(csbi));
		
		if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
			throw win32_exception("GetConsoleScreenBufferInfo");
		
		WORD wAttributes = csbi.wAttributes;
		wAttributes ^= 0b1111 << _Shift;
		wAttributes |= static_cast<WORD>(_Color) << _Shift;

		if (!SetConsoleTextAttribute(hConsole, wAttributes))
			throw win32_exception("SetConsoleTextAttribute");
	}

	void set_console_foreground_color(console_color _Color) {
		set_console_color(_Color, 0);
	}

	void set_console_background_color(console_color _Color) {
		set_console_color(_Color, 4);
	}
}