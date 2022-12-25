//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __CONST_H__
#define __CONST_H__

#include "..\Definitions.h"
#include "..\MemoryDefinitions.h"

namespace Artemis {
	namespace Constants {
		const BASE_OFFSET c_GameManager = 0x6E3FF30;
		const BASE_OFFSET c_UnlockAllOffset = 0x3843080;

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

		const BASE_ASM_PATCH c_UnlockAll = {
			c_UnlockAllOffset,
			{ 0x41, 0xB4, 0x00, 0x90 }, // mov r12l, 00 ; nop
			{ 0x41, 0x80, 0xF4, 0x01 }, // xor r12l, 01
			4
		};
	}
}

#endif // !__CONST_H__