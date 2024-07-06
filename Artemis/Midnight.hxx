#ifndef ARTEMIS_MIDNIGHT_HXX
#define ARTEMIS_MIDNIGHT_HXX

#include "pch.h"

#include "API/Hook.hxx"

namespace Artemis {
	typedef HRESULT(APIENTRY* TPRESENT)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	
	class midnight {
	public:
		struct {
			API::hook<TPRESENT> hkInstance;
			HWND hWnd;
			WNDPROC oWndProc;
			TPRESENT oPresent;
		} _PresentHook;

		static ARTEMIS_API midnight* global();
	};
}

#define athis Artemis::midnight::global()

#endif // !ARTEMIS_MIDNIGHT_HXX