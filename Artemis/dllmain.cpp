#include "pch.h"

#include "External.h"
#include "ExtensionManager.h"

#include "PresentHook.h"
#include "EventEntries.h"
#include "Keybinds.h"
#include "Windows.h"

#include "Events.h"

using namespace Artemis;

const char* const lpszAsciiArt[13] = {
		"<------------------------------------------------------------------------------------------------------>",
		"|                 .........     .......................   ....              ....   ....       ...      |",
		"|       ,d'      .xKKKKKKK0ko, .oKKKKKKKKKKxoOKKKKKK0Kx. .oKKO:           .l0KKc  .xKKo.   ,ok0K0Od:.  |",
		"|      .OWx.     .oOOOOOOOXWMNo.:xxxxxxxxxxloXMWXOOOkOo. .kMMMX:          lNMMMo  '0MMx. .dNMN0O0NMWk. |",
		"|     .dWMNl              .xWMX;            :NMWl        .kMMMMX:        cNMMMMo  '0MMx. cWMNl. .;kK0: |",
		"|     cNMMMX:            .'kWMX;    '::,.   :NMWo....... .kMMMMMK;      :XMMMMMo  '0MMx. cNMNo..   ..  |",
		"|    ;KMX0NM0'   .oOOOOOO0XWMXl.   .dMM0'   :NMMX0000000OoOMMXKWMK;    :XMWXNMMo  '0MMx. .dNMWX0Oxo,   |",
		"|   .OMWo.xWWk.  .OMMNKXWMMXd'      dMM0'   :NMMX0OOO000koOMMk;xWMK;  ;KMWdcKMMo  '0MMx.   'lxkOKWMNx. |",
		"|  .xWMO. '0MWo. 'OMMk'.dWMNc       dMM0'   :NMWo.       .kMMO..xWMK;;KMWx.,KMMo  '0MMx.        .lXMWo |",
		"|  lNMX;   cXMNc 'OMMx. .dWMXc      dMM0'   :NMWo        .kMMk. .xWMXXMWx. .coo,  '0MMx.        .cXMWo |",
		"| :XMWo    .dWMK,'OMMx.  .xWMX:     dMM0'   :NMMXOOOOOd. .kMMO.  .xWMMWx.       'dONMMXkl.     ,ONMWk' |",
		"| dKKd.     .xKKo,dKKo.   .xKKx'    lKKx.   ,OKKKKKKKKx. .oKKd.   .cooc.        ,OKKKKKKx.     ;0Od:.  |",
		"<------------------------------------------------------------------------------------------------------>"
};

static bool bRunning = true;
static ExtensionManager Extensions;
static PresentHook* pHook = nullptr;

ARTEMIS_API void Artemis::Exit() { bRunning = false; }

void LogBasicInformation(const char* lpSender, const Aurora::ProcessInfo& CurrentProcess) {
	Log.LogInfo(lpSender, "Welcome to Artemis!");
	Log.LogInfo(lpSender, "Current process:");
	Log.LogInfo(lpSender, "\t%s : %s", CurrentProcess.GetProcessName(), CurrentProcess.GetProcessPath());
	Log.LogInfo(lpSender, "Process Id: 0x%lX", CurrentProcess.GetProcessId());
	Log.LogInfo(lpSender, "Process Handle: 0x%p\n", CurrentProcess.GetProcessHandle());

	const Aurora::ModuleInfo& Module = CurrentProcess.GetModule();

	Log.LogInfo(lpSender, "Module handle: 0x%p", Module.GetModuleHandle());
	Log.LogInfo(lpSender, "Module base address: 0x%llX", Module.GetModuleBaseAddress());
	Log.LogInfo(lpSender, "Module size: 0x%lX", Module.GetModuleSize());
}

void GetFileExtension(
	_In_z_ const char* lpFileName,
	_Out_writes_z_(nSize) char* lpBuffer,
	_In_ int nSize
) {
	ZeroMemory(lpBuffer, nSize);

	int nLen = lstrlenA(lpFileName);

	for (int i = nLen - 1; i >= 0; i--)
		if (lpFileName[i] == '.') {
			for (int j = 0; j < nLen - i; j++)
				lpBuffer[j] = lpFileName[i + j];
			break;
		}
}

void LoadAllExtensions(const char* lpSender) {
	WIN32_FIND_DATAA FindData;

	CreateDirectoryA("ArtemisExtensions", nullptr);
	HANDLE hFind = FindFirstFileA(
		"ArtemisExtensions\\*",
		&FindData
	);

	if (!hFind) {
		Log.LogError(lpSender, "FindFirstFileA failed: %s", Aurora::WindowsApiException("FindFirstFileA").GetWindowsMessage());
		return;
	}

	do {
		char szExtension[16];
		GetFileExtension(FindData.cFileName, szExtension, sizeof(szExtension));

		if (strcmp(szExtension, ".dll")) continue;

		Extension* pExtension = nullptr;
		try {
			char szPath[64] = "ArtemisExtensions\\";
			strcat_s(szPath, FindData.cFileName);

			pExtension = Extensions.Load(szPath);
		}
		AuroraCatch(Aurora::WindowsApiException) {
			Log.LogError(lpSender, "Failed to load extension library %s:", FindData.cFileName);
			Log.LogError(lpSender, "\t%s: %s", e.GetWindowsFunction(), e.GetWindowsMessage());
			continue;
		}

		if (!pExtension->GetStatus()) {
			Log.LogError(lpSender, "Failed to get function addresses.");
			Extensions.Release(pExtension);
			continue;
		}

		pExtension->LoadInfo();

		const ArtemisExtensionInfo& Info = pExtension->GetInfo();

		Log.LogInfo(lpSender, "Extension '%s' library loaded:", Info.szName);
		Log.LogInfo(lpSender, "Version: %s", Info.szVersion);
		Log.LogInfo(lpSender, "Author: %s", Info.szAuthor);
		Log.LogInfo(lpSender, "Description:\n%s", Info.szDescription);
		Log.LogInfo(lpSender, "Debug: %s", Info.bDebug ? "true" : "false");

		try {
			if (pExtension->LoadExtension()) Log.LogSuccess(lpSender, "Successfully loaded extension %s", Info.szName);
			else {
				Log.LogError(lpSender, "Failed to load extension %s. Releasing library...", Info.szName);
				Extensions.Release(pExtension);
			}
		}
		AuroraCatch(Aurora::WindowsApiException) {
			Log.LogError(lpSender, "Failed to release extension library %s:", FindData.cFileName);
			Log.LogError(lpSender, "\t%s: %s", e.GetWindowsFunction(), e.GetWindowsMessage());
			continue;
		}
	} while (FindNextFileA(hFind, &FindData));

	FindClose(hFind);
}

DWORD APIENTRY Main(HMODULE hModule) {
#ifdef _DEBUG
	Aurora::CreateConsole();
	Aurora::OpenStream(Aurora::StandardStream::Out);
#endif // _DEBUG

	for (const char* lpString : lpszAsciiArt)
		printf("%s\n", lpString);

	LogBasicInformation(__FUNCTION__, Aurora::GetCurrentProcessInfo());

	if (Keybinds.Add(new ExitKeybind()) == INVALID_INDEX) {
		Log.LogError(__FUNCTION__, "Exit keybind could not be added.");
		bRunning = false;
	}
	else Log.LogSuccess(__FUNCTION__, "Successfully registered the exit keybind.");

	if (Windows.Add(new MainWindow()) == INVALID_INDEX) Log.LogError(__FUNCTION__, "Main window could not be added.");
	else Log.LogSuccess(__FUNCTION__, "Successfully registered the main window.");

	if (EventEntries.Add(new EnterMainMenuEventEntry()) == INVALID_INDEX) Log.LogError(__FUNCTION__, "Enter main menu event entry could not be added.");
	else Log.LogSuccess(__FUNCTION__, "Successfully registered the enter main menu event.");
	if (EventEntries.Add(new EnterCustomGameLobbyEventEntry()) == INVALID_INDEX) Log.LogError(__FUNCTION__, "Enter custom game lobby event entry could not be added.");
	else Log.LogSuccess(__FUNCTION__, "Successfully registered the enter custom game event.");
	if (EventEntries.Add(new EnterPickPhaseEventEntry()) == INVALID_INDEX) Log.LogError(__FUNCTION__, "Enter pick phase event entry could not be added.");
	else Log.LogSuccess(__FUNCTION__, "Successfully registered the enter pick phase event.");
	if (EventEntries.Add(new EnterGameEventEntry()) == INVALID_INDEX) Log.LogError(__FUNCTION__, "Enter game event entry could not be added.");
	else Log.LogSuccess(__FUNCTION__, "Successfully registered the enter game event.");

	MH_STATUS status = MH_Initialize();
	if (status != MH_OK) {
		Log.LogError(__FUNCTION__, "Failed to initialize minhook: %s", MH_StatusToString(status));
		bRunning = false;
	}
	else {
		Log.LogSuccess(__FUNCTION__, "Successfully initialized MinHook.");
		pHook = PresentHook::Create();
		pHook->Enable();
	}

	LoadAllExtensions(__FUNCTION__);

	while (bRunning) Keybinds.Invoke();

	if (pHook) pHook->Release();
	MH_Uninitialize();

#ifdef _DEBUG
	Aurora::CloseStream(Aurora::StandardStream::Out);
	Aurora::ReleaseConsole();
#endif // _DEBUG

	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved) {
	HANDLE hThread = nullptr;

	switch (dwReasonForCall) {
	case DLL_PROCESS_ATTACH:
		hThread = CreateThread(
			nullptr,
			0,
			(LPTHREAD_START_ROUTINE)Main,
			hModule,
			0,
			nullptr
		);

		if (!hThread) return FALSE;
		else break;

	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}