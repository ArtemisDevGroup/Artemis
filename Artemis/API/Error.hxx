#ifndef ARTEMIS_API_ERROR_HXX
#define ARTEMIS_API_ERROR_HXX

#include "Definitions.hxx"
#include "Event.hxx"

#include <exception>	// std::exception
#include <string_view>	// std::string_view
#include <vector>		// std::vector
#include <type_traits>	// std::is_base_of_v
#include <memory>		// std::shared_ptr, std::make_shared
#include <functional>	// std::function

#include <Windows.h>	// DWORD, GetLastError()

#define EXCEPTION_CXX 0xE06D7363	// The underlying exception code raised by Windows for C++ 'throw' statements.

#ifdef ARTEMIS_DISABLE_CALL_STACK
#define __stack_record()
#define __stack_escape()
#define __stack_rethrow_try()
#define __stack_rethrow_catch()
#define __stack_rethrow(potentially_throwing_call) potentially_throwing_call
#else
#define __stack_record() Artemis::API::call_stack_manager::global()->record(__FUNCSIG__, __FILE__, __LINE__)
#define __stack_escape() Artemis::API::call_stack_manager::global()->escape()
#define __stack_rethrow_try() try {
#define __stack_rethrow_catch() } catch (const Artemis::API::exception&) { __stack_escape(); throw; }
#define __stack_rethrow(potentially_throwing_call) __stack_rethrow_try() potentially_throwing_call ; __stack_rethrow_catch()
#endif // ARTEMIS_DISABLE_CALL_STACK


namespace Artemis::API {
	struct call_stack_entry {
		std::string_view _Function;
		std::string_view _File;
		int _Line;
	};
	
	class call_stack_manager;

	class call_stack {
		std::vector<call_stack_entry> _StackEntries;
		call_stack_manager* _Owner;
		DWORD _ThreadId;
		bool _IsSnapshot;

	public:
		ARTEMIS_API call_stack(call_stack_manager* _Owner, DWORD _ThreadId) noexcept;

		ARTEMIS_API void push_back(std::string_view&& _Function, std::string_view&& _File, int _Line); 
		ARTEMIS_API void pop_back() noexcept;
		ARTEMIS_API void pop_back(int _Count) noexcept;

		ARTEMIS_API const std::vector<call_stack_entry>& entries() const noexcept;
		ARTEMIS_API DWORD thread_id() const noexcept;

		ARTEMIS_API bool is_empty() const noexcept;
		ARTEMIS_API bool is_snapshot() const noexcept;

		ARTEMIS_API void for_each(std::function<void(const call_stack_entry* const)> _Callback) const;

		ARTEMIS_API std::string to_string() const;

		ARTEMIS_API call_stack snap() const noexcept;
		ARTEMIS_API void drop();
	};

	class call_stack_manager {
		std::vector<call_stack> _CallStacks;

	public:
		ARTEMIS_API call_stack_manager() noexcept;
		call_stack_manager(const call_stack_manager&) = delete;
		call_stack_manager(call_stack_manager&&) = delete;

		ARTEMIS_API call_stack* record(DWORD _ThreadId, std::string_view&& _Function, std::string_view&& _File, int _Line);
		ARTEMIS_API call_stack* record(std::string_view&& _Function, std::string_view&& _File, int _Line);

		ARTEMIS_API call_stack* escape(DWORD _ThreadId);
		ARTEMIS_API call_stack* escape();

		ARTEMIS_API call_stack* fetch(DWORD _ThreadId) const;
		ARTEMIS_API call_stack* fetch() const;

		ARTEMIS_API void drop(DWORD _ThreadId);
		ARTEMIS_API void drop();

		ARTEMIS_API static call_stack_manager* global();
	};

	class exception : public std::exception {
		call_stack _CallStackSnapshot;
		std::shared_ptr<exception> _InnerException;

		ARTEMIS_API void event_invoke();

	public:
		ARTEMIS_API exception() noexcept;
		ARTEMIS_API exception(std::string_view&& _Message) noexcept;

		template<class T>
			requires(std::is_base_of_v<exception, T>)
		inline exception(std::string_view&& _Message, T&& _InnerException) noexcept : std::exception(_Message.data()), _CallStackSnapshot(nullptr, 0) {
			this->_CallStackSnapshot = _InnerException._CallStackSnapshot;
			this->_InnerException = std::make_shared<T>(std::forward<T>(_InnerException));

			event_invoke();
		}

		ARTEMIS_API const std::shared_ptr<exception> inner() const noexcept;

		template<class T>
			requires(std::is_base_of_v<exception, T>)
		inline const std::shared_ptr<T> inner() const noexcept { return (std::shared_ptr<T>)this->inner(); }

		ARTEMIS_API const call_stack* calls() const noexcept;

		ARTEMIS_API static event<exception>* throw_event() noexcept;
	};

	template<typename T>
	concept derived_exception_type = std::is_base_of_v<exception, T>;

	class win32_exception : public exception {
		std::string_view _Win32Function;
		DWORD _Win32ErrorCode;

		ARTEMIS_API static std::string win32_message(DWORD _Win32ErrorCode) noexcept;

	public:
		ARTEMIS_API win32_exception(std::string_view&& _FunctionName) noexcept;
		ARTEMIS_API win32_exception(DWORD _Win32ErrorCode, std::string_view&& _FunctionName) noexcept;

		template<derived_exception_type T>
		inline win32_exception(const char* const _FunctionName, T&& _InnerException) noexcept : exception(win32_message(GetLastError()).c_str(), std::forward<T>(_InnerException)), _Win32Function(_FunctionName), _Win32ErrorCode(GetLastError()) {}

		template<derived_exception_type T>
		inline win32_exception(DWORD _Win32ErrorCode, const char* const _FunctionName, T&& _InnerException) noexcept : exception(win32_message(_Win32ErrorCode).c_str(), _InnerException), _Win32Function(_FunctionName), _Win32ErrorCode(_Win32ErrorCode) {}

		ARTEMIS_API const std::string_view& win32_function() const noexcept;
		ARTEMIS_API DWORD win32_error_code() const noexcept;
	};

	class errno_exception : public exception {
		std::string_view _CStdFunction;
		errno_t _ErrnoCode;

		ARTEMIS_API static std::string errno_message(errno_t _ErrnoCode);

	public:
		ARTEMIS_API errno_exception(std::string_view&& _FunctionName) noexcept;
		ARTEMIS_API errno_exception(errno_t _ErrnoCode, std::string_view&& _FunctionName) noexcept;

		template<derived_exception_type T>
		inline errno_exception(const char* const _FunctionName, T&& _InnerException) : exception(errno_message(errno).c_str(), std::forward<T>(_InnerException)), _ErrnoCode(errno), _CStdFunction(_FunctionName) {}

		template<derived_exception_type T>
		inline errno_exception(errno_t _ErrnoCode, const char* const _FunctionName, T&& _InnerException) : exception(errno_message(_ErrnoCode).c_str(), std::forward<T>(_InnerException)), _ErrnoCode(_ErrnoCode), _CStdFunction(_FunctionName) {}

		ARTEMIS_API const std::string_view& cstd_function() const noexcept;
		ARTEMIS_API errno_t errno_code() const noexcept;
	};

	struct seh_data {
		EXCEPTION_RECORD _Record;
		CONTEXT _Context;
		std::vector<EXCEPTION_RECORD> _InnerRecords;
	};

	ARTEMIS_API seh_data* get_thread_seh_data(DWORD _ThreadId) noexcept;
	ARTEMIS_API seh_data* get_thread_seh_data() noexcept;

	class seh_filter {
		seh_data* _Data;

	public:
		ARTEMIS_API seh_filter(LPEXCEPTION_POINTERS _ExceptionPointers);

		ARTEMIS_API int continue_on(DWORD _ExceptionCode) noexcept;
		ARTEMIS_API int handle_on(DWORD _ExceptionCode) noexcept;
	};

	class system_exception : public exception {
		EXCEPTION_RECORD _Record;
		CONTEXT _Context;
		std::vector<EXCEPTION_RECORD> _InnerRecords;

	public:
		ARTEMIS_API system_exception() noexcept;
		ARTEMIS_API system_exception(std::string_view&& _Message) noexcept;

		template<derived_exception_type T>
		inline system_exception(std::string_view&& _Message, T&& _InnerException) noexcept : exception(std::forward<std::string_view>(_Message), std::forward<T>(_InnerException)) {
			seh_data* data = get_thread_seh_data();
			this->_Record = data->_Record;
			this->_Context = data->_Context;
			this->_InnerRecords = data->_InnerRecords;
		}

		template<derived_exception_type T>
		inline system_exception(T&& _InnerException) noexcept : system_exception("A system exception has occured.", std::forward<T>(_InnerException)) {}

		ARTEMIS_API const EXCEPTION_RECORD& record() const noexcept;
		ARTEMIS_API const CONTEXT& context() const noexcept;
		ARTEMIS_API const std::vector<EXCEPTION_RECORD>& inner_records() const noexcept;
	};

	class argument_exception : public exception {
		std::string_view _ArgumentName;

	public:
		ARTEMIS_API argument_exception(std::string_view&& _Message, std::string_view&& _ArgumentName) noexcept;

		template<derived_exception_type T>
		inline argument_exception(std::string_view&& _Message, std::string_view&& _ArgumentName, T&& _InnerException) noexcept : exception(std::forward<std::string_view>(_Message), std::forward<T>(_InnerException)), _ArgumentName(std::forward<std::string_view>(_ArgumentName)) {}

		ARTEMIS_API const std::string_view& argument() const noexcept;
	};

	class invalid_state_exception : public exception {
	public:
		ARTEMIS_API invalid_state_exception() noexcept;
		ARTEMIS_API invalid_state_exception(std::string_view&& _Message) noexcept;
	};
}

#endif // !ARTEMIS_API_ERROR_HXX