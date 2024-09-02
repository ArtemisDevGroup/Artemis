#ifndef ARTEMIS_MIDNIGHT_HXX
#define ARTEMIS_MIDNIGHT_HXX

#include <Windows.h>	// HRESULT, APIENTRY, UINT, HWND, WNDPROC
#include <dxgi.h>		// IDXGISwapChain

#include "Definitions.hxx"
#include "Extension.hxx"
#include "KeyAction.hxx"
#include "Communications.hxx"
#include "ImGuiWindows.hxx"

#include "API/Hook.hxx"
#include "API/Logging.hxx"

namespace Artemis {
	typedef HRESULT(APIENTRY* TPRESENT)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	
	struct midnight {
		/// <summary>
		/// <para>Contains all persistent data for the DX hook.</para>
		/// <para>Do not mess with this unless you absolutely need to.</para>
		/// </summary>
		struct {
			API::hook<TPRESENT>* hkInstance;
			HWND hWnd;
			WNDPROC oWndProc;
			TPRESENT oPresent;
		} _PresentHook;

		/// <summary>
		/// The logger instance used by Artemis.
		/// </summary>
		API::logger* Logger;

		/// <summary>
		/// Manages loading and unloading of extensions.
		/// </summary>
		extension_manager* Extensions;

		/// <summary>
		/// Manages keybinds/keypress actions.
		/// </summary>
		key_action_manager* KeyActions;

		/// <summary>
		/// Manages ImGui windows.
		/// </summary>
		window_manager* Windows;

		/// <summary>
		/// A message recipent receiving all messages dispatched for the Artemis client.
		/// </summary>
		message_recipent* ClientMessageRecipent;

		/// <summary>
		/// A message dispatcher sending messages to the Artemis Loader.
		/// </summary>
		message_dispatcher* MainRemoteDispatcher;

		/// <summary>
		/// A message dispatcher sending messages to the Artemis client.
		/// </summary>
		message_dispatcher* MainInternalDispatcher;
	};
}

/// <summary>
/// <para>Much like the builtin 'this' keyword for a class,</para>
/// <para>'athis' is used to access all data fields used by the current Artemis instance.</para>
/// </summary>
ARTEMIS_FRAMEWORK extern ::Artemis::midnight* const athis;

#endif // !ARTEMIS_MIDNIGHT_HXX