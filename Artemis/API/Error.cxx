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

	void call_stack::pop_back(int _Count) {
		for (int i = 0; i < _Count; i++)
			this->_StackEntries.pop_back();
	}

	void call_stack::pop_until(call_stack_entry _Entry) {
		while (![_Entry](std::vector<call_stack_entry>& _Entries) -> bool {
			auto end = _Entries.end();
			return end->_Function == _Entry._Function && end->_File == _Entry._File && end->_Line == _Entry._Line;
			}(this->_StackEntries)) this->_StackEntries.pop_back();
	}

	void call_stack::pop_until(std::string _Function, std::string _File, int _Line) {
		call_stack_entry entry;
		entry._Function = _Function;
		entry._File = _File;
		entry._Line = _Line;
		this->pop_until(entry);
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

	call_stack call_stack::snap() const {
		call_stack ret = *this;
		ret._IsSnapshot = true;
		ret._Owner = nullptr;
		return ret;
	}

	void call_stack::drop() {
		if (this->_Owner)
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
		call_stack* stack = this->_CallStacks.end()._Ptr;
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

	exception::exception(call_stack_entry* _PopUntil) : std::exception("An unknown Artemis exception has occured."), _CallStackSnapshot(nullptr, 0) {
		call_stack* current = call_stack_manager::global()->fetch();
		this->_CallStackSnapshot = current->snap();

		if (_PopUntil)
			current->pop_until(*_PopUntil);
		else
			current->drop();
	}

	exception::exception(const char* const _Message, call_stack_entry* _PopUntil) : std::exception(_Message), _CallStackSnapshot(nullptr, 0) {
		call_stack* current = call_stack_manager::global()->fetch();
		this->_CallStackSnapshot = current->snap();

		if (_PopUntil)
			current->pop_until(*_PopUntil);
		else
			current->drop();
			
	}

	exception::exception(const char* const _Message, const exception& _InnerException, call_stack_entry* _PopUntil) : std::exception(_Message), _CallStackSnapshot(nullptr, 0) {
		call_stack* current = call_stack_manager::global()->fetch();
		this->_CallStackSnapshot = current->snap();

		if (_PopUntil)
			current->pop_until(*_PopUntil);
		else
			current->drop();
	}

	exception::exception(const char* const _Message, exception&& _InnerException, call_stack_entry* _PopUntil) : std::exception(_Message), _CallStackSnapshot(nullptr, 0) {
		call_stack* current = call_stack_manager::global()->fetch();
		this->_CallStackSnapshot = current->snap();

		if (_PopUntil)
			current->pop_until(*_PopUntil);
		else
			current->drop();
	}
}