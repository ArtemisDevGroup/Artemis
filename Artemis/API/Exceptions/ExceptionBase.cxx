#include "pch.h"
#include "ExceptionBase.hxx"

namespace Artemis::API {
	thread_local bool _IsNextThrowTraceDisabled = false;

	void exception::disable_next_throw_trace() noexcept {
		_IsNextThrowTraceDisabled = true;
	}

	exception::exception() noexcept : std::exception("An unknown Artemis exception has occured."), _Trace(), _InnerException(nullptr) {
		if (!_IsNextThrowTraceDisabled)
			_Trace = std::stacktrace::current();
		else
			_IsNextThrowTraceDisabled = false;
	}

	exception::exception(std::string_view&& _Message) noexcept : std::exception(_Message.data()), _Trace(), _InnerException(nullptr) {
		if (!_IsNextThrowTraceDisabled)
			_Trace = std::stacktrace::current();
		else
			_IsNextThrowTraceDisabled = false;
	}

	exception::exception(exception&& _Other) noexcept : _Trace(std::move(_Other._Trace)) {
		if (_Other._InnerException) {
			this->_InnerException = _Other._InnerException;
			_Other._InnerException = nullptr;
		}
		else this->_InnerException = nullptr;
	}

	exception::~exception() noexcept {
		if (this->_InnerException) {
			delete this->_InnerException;
			this->_InnerException = nullptr;
		}
	}

	const exception* exception::inner() const noexcept { return this->_InnerException; }

	const std::stacktrace& exception::trace() const noexcept { return this->_Trace; }

	exception& exception::operator=(exception&& _Other) noexcept {
		this->_Trace = std::move(_Other._Trace);

		if (_Other._InnerException) {
			this->_InnerException = _Other._InnerException;
			_Other._InnerException = nullptr;
		}
		else this->_InnerException = nullptr;

		return *this;
	}
}