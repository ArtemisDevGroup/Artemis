#include "FunctionDecl.h"

#define IsKeyPressed(vk) (GetAsyncKeyState(vk) & 1)

DWORD APIENTRY Main(_In_ HMODULE hModule) {
	Initialize();

	while (env::IsRunning()) {
		if (IsKeyPressed(VK_DELETE)) {
			env::Stop();
			break;
		}
	}

	Uninitialize();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}