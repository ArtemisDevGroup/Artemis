//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#include <Windows.h>
#include <signal.h>

#include <ArtemisSpecific/Midnight.h>

#include "Keybinds.h"
#include "Window.h"
#include "Draw.h"

using namespace Artemis;

void RegisterEventHandlers(); // Forward declaration from "ExceptionLoggers.cpp".

Midnight*           pInst           = Midnight::GetInst();
Logger*             pLog            = &pInst->Log;
Console*            pCon            = &pInst->ConInst;
Memory*             pMem            = &pInst->Mem;
WindowManager*      pWndMgr         = &pInst->ImGuiWndManager;
DrawManager*        pDrawMgr        = &pInst->ImGuiDrawManager;
DrawManager*        pESPDrawMgr     = &pInst->ESPDrawManager;
KeybindManager*     pBindMgr        = &pInst->BindManager;
OnFrameManager*     pOnFrameMgr     = &pInst->ActionManager;

DWORD APIENTRY Main(_In_ HMODULE hModule) {
    pCon->Allocate();

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

    RegisterEventHandlers();

    pInst->Initialize(hModule);

    pWndMgr->RegisterWnd(new MainWindow());
    //pWndMgr->RegisterWnd(new TestWindow());

    WallhackWindow* pWhWnd = new WallhackWindow();
    pWndMgr->RegisterWnd(pWhWnd);
    pOnFrameMgr->RegisterOnFrameAction(pWhWnd);

    TerroristWindow* pThWnd = new TerroristWindow();
    pThWnd->Inini();
    pWndMgr->RegisterWnd(pThWnd);

#ifdef _DEBUG
    pBindMgr->RegisterKeybind(new ExitKeybind());
#endif // _DEBUG
    pBindMgr->RegisterKeybind(new HideAllKeybind());

    while (pInst->bRun) pBindMgr->InvokeKeybinds();

    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(
    _In_ HMODULE hModule,
    _In_ DWORD dwReasonForCall,
    _In_ LPVOID lpReserved
) {
    HANDLE hThread = nullptr;
    HMODULE hSdkModule = nullptr;
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
        pInst->Release();
        pWndMgr->Release();
        pDrawMgr->Release();
        pESPDrawMgr->Release();
        pBindMgr->Release();
        pOnFrameMgr->Release();
        pMem->Release();
        pCon->Release();
        break;
    }
    return TRUE;
}