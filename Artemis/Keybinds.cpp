#include "Keybinds.h"

#include <Windows.h>

ExitKeybind::ExitKeybind() : IKeybind(VK_F1) {}

void ExitKeybind::WhenPressed() {
    Midnight* pInst = Midnight::GetInst();
    pInst->bRun = FALSE;
}

HideAllKeybind::HideAllKeybind() : IKeybind(VK_F2) {}

void HideAllKeybind::WhenPressed() {
    LPBOOL lpShow = Midnight::GetInst()->ImGuiWndManager.GetWndVisibilityPtr();
    if (*lpShow) *lpShow = FALSE;
    else *lpShow = TRUE;
}