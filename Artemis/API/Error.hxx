#ifndef ARTEMIS_API_ERROR_HXX
#define ARTEMIS_API_ERROR_HXX

#include "Definitions.hxx"
#include "Event.hxx"

#ifdef ARTEMIS_DISABLE_CALL_STACK
#define __stack_record()
#define __stack_escape()
#else
#define __stack_record() Artemis::API::call_stack_manager::global()->record(__FUNCTION__, __FILE__, __LINE__)
#define __stack_escape() Artemis::API::call_stack_manager::global()->escape()
#define __stack_this_cse() Artemis::API::call_stack_entry(__FUNCTION__, __FILE__, __LINE__)
#endif // ARTEMIS_DISABLE_CALL_STACK


namespace Artemis::API {
	struct call_stack_entry {
		std::string _Function;
		std::string _File;
		int _Line;
	};
	
	class call_stack_manager;

	class call_stack {
		std::vector<call_stack_entry> _StackEntries;
		call_stack_manager* _Owner;
		DWORD _ThreadId;
		bool _IsSnapshot;

	public:
		ARTEMIS_API call_stack(call_stack_manager* _Owner, DWORD _ThreadId);

		ARTEMIS_API void push_back(std::string _Function, std::string _File, int _Line); 
		ARTEMIS_API void pop_back();
		ARTEMIS_API void pop_back(int _Count);
		ARTEMIS_API void pop_until(const call_stack_entry& _Entry);
		ARTEMIS_API void pop_until(std::string _Function, std::string _File, int _Line);

		ARTEMIS_API const std::vector<call_stack_entry>& entries() const;
		ARTEMIS_API DWORD thread_id() const;

		ARTEMIS_API bool is_empty() const;
		ARTEMIS_API bool is_snapshot() const;

		ARTEMIS_API void for_each(std::function<void(const call_stack_entry* const)> _Callback) const;

		ARTEMIS_API std::string to_string() const;

		ARTEMIS_API call_stack snap() const;
		ARTEMIS_API void drop();
	};

	class call_stack_manager {
		std::vector<call_stack> _CallStacks;

	public:
		ARTEMIS_API call_stack_manager();
		call_stack_manager(const call_stack_manager&) = delete;
		call_stack_manager(call_stack_manager&&) = delete;

		ARTEMIS_API call_stack* record(DWORD _ThreadId, std::string _Function, std::string _File, int _Line);
		ARTEMIS_API call_stack* record(std::string _Function, std::string _File, int _Line);

		ARTEMIS_API call_stack* escape(DWORD _ThreadId);
		ARTEMIS_API call_stack* escape();

		ARTEMIS_API call_stack* fetch(DWORD _ThreadId) const;
		ARTEMIS_API call_stack* fetch() const;

		ARTEMIS_API void drop(DWORD _ThreadId);
		ARTEMIS_API void drop();

		ARTEMIS_API static call_stack_manager* global();
	};

	class exception : public std::exception {
		call_stack _CallStackSnapshot;
		std::shared_ptr<exception> _InnerException;

		ARTEMIS_API void event_invoke();

	public:
		ARTEMIS_API exception(call_stack_entry* _PopUntil = nullptr);
		ARTEMIS_API exception(const char* const _Message, call_stack_entry* _PopUntil = nullptr);

		template<class T>
			requires(std::is_base_of_v<exception, T>)
		inline exception(const char* const _Message, const T& _InnerException) : std::exception(_Message), _CallStackSnapshot(nullptr, 0) {
			this->_InnerException = std::make_shared<T>(_InnerException);
			this->_CallStackSnapshot = _InnerException._CallStackSnapshot;

			event_invoke();
		}

		ARTEMIS_API const std::shared_ptr<exception> inner() const;

		template<class T>
			requires(std::is_base_of_v<exception, T>)
		inline const std::shared_ptr<T> inner() const { return (std::shared_ptr<T>)this->inner(); }

		ARTEMIS_API const call_stack* calls() const;

		ARTEMIS_API static event<exception>* throw_event();
	};

	template<typename T>
	concept derived_exception_type = std::is_base_of_v<exception, T>;
}

#endif // !ARTEMIS_API_ERROR_HXX