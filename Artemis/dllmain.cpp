#include "pch.h"

DWORD APIENTRY Main(HMODULE hModule) {

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