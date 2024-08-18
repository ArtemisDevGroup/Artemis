#ifndef ARTEMIS_API_CONSOLE_HXX
#define ARTEMIS_API_CONSOLE_HXX

#include "Definitions.hxx"

#include <Windows.h>	// HWND, DWORD, FOREGROUND_*

namespace Artemis::API {
	/// <summary>
	/// Opens the console window.
	/// </summary>
	/// <exception cref="invalid_state_exception"/>
	/// <exception cref="win32_exception"/>
	ARTEMIS_API void open_console();

	/// <summary>
	/// Closes the console window.
	/// </summary>
	/// <exception cref="invalid_state_exception"/>
	/// <exception cref="win32_exception"/>
	ARTEMIS_API void close_console();

	/// <summary>
	/// Identifies whether the console window is open or not.
	/// </summary>
	/// <returns>true if the console window is open, otherwise false.</returns>
	ARTEMIS_API bool is_console_open() noexcept;

	/// <summary>
	/// Fetches the window handle of the console window.
	/// </summary>
	/// <returns>The console window handle.</returns>
	/// <exception cref="invalid_state_exception"/>
	ARTEMIS_API HWND console_window_handle();

	/// <summary>
	/// Sets the console window title.
	/// </summary>
	/// <param name="_Title">- The title to give the console window.</param>
	/// <exception cref="invalid_state_exception"/>
	/// <exception cref="win32_exception"/>
	ARTEMIS_API void set_console_window_title(const char* const _Title);

	/// <summary>
	/// Associates the console IO devices with their respective streams.
	/// </summary>
	/// <exception cref="invalid_state_exception"/>
	/// <exception cref="errno_exception"/>
	ARTEMIS_API void open_console_io();

	/// <summary>
	/// Closes previously associated IO streams.
	/// </summary>
	/// <exception cref="invalid_state_exception"/>
	/// <exception cref="errno_exception"/>
	ARTEMIS_API void close_console_io();

	/// <summary>
	/// Identifies whether the console IO streams are associated with their respective devices or not.
	/// </summary>
	/// <returns>true of the console IO streams are associated with their respective devices, otherwise false.</returns>
	ARTEMIS_API bool is_console_io_open() noexcept;

	/// <summary>
	/// Represents a color supported by a 16-color terminal.
	/// </summary>
	enum class console_color : WORD {
		/// <summary>
		/// Black, #0C0C0C (12, 12, 12)
		/// </summary>
		black				= 0x0000,

		/// <summary>
		/// Dark Red, #C50F1F (197, 15, 31)
		/// </summary>
		dark_red			= FOREGROUND_RED,

		/// <summary>
		/// Dark Green, #13A10E (19, 161, 14)
		/// </summary>
		dark_green			= FOREGROUND_GREEN,

		/// <summary>
		/// Dark Blue, #0037DA (0, 55, 218)
		/// </summary>
		dark_blue			= FOREGROUND_BLUE,

		/// <summary>
		/// Dark Gray, #767676 (118, 118, 118)
		/// </summary>
		dark_gray			= FOREGROUND_INTENSITY,

		/// <summary>
		/// Dark Yellow, #C19C00 (193, 156, 0)
		/// </summary>
		dark_yellow			= FOREGROUND_RED | FOREGROUND_GREEN,

		/// <summary>
		/// Dark Cyan, #3A96DD (58, 150, 221)
		/// </summary>
		dark_cyan			= FOREGROUND_GREEN | FOREGROUND_BLUE,

		/// <summary>
		/// Dark Mangenta, #881798 (136, 23, 152)
		/// </summary>
		dark_magenta		= FOREGROUND_BLUE | FOREGROUND_RED,

		/// <summary>
		/// Red, #E74856 (231, 72, 86)
		/// </summary>
		red					= FOREGROUND_RED | FOREGROUND_INTENSITY,

		/// <summary>
		/// Green, #16C60C (22, 198, 12)
		/// </summary>
		green				= FOREGROUND_GREEN | FOREGROUND_INTENSITY,

		/// <summary>
		/// Blue, #3B78FF (59, 120, 255)
		/// </summary>
		blue				= FOREGROUND_BLUE | FOREGROUND_INTENSITY,

		/// <summary>
		/// Yellow, #F9F1A5 (249, 241, 165)
		/// </summary>
		yellow				= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,

		/// <summary>
		/// Cyan, #61D6D6 (97, 214, 214)
		/// </summary>
		cyan				= FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,

		/// <summary>
		/// Magenta, #B4009E (180, 0, 158)
		/// </summary>
		magenta				= FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY,

		/// <summary>
		/// Gray, #CCCCCC (204, 204, 204)
		/// </summary>
		gray				= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,

		/// <summary>
		/// White, #F2F2F2 (242, 242, 242)
		/// </summary>
		white				= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
	};

	/// <summary>
	/// Sets the console's foreground color.
	/// </summary>
	/// <param name="_Color">- The color to set.</param>
	/// <exception cref="invalid_state_exception"/>
	/// <exception cref="win32_exception"/>
	ARTEMIS_API void set_console_foreground_color(console_color _Color);

	/// <summary>
	/// Sets the console's background color.
	/// </summary>
	/// <param name="_Color">- The color to set.</param>
	/// <exception cref="invalid_state_exception"/>
	/// <exception cref="win32_exception"/>
	ARTEMIS_API void set_console_background_color(console_color _Color);
}

#endif // !ARTEMIS_API_CONSOLE_HXX