#include "pch.h"

DWORD APIENTRY ArtemisMain(HMODULE hModule);

#ifndef ARTEMIS_TEST

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved) {
    HANDLE hThread = nullptr;
    
    switch (dwReasonForCall) {
    case DLL_PROCESS_ATTACH:
        hThread = CreateThread(
            nullptr,
            NULL,
            (LPTHREAD_START_ROUTINE)ArtemisMain,
            hModule,
            NULL,
            nullptr
        );

        if (!hThread) return FALSE;
        CloseHandle(hThread);

        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

#else

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved) {
    return TRUE;
}

#endif // !ARTEMIS_TEST