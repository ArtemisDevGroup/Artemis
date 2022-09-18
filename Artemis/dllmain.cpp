#include "FunctionDecl.h"

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
    case DLL_PROCESS_DETACH:
        Uninitialize();
        break;
    }
    return TRUE;
}

