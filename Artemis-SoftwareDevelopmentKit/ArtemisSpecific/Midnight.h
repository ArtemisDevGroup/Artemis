//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __MIDNIGHT_H__
#define __MIDNIGHT_H__

#include "..\Definitions.h"
#include "..\Memory.h"
#include "..\Logger.h"
#include "..\Interfaces.h"
#include "..\Configuration.h"
#include "WindowManager.h"
#include "DrawManager.h"
#include "KeybindManager.h"
#include "OnFrameManager.h"
#include "MemoryProtectManager.h"
#include "ExtensionFramework.h"
#include "Console.h"
#include "PresentHook.h"

namespace Artemis {
	/// <summary>
	/// The core component of Artemis, codename Midnight.
	/// </summary>
	class ARTEMIS_API Midnight : public IDisposable {
	public:
		/// <summary>
		/// Gets a pointer to the main instance.
		/// </summary>
		/// <returns>A pointer to a Midnight object.</returns>
		static Midnight* GetInst();
		
		Midnight();

		Memory Mem;									// The main memory instance.
		MemoryProtectManager GameMemProtectManager;	// The memory protection manager for the game allocation.
		ExtensionManager DllManager;				// The extension manager responsible for loading and releasing all extensions inside of Artemis-Extensions.
		WindowManager ImGuiWndManager;				// The main ImGui window manager instance used in the present hook.
		DrawManager ImGuiDrawManager;				// The main ImGui draw manager instance used in the present hook.
		DrawManager ESPDrawManager;					// The ESP draw manager instanced used for drawing all the ESP and bones. Is flushed regularily.
		KeybindManager BindManager;					// The main keybind manager instance used in the main loop.
		OnFrameManager ActionManager;				// The main OnFrame manager executing before the draw and window operations every frame.
		Hook PresentHook;							// The present hook.
		Logger Log;									// The main logger instance.
		Console ConInst;							// The main console instance.
		Configuration GlobalConfig;					// The global configuration file instance.

		BOOL bRun;									// A bool that if set to false will shut the module down and release its resources.
		LPVOID lpPresent;							// A pointer to the original present function.
		WNDPROC oWndProc;							// A pointer to the original window proceidure.
		HMODULE hModule;							// A handle to the Artemis dll module.
		HWND hWnd;									// A handle to the main window.

		/// <summary>
		/// Initializes the Midnight instance.
		/// </summary>
		/// <param name="hModule">- A handle to the Artemis module.</param>
		void Initialize(_In_ HMODULE hModule);

		/// <summary>
		/// Releases the Midnight instance.
		/// </summary>
		void Release();
	};
}

#endif // !__MIDNIGHT_H__