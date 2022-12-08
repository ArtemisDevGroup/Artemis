//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#include "MemoryHook.h"
#include "Exceptions.h"

#include "MinHook/MinHook.h"

namespace Artemis {
	//---------------------------------------------------------------------->
	// Hook
	//---------------------------------------------------------------------->
	// Public static methods:
	void Hook::InitializeLibrary() {
		CONTEXT_BEGIN;

		switch (MH_Initialize()) {
		case MH_ERROR_MEMORY_ALLOC:
			throw HookException::Create::MemoryAlloc();
		case MH_ERROR_ALREADY_INITIALIZED:
			throw HookException::Create::AlreadyInitialized();
		case MH_OK:
			return;
		}

		CONTEXT_END;
	}
	//-------------------------------------//
	void Hook::UninitializeLibrary() {
		CONTEXT_BEGIN;

		switch (MH_Uninitialize()) {
		case MH_ERROR_NOT_INITIALIZED:
			throw HookException::Create::NotInitialized();
		case MH_OK:
			return;
		}

		CONTEXT_END;
	}
	//---------------------------------------------------------------------->
	// Constructors and destructor:
	Hook::Hook() : lpTarget(nullptr) {}
	//-------------------------------------//
	Hook::Hook(
		_In_z_ LPCSTR lpModule,
		_In_z_ LPCSTR lpFunction,
		_In_ LPVOID lpDetour,
		_Out_opt_ LPVOID* lpOriginal
	) {
		CONTEXT_BEGIN;

		switch (MH_CreateHookApiEx(lpModule, lpFunction, lpDetour, lpOriginal, &this->lpTarget)) {
		case MH_ERROR_MODULE_NOT_FOUND:
			throw HookException::Create::ModuleNotFound();
		case MH_ERROR_FUNCTION_NOT_FOUND:
			throw HookException::Create::FunctionNotFound();
		case MH_ERROR_MEMORY_ALLOC:
			throw HookException::Create::MemoryAlloc();
		case MH_ERROR_UNSUPPORTED_FUNCTION:
			throw HookException::Create::UnsupportedFunction();
		case MH_ERROR_ALREADY_CREATED:
			throw HookException::Create::AlreadyCreated();
		case MH_ERROR_NOT_EXECUTABLE:
			throw HookException::Create::NotExecutable();
		case MH_ERROR_NOT_INITIALIZED:
			throw HookException::Create::NotInitialized();
		case MH_OK:
			return;
		}

		CONTEXT_END;
	}
	//-------------------------------------//
	Hook::Hook(
		_In_ HMODULE lpModule,
		_In_z_ LPCSTR lpFunction,
		_In_ LPVOID lpDetour,
		_Out_opt_ LPVOID* lpOriginal
	) {
		CONTEXT_BEGIN;

		switch (MH_CreateHookApiExH(lpModule, lpFunction, lpDetour, lpOriginal, &this->lpTarget)) {
		case MH_ERROR_MODULE_NOT_FOUND:
			throw HookException::Create::ModuleNotFound();
		case MH_ERROR_FUNCTION_NOT_FOUND:
			throw HookException::Create::FunctionNotFound();
		case MH_ERROR_MEMORY_ALLOC:
			throw HookException::Create::MemoryAlloc();
		case MH_ERROR_UNSUPPORTED_FUNCTION:
			throw HookException::Create::UnsupportedFunction();
		case MH_ERROR_ALREADY_CREATED:
			throw HookException::Create::AlreadyCreated();
		case MH_ERROR_NOT_EXECUTABLE:
			throw HookException::Create::NotExecutable();
		case MH_ERROR_NOT_INITIALIZED:
			throw HookException::Create::NotInitialized();
		case MH_OK:
			return;
		}

		CONTEXT_END;
	}
	//-------------------------------------//
	Hook::Hook(
		_In_ LPVOID lpTarget,
		_In_ LPVOID lpDetour,
		_Out_opt_ LPVOID* lpOriginal
	) : lpTarget(lpTarget) {
		CONTEXT_BEGIN;

		switch (MH_CreateHook(lpTarget, lpDetour, lpOriginal)) {
		case MH_ERROR_MEMORY_ALLOC:
			throw HookException::Create::MemoryAlloc();
		case MH_ERROR_UNSUPPORTED_FUNCTION:
			throw HookException::Create::UnsupportedFunction();
		case MH_ERROR_ALREADY_CREATED:
			throw HookException::Create::AlreadyCreated();
		case MH_ERROR_NOT_EXECUTABLE:
			throw HookException::Create::NotExecutable();
		case MH_ERROR_NOT_INITIALIZED:
			throw HookException::Create::NotInitialized();
		case MH_OK:
			return;
		}

		CONTEXT_END;
	}
	//-------------------------------------//
	Hook::Hook(
		_In_ ADDRESS uTarget,
		_In_ LPVOID lpDetour,
		_Out_opt_ LPVOID* lpOriginal
	) : lpTarget((LPVOID)uTarget) {
		CONTEXT_BEGIN;

		switch (MH_CreateHook((LPVOID)uTarget, lpDetour, lpOriginal)) {
		case MH_ERROR_MEMORY_ALLOC:
			throw HookException::Create::MemoryAlloc();
		case MH_ERROR_UNSUPPORTED_FUNCTION:
			throw HookException::Create::UnsupportedFunction();
		case MH_ERROR_ALREADY_CREATED:
			throw HookException::Create::AlreadyCreated();
		case MH_ERROR_NOT_EXECUTABLE:
			throw HookException::Create::NotExecutable();
		case MH_ERROR_NOT_INITIALIZED:
			throw HookException::Create::NotInitialized();
		case MH_OK:
			return;
		}

		CONTEXT_END;
	}
	//---------------------------------------------------------------------->
	// Public methods:
	void Hook::Enable() const {
		CONTEXT_BEGIN;

		switch (MH_EnableHook(lpTarget)) {
		case MH_ERROR_MEMORY_PROTECT:
			throw HookException::Create::MemoryProtect();
		case MH_ERROR_MEMORY_ALLOC:
			throw HookException::Create::MemoryAlloc();
		case MH_ERROR_ENABLED:
			throw HookException::Create::AlreadyEnabled();
		case MH_ERROR_NOT_CREATED:
			throw HookException::Create::NotCreated();
		case MH_ERROR_NOT_INITIALIZED:
			throw HookException::Create::NotInitialized();
		case MH_OK:
			return;
		}

		CONTEXT_END;
	}
	//-------------------------------------//
	void Hook::Disable() const {
		CONTEXT_BEGIN;

		switch (MH_DisableHook(lpTarget)) {
		case MH_ERROR_MEMORY_PROTECT:
			throw HookException::Create::MemoryProtect();
		case MH_ERROR_MEMORY_ALLOC:
			throw HookException::Create::MemoryAlloc();
		case MH_ERROR_DISABLED:
			throw HookException::Create::NotEnabled();
		case MH_ERROR_NOT_CREATED:
			throw HookException::Create::NotCreated();
		case MH_ERROR_NOT_INITIALIZED:
			throw HookException::Create::NotInitialized();
		case MH_OK:
			return;
		}

		CONTEXT_END;
	}
	//-------------------------------------//
	void Hook::Release() {
		CONTEXT_BEGIN;

		switch (MH_RemoveHook(lpTarget)) {
		case MH_ERROR_MEMORY_PROTECT:
			throw HookException::Create::MemoryProtect();
		case MH_ERROR_MEMORY_ALLOC:
			throw HookException::Create::MemoryAlloc();
		case MH_ERROR_NOT_CREATED:
			throw HookException::Create::NotCreated();
		case MH_ERROR_NOT_INITIALIZED:
			throw HookException::Create::NotInitialized();
		case MH_OK:
			return;
		}

		CONTEXT_END;
	}
}