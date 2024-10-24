#ifdef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

#define ISE_CONDITION(condition) (condition), #condition

namespace Artemis::API {
	class invalid_state_exception : public exception {
	public:
		ARTEMIS_API invalid_state_exception() noexcept;

		ARTEMIS_API invalid_state_exception(std::string_view&& _Message) noexcept;

		template<derived_exception_type T>
		inline invalid_state_exception(std::string_view&& _Message, T&& _InnerException) noexcept : exception(std::move(_Message), std::forward<T>(_InnerException)) {}

		ARTEMIS_API static void throw_on_condition_not_met(bool _Condition, std::string_view&& _ConditionString);
	};
}

#else
#error This header cannot be included on its own but has to be included as part of the 'Exception.hxx' header.
#endif // __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__