#ifndef __ARTEMIS_EXTERNAL_H__
#define __ARTEMIS_EXTERNAL_H__

#include "pch.h"

using Aurora::Logger;

#include "Definitions.h"
#include "DrawManager.h"
#include "EventManager.h"
#include "KeybindManager.h"
#include "WindowManager.h"

namespace Artemis {
	ARTEMIS_API void Exit();

	ARTEMIS_API extern Logger Log;

	ARTEMIS_API extern DrawManagerCollection DrawManagers;
	ARTEMIS_API extern DrawManager& MainDrawManager;
	ARTEMIS_API extern EventManager EventEntries;
	ARTEMIS_API extern KeybindManager Keybinds;
	ARTEMIS_API extern WindowManager Windows;
}

#endif // !__ARTEMIS_EXTERNAL_H__