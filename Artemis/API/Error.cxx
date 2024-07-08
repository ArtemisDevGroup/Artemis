#include "pch.h"
#include "Error.hxx"

namespace Artemis::API {
	call_stack::call_stack(call_stack_manager* _Owner, DWORD _ThreadId) : _Owner(_Owner), _ThreadId(_ThreadId), _StackEntries(), _IsSnapshot(false) {}

	void call_stack::push_back(std::string _Function, std::string _File, int _Line) {
		this->_StackEntries.push_back(call_stack_entry(_Function, _File, _Line));
	}

	void call_stack::pop_back() {
		this->_StackEntries.pop_back();
	}

	void call_stack::pop_back(int _Count) {
		for (int i = 0; i < _Count; i++)
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
		std::string ret = "";
		for (const call_stack_entry& entry : _StackEntries | std::views::reverse) {
			ret += "at ";
			ret += entry._Function;
			ret += '\n';
		}
		return ret;
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
		call_stack* stack = std::prev(this->_CallStacks.end())._Ptr;
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

	event<exception> g_ThrowEvent;

	void exception::event_invoke() { g_ThrowEvent.invoke(this, new event_args(), true); }

	exception::exception() : std::exception("An unknown Artemis exception has occured."), _CallStackSnapshot(nullptr, 0) {
		call_stack* current = call_stack_manager::global()->fetch();
		this->_CallStackSnapshot = current->snap();
		current->pop_back();

		event_invoke();
	}

	exception::exception(const char* const _Message) : std::exception(_Message), _CallStackSnapshot(nullptr, 0) {
		call_stack* current = call_stack_manager::global()->fetch();
		this->_CallStackSnapshot = current->snap();
		current->pop_back();
		
		event_invoke();
	}

	const std::shared_ptr<exception> exception::inner() const { return this->_InnerException; }

	const call_stack* exception::calls() const { return &this->_CallStackSnapshot; }

	event<exception>* exception::throw_event() { return &g_ThrowEvent; }

	std::string win32_exception::win32_message(DWORD _Win32ErrorCode) {
		CHAR szBuffer[256];

		DWORD dwReturn = FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr,
			_Win32ErrorCode,
			LANG_ENGLISH,
			szBuffer,
			sizeof(szBuffer),
			nullptr
		);

		if (!dwReturn)
			lstrcpyA(szBuffer, "Fetching error message failed.");

		return "Win32: " + std::string(szBuffer);
	}

	win32_exception::win32_exception(const char* const _FunctionName) : exception(win32_message(GetLastError()).c_str()), _Win32Function(_FunctionName), _Win32ErrorCode(GetLastError()) {}

	win32_exception::win32_exception(DWORD _Win32ErrorCode, const char* const _FunctionName) : exception(win32_message(_Win32ErrorCode).c_str()), _Win32Function(_FunctionName), _Win32ErrorCode(_Win32ErrorCode) {}

	const char* const win32_exception::win32_function() const { return this->_Win32Function.c_str(); }

	DWORD win32_exception::win32_error_code() const { return this->_Win32ErrorCode; }

	std::string errno_exception::errno_message(errno_t _ErrnoCode) {
		char buffer[256];
		strerror_s(buffer, _ErrnoCode);
#pragma warning(push)
#pragma warning(disable:6054)	// String 'buffer' might not be zero-terminated
								// Disabled due to strerror_s not being SAL-annoted as
								// guaranteeing null termination, but does.
		return std::string(buffer);
#pragma warning(pop)
	}

	errno_exception::errno_exception(const char* const _FunctionName) : exception(errno_message(errno).c_str()), _ErrnoCode(errno), _CStdFunction(_FunctionName) {}

	errno_exception::errno_exception(errno_t _ErrnoCode, const char* const _FunctionName) : exception(errno_message(_ErrnoCode).c_str()), _ErrnoCode(_ErrnoCode), _CStdFunction(_FunctionName) {}

	const char* const errno_exception::cstd_function() const { return this->_CStdFunction.c_str(); }

	errno_t errno_exception::errno_code() const { return this->_ErrnoCode; }
}