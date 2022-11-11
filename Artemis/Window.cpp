#include "Window.h"

MainWindow::MainWindow(): IWindow("Main Window", 1) {}

void MainWindow::Window() {
    ImGui::Text("Welcome to Artemis!");
}