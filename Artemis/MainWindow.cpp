#include "Window.h"

MainWindow::MainWindow() : IWindow("Main Window", 1) {}

void MainWindow::Window() {
    ImGui::Text("Welcome to Artemis!");
    ImGui::Text("Brought to you by:");
    ImGui::Text("   Tavreth");
    ImGui::Text("   Monke");
    ImGui::Text("   Sigma");
    ImGui::Text("");
    ImGui::Text("[DEBUG] F1 - Eject Artemis.");
    ImGui::Text("F2 - Hide all windows.");
    ImGui::Text("Windows:");
    
    ImGui::Checkbox("[TEMP] ESP Window", (bool*)&Midnight::GetInst()->ImGuiWndManager.GetWndById(3)->bShow);
}