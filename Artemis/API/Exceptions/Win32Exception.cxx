#include "pch.h"
#include "ExceptionBase.hxx"
#include "Win32Exception.hxx"

namespace Artemis::API {
	std::string_view win32_exception::win32_message(DWORD _Win32ErrorCode) noexcept {
		CHAR szBuffer[256];

		DWORD dwReturn = FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr,
			_Win32ErrorCode,
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
			szBuffer,
			sizeof(szBuffer),
			nullptr
		);

		if (!dwReturn)
			lstrcpyA(szBuffer, "Fetching error message failed.");

		return std::string_view(szBuffer);
	}

	win32_exception::win32_exception(std::string_view&& _FunctionName) noexcept : win32_exception(GetLastError(), std::move(_FunctionName)) {}

	win32_exception::win32_exception(DWORD _Win32ErrorCode, std::string_view&& _FunctionName) noexcept : exception(win32_message(_Win32ErrorCode)), _Win32Function(std::move(_FunctionName)), _Win32ErrorCode(_Win32ErrorCode) {}

	const std::string_view& win32_exception::win32_function() const noexcept { return this->_Win32Function; }

	DWORD win32_exception::win32_error_code() const noexcept { return this->_Win32ErrorCode; }
}