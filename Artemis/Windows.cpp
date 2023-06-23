#include "pch.h"
#include "Windows.h"

#include "External.h"
#include "GameManager.h"

MainWindow::MainWindow() : IWindow("Main Window", true) {}

void MainWindow::Window() {
	ImGui::Text("Artemis RT test 1.0");
}