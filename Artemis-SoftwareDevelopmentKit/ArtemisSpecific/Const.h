#ifndef __CONST_H__
#define __CONST_H__

#include "..\Definitions.h"
#include "..\MemoryDefinitions.h"

namespace Artemis {
	namespace Constants {
		const BASE_OFFSET c_GameManager = 0x6E3FF30;

		typedef char(__fastcall* tSetGameTimer)(__int64 GameManager, int nMilliseconds, char a3);
		const BASE_OFFSET c_fnSetGameTimer = 0x213DC80;

		const POINTER_OFFSET c_PlaylistOffset = 0x68;

		const BASE_POINTER c_Playlist						= { c_GameManager, { c_PlaylistOffset }, 1 };
		const BASE_POINTER c_Map							= { c_GameManager, { c_PlaylistOffset, 0x10 }, 2 };
		const BASE_POINTER c_MapForge						= { c_GameManager, { c_PlaylistOffset, 0x10, 0x10 }, 3 };
		const BASE_POINTER c_Gamemode						= { c_GameManager, { c_PlaylistOffset, 0x8 }, 2 };
		const BASE_POINTER c_GamemodeForge					= { c_GameManager, { c_PlaylistOffset, 0x8, 0x10 }, 3 };
		const BASE_POINTER c_TimeOfDay						= { c_GameManager, { c_PlaylistOffset, 0x38, 0x0 }, 3 };
		const BASE_POINTER c_TimeOfDayForge					= { c_GameManager, { c_PlaylistOffset, 0x38, 0x0, 0x10 }, 4 };
		const BASE_POINTER c_TerroristDifficulty			= { c_GameManager, { c_PlaylistOffset, 0x30 }, 2 };
	}
}

#endif // !__CONST_H__