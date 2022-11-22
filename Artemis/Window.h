#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "ArtemisSpecific/Midnight.h"

using Artemis::Midnight;
using Artemis::IWindow;

class MainWindow : public IWindow {
public:
    MainWindow();
    void Window();
};

class HashWindow : public IWindow {
public:
    HashWindow();
    void Window();
};

class ESPWindow : public IWindow {
public:
    ESPWindow();
    void Window();
};

#endif // !__WINDOW_H__