#include "Logger.h"
#include "Exceptions.h"
#include "DateTime.h"

#include <Windows.h>

namespace Artemis {
	Logger::Logger() : bConsole(FALSE), bFile(FALSE), lpFile(nullptr), bLogTime(FALSE) {}

	Logger::Logger(
		_In_ BOOL bConsole,
		_In_ BOOL bFile,
		_In_ BOOL bLogTime
	) : bConsole(bConsole), bFile(bFile), lpFile(nullptr), bLogTime(bLogTime) {
		if (bFile)
			if (fopen_s(&lpFile, "log.log", "w"))
				throw Exception("fopen failed due to unknown reason.", ExceptionCode::Unknown);

		this->ConsoleLogEvent += DefaultConsoleLogEventHandler;
		this->FileLogEvent += DefaultFileLogEventHandler;
	}

	Logger::~Logger() { Release(); }

	void Logger::Log(
		_In_opt_z_ LPCSTR lpTime,
		_In_z_ LPCSTR lpPrefix,
		_In_ WORD wPrefixColor,
		_In_z_ LPCSTR lpSender,
		_In_z_ _Printf_format_string_ LPCSTR lpFormat,
		_In_ va_list lpArgs
	) {
		if (bConsole) ConsoleLogEvent.Invoke(nullptr, lpTime, lpPrefix, wPrefixColor, lpSender, lpFormat, lpArgs);
		if (bFile) FileLogEvent.Invoke(lpFile, lpTime, lpPrefix, 0, lpSender, lpFormat, lpArgs);
	}

	void Logger::DefaultConsoleLogEventHandler(
		_Reserved_ FILE* lpFile,
		_In_opt_z_ LPCSTR lpTime,
		_In_z_ LPCSTR lpPrefix,
		_In_ WORD wPrefixColor,
		_In_z_ LPCSTR lpSender,
		_In_z_ _Printf_format_string_ LPCSTR lpFormat,
		_In_ va_list lpArgs
	) {
		if (lpTime) printf_s("[%s] ", lpTime);
		printf_s("[%s/", lpSender);

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, wPrefixColor);
		printf_s("%s", lpPrefix);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		printf_s("] ");
		vprintf_s(lpFormat, lpArgs);
		printf_s("\n");
	}

	void Logger::DefaultFileLogEventHandler(
		_In_ FILE* lpFile,
		_In_opt_z_ LPCSTR lpTime,
		_In_z_ LPCSTR lpPrefix,
		_Reserved_ WORD wReserved,
		_In_z_ LPCSTR lpSender,
		_In_z_ _Printf_format_string_ LPCSTR lpFormat,
		_In_ va_list lpArgs
	) {
		if (lpTime) fprintf_s(lpFile, "[%s] ", lpTime);

		fprintf_s(lpFile, "[%s/%s] ", lpSender, lpPrefix);
		vfprintf_s(lpFile, lpFormat, lpArgs);
		fprintf_s(lpFile, "\n");
	}

	void Logger::LogInfo(
		_In_z_ LPCSTR lpSender,
		_In_z_ _Printf_format_string_ LPCSTR lpFormat,
		...
	) {
		CHAR szTime[16] = { '\0' };
		if (bLogTime) {
			DateTime dt = DateTime::Now();
			sprintf_s(szTime, "%.2hu:%.2hu:%.2hu", dt.wHour, dt.wMinute, dt.wSecond);
		}

		va_list v;
		va_start(v, lpFormat);
		Log(szTime[0] ? szTime : nullptr, "INFO", FOREGROUND_BLUE | FOREGROUND_INTENSITY, lpSender, lpFormat, v);
		va_end(v);
	}

	void Logger::LogSuccess(
		_In_z_ LPCSTR lpSender,
		_In_z_ _Printf_format_string_ LPCSTR lpFormat,
		...
	) {
		CHAR szTime[16] = { '\0' };
		if (bLogTime) {
			DateTime dt = DateTime::Now();
			sprintf_s(szTime, "%.2hu:%.2hu:%.2hu", dt.wHour, dt.wMinute, dt.wSecond);
		}

		va_list v;
		va_start(v, lpFormat);
		Log(szTime[0] ? szTime : nullptr, "SUCCESS", FOREGROUND_GREEN, lpSender, lpFormat, v);
		va_end(v);
	}

	void Logger::LogWarning(
		_In_z_ LPCSTR lpSender,
		_In_z_ _Printf_format_string_ LPCSTR lpFormat,
		...
	) {
		CHAR szTime[16] = { '\0' };
		if (bLogTime) {
			DateTime dt = DateTime::Now();
			sprintf_s(szTime, "%.2hu:%.2hu:%.2hu", dt.wHour, dt.wMinute, dt.wSecond);
		}

		va_list v;
		va_start(v, lpFormat);
		Log(szTime[0] ? szTime : nullptr, "WARNING", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, lpSender, lpFormat, v);
		va_end(v);
	}

	void Logger::LogError(
		_In_z_ LPCSTR lpSender,
		_In_z_ _Printf_format_string_ LPCSTR lpFormat,
		...
	) {
		CHAR szTime[16] = { '\0' };
		if (bLogTime) {
			DateTime dt = DateTime::Now();
			sprintf_s(szTime, "%.2hu:%.2hu:%.2hu", dt.wHour, dt.wMinute, dt.wSecond);
		}

		va_list v;
		va_start(v, lpFormat);
		Log(szTime[0] ? szTime : nullptr, "ERROR", FOREGROUND_RED, lpSender, lpFormat, v);
		va_end(v);
	}

	void Logger::Release() {
		if (bFile && lpFile) {
			fclose(lpFile);
			lpFile = nullptr;
		}
	}
}