#include "pch.h"
#include "Hook.hxx"

namespace Artemis::API {
	const char* const minhook_exception::message(MH_STATUS _StatusCode) noexcept {
		switch (_StatusCode) {
		case MH_UNKNOWN:
		default:
			return "Unknown error.";
		case MH_OK:
			return "Successful.";
		case MH_ERROR_ALREADY_INITIALIZED:
			return "MinHook is not initialized yet, or already uninitialized.";
		case MH_ERROR_ALREADY_CREATED:
			return "The hook for the specified target function is already created.";
		case MH_ERROR_NOT_CREATED:
			return "The hook for the specified target function is not created yet.";
		case MH_ERROR_ENABLED:
			return "The hook for the specified target function is already enabled.";
		case MH_ERROR_DISABLED:
			return "The hook for the specified target function is not enabled yet, or already disabled.";
		case MH_ERROR_NOT_EXECUTABLE:
			return "The specified pointer is invalid. It points the address of non-allocated and/or non-executable region.";
		case MH_ERROR_UNSUPPORTED_FUNCTION:
			return "The specified target function cannot be hooked.";
		case MH_ERROR_MEMORY_ALLOC:
			return "Failed to allocate memory.";
		case MH_ERROR_MEMORY_PROTECT:
			return "Failed to change the memory protection.";
		case MH_ERROR_MODULE_NOT_FOUND:
			return "The specified module is not loaded.";
		case MH_ERROR_FUNCTION_NOT_FOUND:
			return "The specified function is not found.";
		}
	}

	minhook_exception::minhook_exception(MH_STATUS _StatusCode) noexcept : exception(message(_StatusCode)), _Status(_StatusCode) {}

	MH_STATUS minhook_exception::mh_status() const noexcept { return this->_Status; }

	void global_hook_alloc() {
		__stack_record();

		MH_STATUS status = MH_Initialize();
		if (status != MH_OK)
			throw minhook_exception(status);

		__stack_escape();
	}

	void global_hook_release() {
		__stack_record();

		MH_STATUS status = MH_Uninitialize();
		if (status != MH_OK)
			throw minhook_exception(status);

		__stack_escape();
	}
}