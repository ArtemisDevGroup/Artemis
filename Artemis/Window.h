//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
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
};

#endif // !__WINDOW_H__