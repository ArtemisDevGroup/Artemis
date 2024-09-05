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

		ARTEMIS_API static std::string_view win32_message(DWORD _Win32ErrorCode) noexcept;

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
		inline win32_exception(std::string_view&& _FunctionName, T&& _InnerException) noexcept : exception(win32_message(GetLastError()), std::forward<T>(_InnerException)), _Win32Function(std::forward<std::string_view>(_FunctionName)), _Win32ErrorCode(GetLastError()) {}

		/// <summary>
		/// Initializes a windows exception using the specified error code for the specified function.
		/// </summary>
		/// <typeparam name="T">- The inner exception type.</typeparam>
		/// <param name="_Win32ErrorCode">- The error code.</param>
		/// <param name="_FunctionName">- The name of the function that failed.</param>
		/// <param name="_InnerException">- The inner exception.</param>
		template<derived_exception_type T>
		inline win32_exception(DWORD _Win32ErrorCode, std::string_view&& _FunctionName, T&& _InnerException) noexcept : exception(win32_message(_Win32ErrorCode), _InnerException), _Win32Function(std::forward<std::string_view>(_FunctionName)), _Win32ErrorCode(_Win32ErrorCode) {}

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

	/// <summary>
	/// An exception type thrown when a C standard function returns an error state in the form of an errno code.
	/// </summary>
	class errno_exception : public exception {
		std::string_view _CStdFunction;
		errno_t _ErrnoCode;

		ARTEMIS_API static std::string_view errno_message(errno_t _ErrnoCode);

	public:
		/// <summary>
		/// Initializes an errno exception using the current errno code and the specified function.
		/// </summary>
		/// <param name="_FunctionName">- The name of the errno function that failed.</param>
		ARTEMIS_API errno_exception(std::string_view&& _FunctionName) noexcept;

		/// <summary>
		/// Initializes an errno exception using the specified errno code and the specified function.
		/// </summary>
		/// <param name="_ErrnoCode">- The errno code returned by the failing function.</param>
		/// <param name="_FunctionName">- The name of the errno function that failed.</param>
		ARTEMIS_API errno_exception(errno_t _ErrnoCode, std::string_view&& _FunctionName) noexcept;

		/// <summary>
		/// Initializes an errno exception using the current errno code and the specified function.
		/// </summary>
		/// <typeparam name="T">- The inner exception type.</typeparam>
		/// <param name="_FunctionName">- The name of the errno function that failed.</param>
		/// <param name="_InnerException">- The inner exception.</param>
		template<derived_exception_type T>
		inline errno_exception(std::string_view&& _FunctionName, T&& _InnerException) : exception(errno_message(errno), std::forward<T>(_InnerException)), _ErrnoCode(errno), _CStdFunction(std::move(_FunctionName)) {}

		/// <summary>
		/// Initializes an errno exception using the current errno code and the specified function.
		/// </summary>
		/// <typeparam name="T">- The inner exception type.</typeparam>
		/// <param name="_ErrnoCode">- The errno code returned by the failing function.</param>
		/// <param name="_FunctionName">- The name of the errno function that failed.</param>
		/// <param name="_InnerException">- The inner exception.</param>
		template<derived_exception_type T>
		inline errno_exception(errno_t _ErrnoCode, std::string_view&& _FunctionName, T&& _InnerException) : exception(errno_message(_ErrnoCode), std::forward<T>(_InnerException)), _ErrnoCode(_ErrnoCode), _CStdFunction(_FunctionName) {}

		/// <summary>
		/// Gets the name of the errno function that failed.
		/// </summary>
		/// <returns>The function name.</returns>
		ARTEMIS_API const std::string_view& cstd_function() const noexcept;

		/// <summary>
		/// Gets the errno code reported by the function that failed.
		/// </summary>
		/// <returns>The errno code.</returns>
		ARTEMIS_API errno_t errno_code() const noexcept;
	};

#pragma warning(push)
#pragma warning(disable:26495)	// Variable 'Artemis::API::seh_data::_Context' is uninitialized. Always initialize a member variable. (type.6)
								// Variable 'Artemis::API::seh_data::_Record' is uninitialized. Always initialize a member variable. (type.6)

	/// <summary>
	/// Data from an SEH exception throw.
	/// </summary>
	struct seh_data {
		/// <summary>
		/// The exception record. This contains all information about the exception itself.
		/// </summary>
		EXCEPTION_RECORD _Record;

		/// <summary>
		/// The exception context. This contains the values of all registers at the time of the exception.
		/// </summary>
		CONTEXT _Context;

		/// <summary>
		/// A vector containing all inner exception records.
		/// </summary>
		std::vector<EXCEPTION_RECORD> _InnerRecords;
	};

#pragma warning(pop)

	/// <summary>
	/// Gets the specified thread's last reported SEH data instance.
	/// </summary>
	/// <param name="_ThreadId">- The ID of the thread to obtain data from.</param>
	/// <returns>A pointer to the last recorded SEH data instance. If none exists for the specified thread, nullptr is returned..</returns>
	ARTEMIS_API seh_data* get_thread_seh_data(DWORD _ThreadId) noexcept;

	/// <summary>
	/// Gets the current thread's last reported SEH data instance.
	/// </summary>
	/// <returns>A pointer to the last recorded SEH data instance. If none exists for the current thread, nullptr is returned.</returns>
	ARTEMIS_API seh_data* get_thread_seh_data() noexcept;

	/// <summary>
	/// A class to be used in the __except filter statement.
	/// </summary>
	class seh_filter {
		seh_data* _Data;

	public:
		/// <summary>
		/// Constructs an SEH filter.
		/// </summary>
		/// <param name="_ExceptionPointers">- The pointer obtained from a call to GetExceptionInformation().</param>
		ARTEMIS_API seh_filter(LPEXCEPTION_POINTERS _ExceptionPointers);

		/// <summary>
		/// <para>Executues the exception handler, unless the exception code matches the provided exception code.</para>
		/// <para>If the exception handler is not executed, the exception will be propagated.</para>
		/// </summary>
		/// <param name="_ExceptionCode">- The exception code to pass.</param>
		/// <returns>The appropriate exception filter value to achieve the desired result.</returns>
		ARTEMIS_API int continue_on(DWORD _ExceptionCode) noexcept;

		/// <summary>
		/// <para>Executes the exception handler, only if the exception code matches the provided exception code.</para>
		/// <para>If the exception handler is not executed, the exception will be propagated.</para>
		/// </summary>
		/// <param name="_ExceptionCode">- The exception code to handle.</param>
		/// <returns>The appropriate exception filter value to achieve the desired result.</returns>
		ARTEMIS_API int handle_on(DWORD _ExceptionCode) noexcept;

		/// <summary>
		/// Returns the EXCEPTION_EXECUTE_HANDLER filter value, meaning the exception handler will always be ran, regardless what the exception code may be.
		/// </summary>
		/// <returns>The appropriate exception filter value to achieve the desired result.</returns>
		ARTEMIS_API int handle_always() noexcept;
	};

	/// <summary>
	/// An exception type thrown when an SEH exception handler is executed.
	/// </summary>
	class system_exception : public exception {
		EXCEPTION_RECORD _Record;
		CONTEXT _Context;
		std::vector<EXCEPTION_RECORD> _InnerRecords;

	public:
		ARTEMIS_API system_exception() noexcept;

		/// <summary>
		/// Initializes a system exception with the specified message.
		/// </summary>
		/// <param name="_Message">- The exception message.</param>
		ARTEMIS_API system_exception(std::string_view&& _Message) noexcept;

		/// <summary>
		/// Initializes a system exception with the specified message.
		/// </summary>
		/// <typeparam name="T">- The inner exception type.</typeparam>
		/// <param name="_Message">- The exception message.</param>
		/// <param name="_InnerException">- The inner exception.</param>
		template<derived_exception_type T>
		inline system_exception(std::string_view&& _Message, T&& _InnerException) noexcept : exception(std::forward<std::string_view>(_Message), std::forward<T>(_InnerException)) {
			seh_data* data = get_thread_seh_data();
			this->_Record = data->_Record;
			this->_Context = data->_Context;
			this->_InnerRecords = data->_InnerRecords;
		}

		/// <summary>
		/// Initializes a system exception with the default message.
		/// </summary>
		/// <typeparam name="T">- The inner exception type.</typeparam>
		/// <param name="_InnerException">- The inner exception.</param>
		template<derived_exception_type T>
		inline system_exception(T&& _InnerException) noexcept : system_exception("A system exception has occured.", std::forward<T>(_InnerException)) {}

		/// <summary>
		/// Gets a reference to the exception record.
		/// </summary>
		/// <returns>The exception record.</returns>
		ARTEMIS_API const EXCEPTION_RECORD& record() const noexcept;

		/// <summary>
		/// Gets a reference to the exception context, which contains the value of every processor register at the time of the exception.
		/// </summary>
		/// <returns>The exception context.</returns>
		ARTEMIS_API const CONTEXT& context() const noexcept;

		/// <summary>
		/// Gets a reference to a vector containing the records of any additional inner exceptions.
		/// </summary>
		/// <returns>The additional exception records.</returns>
		ARTEMIS_API const std::vector<EXCEPTION_RECORD>& inner_records() const noexcept;
	};

	/// <summary>
	/// An exception type thrown when an argument is null or contains an invalid value.
	/// </summary>
	class argument_exception : public exception {
		std::string_view _ArgumentName;

	public:
		/// <summary>
		/// Initializes an argument exception with the provided message for the provided argument.
		/// </summary>
		/// <param name="_Message">- The exception message.</param>
		/// <param name="_ArgumentName">- The name of the argument containing an invalid value.</param>
		ARTEMIS_API argument_exception(std::string_view&& _Message, std::string_view&& _ArgumentName) noexcept;

		/// <summary>
		/// Initializes an argument exception with the provided message for the provided argument.
		/// </summary>
		/// <typeparam name="T">- The inner exception type.</typeparam>
		/// <param name="_Message">- The exception message.</param>
		/// <param name="_ArgumentName">- The name of the argument containing an invalid value.</param>
		/// <param name="_InnerException">- The inner exception.</param>
		template<derived_exception_type T>
		inline argument_exception(std::string_view&& _Message, std::string_view&& _ArgumentName, T&& _InnerException) noexcept : exception(std::forward<std::string_view>(_Message), std::forward<T>(_InnerException)), _ArgumentName(std::forward<std::string_view>(_ArgumentName)) {}

		/// <summary>
		/// Gets a reference to the name of the argument containing an invalid value.
		/// </summary>
		/// <returns>The name of the invalid argument.</returns>
		ARTEMIS_API const std::string_view& argument() const noexcept;
	};

	/// <summary>
	/// An exception type thrown when the state of an object is invalid in order to preform the specified operation.
	/// </summary>
	class invalid_state_exception : public exception {
	public:
		ARTEMIS_API invalid_state_exception() noexcept;

		/// <summary>
		/// Initializes an invalid state exception with the specified message.
		/// </summary>
		/// <param name="_Message">- The exception message.</param>
		ARTEMIS_API invalid_state_exception(std::string_view&& _Message) noexcept;
	};
}

#endif // !ARTEMIS_API_ERROR_HXX