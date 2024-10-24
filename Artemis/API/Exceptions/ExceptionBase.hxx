#ifndef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__
#define __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

#include "..\Definitions.hxx"

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

		call_stack_manager& operator=(const call_stack_manager&) = delete;
		call_stack_manager& operator=(call_stack_manager&&) = delete;
	};

	class exception : public std::exception {
		call_stack _CallStackSnapshot;
		exception* _InnerException;

	public:
		ARTEMIS_API exception() noexcept;

		ARTEMIS_API exception(std::string_view&& _Message) noexcept;

		template<class _Ty>
			requires(std::is_base_of_v<exception, std::remove_reference_t<_Ty>>)
		inline exception(std::string_view&& _Message, _Ty&& _InnerException) noexcept : std::exception(_Message.data()), _CallStackSnapshot(nullptr, 0) {
			this->_CallStackSnapshot = _InnerException._CallStackSnapshot;
			this->_InnerException = new std::remove_reference_t<_Ty>(std::forward<_Ty>(_InnerException));
		}

		template<class _Ty>
			requires(std::is_base_of_v<exception, std::remove_reference_t<_Ty>>)
		inline exception(_Ty&& _InnerException) noexcept : exception("An unknown Artemis exception has occured.", std::forward<_Ty>(_InnerException)) {}

		exception(const exception&) = delete;

		ARTEMIS_API exception(exception&&) noexcept;

		ARTEMIS_API ~exception() noexcept;

		ARTEMIS_API const exception* inner() const noexcept;

		template<class _Ty>
			requires(std::is_base_of_v<exception, _Ty>)
		inline const _Ty* inner() const noexcept { return (_Ty*)this->inner(); }

		ARTEMIS_API const call_stack* calls() const noexcept;

		exception& operator=(const exception&) = delete;
		ARTEMIS_API exception& operator=(exception&&) noexcept;
	};

	/// <summary>
	/// A class type that implements the Artemis exception base class.
	/// </summary>
	template<typename T>
	concept derived_exception_type = std::is_base_of_v<exception, std::remove_reference_t<T>>;
}

#endif // !__ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__