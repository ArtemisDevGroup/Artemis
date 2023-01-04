//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Astrea0014.												|
// @Astrea0014: https://github.com/Astrea0014											|
//-------------------------------------------------------------------------------------->

#include "MemoryAllocation.h"
#include "Exceptions.h"

#include <Windows.h>

namespace Artemis {
	//---------------------------------------------------------------------->
	// Constructors and destructor:
	VirtualAllocation::VirtualAllocation() : hProcess(nullptr), uAddress(0), uSize(0), Type(), Protection(), DisposalType(), uAllocatedAddress(0) {}
	//-------------------------------------//
	VirtualAllocation::~VirtualAllocation() { Release(); }
	//-------------------------------------//
	VirtualAllocation::VirtualAllocation(
		_In_ ADDRESS uAddress,
		_In_ SIZE_T uSize,
		_In_ AllocationType Type,
		_In_ MemoryProtection Protection,
		_In_ FreeType DisposalType
	) : hProcess(nullptr), uAddress(uAddress), uSize(uSize), Type(Type), Protection(Protection), DisposalType(DisposalType), uAllocatedAddress(0) {}
	//---------------------------------------------------------------------->
	// Setters:
	void VirtualAllocation::SetExternalHandle(_In_ HANDLE hProcess) { this->hProcess = hProcess; }
	//---------------------------------------------------------------------->
	// Public methods:
	void VirtualAllocation::Allocate() {
		CONTEXT_BEGIN;

		if (!this->uAllocatedAddress) {
			if (this->hProcess) {
				if ((this->uAllocatedAddress = (ADDRESS)VirtualAllocEx(
					this->hProcess,
					(LPVOID)this->uAddress,
					this->uSize,
					(DWORD)this->Type,
					(DWORD)this->Protection
				)) == 0) throw WindowsApiException("VirtualAllocEx");
			}
			else {
				if ((this->uAllocatedAddress = (ADDRESS)VirtualAlloc(
					(LPVOID)this->uAddress,
					this->uSize,
					(DWORD)this->Type,
					(DWORD)this->Protection
				)) == 0) throw WindowsApiException("VirtualAlloc");
			}
		}

		CONTEXT_END;
	}
	//-------------------------------------//
#pragma warning(push)
#pragma warning(disable:28160)
	void VirtualAllocation::Release() {
		CONTEXT_BEGIN;

		if (this->uAllocatedAddress) {
			if (this->hProcess) {
				if (!VirtualFreeEx(
					this->hProcess,
					(LPVOID)uAllocatedAddress,
					DisposalType == FreeType::Decommit ? uSize : 0,
					(DWORD)DisposalType
				)) throw WindowsApiException("VirtualFreeEx");
			}
			else {
				if (!VirtualFree(
					(LPVOID)uAllocatedAddress,
					DisposalType == FreeType::Decommit ? uSize : 0,
					(DWORD)DisposalType
				)) throw WindowsApiException("VirtualFree");
			}

			this->uAllocatedAddress = 0;
		}

		CONTEXT_END;
	}
#pragma warning(pop)
	//-------------------------------------//
	void VirtualAllocation::Read(
		_In_ INT nIndex,
		_Out_ LPVOID lpBuffer,
		_In_ DWORD dwSize
	) const {
		CONTEXT_BEGIN;

		if (!lpBuffer) throw ParameterException("lpBuffer");

		if (this->hProcess) {
			if (!ReadProcessMemory(
				this->hProcess,
				(LPCVOID)(GetAllocatedAddress() + nIndex),
				lpBuffer,
				dwSize,
				nullptr
			)) throw WindowsApiException("ReadProcessMemory");
		}
		else {
			__try {
				for (UINT i = 0; i < dwSize; i++) {
					*(LPBYTE)((ADDRESS)lpBuffer + i) = *(LPBYTE)(GetAllocatedAddress() + nIndex + i);
				}
			}
			__except (EXCEPTION_EXECUTE_HANDLER) { throw MemoryAccessViolationException(GetAllocatedAddress(), dwSize, OperationType::Read); }
		}

		CONTEXT_END;
	}
	//-------------------------------------//
	void VirtualAllocation::Write(
		_In_ INT nIndex,
		_In_ LPCVOID lpData,
		_In_ DWORD dwSize
	) const {
		CONTEXT_BEGIN;

		if (!lpData) throw ParameterException("lpData");

		if (this->hProcess) {
			if (!WriteProcessMemory(
				this->hProcess,
				(LPVOID)(GetAllocatedAddress() + nIndex),
				lpData,
				dwSize,
				nullptr
			)) throw WindowsApiException("WriteProcessMemory");
		}
		else {
			__try {
				for (UINT i = 0; i < dwSize; i++) {
					*(LPBYTE)(GetAllocatedAddress() + nIndex + i) = *(LPBYTE)((ADDRESS)lpData + i);
				}
			}
			__except (EXCEPTION_EXECUTE_HANDLER) { throw MemoryAccessViolationException(GetAllocatedAddress(), dwSize, OperationType::Write); }
		}

		CONTEXT_END;
	}
	//---------------------------------------------------------------------->
	// Getters:
	_Check_return_ ADDRESS VirtualAllocation::GetAllocatedAddress() const { return this->uAllocatedAddress; }
	//---------------------------------------------------------------------->
}