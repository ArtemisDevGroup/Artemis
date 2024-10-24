#ifdef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

namespace Artemis::API {
	class win32_exception : public exception {
		std::string_view _Win32Function;
		DWORD _Win32ErrorCode;

		ARTEMIS_API static std::string_view win32_message(DWORD _Win32ErrorCode) noexcept;

	public:
		ARTEMIS_API win32_exception(std::string_view&& _FunctionName) noexcept;

		ARTEMIS_API win32_exception(DWORD _Win32ErrorCode, std::string_view&& _FunctionName) noexcept;

		template<derived_exception_type T>
		inline win32_exception(DWORD _Win32ErrorCode, std::string_view&& _FunctionName, T&& _InnerException) noexcept : exception(win32_message(_Win32ErrorCode), _InnerException), _Win32Function(std::move(_FunctionName)), _Win32ErrorCode(_Win32ErrorCode) {}

		template<derived_exception_type T>
		inline win32_exception(std::string_view&& _FunctionName, T&& _InnerException) noexcept : win32_exception(GetLastError(), std::move(_FunctionName), std::forward<T>(_InnerException)) {}

		ARTEMIS_API const std::string_view& win32_function() const noexcept;

		ARTEMIS_API DWORD win32_error_code() const noexcept;
	};
}

#else
#error This header cannot be included on its own but has to be included as part of the 'Exception.hxx' header.
#endif