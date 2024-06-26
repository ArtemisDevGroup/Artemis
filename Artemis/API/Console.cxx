#include "pch.h"

#include "Console.hxx"

#include "Error.hxx"

namespace Artemis::API {
	bool g_ConsoleIsOpen = false;

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
}