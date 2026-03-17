#ifndef __ARTEMIS_EXTENSION_HXX__
#define __ARTEMIS_EXTENSION_HXX__

#include "Definitions.hxx"

#include "API/Exception.hxx"
#include "API/Logging.hxx"

#include <string>			// std::string
#include <string_view>		// std::string_view
#include <unordered_map>	// std::unordered_map
#include <concepts>			// std::derived_from
#include <type_traits>		// std::remove_reference_t

#include <Windows.h>		// HMODULE, DWORD

namespace Artemis {
	namespace API {
		class load_exception : public exception {
		public:
			ARTEMIS_API load_exception(std::string_view _Message) noexcept;

			template<typename _Ty>
				requires std::derived_from<std::remove_reference_t<_Ty>, exception>
			inline load_exception(std::string_view _Message, _Ty&& _InnerException) noexcept : exception(_Message, std::forward<_Ty>(_InnerException)) {}
		};
	}

	using aext_initialize_t = bool(__stdcall*)(void);	// extern "C" __declspec(dllexport) bool __stdcall aext_initialize(void);
	using aext_uninitialize_t = void(__stdcall*)(void);	// extern "C" __declspec(dllexport) void __stdcall aext_uninitialize(void);

	class extension {
		std::string _Name;
		HMODULE hModule;

	public:
		ARTEMIS_FRAMEWORK extension(std::string_view _Name);

		extension(const extension&) = delete;
		ARTEMIS_FRAMEWORK extension(extension&& _Other) noexcept;

		ARTEMIS_FRAMEWORK ~extension() noexcept;

		ARTEMIS_FRAMEWORK void load();
		ARTEMIS_FRAMEWORK void eject();
		ARTEMIS_FRAMEWORK void force_eject() noexcept;

		ARTEMIS_FRAMEWORK const std::string& name() const noexcept;
		ARTEMIS_FRAMEWORK HMODULE handle() const noexcept;

		extension& operator=(const extension&) = delete;

		ARTEMIS_FRAMEWORK extension& operator=(extension&&) noexcept;
	};

	class extension_manager : public API::loggable {
		std::vector<extension*> _Loaded;

		ARTEMIS_FRAMEWORK void eject(std::vector<extension*>::iterator _Iterator) noexcept;

	public:
		ARTEMIS_FRAMEWORK extension_manager() noexcept;

		extension_manager(const extension_manager&) = delete;
		extension_manager(extension_manager&&) = delete;

		ARTEMIS_FRAMEWORK ~extension_manager() noexcept;

		ARTEMIS_FRAMEWORK extension* get(std::string_view _Name);
		ARTEMIS_FRAMEWORK extension* get(HMODULE _ModuleHandle);

		ARTEMIS_FRAMEWORK void load(std::string_view _Name) noexcept;
		ARTEMIS_FRAMEWORK void eject(std::string_view _Name);

		ARTEMIS_FRAMEWORK void eject_all() noexcept;

		extension_manager& operator=(const extension_manager&) = delete;
		extension_manager& operator=(extension_manager&&) = delete;
	};
}

#endif // !__ARTEMIS_EXTENSION_HXX__