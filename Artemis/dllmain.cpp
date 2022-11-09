#include <Windows.h>

#include "ArtemisSpecific/Midnight.h"

using namespace Artemis;

//---------------------------------------------------------------------------------------------------------------------------------->
// NOTE:                                                                                                                            |
// This is a very early test of the system. MOST if not ALL files in this project are or will be outdated.                          |
// The latest files are found at the SDK GitHub reposititory. (https://github.com/ArtemisDevGroup/Artemis-SoftwareDevelopmentKit)   |
//---------------------------------------------------------------------------------------------------------------------------------->

class ExitKeybind : public IKeybind {
public:
    ExitKeybind() : IKeybind(VK_DELETE) {}

    void WhenPressed() {
        Midnight* pInst = Midnight::GetInst();
        pInst->bRun = FALSE;
        SetWindowLongPtr(pInst->hWnd, GWLP_WNDPROC, (LONG_PTR)pInst->oWndProc);
    }
};

DWORD APIENTRY Main(_In_ HMODULE hModule) {
    Midnight* pInst = Midnight::GetInst();              // Gets a pointer to the core Midnight instance.
    Logger* pLog = &pInst->Log;
    Console* pCon = &pInst->ConInst;
    Memory* pMem = &pInst->Mem;
    WindowManager* pWndMgr = &pInst->ImGuiWndManager;   // Gets a pointer to the window manager.
    DrawManager* pDrawMgr = &pInst->ImGuiDrawManager;   // Gets a pointer to the draw manager.
    KeybindManager* pBindMgr = &pInst->BindManager;     // Gets a pointer to the bind manager.

    pCon->Allocate();                                   // Allocates the console and rerouts stdin and stdout to it.

    pLog->LogInfo(__FUNCTION__, "Welcome to Artemis!");
    pLog->LogInfo(__FUNCTION__, "Process name: %s", pMem->GetProcessName());
    pLog->LogInfo(__FUNCTION__, "Process path: %s", pMem->GetProcessPath());
    pLog->LogInfo(__FUNCTION__, "Process id: %lu", pMem->GetProcessId());
    pLog->LogInfo(__FUNCTION__, "Process handle: %p", pMem->GetProcessHandle());
    pLog->LogInfo(__FUNCTION__, "Module name: %s", pMem->GetModuleName());
    pLog->LogInfo(__FUNCTION__, "Module path: %s", pMem->GetModulePath());
    pLog->LogInfo(__FUNCTION__, "Module handle: %p", pMem->GetModuleHandle());
    pLog->LogInfo(__FUNCTION__, "Module base address: 0x%llX", pMem->GetModuleBase());
    pLog->LogInfo(__FUNCTION__, "Module size: %lu", pMem->GetModuleSize());

    pInst->Initialize();                                // Initializes and runs the present hook.
    pBindMgr->RegisterKeybind(new ExitKeybind());       // Registers the exit keybind.

    while (pInst->bRun) pBindMgr->InvokeKeybinds();

    pWndMgr->Release();                                 // Frees the window buffer and releases all memory related to it.
    pDrawMgr->Release();                                // Frees the draw buffer and releases all memory related to it.
    pBindMgr->Release();                                // Frees the keybind buffer and releases all memory related to it.
    pInst->Release();                                   // Releases the Midnight instance and everything related to it.
    pLog->Release();                                    // Releases the log file.
    pCon->Release();                                    // Releases the console and the rerouted streams.

    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(
    _In_ HMODULE hModule,
    _In_ DWORD dwReasonForCall,
    _In_ LPVOID lpReserved
) {
    HANDLE hThread = nullptr;
    switch (dwReasonForCall) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);

        hThread = CreateThread(
            nullptr,
            0,
            (LPTHREAD_START_ROUTINE)Main,
            hModule,
            0,
            nullptr
        );
        if (!hThread) return FALSE;
        
        CloseHandle(hThread);
        break;
    }
    return TRUE;
}

