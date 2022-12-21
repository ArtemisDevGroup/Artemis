//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#include "Window.h"
#include "Identifiers.h"

MainWindow::MainWindow() : IWindow("Main Window", WND_MAINWINDOW) {}

void MainWindow::Window() {
    ImGui::Text("Welcome to Artemis!");
    ImGui::Text("Brought to you by:");
    ImGui::Text("Tavreth | Monke | Sigma");
    ImGui::Text("");
    ImGui::Text("[DEBUG] F1 - Eject Artemis.");
    ImGui::Text("F2 - Hide all windows.");
    ImGui::Text("");
    ImGui::Text("Windows:");
    
    ImGui::Checkbox("[TEMP] Wallhack Window", (bool*)&Midnight::GetInst()->ImGuiWndManager.GetWndById(WND_WALLHACKWINDOW)->bShow);
}