#ifdef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

// A macro to pass both a reference to the argument as well as a string representation of it
// to a static argument exception throw helper.
#define AE_ARGUMENT(x) x, NAMEOF(x)

namespace Artemis::API {
	/**
	 * @brief An exception type thrown when a function argument is not as expected.
	 */
	class argument_exception : public exception {
		std::string _ArgumentName;

	public:
		/**
		 * @brief Constructs an argument exception with a message and an argument name.
		 * @param[in] _Message The exception message.
		 * @param[in] _ArgumentName The name of the invalid argument.
		 */
		ARTEMIS_API argument_exception(std::string_view _Message, std::string_view _ArgumentName) noexcept;

		/**
		 * @brief Constructs an argument exception with a message, an argument name and an underlying exception.
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @param[in] _Message The exception message.
		 * @param[in] _ArgumentName The name of the invalid argument.
		 * @param[in] _InnerException A reference to the underlying exception.
		 */
		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline argument_exception(std::string_view _Message, std::string_view _ArgumentName, _Ty&& _InnerException) noexcept : exception(_Message, std::forward<_Ty>(_InnerException)), _ArgumentName(_ArgumentName) {}

		/**
		 * @brief Gets the name of the invalid argument.
		 * @return A reference to a string containing the name of the invalid argument.
		 */
		ARTEMIS_API const std::string& argument() const noexcept;

		/**
		 * @brief Throws an exception if the referenced value is null.
		 * @tparam _Ty The value type. Must satisfy `std::convertible_to<bool>`.
		 * @param[in] _Value A reference to a variable to null-check.
		 * @param[in] _Name The name of the variable to null check.
		 * @throws argument_exception
		 */
		template<std::convertible_to<bool> _Ty>
		static constexpr void throw_if_null(const _Ty& _Value, std::string_view _Name) {
			if (!_Value)
				throw argument_exception("Argument is null.", _Name);
		}

		/**
		 * @brief Throws an exception if the referenced value is equal to a provided value.
		 * @tparam _Ty The value type. Must satisfy `std::equality_comparable`.
		 * @param[in] _Value A reference to a variable to equality check.
		 * @param[in] _Name The name of the variable to equality check.
		 * @param[in] _InvalidValue The value to equality check against.
		 * @throws argument_exception
		 */
		template<std::equality_comparable _Ty>
		static constexpr void throw_if_equal(const _Ty& _Value, std::string_view _Name, const _Ty& _InvalidValue) {
			if (_Value == _InvalidValue)
				throw argument_exception("Argument is invalid.", _Name);
		}

		/**
		 * @brief Throws an exception if the reference value is not equal to the provided value.
		 * @tparam _Ty The value type. Must satisfy `std::equality_comparable`.
		 * @param[in] _Value A reference to a variable to equality check.
		 * @param[in] _Name The name of the variable to equality check.
		 * @param[in] _ValidValue The value to equality check against.
		 * @throws argument_exception
		 */
		template<std::equality_comparable _Ty>
		static constexpr void throw_if_not_equal(const _Ty& _Value, std::string_view _Name, const _Ty& _ValidValue) {
			if (_Value != _ValidValue)
				throw argument_exception("Argument is invalid.", _Name);
		}

		/**
		 * @brief Throws an exception if the reference value is less than the provided value.
		 * @tparam _Ty The value type. Must satisfy `std::totally_ordered`.
		 * @param[in] _Value A reference to a variable to compare.
		 * @param[in] _Name The name of the variable to compare.
		 * @param[in] _ComparableValue The value to compare to.
		 * @throws argument_exception
		 */
		template<std::totally_ordered _Ty>
		static constexpr void throw_if_less_than(const _Ty& _Value, std::string_view _Name, const _Ty& _ComparableValue) {
			if (_Value < _ComparableValue)
				throw argument_exception("Argument is invalid.", _Name);
		}

		/**
		 * @brief Throws an exception if the reference value is less than or equal to the provided value.
		 * @tparam _Ty The value type. Must satisfy `std::totally_ordered`.
		 * @param[in] _Value A reference to a variable to compare.
		 * @param[in] _Name The name of the variable to compare.
		 * @param[in] _ComparableValue The value to compare to.
		 * @throws argument_exception
		 */
		template<std::totally_ordered _Ty>
		static constexpr void throw_if_less_than_or_equal(const _Ty& _Value, std::string_view _Name, const _Ty& _ComparableValue) {
			if (_Value <= _ComparableValue)
				throw argument_exception("Argument is invalid.", _Name);
		}

		/**
		 * @brief Throws an exception if the reference value is more than the provided value.
		 * @tparam _Ty The value type. Must satisfy `std::totally_ordered`.
		 * @param[in] _Value A reference to a variable to compare.
		 * @param[in] _Name The name of the variable to compare.
		 * @param[in] _ComparableValue The value to compare to.
		 * @throws argument_exception
		 */
		template<std::totally_ordered _Ty>
		static constexpr void throw_if_more_than(const _Ty& _Value, std::string_view _Name, const _Ty& _ComparableValue) {
			if (_Value > _ComparableValue)
				throw argument_exception("Argument is invalid.", _Name);
		}

		/**
		 * @brief Throws an exception if the reference value is more than or equal to the provided value.
		 * @tparam _Ty The value type. Must satisfy `std::totally_ordered`.
		 * @param[in] _Value A reference to a variable to compare.
		 * @param[in] _Name The name of the variable to compare.
		 * @param[in] _ComparableValue The value to compare to.
		 * @throws argument_exception
		 */
		template<std::totally_ordered _Ty>
		static constexpr void throw_if_more_than_or_equal(const _Ty& _Value, std::string_view _Name, const _Ty& _ComparableValue) {
			if (_Value >= _ComparableValue)
				throw argument_exception("Argument is invalid.", _Name);
		}
	};
}

#else
#error This header cannot be included on its own but has to be included as part of the 'Exception.hxx' header.
#endif // __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__