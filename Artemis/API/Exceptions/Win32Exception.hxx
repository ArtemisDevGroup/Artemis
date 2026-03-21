#ifdef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

namespace Artemis::API {
	/**
	 * @brief An exception type thrown when a WinApi function reports an error through GetLastError().
	 */
	class win32_exception : public exception {
		std::string _Win32Function;
		DWORD _Win32ErrorCode;

		ARTEMIS_API static std::string_view win32_message(DWORD _Win32ErrorCode) noexcept;

	public:
		/**
		 * @brief Constructs a win32 exception with the name of a WinApi function.
		 * @param[in] _FunctionName The name of the failing function.
		 */
		ARTEMIS_API win32_exception(std::string_view _FunctionName) noexcept;

		/**
		 * @brief Constructs a win32 exception with the reported error code and the name of a WinApi function.
		 * @param[in] _Win32ErrorCode The error code obtained from a call to GetLastError().
		 * @param[in] _FunctionName The name of the failing function.
		 */
		ARTEMIS_API win32_exception(DWORD _Win32ErrorCode, std::string_view _FunctionName) noexcept;

		/**
		 * @brief Constructs a win32 exception with the reported error code, the name of a WinApi function and an underlying exception.
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @param[in] _Win32ErrorCode The error code obtained from a call to GetLastError().
		 * @param[in] _FunctionName The name of the failing function.
		 * @param[in] _InnerException A reference to the underlying exception.
		 */
		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline win32_exception(DWORD _Win32ErrorCode, std::string_view _FunctionName, _Ty&& _InnerException) noexcept : exception(win32_message(_Win32ErrorCode), std::forward<_Ty>(_InnerException)), _Win32Function(_FunctionName), _Win32ErrorCode(_Win32ErrorCode) {}

		/**
		 * @brief Constructs a win32 exception with the name of a WinApi function and an underlying exception.
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @param[in] _FunctionName The name of the failing function.
		 * @param[in] _InnerException A reference to the underlying exception.
		 */
		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline win32_exception(std::string_view _FunctionName, _Ty&& _InnerException) noexcept : win32_exception(GetLastError(), _FunctionName, std::forward<_Ty>(_InnerException)) {}

		/**
		 * @brief Gets the name of the WinApi function that failed.
		 * @return A reference to a string containing the name of the function that failed.
		 */
		ARTEMIS_API const std::string& win32_function() const noexcept;

		/**
		 * @brief Gets the WinApi error code associated with this exception.
		 * @return The WinApi error code reported by the failing function.
		 */
		ARTEMIS_API DWORD win32_error_code() const noexcept;
	};
}

#else
#error This header cannot be included on its own but has to be included as part of the 'Exception.hxx' header.
#endif