//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#include "Keybinds.h"

#include <Windows.h>

ExitKeybind::ExitKeybind() : IKeybind(VK_F1) {}

void ExitKeybind::WhenPressed() {
    Midnight::GetInst()->bRun = FALSE;
    FreeLibrary(GetModuleHandleA("Artemis-SoftwareDevelopmentKit.dll"));
}

HideAllKeybind::HideAllKeybind() : IKeybind(VK_F2) {}

void HideAllKeybind::WhenPressed() {
    LPBOOL lpShow = Midnight::GetInst()->ImGuiWndManager.GetWndVisibilityPtr();
    if (*lpShow) *lpShow = FALSE;
    else *lpShow = TRUE;
}