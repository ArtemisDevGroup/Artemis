#include "pch.h"
#include "External.h"

namespace Artemis {
#ifdef _DEBUG
	ARTEMIS_API Logger Log(true, true, "Artemis.log");
#else
	ARTEMIS_API Logger Log(false, true, "Artemis.log");
#endif // _DEBUG

	ARTEMIS_API DrawManagerCollection DrawManagers;
	ARTEMIS_API DrawManager& MainDrawManager = *DrawManagers.Get(DrawManagers.AddNew());
	ARTEMIS_API EventManager EventEntries;
	ARTEMIS_API KeybindManager Keybinds;
	ARTEMIS_API WindowManager Windows;
}