#ifdef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

#define AE_ARGUMENT(x) x, NAMEOF(x)

namespace Artemis::API {
	class argument_exception : public exception {
		std::string_view _ArgumentName;

	public:
		ARTEMIS_API argument_exception(std::string_view _Message, std::string_view _ArgumentName) noexcept;

		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline argument_exception(std::string_view _Message, std::string_view _ArgumentName, _Ty&& _InnerException) noexcept : exception(_Message, std::forward<_Ty>(_InnerException)), _ArgumentName(_ArgumentName) {}

		ARTEMIS_API std::string_view argument() const noexcept;

		template<std::convertible_to<bool> _Ty>
		static constexpr void throw_if_null(const _Ty& _Value, std::string_view _Name) {
			if (!_Value)
				throw argument_exception("Argument is null.", _Name);
		}

		template<std::equality_comparable _Ty>
		static constexpr void throw_if_equal(const _Ty& _Value, std::string_view _Name, const _Ty& _InvalidValue) {
			if (_Value == _InvalidValue)
				throw argument_exception("Argument is invalid.", _Name);
		}

		template<std::equality_comparable _Ty>
		static constexpr void throw_if_not_equal(const _Ty& _Value, std::string_view _Name, const _Ty& _ValidValue) {
			if (_Value != _ValidValue)
				throw argument_exception("Argument is invalid.", _Name);
		}

		template<std::totally_ordered _Ty>
		static constexpr void throw_if_less_than(const _Ty& _Value, std::string_view _Name, const _Ty& _ComparableValue) {
			if (_Value < _ComparableValue)
				throw argument_exception("Argument is invalid.", _Name);
		}

		template<std::totally_ordered _Ty>
		static constexpr void throw_if_less_than_or_equal(const _Ty& _Value, std::string_view _Name, const _Ty& _ComparableValue) {
			if (_Value <= _ComparableValue)
				throw argument_exception("Argument is invalid.", _Name);
		}

		template<std::totally_ordered _Ty>
		static constexpr void throw_if_more_than(const _Ty& _Value, std::string_view _Name, const _Ty& _ComparableValue) {
			if (_Value > _ComparableValue)
				throw argument_exception("Argument is invalid.", _Name);
		}

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