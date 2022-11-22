#ifndef __DRAW_H__
#define __DRAW_H__

#include <xmmintrin.h>

#include "ArtemisSpecific/Midnight.h"
#include "Vector.h"

using Artemis::Midnight;
using Artemis::IDraw;
using Artemis::Memory;
using Artemis::ADDRESS;
using Artemis::Vector2;
using Artemis::Vector3;
using Artemis::Vector4;
using Artemis::List;

class BoneAndESPDraw : public IDraw {
	ADDRESS uEntity;

	void TransformsCalculation(__int64 pBones, __m128* ResultPosition, __m128* BoneInfo);

	BOOL WorldToScreen(
		_In_ Vector3<> v3World,
		_Out_opt_ Vector2<>* lpScreenPos = nullptr
	);

	BOOL GetBones(
		_In_ ADDRESS uEntity,
		_Out_opt_ UINT8* lpBoneIds = nullptr,
		_Out_opt_ Vector3<>* lpBonePositions = nullptr
	);

public:
	BoneAndESPDraw(_In_ ADDRESS uEntity);

	static Vector4<>& Color();

	void Draw();
};

#endif // !__DRAW_H__