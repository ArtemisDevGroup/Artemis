#include "pch.h"
#include "KeyAction.hxx"

namespace Artemis {
#pragma region Class key_action_manager

	short key_action_manager::register_action(key _Key, std::function<void()> _Action) noexcept {
		for (short i = 0; i < this->_IdMap.size(); i++)
			if (!this->_IdMap.test(i)) {
				this->_IdMap.set(i);
				this->_DirectKeyActions.push_back(std::make_pair(i, std::make_pair(_Key, _Action)));
				return i;
			}
		return -1;
	}

	void key_action_manager::remove(short _Id) {
		__stack_record();

		if (!this->_IdMap.test(_Id))
			throw API::argument_exception("Argument contains an invalid id.", "_Id");

		__stack_escape();

		this->_IdMap.reset(_Id);

		for (auto it = this->_KeyDownActions.begin(); it != this->_KeyDownActions.end(); ++it)
			if (it->first == _Id) {
				delete it->second;
				this->_KeyDownActions.erase(it);
				return;
			}
		for (auto it = this->_KeyUpActions.begin(); it != this->_KeyUpActions.end(); ++it)
			if (it->first == _Id) {
				delete it->second;
				this->_KeyUpActions.erase(it);
				return;
			}
		for (auto it = this->_KeyActions.begin(); it != this->_KeyActions.end(); ++it)
			if (it->first == _Id) {
				delete it->second;
				this->_KeyActions.erase(it);
				return;
			}
		for (auto it = this->_DirectKeyActions.begin(); it != this->_DirectKeyActions.end(); ++it)
			if (it->first == _Id) {
				this->_DirectKeyActions.erase(it);
				return;
			}

		assert(false && "If this statement is reached, the KeyAction bitset is edited in a way that is not intended.");
	}

#pragma region Function key_action_manager::invoke

	static void __invoke(_::__key_down_action* _Action) {
		__try {
			_Action->on_key_down();
		}
		__except (API::seh_filter(GetExceptionInformation()).continue_on(EXCEPTION_CXX)) {
			__stack_record();
			throw API::system_exception();
			__stack_escape();
		}
	}

	static void __invoke(_::__key_up_action* _Action) {
		__try {
			_Action->on_key_up();
		}
		__except (API::seh_filter(GetExceptionInformation()).continue_on(EXCEPTION_CXX)) {
			__stack_record();
			throw API::system_exception();
			__stack_escape();
		}
	}

	static void __invoke(const std::function<void()>& _Action) {
		__try {
			_Action();
		}
		__except (API::seh_filter(GetExceptionInformation()).continue_on(EXCEPTION_CXX)) {
			__stack_record();
			throw API::system_exception();
			__stack_escape();
		}
	}

	template<typename _Ty>
	static void __invoke(API::logger* _Log, _Ty _Arg) {
		std::stringstream ss;

		try {
			__invoke(_Arg);
		}
		catch (const API::system_exception& e) {
			ss << "Artemis system_exception thrown when invoking keybind handler: " << e.what() << std::endl << e.calls()->to_string();
			_Log->error(ss.str());
		}
		catch (const API::exception& e) {
			ss << "Artemis exception thrown when invoking keybind handler: " << e.what() << std::endl << e.calls()->to_string();
			_Log->error(ss.str());
		}
		catch (const std::exception& e) {
			ss << "C++ std::exception thrown when invoking keybind handler: " << e.what() << std::endl;
			_Log->error(ss.str());
		}
		catch (...) {
			ss << "Unknown C++ exception thrown when invoking keybind handler.";
			_Log->error(ss.str());
		}
	}

	void key_action_manager::invoke(UINT _WindowMessage, key _Key) const noexcept {
		switch (_WindowMessage) {
		case WM_KEYDOWN:
			for (const auto& o : this->_KeyDownActions)
				if (o.second->key() == _Key)
					__invoke(this->Log, o.second);
			for (const auto& o : this->_KeyActions)
				if (o.second->key() == _Key)
					__invoke<_::__key_down_action*>(this->Log, o.second);
			for (const auto& o : this->_DirectKeyActions)
				if (o.second.first == _Key)
					__invoke(this->Log, o.second.second);
			break;

		case WM_KEYUP:
			for (const auto& o : this->_KeyUpActions)
				if (o.second->key() == _Key)
					__invoke(this->Log, o.second);
			for (const auto& o : this->_KeyActions)
				if (o.second->key() == _Key)
					__invoke<_::__key_up_action*>(this->Log, o.second);
			break;
		}
	}

#pragma endregion

#pragma endregion
}