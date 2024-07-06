#ifndef ARTEMIS_API_CONSOLE_HXX
#define ARTEMIS_API_CONSOLE_HXX

#include "Definitions.hxx"

#include <Windows.h>

namespace Artemis::API {
	ARTEMIS_API void console_open();

	ARTEMIS_API void console_close();

	ARTEMIS_API bool is_console_open() noexcept;

	ARTEMIS_API HWND console_window_handle();

	ARTEMIS_API void open_console_io();

	ARTEMIS_API void close_console_io();
}

#endif // !ARTEMIS_API_CONSOLE_HXX