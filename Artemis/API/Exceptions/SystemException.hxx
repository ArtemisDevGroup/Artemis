#ifdef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

namespace Artemis::API {
#pragma warning(push)
#pragma warning(disable:26495)	// Variable 'Artemis::API::seh_data::_Context' is uninitialized. Always initialize a member variable. (type.6)
								// Variable 'Artemis::API::seh_data::_Record' is uninitialized. Always initialize a member variable. (type.6)
	
	/**
	 * @brief Data from an SEH exception throw.
	 */
	struct seh_data {
		/**
		 * @brief The exception record. This contains all information about the exception itself.
		 */
		EXCEPTION_RECORD _Record;

		/**
		 * @brief The exception context. This contains the values of all registers at the time of the exception.
		 */
		CONTEXT _Context;

		/**
		 * @brief A vector containing all inner exception records.
		 */
		std::vector<EXCEPTION_RECORD> _InnerRecords;
	};

#pragma warning(pop)

	/**
	 * @brief Gets the specified thread's last reported SEH data instance.
	 * @param[in] _ThreadId The id of the thread to obtain data from.
	 * @return A pointer to the löast recorded SEH data instance. If non exists for the specified thread, a nullptr is returned.
	 */
	ARTEMIS_API seh_data* get_thread_seh_data(DWORD _ThreadId) noexcept;

	/**
	 * @brief Gets the current thread's last reported SEH data instance.
	 * @return A pointer to the last recorded SEH data instance. If none exists for the current thread, nullptr is returned.
	 */
	ARTEMIS_API seh_data* get_thread_seh_data() noexcept;
	
	/**
	 * @brief An intermediate class to be used in the __except filter statement that allows logging
	 * and obtaining the system exception information.
	 */
	class seh_filter {
		seh_data* _Data;

	public:
		/**
		 * @brief Constructs an SEH filter.
		 * @param[in] _ExceptionPointers The pointer obtained from a call to GetExceptionInformation().
		 */
		ARTEMIS_API seh_filter(LPEXCEPTION_POINTERS _ExceptionPointers);
		
		/**
		 * @brief Executues the exception handler, unless the exception code matches the provided exception code.
		 * If the exception handler is not executed, the exception will be propagated.
		 * @param[in] _ExceptionCode The exception code to pass.
		 * @return The appropriate exception filter value to achieve the desired result.
		 */
		ARTEMIS_API int continue_on(DWORD _ExceptionCode) noexcept;

		/**
		 * @brief Executes the exception handler, only if the exception code matches the provided exception code.
		 * If the exception handler is not executed, the exception will be propagated.
		 * @param _ExceptionCode The code of the exception to invoke the handler on.
		 * @return The appropriate exception filter value to achieve the desired result.
		 */
		ARTEMIS_API int handle_on(DWORD _ExceptionCode) noexcept;

		/**
		 * @brief Returns the EXCEPTION_EXECUTE_HANDLER filter value, meaning the exception handler will always be ran, regardless what the exception code may be.
		 * @return The appropriate exception filter value to achieve the desired result.
		 */
		ARTEMIS_API int handle_always() noexcept;
	};

	/**
	 * @brief An exception type thrown when an underlying system exception is to be propagated.
	 */
	class system_exception : public exception {
		EXCEPTION_RECORD _Record;
		CONTEXT _Context;
		std::vector<EXCEPTION_RECORD> _InnerRecords;

	public:
		/**
		 * @brief Constructs a system exception.
		 */
		ARTEMIS_API system_exception() noexcept;

		/**
		 * @brief Constructs a system exception with a message.
		 * @param[in] _Message The exception message.
		 */
		ARTEMIS_API system_exception(std::string_view _Message) noexcept;

		/**
		 * @brief Constructs a system exception with a message and an underlying exception.
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @param[in] _Message The exception message.
		 * @param[in] _InnerException A reference to the underlying exception.
		 */
		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline system_exception(std::string_view _Message, _Ty&& _InnerException) noexcept : exception(_Message, std::forward<_Ty>(_InnerException)) {
			seh_data* data = get_thread_seh_data();
			this->_Record = data->_Record;
			this->_Context = data->_Context;
			this->_InnerRecords = data->_InnerRecords;
		}

		/**
		 * @brief Constructs a system exception with an underlying exception.
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @param[in] _InnerException A reference to the underlying exception.
		 */
		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline system_exception(_Ty&& _InnerException) noexcept : system_exception("A system exception has occured.", std::forward<_Ty>(_InnerException)) {}

		/**
		 * @brief Gets a pointer to the system exception record.
		 * @return A pointer to the system exception record contained by this instance.
		 */
		ARTEMIS_API const EXCEPTION_RECORD* record() const noexcept;

		/**
		 * @brief Gets a pointer to the CPU register context.
		 * @return A pointer to the register context contained by this instance.
		 */
		ARTEMIS_API const CONTEXT* context() const noexcept;

		/**
		 * @brief Gets a reference to a vector containing exception records of all underlying exceptions.
		 * @return A reference to a vector containing inner exception records.
		 */
		ARTEMIS_API const std::vector<EXCEPTION_RECORD>& inner_records() const noexcept;
	};
}

#else
#error This header cannot be included on its own but has to be included as part of the 'Exception.hxx' header.
#endif // __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__