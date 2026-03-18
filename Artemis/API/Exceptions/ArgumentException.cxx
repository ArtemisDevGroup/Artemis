#include "pch.h"
#include "ExceptionBase.hxx"
#include "ArgumentException.hxx"

namespace Artemis::API {
	argument_exception::argument_exception(std::string_view _Message, std::string_view _ArgumentName) noexcept : exception(_Message), _ArgumentName(_ArgumentName) {}

	const std::string& argument_exception::argument() const noexcept { return this->_ArgumentName; }
}