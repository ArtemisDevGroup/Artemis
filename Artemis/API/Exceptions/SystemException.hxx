#ifdef __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__

namespace Artemis::API {
#pragma warning(push)
#pragma warning(disable:26495)	// Variable 'Artemis::API::seh_data::_Context' is uninitialized. Always initialize a member variable. (type.6)
								// Variable 'Artemis::API::seh_data::_Record' is uninitialized. Always initialize a member variable. (type.6)

	/// <summary>
	/// Data from an SEH exception throw.
	/// </summary>
	struct seh_data {
		/// <summary>
		/// The exception record. This contains all information about the exception itself.
		/// </summary>
		EXCEPTION_RECORD _Record;

		/// <summary>
		/// The exception context. This contains the values of all registers at the time of the exception.
		/// </summary>
		CONTEXT _Context;

		/// <summary>
		/// A vector containing all inner exception records.
		/// </summary>
		std::vector<EXCEPTION_RECORD> _InnerRecords;
	};

#pragma warning(pop)

	/// <summary>
	/// Gets the specified thread's last reported SEH data instance.
	/// </summary>
	/// <param name="_ThreadId">- The ID of the thread to obtain data from.</param>
	/// <returns>A pointer to the last recorded SEH data instance. If none exists for the specified thread, nullptr is returned..</returns>
	ARTEMIS_API seh_data* get_thread_seh_data(DWORD _ThreadId) noexcept;

	/// <summary>
	/// Gets the current thread's last reported SEH data instance.
	/// </summary>
	/// <returns>A pointer to the last recorded SEH data instance. If none exists for the current thread, nullptr is returned.</returns>
	ARTEMIS_API seh_data* get_thread_seh_data() noexcept;

	/// <summary>
	/// A class to be used in the __except filter statement.
	/// </summary>
	class seh_filter {
		seh_data* _Data;

	public:
		/// <summary>
		/// Constructs an SEH filter.
		/// </summary>
		/// <param name="_ExceptionPointers">- The pointer obtained from a call to GetExceptionInformation().</param>
		ARTEMIS_API seh_filter(LPEXCEPTION_POINTERS _ExceptionPointers);

		/// <summary>
		/// <para>Executues the exception handler, unless the exception code matches the provided exception code.</para>
		/// <para>If the exception handler is not executed, the exception will be propagated.</para>
		/// </summary>
		/// <param name="_ExceptionCode">- The exception code to pass.</param>
		/// <returns>The appropriate exception filter value to achieve the desired result.</returns>
		ARTEMIS_API int continue_on(DWORD _ExceptionCode) noexcept;

		/// <summary>
		/// <para>Executes the exception handler, only if the exception code matches the provided exception code.</para>
		/// <para>If the exception handler is not executed, the exception will be propagated.</para>
		/// </summary>
		/// <param name="_ExceptionCode">- The exception code to handle.</param>
		/// <returns>The appropriate exception filter value to achieve the desired result.</returns>
		ARTEMIS_API int handle_on(DWORD _ExceptionCode) noexcept;

		/// <summary>
		/// Returns the EXCEPTION_EXECUTE_HANDLER filter value, meaning the exception handler will always be ran, regardless what the exception code may be.
		/// </summary>
		/// <returns>The appropriate exception filter value to achieve the desired result.</returns>
		ARTEMIS_API int handle_always() noexcept;
	};

	class system_exception : public exception {
		EXCEPTION_RECORD _Record;
		CONTEXT _Context;
		std::vector<EXCEPTION_RECORD> _InnerRecords;

	public:
		ARTEMIS_API system_exception() noexcept;

		ARTEMIS_API system_exception(std::string_view&& _Message) noexcept;

		template<derived_exception_type T>
		inline system_exception(std::string_view&& _Message, T&& _InnerException) noexcept : exception(std::move(_Message), std::forward<T>(_InnerException)) {
			seh_data* data = get_thread_seh_data();
			this->_Record = data->_Record;
			this->_Context = data->_Context;
			this->_InnerRecords = data->_InnerRecords;
		}

		template<derived_exception_type T>
		inline system_exception(T&& _InnerException) noexcept : system_exception("A system exception has occured.", std::forward<T>(_InnerException)) {}

		ARTEMIS_API const EXCEPTION_RECORD* record() const noexcept;

		ARTEMIS_API const CONTEXT* context() const noexcept;

		ARTEMIS_API const std::vector<EXCEPTION_RECORD>& inner_records() const noexcept;
	};
}

#else
#error This header cannot be included on its own but has to be included as part of the 'Exception.hxx' header.
#endif // __ARTEMIS_API_EXCEPTIONS_EXCEPTION_BASE_HXX__