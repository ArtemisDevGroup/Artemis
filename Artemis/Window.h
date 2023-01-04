//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Astrea0014.												|
// @Astrea0014: https://github.com/Astrea0014											|
//-------------------------------------------------------------------------------------->

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <ArtemisSpecific/Midnight.h>

#include "Identifiers.h"

using namespace Artemis;

class MainWindow : public IWindow {
public:
    MainWindow();
    void Window();
};

class WallhackWindow : public IWindow, public IOnFrame {
    bool bBoneEsp;

public:
    WallhackWindow();
    void Window();
    void OnFrame();
};

class TestWindow : public IWindow {
public:
    TestWindow();
    void Window();
};

class TerroristWindow : public IWindow {
public:
    TerroristWindow();
    void Window();
    void Inini();
};

class CreditsWindow : public IWindow {
public:
    CreditsWindow();
    void Window();
};

#endif // !__WINDOW_H__