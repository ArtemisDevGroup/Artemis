#ifndef __DRAW_H__
#define __DRAW_H__

#include <xmmintrin.h>

#include <ArtemisSpecific/Midnight.h>
#include <Vector.h>

using namespace Artemis;

class BoneAndESPDraw : public IDraw {
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
	BoneAndESPDraw(_In_ ADDRESS uEntity);

	static Vector4<>& Color();

	void Draw();
};

#endif // !__DRAW_H__