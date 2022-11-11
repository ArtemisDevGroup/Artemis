#ifndef __KEYBINDS_H__
#define __KEYBINDS_H__

#include "ArtemisSpecific/Midnight.h"

using Artemis::Midnight;
using Artemis::IKeybind;

class ExitKeybind : public IKeybind {
public:
    ExitKeybind();
    void WhenPressed();
};

class HideAllKeybind : public IKeybind {
public:
    HideAllKeybind();
    void WhenPressed();
};

#endif // !__KEYBINDS_H__