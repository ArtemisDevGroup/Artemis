#ifndef __ARTEMIS_EXTERNAL_H__
#define __ARTEMIS_EXTERNAL_H__

#include "pch.h"

using Aurora::Logger;

#include "Definitions.h"
#include "KeybindManager.h"

namespace Artemis {
	ARTEMIS_API extern Logger Log;

	ARTEMIS_API extern KeybindManager Keybinds;

	ARTEMIS_API extern bool bRunning;
}

#endif // !__ARTEMIS_EXTERNAL_H__