#ifndef __ARTEMIS_MIDNIGHT_HXX__
#define __ARTEMIS_MIDNIGHT_HXX__

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
		/**
		 * @brief Contains all persistent data for the DirectX hook.
		 * Do not mess with this unless you absolutely need to.
		 */
		struct {
			API::hook<TPRESENT>* hkInstance;
			HWND hWnd;
			WNDPROC oWndProc;
			TPRESENT oPresent;
		} _DirectXPresentHookData;

		/**
		 * @brief The logger instance used by Artemis.
		 */
		API::logger* Logger;

		/**
		 * @brief Manages loading and unloading of extensions.
		 */
		extension_manager* Extensions;

		/**
		 * @brief Manages keybinds/keypress actions.
		 */
		key_action_manager* KeyActions;

		/**
		 * @brief Manages ImGui windows.
		 */
		window_manager* Windows;

		/**
		 * @brief A message recipent receiving all messages dispatched for the Artemis client remotely.
		 */
		message_recipent* ClientRemoteMessageRecipent;

		/**
		 * @brief A message recipent receiving all messages dispatched for the Artemis client internally.
		 */
		message_recipent* ClientInternalMessageRecipent;

		/**
		 * @brief A message dispatcher sending messages to the Artemis Loader.
		 */
		message_dispatcher* MainRemoteDispatcher;

		/**
		 * @brief A message dispatcher sending messages to the Artemis client.
		 */
		message_dispatcher* MainInternalDispatcher;
	};
}

/**
 * @brief Much like the builtin 'this' keyword for a class,
 * 'athis' is used to access all data fields used by the current Artemis instance.
 */
ARTEMIS_FRAMEWORK extern ::Artemis::midnight* const athis;

#endif // !__ARTEMIS_MIDNIGHT_HXX__