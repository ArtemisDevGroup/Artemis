#ifndef ARTEMIS_API_HOOK_HXX
#define ARTEMIS_API_HOOK_HXX

#include "Definitions.hxx"
#include "Error.hxx"

#include "MinHook/MinHook.h"

namespace Artemis::API {
	class ARTEMIS_API minhook_exception : public exception {
		MH_STATUS _Status;

		static const char* const message(MH_STATUS _StatusCode);

	public:
		minhook_exception(MH_STATUS _StatusCode, call_stack_entry* _PopUntil = nullptr);

		template<derived_exception_type T>
		minhook_exception(MH_STATUS _StatusCode, const T& _InnerException) : exception(message(_StatusCode), _InnerException), _Status(_StatusCode) {}

		MH_STATUS mh_status() const;
	};

	ARTEMIS_API void global_hook_alloc();
	ARTEMIS_API void global_hook_release();

	template<typename _Fn>
	class hook {
		_Fn _Target;
		_Fn _Detour;
		_Fn _Original;

		bool _RemoveHook;

	public:
		hook(void* _Target, _Fn _Detour) : _Target((_Fn)_Target), _Detour(_Detour), _RemoveHook(true) {
			__stack_record();

			MH_STATUS status = MH_CreateHook(_Target, _Detour, (LPVOID*)&this->_Original);
			if (status != MH_OK) {
				call_stack_entry cse = __stack_this_cse();
				throw minhook_exception(status, &cse);
			}

			__stack_escape();
		}

		hook(const hook&) = delete;

		hook(hook&& _From) : _Target(_From._Target), _Detour(_From._Detour), _Original(_From._Original), _RemoveHook(true) { _From._RemoveHook = false; }

		~hook() {
			if (!this->_RemoveHook)
				return;

			__stack_record();

			MH_STATUS status = MH_RemoveHook(this->_Target);
			if (status != MH_OK) {
				call_stack_entry cse = __stack_this_cse();
				throw minhook_exception(status, &cse);
			}

			__stack_escape();
		}

		void enable() {
			__stack_record();

			MH_STATUS status = MH_EnableHook(this->_Target);
			if (status != MH_OK) {
				call_stack_entry cse = __stack_this_cse();
				throw minhook_exception(status, &cse);
			}

			__stack_escape();
		}

		void disable() {
			__stack_record();

			MH_STATUS status = MH_DisableHook(this->_Target);
			if (status != MH_OK) {
				call_stack_entry cse = __stack_this_cse();
				throw minhook_exception(status, &cse);
			}

			__stack_escape();
		}

		_Fn original() const noexcept { return this->_Original; }
	};

	template<typename _Ret, typename... _Args>
	class hook<_Ret(_Args...)> : public hook<_Ret(*)(_Args...)> {
		using hook<_Ret(*)(_Args...)>::hook;
	};
}

#endif // !__ARTEMIS_HOOK_HXX__