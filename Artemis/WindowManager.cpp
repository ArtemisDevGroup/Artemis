#include "pch.h"
#include "WindowManager.h"

namespace Artemis {
	bool g_bVisible = true;

	IWindow::IWindow(_In_z_ const char* lpWindowName, bool bVisible) {
		strcpy_s(szWindowName, lpWindowName);
		this->bVisible = bVisible;
	}

	const char* IWindow::GetWindowName() const { return szWindowName; }

	bool IWindow::GetWindowVisibility() const { return bVisible; }
	bool* IWindow::GetWindowVisibilityPtr() { return &bVisible; }

	void IWindow::SetWindowVisibility(bool bVisible) { this->bVisible = bVisible; }

	void IWindow::Present() {
		if (g_bVisible && bVisible) {
			if (ImGui::Begin(szWindowName))
				Window();
			ImGui::End();
		}
	}

	void WindowManager::PresentAll() {
		for (IWindow* pWindow : InvocableCollection)
			if (pWindow)
				pWindow->Present();
	}
}