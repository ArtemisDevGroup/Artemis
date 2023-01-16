//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Astrea0014.												|
// @Astrea0014: https://github.com/Astrea0014											|
//-------------------------------------------------------------------------------------->

#ifndef __CONST_H__
#define __CONST_H__

#include "..\Definitions.h"
#include "..\MemoryDefinitions.h"

namespace Artemis {
	namespace Constants {
		const BASE_OFFSET c_GameManager = 0x6E3FF30;					// 48 8b 0d ? ? ? ? 80 b9 ? ? ? ? ? 74 ? 48 8b 01
		const BASE_OFFSET c_CameraManager = 0x5E22CA0;					// 48 8d 0d ? ? ? ? 48 8b 14 c1 49 8b 1f

		const BASE_OFFSET c_HudRenderer = 0x7EBF5B8;					// 48 8b 05 ? ? ? ? f3 0f 6f 01

		typedef char(__fastcall* tSetGameTimer)(__int64 GameManager, int nMilliseconds, char a3);
		const BASE_OFFSET c_fnSetGameTimer = 0x213DC80;

		const POINTER_OFFSET c_PlaylistOffset = 0x68;

		const BasePointer<ADDRESS> c_Playlist				= { c_GameManager, { c_PlaylistOffset } };
		const BasePointer<ADDRESS> c_Map					= { c_GameManager, { c_PlaylistOffset, 0x10 } };
		const BasePointer<ADDRESS> c_MapForge				= { c_GameManager, { c_PlaylistOffset, 0x10, 0x10 } };
		const BasePointer<ADDRESS> c_Gamemode				= { c_GameManager, { c_PlaylistOffset, 0x8 } };
		const BasePointer<ADDRESS> c_GamemodeForge			= { c_GameManager, { c_PlaylistOffset, 0x8, 0x10 } };
		const BasePointer<ADDRESS> c_TimeOfDay				= { c_GameManager, { c_PlaylistOffset, 0x38, 0x0 } };
		const BasePointer<ADDRESS> c_TimeOfDayForge			= { c_GameManager, { c_PlaylistOffset, 0x38, 0x0, 0x10 } };
		const BasePointer<ADDRESS> c_TerroristDifficulty	= { c_GameManager, { c_PlaylistOffset, 0x30 } };

		const BasePointer<float> c_CameraX					= { c_CameraManager, { 0x18, 0x34 } };
		const BasePointer<float> c_CameraY					= { c_CameraManager, { 0x18, 0x38 } };
		const BasePointer<float> c_CameraZ					= { c_CameraManager, { 0x18, 0x30 } };

		const BaseAssemblyPatch c_FreezeCamera = {
			0x9704D7,
			{ 0x90, 0x90, 0x90, 0x90, 0x90 },	// nop 5
			{ 0x44, 0x0F, 0x29, 0x5E, 0x30 }	// movaps [rsi+30],xmm11
		};

		const BasePointer<float> c_FieldOfView				= { c_CameraManager, { 0x18, 0x118 } };

		const BaseAssemblyPatch c_FreezeFieldOfView = {
			0x993B21,
			{ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 },	// nop 6
			{ 0x89, 0x9A, 0x18, 0x01, 0x00, 0x00 }	// mov [rdx+00000118],ebx
		};

		const BasePointer<bool> c_ToggleHud					= { c_HudRenderer, { 0x19 } };

		const BaseAssemblyPatch c_UnlockAll = {
			0x3843080,
			{ 0x41, 0xB4, 0x00, 0x90 }, // mov r12l, 00 ; nop
			{ 0x41, 0x80, 0xF4, 0x01 }	// xor r12l, 01
		};
	}
}

#endif // !__CONST_H__