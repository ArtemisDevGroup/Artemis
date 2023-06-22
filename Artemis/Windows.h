#pragma once

#include "WindowManager.h"

class MainWindow : public Artemis::IWindow {
public:
	MainWindow();

	virtual void Window() final;
};