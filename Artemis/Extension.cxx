#include "pch.h"
#include "Extension.hxx"

using namespace std::literals::string_literals;

namespace Artemis {
#pragma region Class load_exception

	namespace API {
		load_exception::load_exception(std::string_view&& _Message) noexcept : exception(std::forward<std::string_view>(_Message)) {}
	}

#pragma endregion

#pragma region Class extension

	extension::~extension() {
		if (this->hModule)
			this->eject();
	}

	void extension::load() {
		__stack_record();

		this->hModule = LoadLibraryA(this->_ExtensionName.data());
		if (!this->hModule)
			throw API::load_exception("Failed to load extension dll.", API::win32_exception("LoadLibraryA"));

		aext_initialize_t aext_initialize = (aext_initialize_t)GetProcAddress(
			this->hModule,
			"aext_initialize"
		);

		if (!aext_initialize)
			throw API::load_exception("Function 'aext_initialize' is not found in module export table.", API::win32_exception("GetProcAddress"));

		bool status = false;

		[aext_initialize, &status]() -> void { // Work around C2712 "Cannot use __try in functions that require object unwinding"
			__try {
				[aext_initialize, &status]() -> void { // Work around C2713 "Only one form of exception handling permitted per function"
					try {
						status = aext_initialize();
					}
					catch (API::exception&) {
						API::call_stack_manager::global()->escape();
						throw;
					}
					catch (std::exception& e) {
						throw API::load_exception("C++ std exception thrown when initializing extension.", API::exception(e.what()));
					}
					catch (...) {
						throw API::load_exception("Unknown C++ exception thrown when initializing extension.");
					}
				}();
			}
			__except (API::seh_filter(GetExceptionInformation()).continue_on(EXCEPTION_CXX)) { // Ignores the exception if it is a C++ exception.
				[]() -> void { // Work around C2712 "Cannot use __try in functions that require object unwinding"
					throw API::load_exception("System exception thrown when initializing extension.", API::system_exception());
				}();
			}
		}();

		if (!status)
			throw API::load_exception("Function 'aext_initialize' failed.");

		__stack_escape();
	}

	void extension::eject() {
		__stack_record();

		if (!this->hModule)
			throw API::exception("No module is currently loaded for this instance.");

		aext_uninitialize_t aext_uninitialize = (aext_uninitialize_t)GetProcAddress(
			this->hModule,
			"aext_uninitialize"
		);

		if (!aext_uninitialize)
			throw API::load_exception("Function 'aext_uninitialize' is not found in module export table.", API::win32_exception("GetProcAddress"));

		[aext_uninitialize]() -> void { // Work around C2712 "Cannot use __try in functions that require object unwinding"
			__try {
				[aext_uninitialize]() -> void { // Work around C2713 "Only one form of exception handling permitted per function"
					try {
						aext_uninitialize();
					}
					catch (API::exception&) {
						API::call_stack_manager::global()->escape();
						throw;
					}
					catch (std::exception& e) {
						throw API::load_exception("C++ std exception thrown when uninitializing extension.", API::exception(e.what()));
					}
					catch (...) {
						throw API::load_exception("Unknown C++ exception thrown when uninitializing extension.");
					}
				}();
			}
			__except (API::seh_filter(GetExceptionInformation()).continue_on(EXCEPTION_CXX)) {
				[]() -> void { // Work around C2712 "Cannot use __try in functions that require object unwinding"
					throw API::load_exception("System exception thrown when uninitializing extension.", API::system_exception());
				}();
			}
		}();

		if (!FreeLibrary(this->hModule))
			throw API::load_exception("Failed to eject extension dll.", API::win32_exception("FreeLibrary"));

		this->hModule = nullptr;

		__stack_escape();
	}

	void extension::force_eject() noexcept {
		if (this->hModule) {
			FreeLibrary(this->hModule);
			this->hModule = nullptr;
		}
	}

	HMODULE extension::module_handle() const noexcept { return this->hModule; }

#pragma endregion

#pragma region Class extension_manager

	extension_manager::extension_manager() noexcept : _Loaded() {}

	extension& extension_manager::get(std::string_view&& _Name) {
		__stack_record();

		for (extension& e : this->_Loaded)
			if (e.name() == _Name)
				return e;
		throw API::argument_exception("No extension with provided name is loaded.", "_Name");

		__stack_escape();
	}

	extension& extension_manager::get(HMODULE _ModuleHandle) {
		__stack_record();

		for (extension& e : this->_Loaded)
			if (e.module_handle() == _ModuleHandle)
				return e;
		throw API::argument_exception("No extension with provided handle is loaded.", "_ModuleHandle");

		__stack_escape();
	}

	void extension_manager::load(const std::string_view& _Name) noexcept {
		this->Log->info("Attempting to load extension '"s + _Name.data() + "'");

		extension ex(_Name);

		try {
			ex.load();
			this->Log->success("Successfully loaded extension.");
			this->_Loaded.push_back(std::move(ex));
		}
		catch (const API::load_exception& e) {
			this->Log->error("Failed loading extension: "s + e.what());
			this->Log->info("Attempting to eject extension module...");

			try {
				ex.eject();
				this->Log->success("Successfully ejected extension.");
			}
			catch (const API::load_exception& e) {
				this->Log->error("Failed ejecting extension: "s + e.what());
				this->Log->info("Force ejecting extension module...");
				ex.force_eject();
			}
		}
	}

	void extension_manager::eject(std::vector<extension>::iterator _Iterator) noexcept {
		this->Log->info("Attempting to eject extension '"s + _Iterator->name().data() + "'");

		try {
			_Iterator->eject();
			this->Log->success("Successfully ejected extension.");
		}
		catch (const API::load_exception& e) {
			this->Log->error("Failed ejecting extension: "s + e.what());
			this->Log->info("Force ejecting extension module...");
			_Iterator->force_eject();
		}

		this->_Loaded.erase(_Iterator);
	}

	void extension_manager::eject(const std::string_view& _Name) {
		for (auto it = this->_Loaded.begin(); it != this->_Loaded.end(); ++it)
			if (it->name() == _Name) {
				this->eject(it);
				break;
			}
	}

	void extension_manager::eject_all() noexcept {
		while (this->_Loaded.size() > 0)
			this->eject(std::prev(this->_Loaded.end()));
	}

#pragma endregion
}