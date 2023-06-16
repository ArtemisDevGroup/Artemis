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

#ifndef __AURORA_LOGGER_H__
#define __AURORA_LOGGER_H__

#include "Definitions.h"
#include "Exceptions.h"
#include "Console.h"
#include "Time.h"

namespace Aurora {
	/// <summary>
	/// Logs messages to the console and/or a file.
	/// </summary>
	class AURORA_API Logger {
	private:
		A_BOOL bLogToConsole;
		A_BOOL bLogToFile;
		FILE* lpLogFile;
		A_CHAR szLogFileName[MAX_PATH];

		A_VOID BeginPrintSession();
		A_VOID EndPrintSession();

	protected:
		/// <summary>
		/// Prints a format to the log file.
		/// </summary>
		/// <param name="lpFormat">- The format to print.</param>
		/// <param name="...">- The format arguments.</param>
		/// <exception cref="ParameterInvalidException"/>
		A_VOID PrintFile(_In_ _Printf_format_string_ A_LPCSTR lpFormat, ...) const;

		/// <summary>
		/// Prints a format to the log file.
		/// </summary>
		/// <param name="lpFormat">- The format to print.</param>
		/// <param name="lpArgs">- The format arguments.</param>
		/// <exception cref="ParameterInvalidException"/>
		A_VOID PrintFileV(_In_ _Printf_format_string_ A_LPCSTR lpFormat, _In_ va_list lpArgs) const;

		/// <summary>
		/// Virtual method that gets called every time the logger instance requests logging to the log file.
		/// </summary>
		/// <param name="refTime">- A reference to a time instance representing the time at which the method was called.</param>
		/// <param name="lpPrefix">- The log level prefix.</param>
		/// <param name="lpFormat">- The format string.</param>
		/// <param name="lpArgs">- The format arguments.</param>
		virtual A_VOID LogToFile(
			_In_ const Time& refTime,
			_In_z_ A_LPCSTR lpPrefix,
			_In_z_ _Printf_format_string_ A_LPCSTR lpFormat,
			_In_ va_list lpArgs
		);

		// <summary>
		/// Virtual method that gets called every time the logger instance requests logging to the console.
		/// </summary>
		/// <param name="refTime">- A reference to a time instance representing the time at which the method was called.</param>
		/// <param name="lpPrefix">- The log level prefix.</param>
		/// <param name="dwPrefixColor">- The color of the prefix string.</param>
		/// <param name="lpFormat">- The format string.</param>
		/// <param name="lpArgs">- The format arguments.</param>
		virtual A_VOID LogToConsole(
			_In_ const Time& refTime,
			_In_z_ A_LPCSTR lpPrefix,
			_In_ ConsoleColorLegacyFlags dwPrefixColor,
			_In_z_ _Printf_format_string_ A_LPCSTR lpFormat,
			_In_ va_list lpArgs
		);

	private:
		A_VOID Log(
			_In_z_ A_LPCSTR lpPrefix,
			_In_ ConsoleColorLegacyFlags dwPrefixColor,
			_In_z_ _Printf_format_string_ A_LPCSTR lpFormat,
			_In_ va_list lpArgs
		);

	public:
		constexpr Logger() noexcept;

		/// <summary>
		/// Constructs a Logger.
		/// </summary>
		/// <param name="bLogToConsole">- If set to true, enables logging to the console. 'LogToConsole' will be called for every log message.</param>
		/// <param name="blogToFile">- If set to true, enables logging to a file. 'LogToFile' will be called for every log message.</param>
		/// <param name="lpLogFileName">
		/// <para>- The name of the log file. If nullptr, "log.log" will be used as the name instead</para>
		/// <para>This parameter is only necessary to specify if you are running multiple Logger instances.</para>
		/// <para>This parameter can also be used to set the log file location for the instance, either to a relative or absolute path.</para>
		/// </param>
		Logger(
			_In_ A_BOOL bLogToConsole,
			_In_ A_BOOL blogToFile,
			_In_opt_z_ A_LPCSTR lpLogFileName = nullptr
		);

		/// <summary>
		/// Logs an informational message.
		/// </summary>
		/// <param name="lpFormat">- The format of the message.</param>
		/// <param name="lpArgs">- The format arguments.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="ErrnoException"/>
		A_VOID LogInfoV(_In_z_ _Printf_format_string_ A_LPCSTR lpFormat, _In_ va_list lpArgs);

		/// <summary>
		/// Logs an informational message.
		/// </summary>
		/// <param name="lpFormat">- The format of the message.</param>
		/// <param name="...">- The format arguments.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="ErrnoException"/>
		A_VOID LogInfo(_In_z_ _Printf_format_string_ A_LPCSTR lpFormat, ...);

		/// <summary>
		/// Logs a message indicating success.
		/// </summary>
		/// <param name="lpFormat">- The format of the message.</param>
		/// <param name="lpArgs">- The format arguments.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="ErrnoException"/>
		A_VOID LogSuccessV(_In_z_ _Printf_format_string_ A_LPCSTR lpFormat, _In_ va_list lpArgs);

		// <summary>
		/// Logs a message indicating success.
		/// </summary>
		/// <param name="lpFormat">- The format of the message.</param>
		/// <param name="...">- The format arguments.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="ErrnoException"/>
		A_VOID LogSuccess(_In_z_ _Printf_format_string_ A_LPCSTR lpFormat, ...);

		/// <summary>
		/// Logs a warning message.
		/// </summary>
		/// <param name="lpFormat">- The format of the message.</param>
		/// <param name="lpArgs">- The format arguments.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="ErrnoException"/>
		A_VOID LogWarningV(_In_z_ _Printf_format_string_ A_LPCSTR lpFormat, _In_ va_list lpArgs);

		// <summary>
		/// Logs a message indicating success.
		/// </summary>
		/// <param name="lpFormat">- The format of the message.</param>
		/// <param name="...">- The format arguments.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="ErrnoException"/>
		A_VOID LogWarning(_In_z_ _Printf_format_string_ A_LPCSTR lpFormat, ...);

		/// <summary>
		/// Logs a message indicating an error.
		/// </summary>
		/// <param name="lpFormat">- The format of the message.</param>
		/// <param name="lpArgs">- The format arguments.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="ErrnoException"/>
		A_VOID LogErrorV(_In_z_ _Printf_format_string_ A_LPCSTR lpFormat, _In_ va_list lpArgs);

		// <summary>
		/// Logs a message indicating success.
		/// </summary>
		/// <param name="lpFormat">- The format of the message.</param>
		/// <param name="...">- The format arguments.</param>
		/// <exception cref="ParameterInvalidException"/>
		/// <exception cref="ErrnoException"/>
		A_VOID LogError(_In_z_ _Printf_format_string_ A_LPCSTR lpFormat, ...);
	};
}

#endif // !__AURORA_LOGGER_H__