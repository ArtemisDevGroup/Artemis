#include <Windows.h>
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "User32.lib")

#include <stdio.h>
#include <thread>

#undef interface
#undef RegisterClass
#undef CreateWindow

#include "resource.h"

#define APPWM_ICONNOTIFY WM_APP + 1

bool bInterrupt = false;

using namespace System::Threading;
using namespace System;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case APPWM_ICONNOTIFY: {
		switch (lParam) {
		case WM_LBUTTONDOWN: {
			bInterrupt = true;
		} break;
		default:
			break;
		}
	} break;
	default:
		return DefWindowProcA(hWnd, message, wParam, lParam);
	}
	return 0;
}

template<typename T>
void ManagedToUnmanaged(array<T>^ managed, T* unmanaged, int unmanagedCount) {
	if (managed->Length > unmanagedCount) return;
	
	for (int i = 0; i < managed->Length; i++) {
		unmanaged[i] = managed[i];
	}
}

void ManagedStrToUnmanagedStr(String^ str, char* buffer, int size) {
	ManagedToUnmanaged(Text::Encoding::ASCII->GetBytes(str), (unsigned char*)buffer, size);
}

namespace Artemis {
	namespace Loader {
		namespace Helper {
			public ref class TrayIconStatic {
			public:
				static IntPtr GetModuleInstanceHandle() {
					return (IntPtr)GetModuleHandleA("Artemis.Loader.Helper.dll");
				}

				static IntPtr RegisterClass(IntPtr hInstance) {
					LPSTR lpszClassName = (LPSTR)malloc(64);
					strcpy_s(lpszClassName, 64, "ArtemisTrayIconWndClass");

					LPSTR lpszMenuName = (LPSTR)malloc(64);
					strcpy_s(lpszMenuName, 64, "MenuName");

					HINSTANCE hInst = (HINSTANCE)hInstance.ToPointer();

					PWNDCLASSEXA pwcex = (PWNDCLASSEXA)calloc(1, sizeof(WNDCLASSEXA));

					pwcex->cbSize = sizeof(WNDCLASSEXA);

					pwcex->style = CS_HREDRAW | CS_VREDRAW;
					pwcex->lpfnWndProc = WndProc;
					pwcex->cbClsExtra = 0;
					pwcex->cbWndExtra = 0;
					pwcex->hInstance = hInst;
					pwcex->hIcon = LoadIconA(hInst, MAKEINTRESOURCEA(IDI_ICON1));
					pwcex->hCursor = LoadCursor(nullptr, IDC_ARROW);
					pwcex->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
					pwcex->lpszMenuName = lpszMenuName;
					pwcex->lpszClassName = lpszClassName;

					RegisterClassExA(pwcex);

					return (IntPtr)(pwcex);
				}

				static IntPtr RegisterNotifyIconData(IntPtr hInstance, IntPtr hWnd) {
					PNOTIFYICONDATAA pnid = (PNOTIFYICONDATAA)calloc(1, sizeof(NOTIFYICONDATAA));
					pnid->cbSize = sizeof(NOTIFYICONDATAA);

					pnid->hWnd = (HWND)hWnd.ToPointer();
					pnid->uFlags = NIF_MESSAGE | NIF_TIP | NIF_ICON;
					lstrcpyA(pnid->szTip, "Artemis");
					pnid->uID = 69;
					pnid->uCallbackMessage = APPWM_ICONNOTIFY;
					pnid->uVersion = NOTIFYICON_VERSION;
					pnid->hIcon = LoadIconA((HINSTANCE)hInstance.ToPointer(), MAKEINTRESOURCEA(IDI_ICON1));

					return (IntPtr)pnid;
				}

				static IntPtr CreateWindow(IntPtr hInstance, IntPtr wndClassEx) {
					return (IntPtr)CreateWindowExA(
						0,
						((PWNDCLASSEXA)(wndClassEx.ToPointer()))->lpszClassName,
						"Artemis.NotifyIconData.DummyWindow",
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						0,
						CW_USEDEFAULT,
						0,
						nullptr,
						nullptr,
						(HINSTANCE)hInstance.ToPointer(),
						nullptr
					);
				}

				enum class NotifyIconFlags : UInt32 {
					Add = 0,
					Remove = 2
				};

				static bool NotifyIcon(NotifyIconFlags flags, IntPtr notifyIconData) {
					return Shell_NotifyIconA((DWORD)flags, (PNOTIFYICONDATAA)notifyIconData.ToPointer());
				}

				static void Release(IntPtr hWnd, IntPtr wndClassEx, IntPtr notifyIconData) {
					PWNDCLASSEXA pwcex = (PWNDCLASSEXA)wndClassEx.ToPointer();

					DestroyWindow((HWND)hWnd.ToPointer());
					UnregisterClassA(pwcex->lpszClassName, GetModuleHandleA("Artemis.Loader.Helper.dll"));

					free((void*)pwcex->lpszClassName);
					free((void*)pwcex->lpszMenuName);
					free(pwcex);
					free(notifyIconData.ToPointer());
				}
			};

			public interface class IBaseTrayIcon {
			public:
				static bool IsInterrupt() { return bInterrupt; }
				static void ResetInterrupt() { bInterrupt = false; }
			};
		}
	}
}