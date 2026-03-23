#ifndef __ARTEMIS_API_HOOK_HXX__
#define __ARTEMIS_API_HOOK_HXX__

#include <concepts>		// std::derived_from
#include <type_traits>	// std::remove_reference_t
#include <utility>		// std::forward
#include <string_view>	// std::string_view

#include "Definitions.hxx"
#include "Exception.hxx"

#include "MinHook/MinHook.h"

namespace Artemis::API {
	/**
	 * @brief An exception type thrown when a MinHook error is to be propagated.
	 */
	class minhook_exception : public exception {
		MH_STATUS _Status;

		ARTEMIS_API static std::string_view message(MH_STATUS _StatusCode) noexcept;

	public:
		/**
		 * @brief Constructs a minhook exception with a status code.
		 * @param[in] _StatusCode The status code returned by the minhook function.
		 */
		ARTEMIS_API minhook_exception(MH_STATUS _StatusCode) noexcept;

		/**
		 * @brief Constructs a minhook exception with a status code and an underlying exception.
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @param[in] _StatusCode The status code returned by the minhook function.
		 * @param[in] _InnerException A reference to the underlying exception.
		 */
		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline minhook_exception(MH_STATUS _StatusCode, _Ty&& _InnerException) noexcept : exception(message(_StatusCode), std::forward<_Ty>(_InnerException)), _Status(_StatusCode) {}

		/**
		 * @brief Gets the status code associated with this instance.
		 * @return The instance status code.
		 */
		ARTEMIS_API MH_STATUS mh_status() const noexcept;
	};

	/**
	 * @brief Does the necessary initialization and allocation for MinHook to operate.
	 * @throws minhook_exception
	 */
	ARTEMIS_API void global_hook_alloc();

	/**
	 * @brief Releases the allocations used by MinHook.
	 * @throws minhook_exception
	 */
	ARTEMIS_API void global_hook_release();

	/**
	 * @brief A wrapper for the MinHook library which allows for hooking of functions.
	 * @tparam _Fn 
	 */
	template<typename _Fn>
	class hook {
		_Fn _Target;
		_Fn _Detour;
		_Fn _Original;

		bool _RemoveHook;

	public:
		/**
		 * @brief Constructs a hook with a target and a detour.
		 * @param[in] _Target A pointer to the target function.
		 * @param[in] _Detour A pointer to the detour function to replace the target with.
		 * @throws minhook_exception
		 */
		inline hook(void* _Target, _Fn _Detour) : _Target((_Fn)_Target), _Detour(_Detour), _RemoveHook(true) {
			MH_STATUS status = MH_CreateHook(_Target, _Detour, (LPVOID*)&this->_Original);
			if (status != MH_OK)
				throw minhook_exception(status);
		}

		hook(const hook&) = delete;
		hook& operator=(const hook&) = delete;

		/**
		 * @brief Moves ownership of the hook from the reference object to this object.
		 * @param[in] _From A reference to the object to move ownership from.
		 */
		inline hook(hook&& _From) noexcept : _Target(_From._Target), _Detour(_From._Detour), _Original(_From._Original), _RemoveHook(true) { _From._RemoveHook = false; }

		/**
		 * @brief Moves ownership of the hook from the reference object to this object.
		 * @param[in] _From A reference to the object to move ownership from.
		 * @return A reference to the current instance.
		 */
		inline hook& operator=(hook&& _From) noexcept {
			this->_Target = _From._Target;
			this->_Detour = _From._Detour;
			this->_Original = _From._Original;
			this->_RemoveHook = true;
			_From._RemoveHook = false;
		}

		inline ~hook() noexcept {
			if (!this->_RemoveHook)
				return;

			MH_RemoveHook(this->_Target);
		}

		/**
		 * @brief Enables the hook, which replaces the target with the detour.
		 * @throws minhook_exception
		 */
		inline void enable() {
			MH_STATUS status = MH_EnableHook(this->_Target);
			if (status != MH_OK)
				throw minhook_exception(status);
		}

		/**
		 * @brief Disables the hook, which replaces the target with the original.
		 * @throws minhook_exception
		 */
		inline void disable() {
			MH_STATUS status = MH_DisableHook(this->_Target);
			if (status != MH_OK)
				throw minhook_exception(status);
		}

		/**
		 * @brief Gets a pointer to the original function.
		 * @return A pointer to the original function.
		 */
		inline _Fn original() const noexcept { return this->_Original; }
	};

	/**
	 * @brief Forwards a C++ functional style of function template.
	 * @tparam _Ret The return type.
	 * @tparam _Args The argument types.
	 */
	template<typename _Ret, typename... _Args>
	class hook<_Ret(_Args...)> : public hook<_Ret(*)(_Args...)> {
		using hook<_Ret(*)(_Args...)>::hook;
	};
}

#endif // !__ARTEMIS_API_HOOK_HXX__