#pragma once

#include "KeybindManager.h"

class ExitKeybind : public Artemis::IKeybind {
public:
	ExitKeybind();

	virtual void OnKeyPress() final;
};