#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include "../H/Definitions.h"
#include "MemoryDefinitions.h"
#include "Events.h"

enum class ExceptionCode {
	Unknown = -1,
	Success = 0,
	Parameter,
	InstanceInvalid,
	MemoryAccessViolation,
	WindowsApi,
	ObjectNotFound,
	NotImplemented
};

//------------------------
// Rules for context:
// 1. Set context in the beginning of the call.
// 2. ALWAYS check if there is a previously set context.
// 3. ALWAYS reset the context at the end of the function, but ONLY if the context is set in that same function.
//------------------------

class ExceptionContext {
	inline ExceptionContext() {}
	inline ExceptionContext(const ExceptionContext&) {}

public:
	static void SetContext(_In_ LPCSTR lpSender);
	static void ResetContext();
	static BOOL HasContext();
};

class Exception {
	ExceptionCode ec;
	CHAR szMessage[MAX_MESSAGE];
	CHAR szFunction[MAX_NAME];

public:
	Exception(
		_In_ LPCSTR lpMessage,
		_In_ ExceptionCode ec,
		_In_ BOOL bNested
	);

#undef GetExceptionCode
	inline ExceptionCode GetExceptionCode() const { return this->ec; }
	inline LPCSTR GetExceptionMessage() const { return (LPCSTR)this->szMessage; }
	inline LPCSTR GetFunctionName() const { return (LPCSTR)this->szFunction; }
};

class ParameterException : public Exception {
	CHAR szParameter[MAX_NAME];
public:
	ParameterException(
		_In_ LPCSTR lpParameter
	);

	inline LPCSTR GetParameterName() const { return (LPCSTR)this->szParameter; }
};

class InstanceInvalidException : public Exception { public: inline InstanceInvalidException() : Exception("This object instance has invalid member fields.", ExceptionCode::InstanceInvalid, TRUE) {} };

enum class OperationType { Read, Write };

class MemoryAccessViolationException : public Exception {
	ADDRESS uAddress;
	OperationType ot;

public:
	MemoryAccessViolationException(
		_In_ ADDRESS uAddress,
		_In_ DWORD dwSize,
		_In_ OperationType ot
	);

	inline ADDRESS GetAddress() const { return uAddress; }
	inline OperationType GetOperationType() const { return ot; }
};

class WindowsApiException : public Exception {
	CHAR szName[MAX_NAME];

	DWORD dwWinCode;
	CHAR szWinMessage[MAX_MESSAGE];
public:
	WindowsApiException(_In_ LPCSTR lpFunction);

	inline LPCSTR GetWindowsFunction() const { return szName; }
	inline DWORD GetWindowsCode() const { return dwWinCode; }
	inline LPCSTR GetWindowsMessage() const { return szWinMessage; }
};

class ObjectNotFoundException : public Exception {
	CHAR szName[MAX_NAME];
	CHAR szType[MAX_NAME];

public:
	ObjectNotFoundException(_In_ LPCSTR lpName, _In_ LPCSTR lpType);

	inline LPCSTR GetObjectName() const { return szName; }
	inline LPCSTR GetObjectType() const { return szType; }
};

class NotImplementedException : public Exception {
public:
	inline NotImplementedException() : Exception("Features has yet to be implemented.", ExceptionCode::NotImplemented, TRUE) { ExceptionEventManager::GetNotImplementedExceptionEvent().Invoke(this); }
};

typedef void(*ExceptionEventHandler)(Exception* lpException);
typedef void(*ParameterExceptionEventHandler)(ParameterException* lpException);
typedef void(*InstanceInvalidExceptionEventHandler)(InstanceInvalidException* lpException);
typedef void(*MemoryAccessViolationExceptionEventHandler)(MemoryAccessViolationException* lpException);
typedef void(*WindowsApiExceptionEventHandler)(WindowsApiException* lpException);
typedef void(*ObjectNotFoundExceptionEventHandler)(ObjectNotFoundException* lpException);
typedef void(*NotImplementedExcepionEventHandler)(NotImplementedException* lpException);

class ExceptionEventManager {
	ExceptionEventManager() {}
	ExceptionEventManager(const ExceptionEventManager&) {}

public:
	static Event<ExceptionEventHandler>& GetExceptionEvent();
	static Event<ParameterExceptionEventHandler>& GetParameterExceptionEvent();
	static Event<InstanceInvalidExceptionEventHandler>& GetInstanceInvalidExceptionEvent();
	static Event<MemoryAccessViolationExceptionEventHandler>& GetMemoryAccessViolationExceptionEvent();
	static Event<WindowsApiExceptionEventHandler>& GetWindowsApiExceptionEvent();
	static Event<ObjectNotFoundExceptionEventHandler>& GetObjectNotFoundExceptionEvent();
	static Event<NotImplementedExcepionEventHandler>& GetNotImplementedExceptionEvent();
};

#endif // !__EXCEPTIONS_H__