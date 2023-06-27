#ifndef __ARTEMIS_EXTENSION_MANAGER_H__
#define __ARTEMIS_EXTENSION_MANAGER_H__

#include <sal.h>
#undef LoadLibrary

#include "Definitions.h"

namespace Artemis {
	struct ArtemisExtensionInfo {
		char szName[64];
		char szVersion[16];
		char szAuthor[64];
		char szDescription[1024];
		bool bDebug;
	};

	class Extension {
		typedef bool(__cdecl* LoadTemplate)();
		typedef void(__cdecl* ReleaseTemplate)();
		typedef void(__cdecl* GetExtensionInfoTemplate)(_Out_ ArtemisExtensionInfo* pExtensionInfo);

		LoadTemplate fnLoad;
		ReleaseTemplate fnRelease;
		GetExtensionInfoTemplate fnGetExtensionInfo;

		HMODULE hModule;
		bool bStatus;

		ArtemisExtensionInfo ExtensionInfo;

	public:
		Extension(_In_ HMODULE hModule);

		const ArtemisExtensionInfo& GetInfo() const;
		HMODULE GetModuleHandle() const;
		bool GetStatus() const;

		void LoadInfo();
		bool LoadExtension();
		void Release();
	};

	class ExtensionManager {
		Extension* ExtensionCollection[MAX_INVOKE];

	public:
		ExtensionManager();
		~ExtensionManager();

		Extension* Load(_In_z_ const char* lpExtensionFilePath);
		void Release(_Post_invalid_ _In_opt_ Extension* pExtension = nullptr);
	};
}

#endif // !__ARTEMIS_EXTENSION_MANAGER_H__