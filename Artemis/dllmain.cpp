#include <Windows.h>

#include "ArtemisSpecific/Midnight.h"

using namespace Artemis;

//------------------------>
// NOTE:
// This is a very early test of the system. MOST if not ALL files in this project are outdated.
// The latest files are found at the SDK GitHub reposititory. (https://github.com/ArtemisDevGroup/Artemis-SoftwareDevelopmentKit)
//------------------------>

class MainWindow : public IWindow {
public:
    MainWindow() : IWindow("Main", 1) {}

    void Window() {
        ImGui::Text("This is my main window!");
    }
};

DWORD APIENTRY Main(_In_ HMODULE hModule) {
    Midnight* pInst = Midnight::GetInst();
    pInst->Initialize();            // Initializes every Midnight sub-class and member field.

    WindowManager* pWndMgr = &pInst->ImGuiWndManager; // Gets a pointer to the wnd manager.
    pWndMgr->RegisterWnd(new MainWindow());           // Reigsters the main window.

    while (pInst->bRun) {
        if (GetAsyncKeyState(VK_DELETE) & 1) {
            pInst->bRun = FALSE;
            Sleep(500);
        }
    }

    pWndMgr->FreeWndBuffer();       // Frees the window buffer and releases all memory related to it.
    pInst->Release();               // Releases the Midnight instance and everything related to it.

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

