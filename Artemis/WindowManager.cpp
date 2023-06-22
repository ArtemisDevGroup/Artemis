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

	WindowManager::WindowManager() { memset(WindowCollection, 0, sizeof(WindowCollection)); }

	WindowManager::~WindowManager() { this->Release(); }

	WindowIndex WindowManager::Add(_In_ IWindow* pWindow) {
		for (WindowIndex i = 0; i < MAX_INVOKE; i++)
			if (!WindowCollection[i]) {
				WindowCollection[i] = pWindow;
				return i;
			}

		delete pWindow;
		return INVALID_INDEX;
	}

	void WindowManager::Release(_In_range_(INVALID_INDEX, MAX_INVOKE) WindowIndex nIndex) {
		if (nIndex == -1) {
			for (IWindow* pWindow : WindowCollection)
				if (pWindow)
					delete pWindow;
			memset(WindowCollection, 0, sizeof(WindowCollection));
		}
		else if (WindowCollection[nIndex]) {
			delete WindowCollection[nIndex];
			WindowCollection[nIndex] = nullptr;
		}
	}

	_Ret_maybenull_ IWindow* WindowManager::Get(_In_range_(0, MAX_INVOKE) WindowIndex nIndex) {
		if (nIndex > 64 || nIndex < 0) return nullptr;
		return WindowCollection[nIndex];
	}

	void WindowManager::PresentAll() {
		for (IWindow* pWindow : WindowCollection)
			if (pWindow)
				pWindow->Present();
	}
}