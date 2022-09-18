#ifndef __OFFSETS_H__
#define __OFFSETS_H__

#include "SDK/Def.h"
#include "SDK/MemShApi.h"
typedef OFFSET PTR, * LPPTR;

#include "ToggleBytePatch.h"

namespace asmp {
	const ASSEMBLY_PATCH c_UnlockAll = {
		0x3843080,
		{ 0x48, 0xB4, 0x00, 0x90 }, // mov r12l,00 -> nop
		{ 0x48, 0x80, 0xF4, 0x01 }, // xor r12l,01
		4
	};

	const ASSEMBLY_PATCH c_SimpleOutlines1 = {
		0x375C37F,
		{ 0x75 }, // jne
		{ 0x74 }, // je
		1
	};

	const ASSEMBLY_PATCH c_SimpleOutlines2 = {
		0x375BE7C,
		{ 0x75 }, // jne
		{ 0x74 }, // je
		1
	};

	const ASSEMBLY_PATCH c_ForceSprint = {
		0x2523DF1,
		{ 0x80, 0xB9, 0x80, 0x00, 0x00, 0x00, 0x01 }, // cmp byte ptr [rcx+00000080],01
		{ 0x80, 0xB9, 0x80, 0x00, 0x00, 0x00, 0x00 }, // cmp byte ptr [rcx+00000080],00
		7
	};

	const ASSEMBLY_PATCH c_RunShoot = {
		0x36B263D,
		{ 0x80, 0xB9, 0x80, 0x00, 0x00, 0x00, 0x01 }, // cmp byte ptr [rcx+00000080],01
		{ 0x80, 0xB9, 0x80, 0x00, 0x00, 0x00, 0x00 }, // cmp byte ptr [rcx+00000080],00
		7
	};

	const ASSEMBLY_PATCH c_NoBolt = {
		0x1C7E2D1,
		{ 0xC6, 0x80, 0x92, 0x00, 0x00, 0x00, 0x00 }, // mov byte ptr [rax+00000092],00
		{ 0xC6, 0x80, 0x92, 0x00, 0x00, 0x00, 0x01 }, // mov byte ptr [rax+00000092],01
		7
	};

	const ASSEMBLY_PATCH c_Flight = {
		0x2EF1CCF,
		{ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }, // nop 6
		{ 0x88, 0x8E, 0x28, 0x0A, 0x00, 0x00 }, // mov [rsi+00000A28],cl
		6
	};

	const ASSEMBLY_PATCH c_NoClip = {
		0x4DAEB51,
		{ 0x44, 0x0F, 0x10, 0xA1, 0x18, 0x01, 0x00, 0x00 }, // movups xmm12,[rcx+00000118]
		{ 0x44, 0x0F, 0x10, 0xA1, 0xA0, 0x00, 0x00, 0x00 }, // movups xmm12,[rcx+000000A0]
		8
	};

	const ASSEMBLY_PATCH c_SoftWallBreak = {
		0x41F8D2B,
		{ 0x34, 0x00 }, // xor al,00
		{ 0x34, 0x01 }, // xor al,01
		2
	};
	
	const ASSEMBLY_PATCH c_CameraWriteInstruction = {
		0x9704D7,
		{ 0x90, 0x90, 0x90, 0x90, 0x90 }, // nop 5
		{ 0x44, 0x0F, 0x29, 0x5E, 0x30 }, // movaps [rsi+30],xmm11
		5
	};
}

namespace ofs {
	// Identifiable classes.
	const BASE_OFFSET c_GameManager = 0x6E3FF30; // GameManager : 48 8b 0d ? ? ? ? 80 b9 ? ? ? ? ? 74 ? 48 8b 01
	const BASE_OFFSET c_CameraManager = 0x5E22CA0; // CameraManager : 48 8d 0d ? ? ? ? 48 8b 14 c1 49 8b 1f
	const BASE_OFFSET c_LightManager = 0x5E1F4B8; // LightManager : 48 8b 3d ? ? ? ? 48 8b 05 ? ? ? ? 48 8d 9f
	const BASE_OFFSET c_GlowManager = 0x5E07858; // GlowManager : 48 8B 0D ? ? ? ? 48 85 C9 74 ? 48 89 F2 E8 ? ? ? ? 48 85 C0 74 ?
	const BASE_OFFSET c_ProfileManager = 0x5E32C50;

	// Unidentifiable objects.
	const BASE_OFFSET c_BuildBase = 0x6E34700;
	const BASE_OFFSET c_MatchTypeBase = 0x5DE3728;
	
	const BASE_OFFSET c_BuildStringCodeCave = 0x5A3426;
	const BASE_OFFSET c_MainBuildString = 0x55201EA;
	const BASE_OFFSET c_MainVersionString = 0x552D6ED;
	const BASE_OFFSET c_ToggleBuildStringCmp = 0x2F34905;
	const BASE_OFFSET c_LoadEffectiveAddressPatch = 0x653D70 + 0xA14C;

	const PTR_OFFSET c_Playlist = 0x68; // [RainbowSix.exe + c_GameManager] + c_Playlist
}

namespace ptr {
	const PTR c_GameState = { { 0x50 }, 1 }; // UINT32, c_GameManager
	const PTR c_Team = { { 0x8D0 }, 1 }; // UINT32, c_GameManager
	const PTR c_MatchType = { { 0x1B8 }, 1 }; // UINT32, c_MatchTypebase
}
#endif // !__OFFSETS_H__