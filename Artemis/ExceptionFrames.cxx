#include "pch.h"
#include "ExceptionFrames.hxx"

namespace Artemis::_ {
	void __safe_exception_propagator::exec_l1cxx(const std::function<void()>& _Fn) const {
		API::call_stack* p = API::call_stack_manager::global()->fetch();
		assert(p != nullptr);
		assert(!p->is_empty());

		try {
			_Fn();
		}
		catch (const API::exception&) {
			throw;
		}
		catch (const std::exception& e) {
			throw API::exception(std::format("From std::exception or derived exception type: {}", e.what()));
		}
		catch (...) {
			throw API::exception("Unknown C++ exception.");
		}
	}

	void __safe_exception_propagator::exec_l1seh(const std::function<void()>& _Fn, bool _LetThroughCxxExceptions) const {
		API::call_stack* p = API::call_stack_manager::global()->fetch();
		assert(p != nullptr);
		assert(!p->is_empty());

		__try {
			_Fn();
		}
		__except (_LetThroughCxxExceptions ? API::seh_filter(GetExceptionInformation()).continue_on(EXCEPTION_CXX) : API::seh_filter(GetExceptionInformation()).handle_always()) {
			throw API::system_exception();
		}
	}

	void __safe_exception_propagator::exec_l2(const std::function<void()>& _Fn) const {
		this->exec_l1cxx([this, &_Fn]() { this->exec_l1seh(_Fn, true); });
	}

	void __safe_exception_net::exec_l1cxx(const std::function<void()>& _Fn) const noexcept {
		assert(this->Log != nullptr);
		
		try {
			_Fn();
		}
		catch (const API::system_exception& e) {
			Log->error(std::format("Artemis system_exception caught (code {}) at address '{}': {}\n{}",
				e.record()->ExceptionCode,
				e.record()->ExceptionAddress,
				e.what(),
				e.calls()->to_string()
			));
		}
		catch (const API::exception& e) {
			Log->error(std::format("Artemis exception caught: {}\n{}",
				e.what(),
				e.calls()->to_string()
			));
		}
		catch (const std::exception& e) {
			Log->error(std::format("C++ std::exception or derived caught: {}",
				e.what()
			));
		}
		catch (...) {
			Log->error("Unknown C++ exception caught.");
		}
	}

	void __safe_exception_net::exec_l1seh(const std::function<void()>& _Fn) const noexcept {
		__try {
			_Fn();
		}
		__except (API::seh_filter(GetExceptionInformation()).handle_always()) {
			API::seh_data* data = API::get_thread_seh_data();
			[this, data]() {
				Log->error(std::format("SEH exception handler caught system exception (code {}) at address '{}'",
					data->_Record.ExceptionCode,
					data->_Record.ExceptionAddress
				));	
			}();
		}
	}

	void __safe_exception_net::exec_l2(const std::function<void()>& _Fn) const noexcept {
		__stack_record();

		this->exec_l1cxx([_Fn]() {
			__try {
				_Fn();
			}
			__except (API::seh_filter(GetExceptionInformation()).continue_on(EXCEPTION_CXX)) {
				throw API::system_exception();
			}
		});

		__stack_escape();
	}
}