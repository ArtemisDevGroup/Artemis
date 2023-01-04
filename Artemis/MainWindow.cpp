//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Astrea0014.												|
// @Astrea0014: https://github.com/Astrea0014											|
//-------------------------------------------------------------------------------------->

#include "Window.h"
#include "Identifiers.h"

MainWindow::MainWindow() : IWindow("Main Window", WND_MAINWINDOW) {}

void MainWindow::Window() {
    static Midnight* pInst = Midnight::GetInst();
    static ConfigurationSection WndSect = pInst->GlobalConfig.GetSection("Window");
    static WindowManager* pWndMgr = &pInst->ImGuiWndManager;

    static IWindow* pWhWnd          = pWndMgr->GetWndById(WND_WALLHACKWINDOW);
    static IWindow* pThWnd          = pWndMgr->GetWndById(WND_TERRORISTWINDOW);
    static IWindow* pCreditsWnd     = pWndMgr->GetWndById(WND_CREDITSWINDOW);

    ImGui::Text("Welcome to Artemis!");
    ImGui::Text("Brought to you by:");
    ImGui::Text("Tavreth | Monke | Sigma");
    ImGui::NewLine();
    ImGui::Text("[DEBUG] F1 - Eject Artemis.");
    ImGui::Text("F2 - Hide all windows.");
    ImGui::NewLine();

    if (ImGui::Checkbox("Wallhack Window", (bool*)&pWhWnd->bShow))
        WndSect.SetPropertyValue("WallhackWindow", pWhWnd->bShow);

    if (ImGui::Checkbox("Terrorist Window", (bool*)&pThWnd->bShow))
        WndSect.SetPropertyValue("TerroristWindow", pThWnd->bShow);

    if (ImGui::Checkbox("Credits Window", (bool*)&pCreditsWnd->bShow))
        WndSect.SetPropertyValue("CreditsWindow", pCreditsWnd->bShow);
}