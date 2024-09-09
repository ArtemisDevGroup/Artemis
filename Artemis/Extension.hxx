#ifndef __ARTEMIS_EXTENSION_HXX__
#define __ARTEMIS_EXTENSION_HXX__

#include "Definitions.hxx"

#include "API/Error.hxx"
#include "API/Logging.hxx"

#include <string_view>		// std::string_view
#include <unordered_map>	// std::unordered_map

#include <Windows.h>		// HMODULE, DWORD

namespace Artemis {
	namespace API {
		class load_exception : public exception {
		public:
			ARTEMIS_API load_exception(std::string_view&& _Message) noexcept;

			template<derived_exception_type T>
			inline load_exception(std::string_view&& _Message, T&& _InnerException) noexcept : exception(std::forward<std::string_view>(_Message), std::forward<T>(_InnerException)) {}
		};
	}

	using aext_initialize_t = bool(__stdcall*)(void);	// extern "C" __declspec(dllexport) bool __stdcall aext_initialize(void);
	using aext_uninitialize_t = void(__stdcall*)(void);	// extern "C" __declspec(dllexport) void __stdcall aext_uninitialize(void);

	class extension {
		std::string_view _Name;
		HMODULE hModule;

	public:
		constexpr extension(const std::string_view& _Name) : _Name(_Name), hModule(nullptr) {}

		extension(const extension&) = delete;

		constexpr extension(extension&& _Other) noexcept : _Name(std::move(_Other._Name)) {
			if (_Other.hModule) {
				this->hModule = _Other.hModule;
				_Other.hModule = nullptr;
			}
			else this->hModule = nullptr;
		}

		ARTEMIS_FRAMEWORK ~extension() noexcept;

		ARTEMIS_FRAMEWORK void load();
		ARTEMIS_FRAMEWORK void eject();
		ARTEMIS_FRAMEWORK void force_eject() noexcept;

		constexpr const std::string_view& name() const noexcept { return this->_Name; }
		constexpr HMODULE handle() const noexcept { return this->hModule; }

		extension& operator=(const extension&) = delete;

		constexpr extension& operator=(extension&& _Other) noexcept {
			this->_Name = std::move(_Other._Name);

			if (_Other.hModule) {
				this->hModule = _Other.hModule;
				_Other.hModule = nullptr;
			}
			else this->hModule = nullptr;

			return *this;
		}
	};

	class extension_manager : public API::loggable {
		std::vector<extension*> _Loaded;

		ARTEMIS_FRAMEWORK void eject(std::vector<extension*>::iterator _Iterator) noexcept;

	public:
		ARTEMIS_FRAMEWORK extension_manager() noexcept;

		extension_manager(const extension_manager&) = delete;
		extension_manager(extension_manager&&) = delete;

		ARTEMIS_FRAMEWORK ~extension_manager() noexcept;

		ARTEMIS_FRAMEWORK extension* get(std::string_view&& _Name);
		ARTEMIS_FRAMEWORK extension* get(HMODULE _ModuleHandle);

		ARTEMIS_FRAMEWORK void load(const std::string_view& _Name) noexcept;
		ARTEMIS_FRAMEWORK void eject(const std::string_view& _Name);

		ARTEMIS_FRAMEWORK void eject_all() noexcept;

		extension_manager& operator=(const extension_manager&) = delete;
		extension_manager& operator=(extension_manager&&) = delete;
	};
}

#endif // !__ARTEMIS_EXTENSION_HXX__