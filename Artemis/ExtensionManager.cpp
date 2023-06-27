#include "pch.h"
#include "ExtensionManager.h"

#include "External.h"

namespace Artemis {
	Extension::Extension(_In_ HMODULE hModule) : hModule(hModule) {
		ZeroMemory(&ExtensionInfo, sizeof(ExtensionInfo));

		bStatus = true;

		fnGetExtensionInfo = (GetExtensionInfoTemplate)GetProcAddress(hModule, "GetExtensionInfo");
		if (!fnGetExtensionInfo) {
			bStatus = false;
			return;
		}

		fnLoad = (LoadTemplate)GetProcAddress(hModule, "Load");
		if (!fnLoad) {
			bStatus = false;
			return;
		}

		fnRelease = (ReleaseTemplate)GetProcAddress(hModule, "Release");
		if (!fnRelease) {
			bStatus = false;
			return;
		}
	}

	const ArtemisExtensionInfo& Extension::GetInfo() const { return ExtensionInfo; }
	HMODULE Extension::GetModuleHandle() const { return hModule; }
	bool Extension::GetStatus() const { return bStatus; }

	void Extension::LoadInfo() { if (fnGetExtensionInfo) fnGetExtensionInfo(&ExtensionInfo); }

	bool Extension::LoadExtension() { return fnLoad ? fnLoad() : false; }

	void Extension::Release() { if (fnRelease) fnRelease(); }

	ExtensionManager::ExtensionManager() { ZeroMemory(ExtensionCollection, sizeof(ExtensionCollection)); }
	ExtensionManager::~ExtensionManager() { this->Release(); }

	Extension* ExtensionManager::Load(_In_z_ const char* lpExtensionFilePath) {
		AuroraContextStart();

		HMODULE hModule = LoadLibraryA(lpExtensionFilePath);
		if (!hModule) AuroraThrow(Aurora::WindowsApiException, "LoadLibraryA");

		Extension* pExtension = new Extension(hModule);

		for (int i = 0; i < MAX_INVOKE; i++)
			if (!ExtensionCollection[i])
				ExtensionCollection[i] = pExtension;

		AuroraContextEnd();
		return pExtension;
	}

	void ExtensionManager::Release(_Post_invalid_ _In_opt_ Extension* pExtension) {
		AuroraContextStart();

		if (pExtension) {
			for (int i = 0; i < MAX_INVOKE; i++)
				if (ExtensionCollection[i] == pExtension) {
					pExtension->Release();
					FreeLibrary(pExtension->GetModuleHandle());

					delete pExtension;
					ExtensionCollection[i] = nullptr;
				}
		}
		else {
			for (Extension* pExtension : ExtensionCollection)
				if (pExtension) {
					pExtension->Release();
					FreeLibrary(pExtension->GetModuleHandle());

					delete pExtension;
				}
			ZeroMemory(ExtensionCollection, sizeof(ExtensionCollection));
		}

		AuroraContextEnd();
	}
}