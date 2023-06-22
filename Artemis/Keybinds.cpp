#include "pch.h"
#include "Keybinds.h"
#include "External.h"

ExitKeybind::ExitKeybind() : IKeybind(Artemis::VirtualKey::F1, true) {}

void ExitKeybind::OnKeyPress() { Artemis::Exit(); }