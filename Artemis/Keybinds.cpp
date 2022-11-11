#include "Keybinds.h"

#include <Windows.h>

ExitKeybind::ExitKeybind() : IKeybind(VK_DELETE) {}

void ExitKeybind::WhenPressed() {
    Midnight* pInst = Midnight::GetInst();
    pInst->bRun = FALSE;
    SetWindowLongPtr(pInst->hWnd, GWLP_WNDPROC, (LONG_PTR)pInst->oWndProc);
}

HideAllKeybind::HideAllKeybind() : IKeybind(VK_HOME) {}

void HideAllKeybind::WhenPressed() {
    LPBOOL lpShow = Midnight::GetInst()->ImGuiWndManager.GetWndVisibilityPtr();
    if (*lpShow) *lpShow = FALSE;
    else *lpShow = TRUE;
}