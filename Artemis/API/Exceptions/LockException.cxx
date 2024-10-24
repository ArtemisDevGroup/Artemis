#include "pch.h"
#include "ExceptionBase.hxx"
#include "LockException.hxx"

namespace Artemis::API {
	lock_exception::lock_exception(std::string_view&& _Message) noexcept : exception(std::move(_Message)) {}

	lock_access_exception::lock_access_exception() noexcept : lock_exception(lock_access_exception::_ExceptionMessage) {}
}