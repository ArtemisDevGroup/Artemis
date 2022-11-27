#include "Window.h"

TestWindow::TestWindow() : IWindow("Test Window", WND_TESTWINDOW) {}

typedef SHORT(WINAPI* tGetAsyncKeyState)(int vKey);
tGetAsyncKeyState oGetAsyncKeyState;

int nLastKey = 0;

SHORT WINAPI hkGetAsyncKeyState(int vKey) {
	static Logger* pLog = &Midnight::GetInst()->Log;
	nLastKey = vKey;

	switch (vKey) {
	case VK_RBUTTON:
		pLog->LogInfo(__FUNCTION__, "VK_RBUTTON");
		break;
	case VK_LBUTTON:
		pLog->LogInfo(__FUNCTION__, "VK_LBUTTON");
		break;
	}

	return oGetAsyncKeyState(vKey);
}

void TestWindow::Window() {
	static Memory* pm = &Midnight::GetInst()->Mem;
	static bool bCheckbox = false;
	static Hook* pHook;
	static Logger* pLog = &Midnight::GetInst()->Log;
	if (ImGui::Checkbox("Hook GetAsyncKeyState", &bCheckbox)) {
		if (bCheckbox) {
			try {
				pHook = new Hook(GetModuleHandleA("User32.dll"), "GetAsyncKeyState", &oGetAsyncKeyState);
				pHook->Enable();
			}
			catch (HookException& e) {
				pLog->LogError("Failed to hook GetAsyncKeyState: %s", e.GetExceptionMessage());
				bCheckbox = false;
			}
		}
		else {
			pHook->Release();
			delete pHook;
		}
	}
}