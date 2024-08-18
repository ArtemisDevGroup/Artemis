#ifndef ARTEMIS_API_HOOK_HXX
#define ARTEMIS_API_HOOK_HXX

#include <utility>	// std::forward

#include "Definitions.hxx"
#include "Error.hxx"

#include "MinHook/MinHook.h"

namespace Artemis::API {
	class minhook_exception : public exception {
		MH_STATUS _Status;

		ARTEMIS_API static const char* const message(MH_STATUS _StatusCode) noexcept;

	public:
		ARTEMIS_API minhook_exception(MH_STATUS _StatusCode) noexcept;

		template<derived_exception_type T>
		inline minhook_exception(MH_STATUS _StatusCode, T&& _InnerException) noexcept : exception(message(_StatusCode), std::forward<T>(_InnerException)), _Status(_StatusCode) {}

		ARTEMIS_API MH_STATUS mh_status() const noexcept;
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
		inline hook(void* _Target, _Fn _Detour) : _Target((_Fn)_Target), _Detour(_Detour), _RemoveHook(true) {
			__stack_record();

			MH_STATUS status = MH_CreateHook(_Target, _Detour, (LPVOID*)&this->_Original);
			if (status != MH_OK)
				throw minhook_exception(status);

			__stack_escape();
		}

		hook(const hook&) = delete;

		inline hook(hook&& _From) noexcept : _Target(_From._Target), _Detour(_From._Detour), _Original(_From._Original), _RemoveHook(true) { _From._RemoveHook = false; }

		inline ~hook() noexcept {
			if (!this->_RemoveHook)
				return;

			MH_RemoveHook(this->_Target);
		}

		inline void enable() {
			__stack_record();

			MH_STATUS status = MH_EnableHook(this->_Target);
			if (status != MH_OK)
				throw minhook_exception(status);

			__stack_escape();
		}

		inline void disable() {
			__stack_record();

			MH_STATUS status = MH_DisableHook(this->_Target);
			if (status != MH_OK)
				throw minhook_exception(status);

			__stack_escape();
		}

		inline _Fn original() const noexcept { return this->_Original; }
	};

	template<typename _Ret, typename... _Args>
	class hook<_Ret(_Args...)> : public hook<_Ret(*)(_Args...)> {
		using hook<_Ret(*)(_Args...)>::hook;
	};
}

#endif // !__ARTEMIS_HOOK_HXX__