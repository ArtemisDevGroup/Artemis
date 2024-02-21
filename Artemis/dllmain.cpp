#include "pch.h"

#include "API/Error.hxx"

#ifndef ARTEMIS_TEST

DWORD APIENTRY Main(_In_ HMODULE hModule) {

    return 0;
}

BOOL APIENTRY DllMain(_In_ HMODULE hModule, _In_ DWORD dwReasonForCall, _In_ LPVOID lpReserved) {
    HANDLE hThread = nullptr;
    
    switch (dwReasonForCall) {
    case DLL_PROCESS_ATTACH:
        hThread = CreateThread(
            nullptr,
            NULL,
            (LPTHREAD_START_ROUTINE)Main,
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

BOOL APIENTRY DllMain(_In_ HMODULE hModule, _In_ DWORD dwReasonForCall, _In_ LPVOID lpReserved) {
    return TRUE;

}

#endif // !ARTEMIS_TEST