#ifndef ARTEMIS_MIDNIGHT_HXX
#define ARTEMIS_MIDNIGHT_HXX

#include <Windows.h>	// HRESULT, APIENTRY, UINT, HWND, WNDPROC
#include <dxgi.h>		// IDXGISwapChain

#include <memory>		// std::unique_ptr, std::shared_ptr

#include "Definitions.hxx"
#include "Extension.hxx"
#include "KeyAction.hxx"

#include "API/Hook.hxx"
#include "API/Logging.hxx"

namespace Artemis {
	typedef HRESULT(APIENTRY* TPRESENT)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	
	struct midnight {
		struct {
			std::unique_ptr<API::hook<TPRESENT>> hkInstance;
			HWND hWnd;
			WNDPROC oWndProc;
			TPRESENT oPresent;
		} _PresentHook;

		std::shared_ptr<API::logger> Logger;
		std::unique_ptr<extension_manager> Extensions;
		std::unique_ptr<key_action_manager> KeyActions;
	};
}

ARTEMIS_FRAMEWORK extern ::Artemis::midnight* const athis;

#endif // !ARTEMIS_MIDNIGHT_HXX