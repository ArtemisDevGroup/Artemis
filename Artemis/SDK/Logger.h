#ifndef __LOGGER_H__
#define __LOGGER_H__

#ifdef __cplusplus

#include "Def.h"
#include "Event.h"
#include "String.h"

class Logger {
public:
	enum class LogLevel : WORD {
		Success,
		Info,
		Warning,
		Error
	};

	event<DelegateClass<void(*)(LogLevel, LPCSTR, LPCSTR)>> LogEvent;
private:
	void Log(
		LogLevel wLogLevel,
		LPCSTR lpSender,
		LPCSTR lpFormat,
		va_list lpArgs
	);
public:
	Logger();

	static void DefaultLogEventHandler(LogLevel wLogLevel, LPCSTR lpSender, LPCSTR lpMessage);

	void LogSuccessV(LPCSTR lpSender, LPCSTR lpFormat, va_list lpArgs);
	void LogSuccess(LPCSTR lpSender, LPCSTR lpFormat, ...);

	void LogInfoV(LPCSTR lpSender, LPCSTR lpFormat, va_list lpArgs);
	void LogInfo(LPCSTR lpSender, LPCSTR lpFormat, ...);

	void LogWarningV(LPCSTR lpSender, LPCSTR lpFormat, va_list lpArgs);
	void LogWarning(LPCSTR lpSender, LPCSTR lpFormat, ...);

	void LogErrorV(LPCSTR lpSender, LPCSTR lpFormat, va_list lpArgs);
	void LogError(LPCSTR lpSender, LPCSTR lpFormat, ...);
};

#endif // __cplusplus

#endif // !__LOGGER_H__