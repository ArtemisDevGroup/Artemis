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

class ESPWindow : public IWindow, public IOnFrame {
    bool bBoneEsp;

public:
    ESPWindow();
    void Window();
    void OnFrame();
};

class TestWindow : public IWindow {
public:
    TestWindow();
    void Window();
};

#endif // !__WINDOW_H__