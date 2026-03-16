#ifndef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__
#define __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

#include "..\Definitions.hxx"

#include <exception>	// std::exception
#include <string_view>	// std::string_view
#include <type_traits>	// std::remove_reference_t
#include <concepts>		// std::derived_from
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

		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline exception(std::string_view _Message, _Ty&& _InnerException) noexcept : std::exception(_Message.data()), _Trace() {
			this->_Trace = _InnerException._Trace;
			this->_InnerException = new std::remove_reference_t<_Ty>(std::forward<_Ty>(_InnerException));
		}

		template<class _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline exception(_Ty&& _InnerException) noexcept : exception("An unknown Artemis exception has occured.", std::forward<_Ty>(_InnerException)) {}

		exception(const exception&) = delete;

		ARTEMIS_API exception(exception&&) noexcept;

		ARTEMIS_API virtual ~exception() noexcept;

		ARTEMIS_API const exception* inner() const noexcept;

		template<std::derived_from<exception> _Ty>
		inline const _Ty* inner() const noexcept { return static_cast<const _Ty*>(this->inner()); }

		ARTEMIS_API const std::stacktrace& trace() const noexcept;

		exception& operator=(const exception&) = delete;
		ARTEMIS_API exception& operator=(exception&&) noexcept;
	};
}

#endif // !__ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__