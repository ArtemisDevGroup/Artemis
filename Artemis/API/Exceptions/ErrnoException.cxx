#include "pch.h"
#include "ExceptionBase.hxx"
#include "ErrnoException.hxx"

namespace Artemis::API {
	std::string_view errno_exception::errno_message(errno_t _ErrnoCode) {
		char buffer[256];
		strerror_s(buffer, _ErrnoCode);
#pragma warning(push)
#pragma warning(disable:6054)	// String 'buffer' might not be zero-terminated
		// Disabled due to strerror_s not being SAL-annoted as
		// guaranteeing null termination, but does.
		return std::string_view(buffer);
#pragma warning(pop)
	}

	errno_exception::errno_exception(std::string_view&& _FunctionName) noexcept : exception(errno_message(errno)), _ErrnoCode(errno), _CStdFunction(std::move(_FunctionName)) {}

	errno_exception::errno_exception(errno_t _ErrnoCode, std::string_view&& _FunctionName) noexcept : exception(errno_message(_ErrnoCode)), _ErrnoCode(_ErrnoCode), _CStdFunction(std::move(_FunctionName)) {}

	const std::string_view& errno_exception::cstd_function() const noexcept { return this->_CStdFunction; }

	errno_t errno_exception::errno_code() const noexcept { return this->_ErrnoCode; }
}