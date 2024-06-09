#ifndef ARTEMIS_API_ERROR_HXX
#define ARTEMIS_API_ERROR_HXX

#include "Definitions.hxx"

#define stack_record() Artemis::API::call_stack_manager::global()->record(__FUNCTION__, __FILE__, __LINE__)
#define stack_escape() Artemis::API::call_stack_manager::global()->escape()

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
		call_stack(call_stack_manager* _Owner, DWORD _ThreadId);
		call_stack(const call_stack&) = delete;
		call_stack(call_stack&&) = delete;

		void push_back(std::string _Function, std::string _File, int _Line);
		void pop_back();

		const std::vector<call_stack_entry>& entries() const;
		DWORD thread_id() const;

		bool is_empty() const;
		bool is_snapshot() const;

		void for_each(std::function<void(const call_stack_entry* const)> _Callback) const;

		std::string to_string() const;

		void drop();
	};

	class call_stack_manager {
		std::vector<call_stack> _CallStacks;

	public:
		call_stack_manager();
		call_stack_manager(const call_stack_manager&) = delete;
		call_stack_manager(call_stack_manager&&) = delete;

		call_stack* record(DWORD _ThreadId, std::string _Function, std::string _File, int _Line);
		call_stack* record(std::string _Function, std::string _File, int _Line);

		call_stack* escape(DWORD _ThreadId);
		call_stack* escape();

		call_stack* fetch(DWORD _ThreadId) const;
		call_stack* fetch() const;

		void drop(DWORD _ThreadId);
		void drop();

		static call_stack_manager* global();
	};

	class exception : std::exception {
		call_stack _CallStackSnapshot;

	public:
		exception();
		exception(const char* const _Message);
		exception(const char* const _Message, const exception& _InnerException);
		exception(const char* const _Message, exception&& _InnerException);
	};
}

#endif // !ARTEMIS_API_ERROR_HXX