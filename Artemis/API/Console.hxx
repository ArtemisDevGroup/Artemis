#ifndef ARTEMIS_API_CONSOLE_HXX
#define ARTEMIS_API_CONSOLE_HXX

#include "Definitions.hxx"

#include <Windows.h>	// HWND, DWORD, FOREGROUND_*

namespace Artemis::API {
	ARTEMIS_API void console_open();

	ARTEMIS_API void console_close();

	ARTEMIS_API bool is_console_open() noexcept;

	ARTEMIS_API HWND console_window_handle();

	ARTEMIS_API void set_console_window_title(const char* const _Title);

	ARTEMIS_API void open_console_io();

	ARTEMIS_API void close_console_io();

	ARTEMIS_API bool is_console_io_open() noexcept;

	enum class console_color : WORD {
		black				= 0x0000,
		dark_red			= FOREGROUND_RED,
		dark_green			= FOREGROUND_GREEN,
		dark_blue			= FOREGROUND_BLUE,
		dark_gray			= FOREGROUND_INTENSITY,
		dark_yellow			= FOREGROUND_RED | FOREGROUND_GREEN,
		dark_cyan			= FOREGROUND_GREEN | FOREGROUND_BLUE,
		dark_magenta		= FOREGROUND_BLUE | FOREGROUND_RED,
		red					= FOREGROUND_RED | FOREGROUND_INTENSITY,
		green				= FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		blue				= FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		yellow				= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		cyan				= FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		magenta				= FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY,
		gray				= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		white				= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
	};

	ARTEMIS_API void set_console_foreground_color(console_color _Color);
	ARTEMIS_API void set_console_background_color(console_color _Color);
}

#endif // !ARTEMIS_API_CONSOLE_HXX