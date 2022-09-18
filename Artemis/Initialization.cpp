#include "FunctionDecl.h"
#include "Console.h"
#include "SDK/MemApi.h"
#include "PresentHook.h"
#include "Offsets.h"

#ifdef _DEBUG
    #define __VERSION__ "v1.0 DEV"
#else
    #define __VERSION__ "v1.0"
#endif

typedef struct _HANDLEDATA {
    DWORD dwProcessId;
    HWND hWnd;
} HANDLEDATA, * LPHANDLEDATA;

BOOL IsMainWindow(HWND hWnd) {
    return (BOOL)((GetWindow(hWnd, GW_OWNER) == nullptr) && IsWindowVisible(hWnd));
}

BOOL CALLBACK EnumWindowsCallback(HWND hWnd, LPARAM lParam) {
    HANDLEDATA& data = *(LPHANDLEDATA)lParam;
    DWORD dwProcessId = 0;
    GetWindowThreadProcessId(hWnd, &dwProcessId);
    if (data.dwProcessId != dwProcessId || !IsMainWindow(hWnd))
        return TRUE;
    data.hWnd = hWnd;
    return FALSE;
}

HWND FindMainWindow() {
    HANDLEDATA data;
    data.dwProcessId = GetCurrentProcessId();
    data.hWnd = nullptr;
    EnumWindows(EnumWindowsCallback, (LPARAM)&data);
    return data.hWnd;
}

HOOK hk;

void Initialize() {
	InitializeConsole();
    env::LogClass().LogSuccess(__FUNCTION__, "Console initialized!");

    env::InitMainModule();
    env::LogClass().LogSuccess(__FUNCTION__, "Fetched process module allocation base address: 0x%llX", env::GetMainModule()->uModuleBase);

    const BASE_OFFSET szOffsets[] = {
        ofs::c_BuildStringCodeCave,
        ofs::c_MainBuildString,
        ofs::c_MainVersionString,
        ofs::c_ToggleBuildStringCmp,
        ofs::c_LoadEffectiveAddressPatch
    };

    for (int i = 0; i < (sizeof(szOffsets) / sizeof(BASE_OFFSET)); i++) {
        if (XEVirtualProtect(env::GetMainModule()->uModuleBase + szOffsets[i], 16, MP_EXECUTE_READWRITE, nullptr))
            env::LogClass().LogSuccess(__FUNCTION__, "Successfully obtained write permissions for 0x%llX!", env::GetMainModule()->uModuleBase + szOffsets[i]);
        else
            env::LogClass().LogError(__FUNCTION__, "Failed to obtain write permissions for 0x%llX.", env::GetMainModule()->uModuleBase + szOffsets[i]);
    }

    BYTE b = 1;
    if (XEWrite(env::GetMainModule()->uModuleBase + ofs::c_ToggleBuildStringCmp + 6, &b, sizeof(BYTE)))
        env::LogClass().LogSuccess(__FUNCTION__, "Successfully patched instruction.");
    else
        env::LogClass().LogError(__FUNCTION__, "Failed to patch instruction.");

    if (XEWriteStringA(env::GetMainModule()->uModuleBase + ofs::c_BuildStringCodeCave, "ARTEMIS", TRUE))
        env::LogClass().LogSuccess(__FUNCTION__, "Successfully wrote string to code cave.");
    else
        env::LogClass().LogError(__FUNCTION__, "Failed to write string to code cave.");

    if (XEWriteStringA(env::GetMainModule()->uModuleBase + ofs::c_MainBuildString, __VERSION__, TRUE))
        env::LogClass().LogSuccess(__FUNCTION__, "Successfully wrote string to code cave.");
    else
        env::LogClass().LogError(__FUNCTION__, "Failed to write string to code cave.");

    const BYTE szPatch[] = {
        0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
        0x48, 0x8D, 0x15, 0x5A, 0x55, 0xF4, 0xFF
    };

    if (XEWriteBytes(env::GetMainModule()->uModuleBase + ofs::c_LoadEffectiveAddressPatch, szPatch, sizeof(szPatch), 0))
        env::LogClass().LogSuccess(__FUNCTION__, "Successfully patched the lea instruction.");
    else
        env::LogClass().LogError(__FUNCTION__, "Failed to patch the lea instruction.");

    CHAR c = '\0';
    if (XEWrite(env::GetMainModule()->uModuleBase + ofs::c_MainVersionString, &c, sizeof(CHAR)))
        env::LogClass().LogSuccess(__FUNCTION__, "Successfully wrote null-terminator to minor version string.");
    else
        env::LogClass().LogError(__FUNCTION__, "Failed to write null-terminator to minor version string.");

    env::LogClass().LogInfo(__FUNCTION__, "Waiting for window to inject ImGui...");
    int n = 0;
    do {
        if (!XEReadPtr(env::GetMainModule()->uModuleBase + ofs::c_GameManager, (LPOFFSET)&ptr::c_GameState, &n, sizeof(int))) {
            env::LogClass().LogError(__FUNCTION__, "Failed to read GameState value.");
            Sleep(1000);
        }
    } while (n < 2);

    if (!n) return;

    Sleep(2000);

    if (XEInitHook()) env::LogClass().LogSuccess(__FUNCTION__, "Hook initialized.");
    else env::LogClass().LogSuccess(__FUNCTION__, "Error initializing hook library. Message: %s", XEGetLastErrorEntry().szLibMessage);

    HWND hWnd = FindMainWindow();
    if (hWnd) env::LogClass().LogSuccess(__FUNCTION__, "Got main window handle: %p", (void*)hWnd);
    else env::LogClass().LogError(__FUNCTION__, "Failed to retrieve main window handle.");

    LPVOID lpPresent = GetPresentPtr(hWnd);
    if (lpPresent) env::LogClass().LogSuccess(__FUNCTION__, "Got present ptr: %p", lpPresent);
    else env::LogClass().LogError(__FUNCTION__, "Present not found.");

    if (XECreateHook(&hk, lpPresent, hkPresent, FALSE)) env::LogClass().LogSuccess(__FUNCTION__, "Hook Created.");
    else env::LogClass().LogError(__FUNCTION__, "Creating hook failed. Message: %s", XEGetLastErrorEntry().szLibMessage);

    env::LogClass().LogInfo(__FUNCTION__, "Set present ptr to: %p", lpPresent);
	SetOriginalPresentPtr(hk.lpOriginal);

    if (XEEnableHook(&hk)) env::LogClass().LogSuccess(__FUNCTION__, "Hook enabled.");
    else env::LogClass().LogError(__FUNCTION__, "Unable to enable hook. Message: %s", XEGetLastErrorEntry().szLibMessage);
}

void Uninitialize() {
    XEDisableHook(&hk);
    XERemoveHook(&hk);
    XEUninitHook();

	UninitializeConsole();
}