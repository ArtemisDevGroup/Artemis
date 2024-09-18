#include "pch.h"
#include "KeyAction.hxx"

#include "ExceptionFrames.hxx"

namespace Artemis {
#pragma region Class key_action_manager

	short key_action_manager::register_action(key _Key, std::function<void()>&& _Action) noexcept {
		for (short i = 0; i < this->_IdMap.size(); i++)
			if (!this->_IdMap.test(i)) {
				this->_IdMap.set(i);
				this->_DirectKeyActions.push_back({ i, { _::__execution_context::get(), std::make_pair(_Key, std::move(_Action)) } });
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
			if (it->_Id == _Id) {
				delete it->_Object._Object;
				this->_KeyDownActions.erase(it);
				return;
			}
		for (auto it = this->_KeyUpActions.begin(); it != this->_KeyUpActions.end(); ++it)
			if (it->_Id == _Id) {
				delete it->_Object._Object;
				this->_KeyUpActions.erase(it);
				return;
			}
		for (auto it = this->_KeyActions.begin(); it != this->_KeyActions.end(); ++it)
			if (it->_Id == _Id) {
				delete it->_Object._Object;
				this->_KeyActions.erase(it);
				return;
			}
		for (auto it = this->_DirectKeyActions.begin(); it != this->_DirectKeyActions.end(); ++it)
			if (it->_Id == _Id) {
				this->_DirectKeyActions.erase(it);
				return;
			}

		assert(false && "If this statement is reached, the KeyAction bitset is edited in a way that is not intended.");
	}

	void key_action_manager::invoke(UINT _WindowMessage, key _Key) const noexcept {
		_::__safe_exception_net net{};
		net.set_instance_logger(this->Log);

		extension* context = _::__execution_context::get();

		switch (_WindowMessage) {
		case WM_KEYDOWN:
			for (const auto& o : this->_KeyDownActions)
				if (o._Object._Object->key() == _Key) {
					_::__execution_context::set(o._Object._LoadedObjectOwner);
					net.exec_l2([&o]() { o._Object._Object->on_key_down(); });
				}
			for (const auto& o : this->_KeyActions)
				if (o._Object._Object->key() == _Key) {
					_::__execution_context::set(o._Object._LoadedObjectOwner);
					net.exec_l2([&o]() { o._Object._Object->on_key_down(); });
				}
			for (const auto& o : this->_DirectKeyActions)
				if (o._Object._Object.first == _Key) {
					_::__execution_context::set(o._Object._LoadedObjectOwner);
					net.exec_l2(o._Object._Object.second);
				}
			break;

		case WM_KEYUP:
			for (const auto& o : this->_KeyUpActions)
				if (o._Object._Object->key() == _Key) {
					_::__execution_context::set(o._Object._LoadedObjectOwner);
					net.exec_l2([&o]() { o._Object._Object->on_key_up(); });
				}
			for (const auto& o : this->_KeyActions)
				if (o._Object._Object->key() == _Key) {
					_::__execution_context::set(o._Object._LoadedObjectOwner);
					net.exec_l2([&o]() { o._Object._Object->on_key_up(); });
				}
			break;
		}

		_::__execution_context::set(context);
	}

#pragma endregion
}