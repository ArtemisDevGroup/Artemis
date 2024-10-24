#include "pch.h"
#include "ExceptionBase.hxx"
#include "ArgumentException.hxx"

namespace Artemis::API {
	argument_exception::argument_exception(std::string_view&& _Message, std::string_view&& _ArgumentName) noexcept : exception(std::move(_Message)), _ArgumentName(std::move(_ArgumentName)) {}

	const std::string_view& argument_exception::argument() const noexcept { return this->_ArgumentName; }
}