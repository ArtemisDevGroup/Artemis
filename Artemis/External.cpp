#include "pch.h"
#include "External.h"

namespace Artemis {
	ARTEMIS_API bool bRunning = true;

#ifdef _DEBUG
	ARTEMIS_API Logger Log(true, true, "Artemis.log");
#else
	ARTEMIS_API Logger Log(false, true, "Artemis.log");
#endif // _DEBUG

	ARTEMIS_API KeybindManager Keybinds;
}