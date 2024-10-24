#include "pch.h"
#include "Threading.hxx"

namespace Artemis::API {
	resource_awaiter::resource_awaiter(std::thread::id _ThreadId) noexcept : _ThreadId(_ThreadId), _IsAwaiting(true) {}

	std::thread::id resource_awaiter::awaiting_thread() const noexcept { return this->_ThreadId; }

	bool resource_awaiter::is_awaiting() const noexcept { return this->_IsAwaiting; }
	void resource_awaiter::cancel_await() noexcept { this->_IsAwaiting = false; }

	resource_awaiter* make_awaiter() noexcept { return new resource_awaiter(std::this_thread::get_id()); }
}