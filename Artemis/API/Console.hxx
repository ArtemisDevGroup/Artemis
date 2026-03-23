#ifndef __ARTEMIS_API_CONSOLE_HXX__
#define __ARTEMIS_API_CONSOLE_HXX__

#include "Definitions.hxx"

#include <Windows.h>	// HWND, WORD, FOREGROUND_*

#include <string_view>	// std::string_view

namespace Artemis::API {
	/**
	 * @brief Opens the console window.
	 * @throws invalid_state_exception
	 * @throws win32_exception
	 */
	ARTEMIS_API void open_console();

	/**
	 * @brief Closes the console window.
	 * @throws invalid_state_exception
	 * @throws win32_exception
	 */
	ARTEMIS_API void close_console();

	/**
	 * @brief Identifies whether the console window is open or not.
	 * @return `true` if the console window is open, otherwise `false`.
	 */
	ARTEMIS_API bool is_console_open() noexcept;

	/**
	 * @brief Fetches the window handle of the console window.
	 * @return The console window handle.
	 * @throws invalid_state_exception
	 */
	ARTEMIS_API HWND console_window_handle();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_Title">- The title to give the console window.</param>
	/// <exception cref="invalid_state_exception"/>
	/// <exception cref="win32_exception"/>

	/**
	 * @brief Sets the console window title.
	 * @param[in] _Title The title to set the console window to.
	 */
	ARTEMIS_API void set_console_window_title(std::string_view _Title);

	/**
	 * @brief Associates the console IO devices with their respective streams.
	 * @throws invalid_state_exception
	 * @throws errno_exception
	 */
	ARTEMIS_API void open_console_io();

	/**
	 * @brief Closes previously associated IO streams.
	 * @throws invalid_state_exception
	 * @throws errno_exception
	 */
	ARTEMIS_API void close_console_io();

	/**
	 * @brief Identifies whether the console IO streams are associated with their respective devices or not.
	 * @return `true` of the console IO streams are associated with their respective devices, otherwise `false`.
	 */
	ARTEMIS_API bool is_console_io_open() noexcept;

	/**
	 * @brief Represents a color supported by a 16-color terminal.
	 */
	enum class console_color : WORD {
		/**
		 * @brief Black, #0C0C0C (12, 12, 12)
		 */
		black				= 0x0000,

		/**
		 * @brief Dark Red, #C50F1F (197, 15, 31)
		 */
		dark_red			= FOREGROUND_RED,

		/**
		 * @brief Dark Green, #13A10E (19, 161, 14)
		 */
		dark_green			= FOREGROUND_GREEN,

		/**
		 * @brief Dark Blue, #0037DA (0, 55, 218)
		 */
		dark_blue			= FOREGROUND_BLUE,

		/**
		 * @brief Dark Gray, #767676 (118, 118, 118)
		 */
		dark_gray			= FOREGROUND_INTENSITY,

		/**
		 * @brief Dark Yellow, #C19C00 (193, 156, 0)
		 */
		dark_yellow			= FOREGROUND_RED | FOREGROUND_GREEN,

		/**
		 * @brief Dark Cyan, #3A96DD (58, 150, 221)
		 */
		dark_cyan			= FOREGROUND_GREEN | FOREGROUND_BLUE,

		/**
		 * @brief Dark Mangenta, #881798 (136, 23, 152)
		 */
		dark_magenta		= FOREGROUND_BLUE | FOREGROUND_RED,

		/**
		 * @brief Red, #E74856 (231, 72, 86)
		 */
		red					= FOREGROUND_RED | FOREGROUND_INTENSITY,

		/**
		 * @brief Green, #16C60C (22, 198, 12)
		 */
		green				= FOREGROUND_GREEN | FOREGROUND_INTENSITY,

		/**
		 * @brief Blue, #3B78FF (59, 120, 255)
		 */
		blue				= FOREGROUND_BLUE | FOREGROUND_INTENSITY,

		/**
		 * @brief Yellow, #F9F1A5 (249, 241, 165)
		 */
		yellow				= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,

		/**
		 * @brief Cyan, #61D6D6 (97, 214, 214)
		 */
		cyan				= FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,

		/**
		 * @brief Magenta, #B4009E (180, 0, 158)
		 */
		magenta				= FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY,

		/**
		 * @brief Gray, #CCCCCC (204, 204, 204)
		 */
		gray				= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,

		/**
		 * @brief White, #F2F2F2 (242, 242, 242)
		 */
		white				= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
	};

	/**
	 * @brief Sets the console's foreground color.
	 * @param[in] _Color The color to set.
	 * @throws invalid_state_exception
	 * @throws win32_exception
	 */
	ARTEMIS_API void set_console_foreground_color(console_color _Color);

	/**
	 * @brief Sets the console's background color.
	 * @param[in] _Color The color to set.
	 * @throws invalid_state_exception
	 * @throws win32_exception
	 */
	ARTEMIS_API void set_console_background_color(console_color _Color);
}

#endif // !__ARTEMIS_API_CONSOLE_HXX__