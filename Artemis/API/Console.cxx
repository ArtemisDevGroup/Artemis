#include "pch.h"

#include "Console.hxx"

#include "Error.hxx"

namespace Artemis::API {
	bool	g_ConsoleIsOpen	= false;
	FILE*	g_StdIn			= nullptr;
	FILE*	g_StdOut		= nullptr;

	void console_open() {
		__stack_record();

		if (g_ConsoleIsOpen)
			throw exception("Console is already open.");

		if (!AllocConsole())
			throw win32_exception("AllocConsole");

		__stack_escape();
	}

	void console_close() {
		__stack_record();

		if (!g_ConsoleIsOpen)
			throw exception("Console is not open.");

		if (!FreeConsole())
			throw win32_exception("FreeConsole");

		__stack_escape();
	}

	bool is_console_open() noexcept { return g_ConsoleIsOpen; }

	HWND console_window_handle() {
		__stack_record();

		if (!g_ConsoleIsOpen)
			throw exception("Console is not open.");

		__stack_escape();
		return GetConsoleWindow();
	}

	void set_console_window_title(const char* const _Title) {
		__stack_record();

		if (!g_ConsoleIsOpen)
			throw exception("Console is not open.");

		if (!SetConsoleTitleA(_Title))
			throw win32_exception("SetConsoleTitleA");
			
		__stack_escape();
	}

	void open_console_io() {
		__stack_record();

		if (!g_ConsoleIsOpen)
			throw exception("Console is not open.");

		if (g_StdOut || g_StdIn)
			throw exception("Console IO streams are already open.");

		errno_t e = freopen_s(&g_StdOut, "CONOUT$", "w", stdout);
		if (e)
			throw errno_exception(e, "freopen_s#stdout");

		e = freopen_s(&g_StdIn, "CONIN$", "r", stdin);
		if (e)
			throw errno_exception(e, "freopen_s#stdin");

		__stack_escape();
	}

	void close_console_io() {
		__stack_record();

		if (!g_StdOut || !g_StdIn)
			throw exception("Console IO streams are not open.");

		fclose(g_StdOut);
		fclose(g_StdIn);

		g_StdOut = nullptr;
		g_StdIn = nullptr;

		__stack_escape();
	}

	bool is_console_io_open() noexcept { return g_StdOut && g_StdIn; }

	void set_console_color(console_color _Color, int _Shift) {
		__stack_record();

		if (!g_ConsoleIsOpen)
			throw exception("Console is not open.");

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFOEX csbi;
		ZeroMemory(&csbi, sizeof(csbi));

		if (!GetConsoleScreenBufferInfoEx(hConsole, &csbi))
			throw win32_exception("GetConsoleScreenBufferInfoEx");

		WORD wAttributes = csbi.wAttributes;
		wAttributes ^= 0b1111 << _Shift;
		wAttributes |= static_cast<WORD>(_Color) << _Shift;

		if (!SetConsoleTextAttribute(hConsole, wAttributes))
			throw win32_exception("SetConsoleTextAttribute");

		__stack_escape();
	}

	void set_console_foreground_color(console_color _Color) {
		__stack_record();
		__stack_rethrow(set_console_color(_Color, 0));
		__stack_escape();
	}

	void set_console_background_color(console_color _Color) {
		__stack_record();
		__stack_rethrow(set_console_color(_Color, 4));
		__stack_escape();
	}
}