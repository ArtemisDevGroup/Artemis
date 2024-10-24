#ifdef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

namespace Artemis::API {
	class errno_exception : public exception {
		std::string_view _CStdFunction;
		errno_t _ErrnoCode;

		ARTEMIS_API static std::string_view errno_message(errno_t _ErrnoCode);

	public:
		ARTEMIS_API errno_exception(std::string_view&& _FunctionName) noexcept;

		ARTEMIS_API errno_exception(errno_t _ErrnoCode, std::string_view&& _FunctionName) noexcept;

		template<derived_exception_type T>
		inline errno_exception(std::string_view&& _FunctionName, T&& _InnerException) : exception(errno_message(errno), std::forward<T>(_InnerException)), _ErrnoCode(errno), _CStdFunction(std::move(_FunctionName)) {}

		template<derived_exception_type T>
		inline errno_exception(errno_t _ErrnoCode, std::string_view&& _FunctionName, T&& _InnerException) : exception(errno_message(_ErrnoCode), std::forward<T>(_InnerException)), _ErrnoCode(_ErrnoCode), _CStdFunction(_FunctionName) {}

		ARTEMIS_API const std::string_view& cstd_function() const noexcept;

		ARTEMIS_API errno_t errno_code() const noexcept;
	};
}

#else
#error This header cannot be included on its own but has to be included as part of the 'Exception.hxx' header.
#endif // __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__