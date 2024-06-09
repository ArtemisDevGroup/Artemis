#include "pch.h"
#include "Error.hxx"

namespace Artemis::API {
	call_stack::call_stack(call_stack_manager* _Owner, DWORD _ThreadId) : _Owner(_Owner), _ThreadId(_ThreadId), _StackEntries(), _IsSnapshot(false) {}

	void call_stack::push_back(std::string _Function, std::string _File, int _Line) {
		call_stack_entry entry;
		entry._Function = _Function;
		entry._File = _File;
		entry._Line = _Line;

		this->_StackEntries.push_back(entry);
	}

	void call_stack::pop_back() {
		this->_StackEntries.pop_back();
	}

	const std::vector<call_stack_entry>& call_stack::entries() const { return this->_StackEntries; }

	DWORD call_stack::thread_id() const { return _ThreadId; }

	bool call_stack::is_empty() const { return !(this->_StackEntries.size() > 0); }

	bool call_stack::is_snapshot() const { return this->_IsSnapshot; }

	void call_stack::for_each(std::function<void(const call_stack_entry* const)> _Callback) const {
		for (const call_stack_entry& entry : this->_StackEntries)
			_Callback(&entry);
	}

	std::string call_stack::to_string() const {
#error Implement call_stack::to_string
	}

	void call_stack::drop() {
		this->_Owner->drop(this->_ThreadId);
	}

	call_stack_manager::call_stack_manager() : _CallStacks() {}

	call_stack* call_stack_manager::record(DWORD _ThreadId, std::string _Function, std::string _File, int _Line) {
		for (call_stack& stack : this->_CallStacks)
			if (stack.thread_id() == _ThreadId) {
				stack.push_back(_Function, _File, _Line);
				return &stack;
			}

		this->_CallStacks.push_back(call_stack(this, _ThreadId));
		call_stack* stack = &this->_CallStacks[this->_CallStacks.size() - 1];
		stack->push_back(_Function, _File, _Line);
		return stack;
	}

	call_stack* call_stack_manager::record(std::string _Function, std::string _File, int _Line) {
		return this->record(GetCurrentThreadId(), _Function, _File, _Line);
	}

	call_stack* call_stack_manager::escape(DWORD _ThreadId) {
		for (call_stack& stack : this->_CallStacks)
			if (stack.thread_id() == _ThreadId) {
				stack.pop_back();
				
				if (stack.is_empty()) {
					stack.drop();
					return nullptr;
				}

				return &stack;
			}
		return nullptr;
	}

	call_stack* call_stack_manager::escape() {
		return this->escape(GetCurrentThreadId());
	}

	call_stack* call_stack_manager::fetch(DWORD _ThreadId) const {
		for (std::vector<call_stack>::const_iterator i = this->_CallStacks.begin(); i != this->_CallStacks.end(); ++i)
			if (i->thread_id() == _ThreadId)
				return i._Ptr;
		return nullptr;
	}

	call_stack* call_stack_manager::fetch() const { return this->fetch(GetCurrentThreadId()); }

	void call_stack_manager::drop(DWORD _ThreadId) {
		for (std::vector<call_stack>::iterator i = this->_CallStacks.begin(); i != this->_CallStacks.end(); ++i)
			if (i->thread_id() == _ThreadId)
				this->_CallStacks.erase(i);
	}

	void call_stack_manager::drop() {
		this->drop(GetCurrentThreadId());
	}

	static call_stack_manager g_CallStackManager;
	call_stack_manager* call_stack_manager::global() { return &g_CallStackManager; }

	exception::exception() : std::exception("An unknown Artemis exception has occured.") {

	}

	exception::exception(const char* const _Message) : std::exception(_Message) {

	}

	exception::exception(const char* const _Message, const exception& _InnerException) {

	}

	exception::exception(const char* const _Message, exception&& _InnerException) {

	}
}