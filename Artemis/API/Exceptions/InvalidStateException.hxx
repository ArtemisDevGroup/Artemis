#ifdef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

// A macro to pass both the evaluated condition result and a string representation of the
// conditon to a static invalid state exception throw helper.
#define ISE_CONDITION(condition) (condition), #condition

namespace Artemis::API {
	/**
	 * @brief An exception type thrown when the state of an object is not correct in order to call a function.
	 */
	class invalid_state_exception : public exception {
	public:
		/**
		 * @brief Constructs an invalid state exception.
		 */
		ARTEMIS_API invalid_state_exception() noexcept;

		/**
		 * @brief Constructs an invalid state exception with a message.
		 * @param[in] _Message The exception message.
		 */
		ARTEMIS_API invalid_state_exception(std::string_view _Message) noexcept;

		/**
		 * @brief Constructs an invalid state exception with a message and an underlying exception.
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @param[in] _Message The exception message.
		 * @param[in] _InnerException A reference to the underlying exception.
		 */
		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline invalid_state_exception(std::string_view _Message, _Ty&& _InnerException) noexcept : exception(_Message, std::forward<_Ty>(_InnerException)) {}

		/**
		 * @brief Throws an exception if the condition boolean evaluates to false.
		 * @param[in] _Condition The condition to evaluate.
		 * @param[in] _ConditionString A string representation of the condition to evaluate.
		 * @throws invalid_state_exception
		 */
		ARTEMIS_API static void throw_on_condition_not_met(bool _Condition, std::string_view _ConditionString);
	};
}

#else
#error This header cannot be included on its own but has to be included as part of the 'Exception.hxx' header.
#endif // __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__