//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Astrea0014.												|
// @Astrea0014: https://github.com/Astrea0014											|
//-------------------------------------------------------------------------------------->

#include "Exceptions.h"

#include <string.h>
#include <Windows.h>

namespace Artemis {

	CHAR g_szContext[64];

	void ExceptionContext::SetContext(_In_ LPCSTR lpSender) { strcpy_s(g_szContext, lpSender); }
	void ExceptionContext::ResetContext() { g_szContext[0] = '\0'; }
	_Check_return_ BOOL ExceptionContext::HasContext() { return g_szContext[0] ? TRUE : FALSE; }

	Exception::Exception(
		_In_z_ LPCSTR lpMessage,
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

	InstanceInvalidException::InstanceInvalidException() : Exception("This object instance has invalid member fields.", ExceptionCode::InstanceInvalid, TRUE) { ExceptionEventManager::GetInstanceInvalidExceptionEvent().Invoke(this); }

	MemoryAccessViolationException::MemoryAccessViolationException(
		_In_ ADDRESS uAddress,
		_In_ DWORD dwSize,
		_In_ OperationType ot
	) : Exception(ot == OperationType::Read ? "Memory access violation reading memory." : "Memory access violation writing memory.", ExceptionCode::MemoryAccessViolation, TRUE) {
		this->uAddress = uAddress;
		this->dwSize = dwSize;
		this->ot = ot;

		ExceptionEventManager::GetMemoryAccessViolationExceptionEvent().Invoke(this);
	}

	WindowsApiException::WindowsApiException(_In_z_ LPCSTR lpFunction) : Exception("A Windows API function has failed.", ExceptionCode::WindowsApi, TRUE) {
		strcpy_s(this->szName, lpFunction);
		this->szWinMessage[0] = '\0';

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

	WindowsApiException::WindowsApiException(_In_z_ LPCSTR lpFunction, _In_ DWORD dwWindowsCode) : Exception("A Windows API function has failed.", ExceptionCode::WindowsApi, TRUE), dwWinCode(dwWindowsCode) {
		strcpy_s(this->szName, lpFunction);
		this->szWinMessage[0] = '\0';

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

	NotImplementedException::NotImplementedException() : Exception("Feature has yet to be implemented.", ExceptionCode::NotImplemented, TRUE) { ExceptionEventManager::GetNotImplementedExceptionEvent().Invoke(this); }

	CompatibilityException::CompatibilityException() : Exception("Two objects are incompatible with each other, or an object has attributes that are not compatible with the current operation.", ExceptionCode::Compatibility, TRUE) { ExceptionEventManager::GetCompatibilityExceptionEvent().Invoke(this); }

	AttributeException::AttributeException(_In_z_ LPCSTR lpAttributeName) : Exception("An object has incompatible attributes.", ExceptionCode::Attribute, TRUE) {
		strcpy_s(szAttributeName, lpAttributeName);
		ExceptionEventManager::GetAttributeExceptionEvent().Invoke(this);
	}

	IndexOutOfRangeException::IndexOutOfRangeException(_In_ INT nIndex, _In_ INT nMaxIndex) : Exception("An index that was out of range has been specified.", ExceptionCode::IndexOutOfRange, TRUE), nIndex(nIndex), nMaxIndex(nMaxIndex) { ExceptionEventManager::GetIndexOutOfRangeExceptionEvent().Invoke(this); }

	HookException::HookException(_In_z_ LPCSTR lpMessage, _In_ ExceptionCode ec) : Exception(lpMessage, ec, TRUE) { ExceptionEventManager::GetHookExceptionEvent().Invoke(this); }

	Event<ExceptionEventHandler> g_ExceptionEvent;
	Event<ParameterExceptionEventHandler> g_ParameterExceptionEvent;
	Event<InstanceInvalidExceptionEventHandler> g_InstanceInvalidExceptionEvent;
	Event<MemoryAccessViolationExceptionEventHandler> g_MemoryAccessViolationExceptionEvent;
	Event<WindowsApiExceptionEventHandler> g_WindowsApiExceptionEvent;
	Event<ObjectNotFoundExceptionEventHandler> g_ObjectNotFoundExceptionEvent;
	Event<NotImplementedExceptionEventHandler> g_NotImplementedExceptionEvent;
	Event<CompatibilityExceptionEventHandler> g_CompatibilityExceptionEvent;
	Event<AttributeExceptionEventHandler> g_AttributeExceptionEvent;
	Event<IndexOutOfRangeExceptionEventHandler> g_IndexOutOfRangeExceptionEvent;
	Event<HookExceptionEventHandler> g_HookExceptionEvent;

	_Check_return_ Event<ExceptionEventHandler>& ExceptionEventManager::GetExceptionEvent() { return g_ExceptionEvent; }
	_Check_return_ Event<ParameterExceptionEventHandler>& ExceptionEventManager::GetParameterExceptionEvent() { return g_ParameterExceptionEvent; }
	_Check_return_ Event<InstanceInvalidExceptionEventHandler>& ExceptionEventManager::GetInstanceInvalidExceptionEvent() { return g_InstanceInvalidExceptionEvent; }
	_Check_return_ Event<MemoryAccessViolationExceptionEventHandler>& ExceptionEventManager::GetMemoryAccessViolationExceptionEvent() { return g_MemoryAccessViolationExceptionEvent; }
	_Check_return_ Event<WindowsApiExceptionEventHandler>& ExceptionEventManager::GetWindowsApiExceptionEvent() { return g_WindowsApiExceptionEvent; }
	_Check_return_ Event<ObjectNotFoundExceptionEventHandler>& ExceptionEventManager::GetObjectNotFoundExceptionEvent() { return g_ObjectNotFoundExceptionEvent; }
	_Check_return_ Event<NotImplementedExceptionEventHandler>& ExceptionEventManager::GetNotImplementedExceptionEvent() { return g_NotImplementedExceptionEvent; }
	_Check_return_ Event<CompatibilityExceptionEventHandler>& ExceptionEventManager::GetCompatibilityExceptionEvent() { return g_CompatibilityExceptionEvent; }
	_Check_return_ Event<AttributeExceptionEventHandler>& ExceptionEventManager::GetAttributeExceptionEvent() { return g_AttributeExceptionEvent; }
	_Check_return_ Event<IndexOutOfRangeExceptionEventHandler>& ExceptionEventManager::GetIndexOutOfRangeExceptionEvent() { return g_IndexOutOfRangeExceptionEvent; }
	_Check_return_ Event<HookExceptionEventHandler>& ExceptionEventManager::GetHookExceptionEvent() { return g_HookExceptionEvent; }

}