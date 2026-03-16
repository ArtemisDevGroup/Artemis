#ifndef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__
#define __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

#include "..\Definitions.hxx"

#include <exception>	// std::exception
#include <string_view>	// std::string_view
#include <type_traits>	// std::is_base_of_v
#include <stacktrace>	// std::stacktrace

#include <Windows.h>	// DWORD, GetLastError()

#define EXCEPTION_CXX 0xE06D7363	// The underlying exception code raised by Windows for C++ 'throw' statements.

#pragma warning(disable:5272)

namespace Artemis::API {
	class exception : public std::exception {
		std::stacktrace _Trace;
		exception* _InnerException;

	public:
		ARTEMIS_API static void disable_next_throw_trace() noexcept;

		ARTEMIS_API exception() noexcept;

		ARTEMIS_API exception(std::string_view _Message) noexcept;

		template<class _Ty>
			requires(std::is_base_of_v<exception, std::remove_reference_t<_Ty>>)
		inline exception(std::string_view _Message, _Ty&& _InnerException) noexcept : std::exception(_Message.data()), _Trace() {
			this->_Trace = _InnerException._Trace;
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

		ARTEMIS_API const std::stacktrace& trace() const noexcept;

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