//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Astrea0014.												|
// @Astrea0014: https://github.com/Astrea0014											|
//-------------------------------------------------------------------------------------->

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