#ifndef __MIDNIGHT_H__
#define __MIDNIGHT_H__

#include "..\Definitions.h"
#include "..\Memory.h"
#include "..\Logger.h"
#include "..\Interfaces.h"
#include "WindowManager.h"
#include "DrawManager.h"
#include "Console.h"
#include "PresentHook.h"

namespace Artemis {
	/// <summary>
	/// The core component of Artemis, codename Midnight.
	/// </summary>
	class Midnight : public IDisposable {
	public:
		/// <summary>
		/// Gets a pointer to the main instance.
		/// </summary>
		/// <returns>A pointer to a Midnight object.</returns>
		static Midnight* GetInst();

		Midnight();

		Memory m;						// The main memory instance.
		WindowManager ImGuiWndManager;	// The main ImGui window manager instance used in the present hook.
		DrawManager ImGuiDrawManager;	// The main ImGui draw manager instance used in the present hook.
		Hook PresentHook;				// The present hook.
		Logger Log;						// The main logger instance.
		Console ConInst;				// The main console instance.

		BOOL bRun;						// A bool that if set to false will shut the module down and release its resources.
		LPVOID lpPresent;				// A pointer to the original present function.

		/// <summary>
		/// Initializes the Midnight instance.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Releases the Midnight instance.
		/// </summary>
		void Release();
	};
}

#endif // !__MIDNIGHT_H__