#include "pch.h"

#include "External.h"

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

void PrintAsciiArt() {
    for (const char* lpString : lpszAsciiArt)
        printf("%s\n", lpString);
}

void LogBasicInformation(const char* lpSender, const Aurora::ProcessInfo& CurrentProcess) {
    Log.LogInfo(lpSender, "Welcome to Artemis!");
    Log.LogInfo(lpSender, "Current process:");
    Log.LogInfo(lpSender, "    %s : %s", CurrentProcess.GetProcessName(), CurrentProcess.GetProcessPath());
    Log.LogInfo(lpSender, "Process Id: 0x%lX", CurrentProcess.GetProcessId());
    Log.LogInfo(lpSender, "Process Handle: 0x%p\n", CurrentProcess.GetProcessHandle());

    const Aurora::ModuleInfo& Module = CurrentProcess.GetModule();

    Log.LogInfo(lpSender, "Module: %s", Module.GetModuleName());
    Log.LogInfo(lpSender, "    %s", Module.GetModulePath());
    Log.LogInfo(lpSender, "Module handle: 0x%p", Module.GetModuleHandle());
    Log.LogInfo(lpSender, "Module base address: 0x%llX", Module.GetModuleBaseAddress());
    Log.LogInfo(lpSender, "Module size: 0x%lX", Module.GetModuleSize());
}

class StopProcessKeybind : public IKeybind {
public:
    StopProcessKeybind() : IKeybind(VirtualKey::F1, true) {}

    virtual void OnKeyPress() final { bRunning = false; }
};

DWORD APIENTRY Main(HMODULE hModule) {
#ifdef _DEBUG
    Aurora::CreateConsole();
    Aurora::OpenStream(Aurora::StandardStream::Out);
#endif // _DEBUG

    PrintAsciiArt();
    LogBasicInformation(__FUNCTION__, Aurora::GetCurrentProcessInfo());

    if (Keybinds.Add(new StopProcessKeybind()) == INVALID_INDEX) {
        Log.LogError(__FUNCTION__, "Stop process keybind could not be added. Aborting...");
        bRunning = false;
    }
    else Log.LogSuccess(__FUNCTION__, "Successfully registered the stop process keybind.");

    while (bRunning) Keybinds.Invoke();

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