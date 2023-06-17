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

class StopProcessKeybind : public IKeybind {
public:
    StopProcessKeybind() : IKeybind(VirtualKey::F1, true) {}

    virtual void OnKeyPress() final { bRunning = false; }
};

DWORD APIENTRY Main(HMODULE hModule) {
    Aurora::CreateConsole();
    Aurora::OpenStream(Aurora::StandardStream::Out);

    PrintAsciiArt();

    const Aurora::ProcessInfo& CurrentProcess = Aurora::GetCurrentProcessInfo();

    Log.LogInfo("Welcome to Artemis!");
    Log.LogInfo("Current process:");
    Log.LogInfo("    %s : %s", CurrentProcess.GetProcessName(), CurrentProcess.GetProcessPath());
    Log.LogInfo("Process Id: 0x%lX", CurrentProcess.GetProcessId());
    Log.LogInfo("Process Handle: 0x%p", CurrentProcess.GetProcessHandle());

    Keybinds.Add(new StopProcessKeybind());

    while (bRunning) Keybinds.Invoke();

    Aurora::CloseStream(Aurora::StandardStream::Out);
    Aurora::ReleaseConsole();

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