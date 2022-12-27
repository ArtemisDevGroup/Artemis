#include "MemoryProtectManager.h"

namespace Artemis {
	MemoryProtectManager::MemoryProtectManager(_In_z_ LPCSTR lpProcessName) : nPendingCount(0), nReflectionCount(0) {
		CONTEXT_BEGIN;

		memset(szPending, 0x00, sizeof(szPending));
		memset(szReflections, 0x00, sizeof(szReflections));

		pMem = new Memory(lpProcessName, nullptr);

		CONTEXT_END;
	}

	MemoryProtectManager::MemoryProtectManager(_In_ Memory& TargetInstance) : nPendingCount(0), nReflectionCount(0) {
		CONTEXT_BEGIN;

		memset(szPending, 0x00, sizeof(szPending));
		memset(szReflections, 0x00, sizeof(szReflections));

		pMem = new Memory(TargetInstance.GetProcessName(), nullptr);

		CONTEXT_END;
	}

	MemoryProtectManager::MemoryProtectManager(const MemoryProtectManager& cpy) : nPendingCount(cpy.nPendingCount), nReflectionCount(cpy.nReflectionCount) {
		CONTEXT_BEGIN;

		memcpy(szPending, cpy.szPending, sizeof(szPending));
		memcpy(szReflections, cpy.szReflections, sizeof(szReflections));

		this->pMem = new Memory(cpy.pMem->GetProcessName(), nullptr);

		CONTEXT_END;
	}

	MemoryProtectManager::~MemoryProtectManager() { Release(); }

	void MemoryProtectManager::AddReflection(_In_ REFLECTION Reflection) {
		CONTEXT_BEGIN;

		if (nReflectionCount >= ARRAYSIZE(szReflections)) throw IndexOutOfRangeException(nReflectionCount, ARRAYSIZE(szReflections) - 1);
		szReflections[nReflectionCount] = Reflection;
		nReflectionCount++;

		CONTEXT_END;
	}


	void MemoryProtectManager::ClearReflections() {
		memset(szReflections, 0x00, sizeof(szReflections));
		nReflectionCount = 0;
	}

	void MemoryProtectManager::Add(
		_In_ ADDRESS uAddress,
		_In_ DWORD dwSize,
		_In_ MemoryProtection nProtection
	) {
		CONTEXT_BEGIN;

		if (nPendingCount >= ARRAYSIZE(szPending)) throw IndexOutOfRangeException(nPendingCount, ARRAYSIZE(szPending) - 1);
		szReflections[nPendingCount] = { uAddress, dwSize, nProtection };
		nPendingCount++;

		CONTEXT_END;
	}

	void MemoryProtectManager::Clear() {
		memset(szPending, 0x00, sizeof(szPending));
		nReflectionCount = 0;
	}

	void MemoryProtectManager::Apply() {
		CONTEXT_BEGIN;

		MemoryProtection nOldProtect;

		for (INT i = 0; i < nPendingCount; i++) {
			pMem->VirtualProtect(
				szPending[i].uAddress,
				szPending[i].dwSize,
				szPending[i].nProtection,
				&nOldProtect
			);

			AddReflection({ szPending[i].uAddress, szPending[i].dwSize, nOldProtect });
		}

		Clear();

		CONTEXT_END;
	}

	void MemoryProtectManager::Restore() {
		CONTEXT_BEGIN;

		for (INT i = 0; i < nReflectionCount; i++) {
			pMem->VirtualProtect(
				szReflections[i].uAddress,
				szReflections[i].dwSize,
				szReflections[i].nProtection,
				nullptr
			);
		}

		ClearReflections();

		CONTEXT_END;
	}

	void MemoryProtectManager::Release() {
		Restore();
		delete pMem;
	}
}
