#ifndef __MEMORY_PROTECT_MANAGER_H__
#define __MEMORY_PROTECT_MANAGER_H__

#include "..\Definitions.h"
#include "..\Interfaces.h"
#include "..\Memory.h"

namespace Artemis {
	class MemoryProtectManager : public IDisposable {
		typedef struct _REFLECTION {
			ADDRESS uAddress;
			DWORD dwSize;
			MemoryProtection nProtection;
		} REFLECTION, * LPREFLECTION;

		Memory* pMem;

		REFLECTION szPending[32];
		INT nPendingCount;

		REFLECTION szReflections[128];
		INT nReflectionCount;

		void AddReflection(_In_ REFLECTION Reflection);
		void ClearReflections();

	public:
		MemoryProtectManager(_In_z_ LPCSTR lpProcessName);
		MemoryProtectManager(_In_ Memory& TargetInstance);

		MemoryProtectManager(const MemoryProtectManager& cpy);

		~MemoryProtectManager();

		void Add(
			_In_ ADDRESS uAddress,
			_In_ DWORD dwSize,
			_In_ MemoryProtection nProtection
		);

		void Clear();
		void Apply();

		void Restore();

		void Release();
	};
}

#endif // !__MEMORY_PROTECT_MANAGER_H__