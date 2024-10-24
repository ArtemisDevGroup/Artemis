#include "pch.h"
#include "ExceptionBase.hxx"
#include "SystemException.hxx"

namespace Artemis::API {
#pragma region Class seh_filter

	std::unordered_map<DWORD, seh_data> g_SystemExceptionData;

	seh_data* get_thread_seh_data(DWORD _ThreadId) noexcept {
		if (g_SystemExceptionData.contains(_ThreadId))
			return &g_SystemExceptionData[_ThreadId];
		return nullptr;
	}

	seh_data* get_thread_seh_data() noexcept { return get_thread_seh_data(GetCurrentThreadId()); }

	seh_filter::seh_filter(LPEXCEPTION_POINTERS _ExceptionPointers) {
		seh_data data;
		memcpy(&data._Record, _ExceptionPointers->ExceptionRecord, sizeof(EXCEPTION_RECORD));

		while (LPEXCEPTION_RECORD record = _ExceptionPointers->ExceptionRecord->ExceptionRecord) {
			EXCEPTION_RECORD obj;
			memcpy(&obj, record, sizeof(EXCEPTION_RECORD));
			record = obj.ExceptionRecord;
			data._InnerRecords.push_back(std::move(obj));
		}

		memcpy(&data._Context, _ExceptionPointers->ContextRecord, sizeof(CONTEXT));

		DWORD dwThreadId = GetCurrentThreadId();

		g_SystemExceptionData[dwThreadId] = std::move(data);
		this->_Data = &g_SystemExceptionData[dwThreadId];
	}

	int seh_filter::continue_on(DWORD _ExceptionCode) noexcept {
		return this->_Data->_Record.ExceptionCode == _ExceptionCode ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
	}

	int seh_filter::handle_on(DWORD _ExceptionCode) noexcept {
		return this->_Data->_Record.ExceptionCode == _ExceptionCode ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
	}

	int seh_filter::handle_always() noexcept { return EXCEPTION_EXECUTE_HANDLER; }

#pragma endregion

#pragma region Class system_exception

	system_exception::system_exception() noexcept : exception("A system exception has occured.") {
		seh_data* data = get_thread_seh_data();
		this->_Record = data->_Record;
		this->_Context = data->_Context;
		this->_InnerRecords = data->_InnerRecords;
	}

	system_exception::system_exception(std::string_view&& _Message) noexcept : exception(std::move(_Message)) {
		seh_data* data = get_thread_seh_data();
		this->_Record = data->_Record;
		this->_Context = data->_Context;
		this->_InnerRecords = data->_InnerRecords;
	}

	const EXCEPTION_RECORD* system_exception::record() const noexcept { return &this->_Record; }
	const CONTEXT* system_exception::context() const noexcept { return &this->_Context; }
	const std::vector<EXCEPTION_RECORD>& system_exception::inner_records() const noexcept { return this->_InnerRecords; }

#pragma endregion
}