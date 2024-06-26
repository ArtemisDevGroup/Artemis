#ifndef ARTEMIS_MIDNIGHT_HXX
#define ARTEMIS_MIDNIGHT_HXX

#include "pch.h"

#include "API/Hook.hxx"

namespace Artemis {
	typedef HRESULT(APIENTRY* present_function)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	
	class midnight {
	public:
		struct {
			API::hook<present_function> hkInstance;
			HWND hWnd;
			WNDPROC oWndProc;
			present_function oPresent;
		} _PresentHook;

		static ARTEMIS_API midnight* global();
	};
}

#define athis Artemis::midnight::global()

#endif // !ARTEMIS_MIDNIGHT_HXX