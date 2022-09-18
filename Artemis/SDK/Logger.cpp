#include "Logger.h"
#include <stdio.h>

void Logger::Log(
	LogLevel wLogLevel,
	LPCSTR lpSender,
	LPCSTR lpFormat,
	va_list lpArgs
) {
	CHAR szBuffer[256];
	vsprintf_s(szBuffer, lpFormat, lpArgs);

	LogEvent.Invoke(wLogLevel, lpSender, szBuffer);
}

Logger::Logger() {
	LogEvent += DefaultLogEventHandler;
}

void Logger::DefaultLogEventHandler(LogLevel wLogLevel, LPCSTR lpSender, LPCSTR lpMessage) {
	SYSTEMTIME st;
	GetLocalTime(&st);

	CHAR szTime[64];
	sprintf_s(szTime, "[%.2hu:%.2hu:%.2hu]", st.wHour, st.wMinute, st.wSecond);

	WORD wColor;
	CHAR szPrefix[16];
	switch (wLogLevel) {
	case LogLevel::Success:
		strcpy_s(szPrefix, "SUCCESS");
		wColor = FOREGROUND_GREEN;
		break;
	case LogLevel::Info:
		strcpy_s(szPrefix, "INFO");
		wColor = FOREGROUND_BLUE;
		break;
	case LogLevel::Warning:
		strcpy_s(szPrefix, "WARNING");
		wColor = FOREGROUND_RED | FOREGROUND_GREEN;
		break;
	case LogLevel::Error:
		strcpy_s(szPrefix, "ERROR");
		wColor = FOREGROUND_RED;
		break;
	default:
		return;
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	printf("%s [%s/", szTime, lpSender);
	SetConsoleTextAttribute(hConsole, wColor);
	printf("%s", szPrefix);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	printf("] %s\n", lpMessage);
}

void Logger::LogSuccessV(LPCSTR lpSender, LPCSTR lpFormat, va_list lpArgs) { Log(LogLevel::Success, lpSender, lpFormat, lpArgs); }

void Logger::LogSuccess(LPCSTR lpSender, LPCSTR lpFormat, ...) {
	va_list va;
	va_start(va, lpFormat);
	LogSuccessV(lpSender, lpFormat, va);
	va_end(va);
}

void Logger::LogInfoV(LPCSTR lpSender, LPCSTR lpFormat, va_list lpArgs) { Log(LogLevel::Info, lpSender, lpFormat, lpArgs); }

void Logger::LogInfo(LPCSTR lpSender, LPCSTR lpFormat, ...) {
	va_list va;
	va_start(va, lpFormat);
	LogInfoV(lpSender, lpFormat, va);
	va_end(va);
}

void Logger::LogWarningV(LPCSTR lpSender, LPCSTR lpFormat, va_list lpArgs) { Log(LogLevel::Warning, lpSender, lpFormat, lpArgs); }

void Logger::LogWarning(LPCSTR lpSender, LPCSTR lpFormat, ...) {
	va_list va;
	va_start(va, lpFormat);
	LogWarningV(lpSender, lpFormat, va);
	va_end(va);
}

void Logger::LogErrorV(LPCSTR lpSender, LPCSTR lpFormat, va_list lpArgs) { Log(LogLevel::Error, lpSender, lpFormat, lpArgs); }

void Logger::LogError(LPCSTR lpSender, LPCSTR lpFormat, ...) {
	va_list va;
	va_start(va, lpFormat);
	LogErrorV(lpSender, lpFormat, va);
	va_end(va);
}