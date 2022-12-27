#ifndef __MEMORY_PROTECT_MANAGER_H__
#define __MEMORY_PROTECT_MANAGER_H__

#include "..\Definitions.h"
#include "..\Interfaces.h"
#include "..\Memory.h"

namespace Artemis {
	/// <summary>
	/// A class for changing the protection of memory inside the game allocation.
	/// </summary>
	class ARTEMIS_API MemoryProtectManager : public IDisposable {
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
		/// <summary>
		/// Constructs a MemoryProtectManager class.
		/// </summary>
		/// <param name="lpProcessName">- The name of the target process.</param>
		/// <exception cref="ObjectNotFoundException"/>
		/// <exception cref="WindowsApiException"/>
		MemoryProtectManager(_In_z_ LPCSTR lpProcessName);

		/// <summary>
		/// Constructs a MemoryProtectManager class.
		/// </summary>
		/// <param name="TargetInstance">- A reference to a Memory object associated with the target process.</param>
		/// <exception cref="ObjectNotFoundException"/>
		/// <exception cref="WindowsApiException"/>
		MemoryProtectManager(_In_ Memory& TargetInstance);

		MemoryProtectManager(const MemoryProtectManager& cpy);
		~MemoryProtectManager();

		/// <summary>
		/// Adds a memory protection change to the queue.
		/// </summary>
		/// <param name="uAddress">- The address of the memory to change.</param>
		/// <param name="dwSize">- The size of the region to change.</param>
		/// <param name="nProtection">- The new memory protection.</param>
		/// <exception cref="IndexOutOfRangeException"/>
		void Add(
			_In_ ADDRESS uAddress,
			_In_ DWORD dwSize,
			_In_ MemoryProtection nProtection
		);

		/// <summary>
		/// Clears the current queue.
		/// </summary>
		void Clear();

		/// <summary>
		/// Applies the queued changes.
		/// </summary>
		/// <exception cref="IndexOutOfRangeException"/>
		/// <exception cref="WindowsApiException"/>
		void Apply();

		/// <summary>
		/// Restores the old memory protections of all pages changed with this instance.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Restore();

		/// <summary>
		/// Releases the instance.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Release();
	};
}

#endif // !__MEMORY_PROTECT_MANAGER_H__