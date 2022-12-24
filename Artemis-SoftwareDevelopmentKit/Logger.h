//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "Definitions.h"
#include "Events.h"
#include "Interfaces.h"

#include <stdio.h>

namespace Artemis {
	/// <summary>
	/// An event handler for logging. Can be used for both Console logging and File logging.
	/// <para>NOTES:</para>
	/// <para>Whenever an event handler for Console logging is created, lpFile is reserved and shall be SAL annotated as such.</para>
	/// <para>Whenever an event handler for File logging is created, wPrefixColor is reserved and shall be SAL annotated as such.</para>
	/// </summary>
	typedef void(*LogEventHandler)(
		_In_ /* When File. */ _Reserved_ /* When Console. */ FILE* lpFile,
		_In_opt_z_ LPCSTR lpTime,
		_In_z_ LPCSTR lpPrefix,
		_In_ /* When Console. */ _Reserved_ /* When File. */ WORD wPrefixColor,
		_In_z_ LPCSTR lpSender,
		_In_z_ _Printf_format_string_ LPCSTR lpFormat,
		_In_ va_list lpArgs
		);

	/// <summary>
	/// A class for logging events to files and/or the console.
	/// </summary>
	class ARTEMIS_API Logger : public IDisposable {
		BOOL bConsole;
		BOOL bFile;
		BOOL bLogTime;

		FILE* pFile;

		void Log(
			_In_opt_z_ LPCSTR lpTime,
			_In_z_ LPCSTR lpPrefix,
			_In_ WORD wPrefixColor,
			_In_z_ LPCSTR lpSender,
			_In_z_ _Printf_format_string_ LPCSTR lpFormat,
			_In_ va_list lpArgs
		) const;

	public:
		Logger();

		/// <summary>
		/// Constructs a Logger object logging in the specified ways.
		/// </summary>
		/// <param name="bConsole">- Logs to stdout A.K.A. the console.</param>
		/// <param name="bFile">- Logs to a file called log.log in the host process directory.</param>
		/// <param name="bLogTime">- Defines wether to add timestamps to logs or not.</param>
		Logger(
			_In_ BOOL bConsole,
			_In_ BOOL bFile,
			_In_ BOOL bLogTime = TRUE
		);

		/// <summary>
		/// An event that gets invoked every time something gets logged to the console through this instance.
		/// </summary>
		Event<LogEventHandler> ConsoleLogEvent;

		/// <summary>
		/// An event that gets invoked every time something gets logged to the log.log file through this instance.
		/// </summary>
		Event<LogEventHandler> FileLogEvent;

		/// <summary>
		/// The default event handler for console logging.
		/// </summary>
		/// <param name="lpReserved">- A reserved parameter. This shall be set to nullptr when called.</param>
		/// <param name="lpTime">- A string describing the time of the log action. Can be null.</param>
		/// <param name="lpPrefix">- The message prefix. Can be one of these values: "INFO", "SUCCESS", "WARNING", "ERROR".</param>
		/// <param name="wPrefixColor">- The Windows color constant of the message prefix. Can be used in calls to for example SetConsoleTextAttribute.</param>
		/// <param name="lpSender">- The sender of the log message. For example the name of the function where the message was logged.</param>
		/// <param name="lpFormat">- A format specifier of the message. Can be used in functions like for example vsprintf.</param>
		/// <param name="lpArgs">- The argument list belonging to the format specifier.</param>
		static void DefaultConsoleLogEventHandler(
			_Reserved_ FILE* lpReserved,
			_In_opt_z_ LPCSTR lpTime,
			_In_z_ LPCSTR lpPrefix,
			_In_ WORD wPrefixColor,
			_In_z_ LPCSTR lpSender,
			_In_z_ _Printf_format_string_ LPCSTR lpFormat,
			_In_ va_list lpArgs
		);

		/// <summary>
		/// The default event handler for file logging.
		/// </summary>
		/// <param name="lpFile">- A pointer to the file to log to.</param>
		/// <param name="lpTime">- A string describing the time of the log action. Can be null.</param>
		/// <param name="lpPrefix">- The message prefix. Can be one of these values: "INFO", "SUCCESS", "WARNING", "ERROR".</param>
		/// <param name="wReserved">- A reserved parameter. This shall be set to 0 when called.</param>
		/// <param name="lpSender">- The sender of the log message. For example the name of the function where the message was logged.</param>
		/// <param name="lpFormat">- A format specifier of the message. Can be used in functions like for example vsprintf.</param>
		/// <param name="lpArgs">- The argument list belonging to the format specifier.</param>
		static void DefaultFileLogEventHandler(
			_In_ FILE* lpFile,
			_In_opt_z_ LPCSTR lpTime,
			_In_z_ LPCSTR lpPrefix,
			_Reserved_ WORD wReserved,
			_In_z_ LPCSTR lpSender,
			_In_z_ _Printf_format_string_ LPCSTR lpFormat,
			_In_ va_list lpArgs
		);

		/// <summary>
		/// Logs a message containing information.
		/// </summary>
		/// <param name="lpSender">- The sender of the log message. For example the name of the function where the message was logged.</param>
		/// <param name="lpFormat">- A printf format specifier.</param>
		void LogInfo(
			_In_z_ LPCSTR lpSender,
			_In_z_ _Printf_format_string_ LPCSTR lpFormat,
			...
		) const;

		/// <summary>
		/// Logs a message noting an operation succeeding.
		/// </summary>
		/// <param name="lpSender">- The sender of the log message. For example the name of the function where the message was logged.</param>
		/// <param name="lpFormat">- A printf format specifier.</param>
		void LogSuccess(
			_In_z_ LPCSTR lpSender,
			_In_z_ _Printf_format_string_ LPCSTR lpFormat,
			...
		) const;

		/// <summary>
		/// Logs a message noting an operation warning about potential error risks.
		/// </summary>
		/// <param name="lpSender">- The sender of the log message. For example the name of the function where the message was logged.</param>
		/// <param name="lpFormat">- A printf format specifier.</param>
		void LogWarning(
			_In_z_ LPCSTR lpSender,
			_In_z_ _Printf_format_string_ LPCSTR lpFormat,
			...
		) const;

		/// <summary>
		/// Logs a message noting an operation throwing an exception.
		/// </summary>
		/// <param name="lpSender">- The sender of the log message. For example the name of the function where the message was logged.</param>
		/// <param name="lpFormat">- A printf format specifier.</param>
		void LogError(
			_In_z_ LPCSTR lpSender,
			_In_z_ _Printf_format_string_ LPCSTR lpFormat,
			...
		) const;

		/// <summary>
		/// Releases the associated log file.
		/// </summary>
		void Release();
	};
}

#endif // !__LOGGER_H__