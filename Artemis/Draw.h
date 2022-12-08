//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014 and @Monke-exe.									|
// @Sigma0014: https://github.com/Sigma0014												|
// @Monke-exe:	https://github.com/Monke-exe											|
//-------------------------------------------------------------------------------------->

#ifndef __DRAW_H__
#define __DRAW_H__

#include <xmmintrin.h>

#include <ArtemisSpecific/Midnight.h>
#include <Vector.h>

using namespace Artemis;

class BoneDraw : public IDraw {
	ADDRESS uEntity;

	void TransformsCalculation(
		_In_ __int64 pBones,
		_Out_ __m128* ResultPosition,
		_In_ __m128* BoneInfo
	);

	BOOL WorldToScreen(
		_In_ Vector3<> v3World,
		_Out_opt_ Vector2<>* lpScreenPos = nullptr
	);

	BOOL GetBones(
		_In_ ADDRESS uEntity,
		_Out_writes_opt_(27) UINT8* lpBoneIds = nullptr,
		_Out_writes_opt_(27) Vector3<>* lpBonePositions = nullptr
	);

public:
	BoneDraw(_In_ ADDRESS uEntity);

	static Vector4<>& Color();

	void Draw();
};

#endif // !__DRAW_H__