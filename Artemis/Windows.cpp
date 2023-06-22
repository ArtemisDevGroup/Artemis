#include "pch.h"
#include "Windows.h"

MainWindow::MainWindow() : IWindow("Main Window", true) {}

void MainWindow::Window() {
	ImGui::Text("Artemis RT test 1.0");
}