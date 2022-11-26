#include "Window.h"

TestWindow::TestWindow() : IWindow("Test Window", WND_TESTWINDOW) {}

void TestWindow::Window() {
	static Memory* pm = &Midnight::GetInst()->Mem;

}