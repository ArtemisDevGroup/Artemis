#ifdef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

namespace Artemis::API {
	/**
	 * @brief An exception type thrown when a locking mechanism malfunctions.
	 */
	class lock_exception : public exception {
	public:
		/**
		 * @brief Constructs a lock exception with a message.
		 * @param[in] _Message The exception message.
		 */
		ARTEMIS_API lock_exception(std::string_view _Message) noexcept;

		/**
		 * @brief Constructs a lock exception with a message and an underlying exception
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @param[in] _Message The exception message.
		 * @param[in] _InnerException A reference to the underlying exception.
		 */
		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		lock_exception(std::string_view _Message, _Ty&& _InnerException) noexcept : exception(_Message, std::forward<_Ty>(_InnerException)) {}
	};

	/**
	 * @brief An exception type thrown when the caller does not have access to the action protected by the lock.
	 */
	class lock_access_exception : public lock_exception {
		static constexpr inline std::string_view _ExceptionMessage = "The current thread does not own this resource's key, and therefore does not have access to this resource.";

	public:
		/**
		 * @brief Constructs a lock access exception.
		 */
		ARTEMIS_API lock_access_exception() noexcept;

		/**
		 * @brief Constructs a lock access exception with an underlying exception.
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @param _InnerException A reference to the underlying exception.
		 */
		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		lock_access_exception(_Ty&& _InnerException) noexcept : lock_exception(_ExceptionMessage, std::forward<_Ty>(_InnerException)) {}
	};
}

#else
#error This header cannot be included on its own but has to be included as part of the 'Exception.hxx' header.
#endif // __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__