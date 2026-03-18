#ifdef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

namespace Artemis::API {
	/**
	 * @brief An exception type that wraps the errno system from the C standard API.
	 */
	class errno_exception : public exception {
		std::string _CStdFunction;
		errno_t _ErrnoCode;

		ARTEMIS_API static std::string_view errno_message(errno_t _ErrnoCode);

	public:
		/**
		 * @brief Constructs an errno exception with a function name.
		 * @param[in] _FunctionName The name of the function that caused the error.
		 */
		ARTEMIS_API errno_exception(std::string_view _FunctionName) noexcept;

		/**
		 * @brief Constructs an errno exception with an errno code and a function name.
		 * @param[in] _ErrnoCode The errno code associated with the error.
		 * @param[in] _FunctionName The name of the function that caused the error.
		 */
		ARTEMIS_API errno_exception(errno_t _ErrnoCode, std::string_view _FunctionName) noexcept;

		/**
		 * @brief Constructs an errno exception with a function name and an underlying exception.
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @param[in] _FunctionName The name of the function that caused the error.
		 * @param[in] _InnerException A reference to the underlying exception.
		 */
		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline errno_exception(std::string_view _FunctionName, _Ty&& _InnerException) : exception(errno_message(errno), std::forward<_Ty>(_InnerException)), _ErrnoCode(errno), _CStdFunction(_FunctionName) {}

		/**
		 * @brief Constructs an errno exception with an errno code, a function name and an underlying exception.
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @param[in] _ErrnoCode The errno code associated with the error.
		 * @param[in] _FunctionName The name of the function that caused the error.
		 * @param[in] _InnerException A reference to the underlying exception.
		 */
		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline errno_exception(errno_t _ErrnoCode, std::string_view _FunctionName, _Ty&& _InnerException) : exception(errno_message(_ErrnoCode), std::forward<_Ty>(_InnerException)), _ErrnoCode(_ErrnoCode), _CStdFunction(_FunctionName) {}

		/**
		 * @brief Gets the name of the C standard function that caused the error.
		 * @return The name of the erronious C standard function.
		 */
		ARTEMIS_API const std::string& cstd_function() const noexcept;

		/**
		 * @brief Gets the associated errno code.
		 * @return The associated errno code.
		 */
		ARTEMIS_API errno_t errno_code() const noexcept;
	};
}

#else
#error This header cannot be included on its own but has to be included as part of the 'Exception.hxx' header.
#endif // __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__