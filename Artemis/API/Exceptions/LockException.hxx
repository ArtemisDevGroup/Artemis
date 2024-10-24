#ifdef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

namespace Artemis::API {
	class lock_exception : public exception {
	public:
		ARTEMIS_API lock_exception(std::string_view&& _Message) noexcept;

		template<derived_exception_type _Ty>
		lock_exception(std::string_view&& _Message, _Ty&& _InnerException) noexcept : exception(std::move(_Message), std::forward<_Ty>(_InnerException)) {}
	};

	class lock_access_exception : public lock_exception {
		static constexpr inline const char* const _ExceptionMessage = "The current thread does not own this resource's key, and therefore does not have access to this resource.";

	public:
		ARTEMIS_API lock_access_exception() noexcept;

		template<derived_exception_type _Ty>
		lock_access_exception(_Ty&& _InnerException) noexcept : lock_exception(_ExceptionMessage, std::forward<_Ty>(_InnerException)) {}
	};
}

#else
#error This header cannot be included on its own but has to be included as part of the 'Exception.hxx' header.
#endif // __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__