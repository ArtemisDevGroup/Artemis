#ifndef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__
#define __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

#include "..\Definitions.hxx"

#include <exception>	// std::exception
#include <string>		// std::string
#include <string_view>	// std::string_view
#include <type_traits>	// std::remove_reference_t
#include <concepts>		// std::derived_from
#include <stacktrace>	// std::stacktrace

#include <Windows.h>	// DWORD, GetLastError()

// The underlying exception code raised by Windows for C++ 'throw' statements.
#define EXCEPTION_CXX 0xE06D7363

#pragma warning(disable:5272)

namespace Artemis::API {
	/**
	 * @brief The base exception type for all Artemis exceptions. Derived from `std::exception`.
	 */
	class exception : public std::exception {
		std::stacktrace _Trace;
		exception* _InnerException;

	public:
		/**
		 * @brief Disables collecting the stacktrace from `std::stacktrace` for the next thrown exception on the current thread.
		 */
		ARTEMIS_API static void disable_next_throw_trace() noexcept;

		/**
		 * @brief Constructs an empty exception.
		 */
		ARTEMIS_API exception() noexcept;

		/**
		 * @brief Constructs an exception with a message.
		 * @param[in] _Message The exception message.
		 */
		ARTEMIS_API exception(std::string_view _Message) noexcept;

		/**
		 * @brief Constructs an exception with a message and an underlying exception.
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @param[in] _Message The exception message.
		 * @param[in] _InnerException A reference to the underlying exception.
		 */
		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline exception(std::string_view _Message, _Ty&& _InnerException) noexcept : std::exception(_Message.data()), _Trace() {
			this->_Trace = _InnerException._Trace;
			this->_InnerException = new std::remove_reference_t<_Ty>(std::forward<_Ty>(_InnerException));
		}

		/**
		 * @brief Constructs an exception with an underlying exception.
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @param[in] _InnerException A reference to the underlying exception.
		 */
		template<class _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline exception(_Ty&& _InnerException) noexcept : exception("An unknown Artemis exception has occured.", std::forward<_Ty>(_InnerException)) {}

		exception(const exception&) = delete;

		ARTEMIS_API exception(exception&&) noexcept;

		ARTEMIS_API virtual ~exception() noexcept;

		/**
		 * @brief Gets a generic pointer to the underlying exception of this instance.
		 * @return A pointer to the inner exception.
		 */
		ARTEMIS_API const exception* inner() const noexcept;

		/**
		 * @brief Gets a specific pointer to the underlying exception of this instance.
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @return A pointer to the inner exception.
		 */
		template<std::derived_from<exception> _Ty>
		inline const _Ty* inner() const noexcept { return static_cast<const _Ty*>(this->inner()); }

		/**
		 * @brief Gets a reference to the stacktrace of this instance.
		 * @return The instance stacktrace.
		 */
		ARTEMIS_API const std::stacktrace& trace() const noexcept;

		exception& operator=(const exception&) = delete;
		ARTEMIS_API exception& operator=(exception&&) noexcept;
	};
}

#endif // !__ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__