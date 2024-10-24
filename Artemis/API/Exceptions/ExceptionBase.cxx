#include "pch.h"
#include "ExceptionBase.hxx"

namespace Artemis::API {
#pragma region Class call_stack

	call_stack::call_stack(call_stack_manager* _Owner, DWORD _ThreadId) noexcept : _Owner(_Owner), _ThreadId(_ThreadId), _StackEntries(), _IsSnapshot(false) {}

	void call_stack::push_back(std::string_view&& _Function, std::string_view&& _File, int _Line) {
		call_stack_entry cse;
		cse._Function = std::move(_Function);
		cse._File = std::move(_File);
		cse._Line = _Line;

		this->_StackEntries.push_back(std::move(cse));
	}

	void call_stack::pop_back() noexcept {
		this->_StackEntries.pop_back();
	}

	void call_stack::pop_back(int _Count) noexcept {
		for (int i = 0; i < _Count; i++)
			this->_StackEntries.pop_back();
	}

	const std::vector<call_stack_entry>& call_stack::entries() const noexcept { return this->_StackEntries; }

	DWORD call_stack::thread_id() const noexcept { return _ThreadId; }

	bool call_stack::is_empty() const noexcept { return !(this->_StackEntries.size() > 0); }

	bool call_stack::is_snapshot() const noexcept { return this->_IsSnapshot; }

	void call_stack::for_each(std::function<void(const call_stack_entry* const)> _Callback) const {
		for (const call_stack_entry& entry : this->_StackEntries)
			_Callback(&entry);
	}

	std::string call_stack::to_string() const {
		std::string ret = "";
		for (const call_stack_entry& entry : _StackEntries | std::views::reverse) {
			ret += "at ";
			ret += entry._Function;
			ret += '\n';
		}
		return ret;
	}

	call_stack call_stack::snap() const noexcept {
		call_stack ret = *this;
		ret._IsSnapshot = true;
		ret._Owner = nullptr;
		return ret;
	}

	void call_stack::drop() {
		if (this->_Owner)
			this->_Owner->drop(this->_ThreadId);
	}

#pragma endregion

#pragma region Class call_stack_manager

	call_stack_manager::call_stack_manager() noexcept : _CallStacks() {}

	call_stack* call_stack_manager::record(DWORD _ThreadId, std::string_view&& _Function, std::string_view&& _File, int _Line) {
		for (call_stack& stack : this->_CallStacks)
			if (stack.thread_id() == _ThreadId) {
				stack.push_back(std::move(_Function), std::move(_File), _Line);
				return &stack;
			}

		this->_CallStacks.push_back(call_stack(this, _ThreadId));
		call_stack* stack = std::prev(this->_CallStacks.end())._Ptr;
		stack->push_back(std::move(_Function), std::move(_File), _Line);
		return stack;
	}

	call_stack* call_stack_manager::record(std::string_view&& _Function, std::string_view&& _File, int _Line) {
		return this->record(GetCurrentThreadId(), std::move(_Function), std::move(_File), _Line);
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
		for (auto i = this->_CallStacks.begin(); i != this->_CallStacks.end(); i = std::next(i))
			if (i->thread_id() == _ThreadId)
				return i._Ptr;
		return nullptr;
	}

	call_stack* call_stack_manager::fetch() const { return this->fetch(GetCurrentThreadId()); }

	void call_stack_manager::drop(DWORD _ThreadId) {
		for (auto i = this->_CallStacks.begin(); i != this->_CallStacks.end(); i = std::next(i))
			if (i->thread_id() == _ThreadId) {
				this->_CallStacks.erase(i);
				break;
			}
	}

	void call_stack_manager::drop() {
		this->drop(GetCurrentThreadId());
	}

	static call_stack_manager g_CallStackManager;
	call_stack_manager* call_stack_manager::global() { return &g_CallStackManager; }

#pragma endregion

#pragma region Class exception

	exception::exception() noexcept : std::exception("An unknown Artemis exception has occured."), _CallStackSnapshot(nullptr, 0), _InnerException(nullptr) {
		call_stack* current = call_stack_manager::global()->fetch();
		this->_CallStackSnapshot = current->snap();
		current->pop_back();
	}

	exception::exception(std::string_view&& _Message) noexcept : std::exception(_Message.data()), _CallStackSnapshot(nullptr, 0), _InnerException(nullptr) {
		call_stack* current = call_stack_manager::global()->fetch();
		this->_CallStackSnapshot = current->snap();
		current->pop_back();
	}

	exception::exception(exception&& _Other) noexcept : _CallStackSnapshot(std::move(_Other._CallStackSnapshot)) {
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

	const call_stack* exception::calls() const noexcept { return &this->_CallStackSnapshot; }

	exception& exception::operator=(exception&& _Other) noexcept {
		this->_CallStackSnapshot = std::move(_Other._CallStackSnapshot);

		if (_Other._InnerException) {
			this->_InnerException = _Other._InnerException;
			_Other._InnerException = nullptr;
		}
		else this->_InnerException = nullptr;

		return *this;
	}

#pragma endregion
}