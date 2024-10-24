#ifdef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

#define AE_ARGUMENT(x) x, NAMEOF(x)

namespace Artemis::API {
	class argument_exception : public exception {
		std::string_view _ArgumentName;

	public:
		ARTEMIS_API argument_exception(std::string_view&& _Message, std::string_view&& _ArgumentName) noexcept;

		template<derived_exception_type T>
		inline argument_exception(std::string_view&& _Message, std::string_view&& _ArgumentName, T&& _InnerException) noexcept : exception(std::move(_Message), std::forward<T>(_InnerException)), _ArgumentName(std::move(_ArgumentName)) {}

		ARTEMIS_API const std::string_view& argument() const noexcept;

		template<typename _Ty> requires requires (_Ty x) { (bool)x; }
		static constexpr void throw_if_null(const _Ty& _Value, std::string_view&& _Name) {
			if (!_Value)
				throw argument_exception("Argument is null.", std::move(_Name));
		}

		template<typename _Ty> requires requires (_Ty x, _Ty y) { x == y; }
		static constexpr void throw_if_equal(const _Ty& _Value, std::string_view&& _Name, _Ty&& _InvalidValue) {
			if (_Value == _InvalidValue)
				throw argument_exception("Argument is invalid.", std::move(_Name));
		}

		template<typename _Ty> requires requires (_Ty x, _Ty y) { x <= y; }
		static constexpr void throw_if_less_than_or_equal(const _Ty& _Value, std::string_view&& _Name, _Ty&& _InvalidValue) {
			if (_Value <= _InvalidValue)
				throw argument_exception("Argument is invalid.", std::move(_Name));
		}
	};
}

#else
#error This header cannot be included on its own but has to be included as part of the 'Exception.hxx' header.
#endif // __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__