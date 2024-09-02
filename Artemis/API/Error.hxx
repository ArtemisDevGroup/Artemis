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
// Records the current function to the thread call stack.
#define __stack_record() Artemis::API::call_stack_manager::global()->record(__FUNCSIG__, __FILE__, __LINE__)
// Removes the last recorded function from the thread call stack.
#define __stack_escape() Artemis::API::call_stack_manager::global()->escape()
// Initial statement of rethrow try block.
#define __stack_rethrow_try() try {
// Catch statement of rethrow try block.
#define __stack_rethrow_catch() } catch (const Artemis::API::exception&) { __stack_escape(); throw; }
// Rethrows an Artemis exception thrown by a function.
#define __stack_rethrow(potentially_throwing_call) __stack_rethrow_try() potentially_throwing_call ; __stack_rethrow_catch()
#endif // ARTEMIS_DISABLE_CALL_STACK

namespace Artemis::API {
	/// <summary>
	/// Represents a single entry in a function call stack.
	/// </summary>
	struct call_stack_entry {
		/// <summary>
		/// The function signature.
		/// </summary>
		std::string_view _Function;

		/// <summary>
		/// The name of the file containing the definition of the function.
		/// </summary>
		std::string_view _File;

		/// <summary>
		/// The line in the file where the function is at.
		/// </summary>
		int _Line;
	};
	
	class call_stack_manager;

	/// <summary>
	/// Represents a function call stack.
	/// </summary>
	class call_stack {
		std::vector<call_stack_entry> _StackEntries;
		call_stack_manager* _Owner;
		DWORD _ThreadId;
		bool _IsSnapshot;

	public:
		/// <summary>
		/// Creates an instance of a call stack.
		/// </summary>
		/// <param name="_Owner">- A pointer to the call stack manager owning the call stack.</param>
		/// <param name="_ThreadId">- The ID of the thread the call stack belongs to.</param>
		ARTEMIS_API call_stack(call_stack_manager* _Owner, DWORD _ThreadId) noexcept;

		/// <summary>
		/// Pushes information about a function onto the back of the call stack.
		/// </summary>
		/// <param name="_Function">- The name of the function.</param>
		/// <param name="_File">- The name of the file containing the function.</param>
		/// <param name="_Line">- The line where the function is located.</param>
		ARTEMIS_API void push_back(std::string_view&& _Function, std::string_view&& _File, int _Line);

		/// <summary>
		/// Pops the last entry from the back of the call stack.
		/// </summary>
		ARTEMIS_API void pop_back() noexcept;

		/// <summary>
		/// Pops a number of entries from the back of the call stack.
		/// </summary>
		/// <param name="_Count">- The number of elements to pop.</param>
		ARTEMIS_API void pop_back(int _Count) noexcept;

		/// <summary>
		/// Returns a reference to a vector containing all function entries of the current call stack.
		/// </summary>
		/// <returns>A reference to a vector containing all function entries.</returns>
		ARTEMIS_API const std::vector<call_stack_entry>& entries() const noexcept;

		/// <summary>
		/// Returns the id of the thread associated with this call stack.
		/// </summary>
		/// <returns>The thread id.</returns>
		ARTEMIS_API DWORD thread_id() const noexcept;

		/// <summary>
		/// Identifies whether the call stack contains any function entries or not.
		/// </summary>
		/// <returns>true if the instance contains function entries, otherwise false.</returns>
		ARTEMIS_API bool is_empty() const noexcept;

		/// <summary>
		/// Identifies whether the call stack is a snapshot or a currently active call stack on a thread.
		/// </summary>
		/// <returns>true if the instance is a snapshot, otherwise false.</returns>
		ARTEMIS_API bool is_snapshot() const noexcept;

		/// <summary>
		/// Iterates through the vector of function entires and calls the function provided in callback for each element.
		/// </summary>
		/// <param name="_Callback">- A function to be executed for every element in the vector.</param>
		ARTEMIS_API void for_each(std::function<void(const call_stack_entry* const)> _Callback) const;

		/// <summary>
		/// Formats a string representation of the current instance.
		/// </summary>
		/// <returns>A string representation of the current instance.</returns>
		ARTEMIS_API std::string to_string() const;

		/// <summary>
		/// Creates a snapshot of the current call stack.
		/// </summary>
		/// <returns>The created snapshot.</returns>
		ARTEMIS_API call_stack snap() const noexcept;

		/// <summary>
		/// Deletes the call stack and removes it from the list.
		/// </summary>
		ARTEMIS_API void drop();
	};

	/// <summary>
	/// Manages all threads' function call stacks.
	/// </summary>
	class call_stack_manager {
		std::vector<call_stack> _CallStacks;

	public:
		ARTEMIS_API call_stack_manager() noexcept;
		call_stack_manager(const call_stack_manager&) = delete;
		call_stack_manager(call_stack_manager&&) = delete;

		/// <summary>
		/// Records the function onto the specified thread's call stack.
		/// </summary>
		/// <param name="_ThreadId">- The id of the thread whose call stack to record to.</param>
		/// <param name="_Function">- The name of the function.</param>
		/// <param name="_File">- The name of the file containing the function.</param>
		/// <param name="_Line">- The line where the function is located.</param>
		/// <returns>A pointer to the thread's call stack.</returns>
		ARTEMIS_API call_stack* record(DWORD _ThreadId, std::string_view&& _Function, std::string_view&& _File, int _Line);

		/// <summary>
		/// Records the function onto the current thread's call stack.
		/// </summary>
		/// <param name="_Function">- The name of the function.</param>
		/// <param name="_File">- The name of the file containing the function.</param>
		/// <param name="_Line">- The line where the function is located.</param>
		/// <returns>A pointer to the current thread's call stack.</returns>
		ARTEMIS_API call_stack* record(std::string_view&& _Function, std::string_view&& _File, int _Line);

		/// <summary>
		/// Pops the last function added to the thread's call stack.
		/// </summary>
		/// <param name="_ThreadId">- The id of the thread whose call stack to be popped from.</param>
		/// <returns>A pointer to the thread's call stack.</returns>
		ARTEMIS_API call_stack* escape(DWORD _ThreadId);

		/// <summary>
		/// Pops the last function added to the current thread's call stack.
		/// </summary>
		/// <returns>A pointer to the current thread's call stack.</returns>
		ARTEMIS_API call_stack* escape();

		/// <summary>
		/// Fetches the specified thread's call stack.
		/// </summary>
		/// <param name="_ThreadId">- The id of the thread whose call stack to fetch.</param>
		/// <returns>A pointer to the thread's call stack.</returns>
		ARTEMIS_API call_stack* fetch(DWORD _ThreadId) const;

		/// <summary>
		/// Fetches the current thread's call stack.
		/// </summary>
		/// <returns>A pointer to the current thread's call stack.</returns>
		ARTEMIS_API call_stack* fetch() const;

		/// <summary>
		/// Drops the specified thread's call stack.
		/// </summary>
		/// <param name="_ThreadId">- The id of the thread whose call stack to drop.</param>
		ARTEMIS_API void drop(DWORD _ThreadId);

		/// <summary>
		/// Drops the current thread's call stack.
		/// </summary>
		ARTEMIS_API void drop();

		/// <summary>
		/// Gets a pointer to the global call stack manager instance.
		/// </summary>
		/// <returns>A pointer to the global call stack manager.</returns>
		ARTEMIS_API static call_stack_manager* global();
	};

	/// <summary>
	/// A base exception type for all Artemis exceptions. Inherits std::exception for compatibility.
	/// </summary>
	class exception : public std::exception {
		call_stack _CallStackSnapshot;
		std::shared_ptr<exception> _InnerException;

		ARTEMIS_API void event_invoke();

	public:
		ARTEMIS_API exception() noexcept;

		/// <summary>
		/// Initializes an exception with a message.
		/// </summary>
		/// <param name="_Message">- The exception message.</param>
		ARTEMIS_API exception(std::string_view&& _Message) noexcept;

		/// <summary>
		/// Initializes an exception with a message and an inner exception.
		/// </summary>
		/// <typeparam name="T">The inner exception type.</typeparam>
		/// <param name="_Message">- The exception message.</param>
		/// <param name="_InnerException">- The inner exception type.</param>
		template<class T>
			requires(std::is_base_of_v<exception, T>)
		inline exception(std::string_view&& _Message, T&& _InnerException) noexcept : std::exception(_Message.data()), _CallStackSnapshot(nullptr, 0) {
			this->_CallStackSnapshot = _InnerException._CallStackSnapshot;
			this->_InnerException = std::make_shared<T>(std::forward<T>(_InnerException));

			event_invoke();
		}

		/// <summary>
		/// Gets a pointer to the inner exception type. Note that this is a pointer to the base exception type, when in reality the object pointed to can be any exception type.
		/// </summary>
		/// <returns>A shared pointer to the inner exception.</returns>
		ARTEMIS_API const std::shared_ptr<exception> inner() const noexcept;

		/// <summary>
		/// Gets a const pointer to the inner exception object.
		/// </summary>
		/// <typeparam name="T">The inner exception type.</typeparam>
		/// <returns>A shared pointer to the inner exception.</returns>
		template<class T>
			requires(std::is_base_of_v<exception, T>)
		inline const std::shared_ptr<T> inner() const noexcept { return (std::shared_ptr<T>)this->inner(); }

		/// <summary>
		/// Gets a pointer to the current exception's call stack snapshot.
		/// </summary>
		/// <returns>A pointer to a snapshot of the call stack at the time the exception was thrown.</returns>
		ARTEMIS_API const call_stack* calls() const noexcept;

		/// <summary>
		/// Gets a pointer to the event invoked when an Artemis exception is constructed.
		/// </summary>
		/// <returns>A pointer to the throw event.</returns>
		ARTEMIS_API static event<exception>* throw_event() noexcept;
	};

	/// <summary>
	/// A class type that implements the Artemis exception base class.
	/// </summary>
	template<typename T>
	concept derived_exception_type = std::is_base_of_v<exception, T>;

	/// <summary>
	/// An exception type thrown when a function in the Windows API fails.
	/// </summary>
	class win32_exception : public exception {
		std::string_view _Win32Function;
		DWORD _Win32ErrorCode;

		ARTEMIS_API static std::string win32_message(DWORD _Win32ErrorCode) noexcept;

	public:
		/// <summary>
		/// Initializes a windows exception using the last error code for the specified function.
		/// </summary>
		/// <param name="_FunctionName">- The name of the function that failed.</param>
		ARTEMIS_API win32_exception(std::string_view&& _FunctionName) noexcept;

		/// <summary>
		/// Initializes a windows exception using the specified error code for the specified function.
		/// </summary>
		/// <param name="_Win32ErrorCode">- The error code.</param>
		/// <param name="_FunctionName">- The name of the function that failed.</param>
		ARTEMIS_API win32_exception(DWORD _Win32ErrorCode, std::string_view&& _FunctionName) noexcept;

		/// <summary>
		/// Initializes a windows exception using the last error code for the specified function.
		/// </summary>
		/// <typeparam name="T">- The inner exception type.</typeparam>
		/// <param name="_FunctionName">- The name of the function that failed.</param>
		/// <param name="_InnerException">- The inner exception.</param>
		template<derived_exception_type T>
		inline win32_exception(std::string_view&& _FunctionName, T&& _InnerException) noexcept : exception(win32_message(GetLastError()).c_str(), std::forward<T>(_InnerException)), _Win32Function(std::forward<std::string_view>(_FunctionName)), _Win32ErrorCode(GetLastError()) {}

		/// <summary>
		/// Initializes a windows exception using the specified error code for the specified function.
		/// </summary>
		/// <typeparam name="T">- The inner exception type.</typeparam>
		/// <param name="_Win32ErrorCode">- The error code.</param>
		/// <param name="_FunctionName">- The name of the function that failed.</param>
		/// <param name="_InnerException">- The inner exception.</param>
		template<derived_exception_type T>
		inline win32_exception(DWORD _Win32ErrorCode, std::string_view&& _FunctionName, T&& _InnerException) noexcept : exception(win32_message(_Win32ErrorCode).c_str(), _InnerException), _Win32Function(std::forward<std::string_view>(_FunctionName)), _Win32ErrorCode(_Win32ErrorCode) {}

		/// <summary>
		/// Gets the name of the Windows API function that failed.
		/// </summary>
		/// <returns>The name of the function that failed.</returns>
		ARTEMIS_API const std::string_view& win32_function() const noexcept;

		/// <summary>
		/// Gets the error code of the current instance.
		/// </summary>
		/// <returns>The error code.</returns>
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

#pragma warning(push)
#pragma warning(disable:26495)	// Variable 'Artemis::API::seh_data::_Context' is uninitialized. Always initialize a member variable. (type.6)
								// Variable 'Artemis::API::seh_data::_Record' is uninitialized. Always initialize a member variable. (type.6)

	struct seh_data {
		EXCEPTION_RECORD _Record;
		CONTEXT _Context;
		std::vector<EXCEPTION_RECORD> _InnerRecords;
	};

#pragma warning(pop)

	ARTEMIS_API seh_data* get_thread_seh_data(DWORD _ThreadId) noexcept;
	ARTEMIS_API seh_data* get_thread_seh_data() noexcept;

	class seh_filter {
		seh_data* _Data;

	public:
		ARTEMIS_API seh_filter(LPEXCEPTION_POINTERS _ExceptionPointers);

		ARTEMIS_API int continue_on(DWORD _ExceptionCode) noexcept;
		ARTEMIS_API int handle_on(DWORD _ExceptionCode) noexcept;

		ARTEMIS_API int handle_always() noexcept;
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