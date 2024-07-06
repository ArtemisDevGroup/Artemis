#include "pch.h"

#include "Console.hxx"

#include "Error.hxx"

namespace Artemis::API {
	bool g_ConsoleIsOpen = false;
	FILE* g_StdIn;
	FILE* g_StdOut;

	void console_open() {
		__stack_record();
		call_stack_entry cse = __stack_this_cse();

		if (g_ConsoleIsOpen)
			throw exception("Console is already open.", &cse);

		if (!AllocConsole())
			throw win32_exception("AllocConsole", &cse);

		__stack_escape();
	}

	void console_close() {
		__stack_record();
		call_stack_entry cse = __stack_this_cse();

		if (!g_ConsoleIsOpen)
			throw exception("Console is not open.", &cse);

		if (!FreeConsole())
			throw win32_exception("FreeConsole", &cse);

		__stack_escape();
	}

	bool is_console_open() noexcept { return g_ConsoleIsOpen; }

	HWND console_window_handle() {
		__stack_record();

		if (!g_ConsoleIsOpen) {
			call_stack_entry cse = __stack_this_cse();
			throw exception("Console is not open.");
		}

		return GetConsoleWindow();

		__stack_escape();
	}

	void open_console_io() {
		__stack_record();
		call_stack_entry cse = __stack_this_cse();


		if (!g_ConsoleIsOpen)
			throw exception("Console is not open.", &cse);

		if (g_StdOut || g_StdIn)
			throw exception("Console IO streams are already open.", &cse);

		errno_t e = freopen_s(&g_StdOut, "CONOUT$", "w", stdout);
		if (e)
			throw errno_exception(e, "freopen_s#stdout", &cse);

		e = freopen_s(&g_StdIn, "CONIN$", "r", stdin);
		if (e)
			throw errno_exception(e, "freopen_s#stdin", &cse);

		__stack_escape();
	}

	void close_console_io() {
		__stack_record();
		call_stack_entry cse = __stack_this_cse();

		if (!g_StdOut || !g_StdIn)
			throw exception("Console IO streams are not open.", &cse);

		fclose(g_StdOut);
		fclose(g_StdIn);

		g_StdOut = nullptr;
		g_StdIn = nullptr;

		__stack_escape();
	}
}