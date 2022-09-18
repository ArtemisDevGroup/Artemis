#include "Exceptions.h"

#include <string.h>
#include <Windows.h>

CHAR g_szContext[64];

void ExceptionContext::SetContext(_In_ LPCSTR lpSender) { strcpy_s(g_szContext, lpSender); }
void ExceptionContext::ResetContext() { g_szContext[0] = '\0'; }
BOOL ExceptionContext::HasContext() { return g_szContext[0] ? TRUE : FALSE; }

Exception::Exception(
	_In_ LPCSTR lpMessage,
	_In_ ExceptionCode ec,
	_In_ BOOL bNested
) {
	strcpy_s(this->szFunction, g_szContext);
	ExceptionContext::ResetContext();

	strcpy_s(this->szMessage, lpMessage);
	this->ec = ec;

	if (!bNested) ExceptionEventManager::GetExceptionEvent().Invoke(this);
}

ParameterException::ParameterException(_In_ LPCSTR lpParameter) : Exception("A required parameter contained an invalid value.", ExceptionCode::Parameter, TRUE) {
	strcpy_s(this->szParameter, lpParameter);

	ExceptionEventManager::GetParameterExceptionEvent().Invoke(this);
}

MemoryAccessViolationException::MemoryAccessViolationException(
	_In_ ADDRESS uAddress,
	_In_ DWORD dwSize,
	_In_ OperationType ot
) : Exception(ot == OperationType::Read ? "Memory access violation reading memory." : "Memory access violation writing memory.", ExceptionCode::MemoryAccessViolation, TRUE) {
	this->uAddress = uAddress;
	this->ot = ot;

	ExceptionEventManager::GetMemoryAccessViolationExceptionEvent().Invoke(this);
}

WindowsApiException::WindowsApiException(_In_ LPCSTR lpFunction) : Exception("A Windows API function has failed.", ExceptionCode::WindowsApi, TRUE) {
	strcpy_s(this->szName, lpFunction);

	this->dwWinCode = GetLastError();
	FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		this->dwWinCode,
		LANG_SYSTEM_DEFAULT,
		this->szWinMessage,
		sizeof(this->szWinMessage),
		nullptr
	);

	ExceptionEventManager::GetWindowsApiExceptionEvent().Invoke(this);
}

ObjectNotFoundException::ObjectNotFoundException(_In_ LPCSTR lpName, _In_ LPCSTR lpType) : Exception("An object could not be found.", ExceptionCode::ObjectNotFound, TRUE) {
	strcpy_s(this->szName, lpName);
	strcpy_s(this->szType, lpType);

	ExceptionEventManager::GetObjectNotFoundExceptionEvent().Invoke(this);
}

Event<ExceptionEventHandler> g_ExceptionEvent;
Event<ParameterExceptionEventHandler> g_ParameterExceptionEvent;
Event<InstanceInvalidExceptionEventHandler> g_InstanceInvalidExceptionEvent;
Event<MemoryAccessViolationExceptionEventHandler> g_MemoryAccessViolationExceptionEvent;
Event<WindowsApiExceptionEventHandler> g_WindowsApiExceptionEvent;
Event<ObjectNotFoundExceptionEventHandler> g_ObjectNotFoundExceptionEvent;
Event<NotImplementedExcepionEventHandler> g_NotImplementedExceptionEvent;

Event<ExceptionEventHandler>& ExceptionEventManager::GetExceptionEvent() { return g_ExceptionEvent; }
Event<ParameterExceptionEventHandler>& ExceptionEventManager::GetParameterExceptionEvent() { return g_ParameterExceptionEvent; }
Event<InstanceInvalidExceptionEventHandler>& ExceptionEventManager::GetInstanceInvalidExceptionEvent() { return g_InstanceInvalidExceptionEvent; }
Event<MemoryAccessViolationExceptionEventHandler>& ExceptionEventManager::GetMemoryAccessViolationExceptionEvent() { return g_MemoryAccessViolationExceptionEvent; }
Event<WindowsApiExceptionEventHandler>& ExceptionEventManager::GetWindowsApiExceptionEvent() { return g_WindowsApiExceptionEvent; }
Event<ObjectNotFoundExceptionEventHandler>& ExceptionEventManager::GetObjectNotFoundExceptionEvent() { return g_ObjectNotFoundExceptionEvent; }
Event<NotImplementedExcepionEventHandler>& ExceptionEventManager::GetNotImplementedExceptionEvent() { return g_NotImplementedExceptionEvent; }