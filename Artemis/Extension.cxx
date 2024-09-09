#include "pch.h"
#include "Extension.hxx"

#include "ExceptionFrames.hxx"
#include "ExecutionContext.hxx"

namespace Artemis {
	namespace API {
		load_exception::load_exception(std::string_view&& _Message) noexcept : exception(std::forward<std::string_view>(_Message)) {}
	}

	extension::~extension() noexcept {
		if (this->hModule) {
			try {
				this->eject();
			}
			catch (...) {
				this->force_eject();
			}
		}

		_::__execution_context::invalidate(this);
	}

	void extension::load() {
		__stack_record();

		_::__execution_context::set(this);

		this->hModule = LoadLibraryA(this->_Name.data());
		if (!this->hModule)
			throw API::load_exception("Failed to load extension dll.", API::win32_exception("LoadLibraryA"));

		aext_initialize_t aext_initialize = (aext_initialize_t)GetProcAddress(
			this->hModule,
			"aext_initialize"
		);

		if (!aext_initialize)
			throw API::load_exception("Function 'aext_initialize' is not found in module export table.", API::win32_exception("GetProcAddress"));

		bool status = false;

		_::__safe_exception_propagator().exec_l2([aext_initialize, &status]() { status = aext_initialize(); });

		if (!status)
			throw API::load_exception("Function 'aext_initialize' failed.");

		_::__execution_context::set(nullptr);

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

		_::__safe_exception_propagator().exec_l2(aext_uninitialize);

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

	extension_manager::extension_manager() noexcept : _Loaded() {}

	extension_manager::~extension_manager() noexcept {
		for (extension* ext : this->_Loaded)
			delete ext;
		this->_Loaded.clear();
	}

	extension* extension_manager::get(std::string_view&& _Name) {
		__stack_record();

		for (extension* e : this->_Loaded)
			if (e->name() == _Name)
				return e;
		throw API::argument_exception("No extension with provided name is loaded.", "_Name");

		__stack_escape();
	}

	extension* extension_manager::get(HMODULE _ModuleHandle) {
		__stack_record();

		for (extension* e : this->_Loaded)
			if (e->handle() == _ModuleHandle)
				return e;
		throw API::argument_exception("No extension with provided handle is loaded.", "_ModuleHandle");

		__stack_escape();
	}

	void extension_manager::load(const std::string_view& _Name) noexcept {
		this->Log->info(std::format("Attempting to load extension '{}'", _Name));

		extension* ex = new extension(_Name);

		try {
			ex->load();
			this->Log->success("Successfully loaded extension.");
			this->_Loaded.push_back(ex);
		}
		catch (const API::exception& e) {
			this->Log->error(std::format("Failed loading extension: {}", e.what()));
			this->Log->info("Attempting to eject extension module...");

			try {
				ex->eject();
				delete ex;
				this->Log->success("Successfully ejected extension.");
			}
			catch (const API::exception& e) {
				this->Log->error(std::format("Failed ejecting extension: {}", e.what()));
				this->Log->info("Force ejecting extension module...");
				ex->force_eject();
				delete ex;
			}
		}
	}

	void extension_manager::eject(std::vector<extension*>::iterator _Iterator) noexcept {
		this->Log->info(std::format("Attempting to eject extension '{}'", (*_Iterator)->name()));

		try {
			(*_Iterator)->eject();
			this->Log->success("Successfully ejected extension.");
		}
		catch (const API::load_exception& e) {
			this->Log->error(std::format("Failed ejecting extension: {}", e.what()));
			this->Log->info("Force ejecting extension module...");
			(*_Iterator)->force_eject();
		}

		delete (*_Iterator);
		this->_Loaded.erase(_Iterator);
	}

	void extension_manager::eject(const std::string_view& _Name) {
		for (auto it = this->_Loaded.begin(); it != this->_Loaded.end(); ++it)
			if ((*it)->name() == _Name) {
				this->eject(it);
				break;
			}
	}

	void extension_manager::eject_all() noexcept {
		while (this->_Loaded.size() > 0)
			this->eject(std::prev(this->_Loaded.end()));
	}
}