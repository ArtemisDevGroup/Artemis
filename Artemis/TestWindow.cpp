#include "Window.h"

TestWindow::TestWindow() : IWindow("Test Window", 4) {}

typedef char(__fastcall* tSetGameTimer)(__int64 GameManager, int nMilliseconds, char a3);

void TestWindow::Window() {
	static Memory* pm = &Midnight::GetInst()->Mem;
	static const __int64 c_GameManager = pm->Read<__int64>(pm->GetModuleBase() + 0x6E3FF30);
	static const tSetGameTimer SetGameTimer = (tSetGameTimer)(pm->GetModuleBase() + 0x213DC80);

	if (ImGui::Button("Set Timer to INT_MAX ms.")) {
		SetGameTimer(c_GameManager, 30 * 1000, 0);
	}
}