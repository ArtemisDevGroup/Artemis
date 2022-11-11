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

#endif // !__WINDOW_H__