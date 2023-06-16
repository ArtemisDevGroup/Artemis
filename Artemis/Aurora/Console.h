//------------------------------------------------------------------------>
// MIT License
// 
// Copyright (c) 2023 Artemis Group
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------>
// Aurora: https://github.com/ArtemisDevGroup/Aurora
// This file was authored by:
// @Astrea0014: https://github.com/Astrea0014
//------------------------------------------------------------------------>

#ifndef __AURORA_CONSOLE_H__
#define __AURORA_CONSOLE_H__

#include "Definitions.h"
#include "Exceptions.h"
#include "Color.h"

namespace Aurora {
	/// <summary>
	/// Represents a standard data stream.
	/// </summary>
	enum class StandardStream {
		Out,	// The stdout stream.
		In		// The stdin stream.
	};

	/// <summary>
	/// A set of flags representing a color from the 16-color console standard.
	/// </summary>
	struct ConsoleColorLegacyFlags {
		A_WORD wValue;

		enum Enumeration : A_WORD {
			Black = 0x0000,

			DarkRed = 0x0004,
			DarkGreen = 0x0002,
			DarkBlue = 0x0001,
			DarkGray = 0x0008,

			DarkCyan = DarkBlue | DarkGreen,
			DarkMagenta = DarkRed | DarkBlue,
			DarkYellow = DarkRed | DarkGreen,

			Gray = DarkRed | DarkGreen | DarkBlue,
			Red = DarkRed | DarkGray,
			Green = DarkGreen | DarkGray,
			Blue = DarkBlue | DarkGray,

			Cyan = DarkCyan | DarkGray,
			Magenta = DarkMagenta | DarkGray,
			Yellow = DarkYellow | DarkGray,
			White = DarkRed | DarkGreen | DarkBlue | DarkGray
		};

		constexpr ConsoleColorLegacyFlags() noexcept : wValue(0) {}
		constexpr ConsoleColorLegacyFlags(A_WORD wValue) noexcept : wValue(wValue) {}

		/// <summary>
		/// Gets a WORD representation of the color assigned to this instance to pass to SetConsoleTextAttribute to set the foreground.
		/// </summary>
		/// <returns>A WORD representing the foreground color.</returns>
		AURORA_NDWR_GET("GetForegroundLiteral") constexpr A_WORD GetForegroundLiteral() const noexcept { return wValue; }

		/// <summary>
		/// Gets a WORD representation of the color assigned to this instace to pass to SetConsoleTextAttribute to set the background.
		/// </summary>
		/// <returns>A WORD representing the background color.</returns>
		AURORA_NDWR_GET("GetBackgroundLiteral") constexpr A_WORD GetBackgroundLiteral() const noexcept { return wValue << 4; }
	};

	/// <summary>
	/// Creates a console window if the process does not already have one open.
	/// </summary>
	/// <exception cref="WindowsApiException"/>
	AURORA_API A_VOID CreateConsole();

	/// <summary>
	/// Releases the console window. If the console window was not manually created,
	/// calling this function is unnecessary.
	/// </summary>
	/// <exception cref="WindowsApiException"/>
	AURORA_API A_VOID ReleaseConsole();

	/// <summary>
	/// Opens the specified standard stream for being used in console in or out operations.
	/// </summary>
	/// <param name="Stream">- The stream to open.</param>
	/// <exception cref="ErrnoException"/>
	/// <exception cref="ParameterInvalidException"/>
	AURORA_API A_VOID OpenStream(_In_ StandardStream Stream);

	/// <summary>
	/// <para>Closes the specified standard stream from being used in console in or out operations.</para>
	/// <para>If the standard stream was not manually opened, this function does nothing.</para>
	/// </summary>
	/// <param name="Stream">- The stream to close.</param>
	/// <exception cref="ErrnoException"/>
	/// <exception cref="ParameterInvalidException"/>
	AURORA_API A_VOID CloseStream(_In_ StandardStream Stream);

	/// <summary>
	/// <para>Closes all opened standard streams from being used in console in or out operations.</para>
	/// <para>If the standard stream was not manually opened, this function does nothing.</para>
	/// </summary>
	/// <exception cref="ErrnoException"/>
	/// <exception cref="ParameterInvalidException"/>
	AURORA_API A_VOID CloseAllStreams();

	/// <summary>
	/// <para>Sets the foreground color of the console. This function only works with applications ran in a TrueColor enabled terminal.</para>
	/// <para>For example: Windows Terminal is TrueColor enabled, whereas the Legacy Windows Command Prompt is not.</para>
	/// </summary>
	/// <param name="TrueColor">- The color to set the foreground to, in RGB.</param>
	AURORA_API A_VOID SetConsoleForegroundColor(_In_ const RGB& TrueColor);

	/// <summary>
	/// <para>Resets the foreground color of the console. This function only works with applications ran in a TrueColor enabled terminal.</para>
	/// <para>For example: Windows Terminal is TrueColor enabled, whereas the Legacy Windows Command Prompt is not.</para>
	/// </summary>
	AURORA_API A_VOID ResetConsoleForegroundColor();

	/// <summary>
	/// <para>Sets the background color of the console. This function only works with applications ran in a TrueColor enabled terminal.</para>
	/// <para>For example: Windows Terminal is TrueColor enabled, whereas the Legacy Windows Command Prompt is not.</para>
	/// </summary>
	/// <param name="TrueColor">- The color to set the foreground to, in RGB.</param>
	AURORA_API A_VOID SetConsoleBackgroundColor(_In_ const RGB& TrueColor);

	/// <summary>
	/// <para>Resets the background color of the console. This function only works with applications ran in a TrueColor enabled terminal.</para>
	/// <para>For example: Windows Terminal is TrueColor enabled, whereas the Legacy Windows Command Prompt is not.</para>
	/// </summary>
	AURORA_API A_VOID ResetConsoleBackgroundColor();

	/// <summary>
	/// Sets the foreground color of the console. Supports all windows terminals.
	/// </summary>
	/// <param name="wColorFlags">- The colors to set.</param>
	/// <exception cref="WindowsApiException"/>
	AURORA_API A_VOID SetConsoleForegroundColorLegacy(_In_ ConsoleColorLegacyFlags wColorFlags);

	/// <summary>
	/// Sets the background color of the console. Supports all windows terminals.
	/// </summary>
	/// <param name="wColorFlags">- The colors to set.</param>
	/// <exception cref="WindowsApiException"/>
	AURORA_API A_VOID SetConsoleBackgroundColorLegacy(_In_ ConsoleColorLegacyFlags wColorFlags);
}

#endif // !__AURORA_CONSOLE_H__