#ifndef ARTEMIS_EXTENSION_HXX
#define ARTEMIS_EXTENSION_HXX

#include <string_view>	// std::string_view
#include <utility>		// std::move, std::forward

#include <Windows.h>	// HMODULE

#include "Definitions.hxx"

#include "API/Error.hxx"
#include "API/Logging.hxx"

namespace Artemis {
#pragma region Class load_exception

	namespace API {
		class load_exception : public exception {
		public:
			ARTEMIS_API load_exception(std::string_view&& _Message) noexcept;

			template<derived_exception_type T>
			inline load_exception(std::string_view&& _Message, T&& _InnerException) noexcept : exception(std::forward<std::string_view>(_Message), std::forward<T>(_InnerException)) {}
		};
	}

#pragma endregion

#pragma region Class extension

	class extension {
		std::string_view _ExtensionName;
		HMODULE hModule;

	public:
		using aext_initialize_t = bool(__stdcall*)(void);	// extern "C" __declspec(dllexport) bool __stdcall aext_initialize(void);
		using aext_uninitialize_t = void(__stdcall*)(void);	// extern "C" __declspec(dllexport) void __stdcall aext_uninitialize(void);

		constexpr extension(const std::string_view& _Name) noexcept : _ExtensionName(_Name), hModule(nullptr) {}

		extension(const extension&) = delete;

		constexpr extension(extension&& _Other) noexcept : _ExtensionName(std::move(_Other._ExtensionName)) {
			if (_Other.hModule) {
				this->hModule = _Other.hModule;
				_Other.hModule = nullptr;
			}
			else this->hModule = nullptr;
		}

		extension& operator=(const extension&) = delete;

		constexpr extension& operator=(extension&& _Other) noexcept {
			this->_ExtensionName = std::move(_Other._ExtensionName);

			if (_Other.hModule) {
				this->hModule = _Other.hModule;
				_Other.hModule = nullptr;
			}
			else this->hModule = nullptr;

			return *this;
		}

		ARTEMIS_FRAMEWORK ~extension();

		ARTEMIS_FRAMEWORK void load();
		ARTEMIS_FRAMEWORK void eject();
		ARTEMIS_FRAMEWORK void force_eject() noexcept;

		constexpr const std::string_view& name() const noexcept { this->_ExtensionName; }
		ARTEMIS_FRAMEWORK HMODULE module_handle() const noexcept;
	};

#pragma endregion

#pragma region Class extension_manager

	class extension_manager : public API::loggable {
		std::vector<extension> _Loaded;

		ARTEMIS_FRAMEWORK void eject(std::vector<extension>::iterator _Iterator) noexcept;

	public:
		ARTEMIS_FRAMEWORK extension_manager() noexcept;

		ARTEMIS_FRAMEWORK extension& get(std::string_view&& _Name);
		ARTEMIS_FRAMEWORK extension& get(HMODULE _ModuleHandle);

		ARTEMIS_FRAMEWORK void load(const std::string_view& _Name) noexcept;
		ARTEMIS_FRAMEWORK void eject(const std::string_view& _Name);

		ARTEMIS_FRAMEWORK void eject_all() noexcept;
	};

#pragma endregion
}

#endif // !ARTEMIS_EXTENSION_HXX