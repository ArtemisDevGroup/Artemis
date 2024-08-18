#include "pch.h"
#include "Error.hxx"

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
				stack.push_back(std::forward<std::string_view>(_Function), std::forward<std::string_view>(_File), _Line);
				return &stack;
			}

		this->_CallStacks.push_back(call_stack(this, _ThreadId));
		call_stack* stack = std::prev(this->_CallStacks.end())._Ptr;
		stack->push_back(std::forward<std::string_view>(_Function), std::forward<std::string_view>(_File), _Line);
		return stack;
	}

	call_stack* call_stack_manager::record(std::string_view&& _Function, std::string_view&& _File, int _Line) {
		return this->record(GetCurrentThreadId(), std::forward<std::string_view>(_Function), std::forward<std::string_view>(_File), _Line);
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

	event<exception> g_ThrowEvent;

	void exception::event_invoke() { g_ThrowEvent.invoke(this, new event_args(), true); }

	exception::exception() noexcept : std::exception("An unknown Artemis exception has occured."), _CallStackSnapshot(nullptr, 0) {
		call_stack* current = call_stack_manager::global()->fetch();
		this->_CallStackSnapshot = current->snap();
		current->pop_back();

		event_invoke();
	}

	exception::exception(std::string_view&& _Message) noexcept : std::exception(_Message.data()), _CallStackSnapshot(nullptr, 0) {
		call_stack* current = call_stack_manager::global()->fetch();
		this->_CallStackSnapshot = current->snap();
		current->pop_back();
		
		event_invoke();
	}

	const std::shared_ptr<exception> exception::inner() const noexcept { return this->_InnerException; }

	const call_stack* exception::calls() const noexcept { return &this->_CallStackSnapshot; }

	event<exception>* exception::throw_event() noexcept { return &g_ThrowEvent; }

#pragma endregion

#pragma region Class win32_exception

	std::string win32_exception::win32_message(DWORD _Win32ErrorCode) noexcept {
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

	win32_exception::win32_exception(std::string_view&& _FunctionName) noexcept : exception(win32_message(GetLastError())), _Win32Function(std::forward<std::string_view>(_FunctionName)), _Win32ErrorCode(GetLastError()) {}

	win32_exception::win32_exception(DWORD _Win32ErrorCode, std::string_view&& _FunctionName) noexcept : exception(win32_message(_Win32ErrorCode)), _Win32Function(std::forward<std::string_view>(_FunctionName)), _Win32ErrorCode(_Win32ErrorCode) {}

	const std::string_view& win32_exception::win32_function() const noexcept { return this->_Win32Function; }

	DWORD win32_exception::win32_error_code() const noexcept { return this->_Win32ErrorCode; }

#pragma endregion

#pragma region Class errno_exception

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

	errno_exception::errno_exception(std::string_view&& _FunctionName) noexcept : exception(errno_message(errno).c_str()), _ErrnoCode(errno), _CStdFunction(std::forward<std::string_view>(_FunctionName)) {}

	errno_exception::errno_exception(errno_t _ErrnoCode, std::string_view&& _FunctionName) noexcept : exception(errno_message(_ErrnoCode)), _ErrnoCode(_ErrnoCode), _CStdFunction(std::forward<std::string_view>(_FunctionName)) {}

	const std::string_view& errno_exception::cstd_function() const noexcept { return this->_CStdFunction; }

	errno_t errno_exception::errno_code() const noexcept { return this->_ErrnoCode; }

#pragma endregion

#pragma region Class seh_filter

	std::unordered_map<DWORD, seh_data> g_SystemExceptionData;

	seh_data* get_thread_seh_data(DWORD _ThreadId) noexcept {
		if (g_SystemExceptionData.contains(_ThreadId))
			return &g_SystemExceptionData[_ThreadId];
		return nullptr;
	}

	seh_data* get_thread_seh_data() noexcept { return get_thread_seh_data(GetCurrentThreadId()); }

	seh_filter::seh_filter(LPEXCEPTION_POINTERS _ExceptionPointers) {
		seh_data data;
		memcpy(&data._Record, _ExceptionPointers->ExceptionRecord, sizeof(EXCEPTION_RECORD));

		while (LPEXCEPTION_RECORD record = _ExceptionPointers->ExceptionRecord->ExceptionRecord) {
			EXCEPTION_RECORD obj;
			memcpy(&obj, record, sizeof(EXCEPTION_RECORD));
			record = obj.ExceptionRecord;
			data._InnerRecords.push_back(std::move(obj));
		}

		memcpy(&data._Context, _ExceptionPointers->ContextRecord, sizeof(CONTEXT));

		DWORD dwThreadId = GetCurrentThreadId();

		g_SystemExceptionData[dwThreadId] = std::move(data);
		this->_Data = &g_SystemExceptionData[dwThreadId];
	}

	int seh_filter::continue_on(DWORD _ExceptionCode) noexcept {
		return this->_Data->_Record.ExceptionCode == _ExceptionCode ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
	}

	int seh_filter::handle_on(DWORD _ExceptionCode) noexcept {
		return this->_Data->_Record.ExceptionCode == _ExceptionCode ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
	}

#pragma endregion

#pragma region Class system_exception

	system_exception::system_exception() noexcept : exception("A system exception has occured.") {
		seh_data* data = get_thread_seh_data();
		this->_Record = data->_Record;
		this->_Context = data->_Context;
		this->_InnerRecords = data->_InnerRecords;
	}

	system_exception::system_exception(std::string_view&& _Message) noexcept : exception(std::forward<std::string_view>(_Message)) {
		seh_data* data = get_thread_seh_data();
		this->_Record = data->_Record;
		this->_Context = data->_Context;
		this->_InnerRecords = data->_InnerRecords;
	}

	const EXCEPTION_RECORD& system_exception::record() const noexcept { return this->_Record; }
	const CONTEXT& system_exception::context() const noexcept { return this->_Context; }
	const std::vector<EXCEPTION_RECORD>& system_exception::inner_records() const noexcept { return this->_InnerRecords; }

#pragma endregion

#pragma region Class argument_exception

	argument_exception::argument_exception(std::string_view&& _Message, std::string_view&& _ArgumentName) noexcept : exception(std::forward<std::string_view>(_Message)), _ArgumentName(std::forward<std::string_view>(_ArgumentName)) {}

	const std::string_view& argument_exception::argument() const noexcept { return this->_ArgumentName; }

#pragma endregion

#pragma region Class invalid_state_exception

	invalid_state_exception::invalid_state_exception() noexcept : exception("Object has an invalid state to preform this operation.") {}

	invalid_state_exception::invalid_state_exception(std::string_view&& _Message) noexcept : exception(std::forward<std::string_view>(_Message)) {}

#pragma endregion
}