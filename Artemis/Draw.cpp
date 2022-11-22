#include "Draw.h"

void BoneAndESPDraw::TransformsCalculation(__int64 pBones, __m128* ResultPosition, __m128* BoneInfo) {
	__m128 v5; // xmm2
	__m128 v6; // xmm3
	__m128 v7; // xmm0
	__m128 v8; // xmm4
	__m128 v9; // xmm1

	__m128 v10 = { 0.500f, 0.500f, 0.500f, 0.500f };
	__m128 v11 = { 2.000f, 2.000f, 2.000f, 0.000f };

	__m128 v12 = *(__m128*)(pBones);
	__m128 v13 = *(__m128*)(pBones + 0x10);

	v5 = v13;

	v6 = _mm_mul_ps(*(__m128*)BoneInfo, v5);
	v6.m128_f32[0] = v6.m128_f32[0]
		+ (float)(_mm_cvtss_f32(_mm_shuffle_ps(v6, v6, 0x55)) + _mm_cvtss_f32(_mm_shuffle_ps(v6, v6, 0xAA)));
	v7 = _mm_shuffle_ps(v13, v5, 0xFF);
	v8 = _mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(v5, v5, 0xD2), *(__m128*)BoneInfo),
		_mm_mul_ps(_mm_shuffle_ps(*(__m128*)BoneInfo, *(__m128*)BoneInfo, 0xD2), v5));
	v9 = _mm_shuffle_ps(v12, v12, 0x93);
	v9.m128_f32[0] = 0.0;
	*(__m128*)ResultPosition = _mm_add_ps(
		_mm_shuffle_ps(v9, v9, 0x39),
		_mm_mul_ps(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(_mm_shuffle_ps(v8, v8, 0xD2), v7),
					_mm_mul_ps(_mm_shuffle_ps(v6, v6, 0), v5)),
				_mm_mul_ps(
					_mm_sub_ps(_mm_mul_ps(v7, v7), (__m128)v10),
					*(__m128*)BoneInfo)),
			(__m128)v11));
}

BOOL BoneAndESPDraw::WorldToScreen(
	_In_ Vector3<> v3World,
	_Out_opt_ Vector2<>* lpScreenPos
) {
	Memory* pm = &Midnight::GetInst()->Mem;

	ADDRESS uPtr = *(ADDRESS*)(*(ADDRESS*)(*(ADDRESS*)(pm->GetModuleBase() + 0x5B1EEA8) + 0x40));

	Vector3<>
		v3Right = { *(float*)(uPtr + 0x1B0), *(float*)(uPtr + 0x1B4), *(float*)(uPtr + 0x1B8) },
		v3Up = { *(float*)(uPtr + 0x1C0), *(float*)(uPtr + 0x1C4), *(float*)(uPtr + 0x1C8) },
		v3Forward = { *(float*)(uPtr + 0x1D0), *(float*)(uPtr + 0x1D4), *(float*)(uPtr + 0x1D8) },
		v3Translation = { *(float*)(uPtr + 0x1E0), *(float*)(uPtr + 0x1E4), *(float*)(uPtr + 0x1E8) };

	Vector3<> v3Delta = v3World - v3Translation;

	float x = v3Delta.Dot(v3Right);
	float y = v3Delta.Dot(v3Up);
	float z = v3Delta.Dot(v3Forward * -1);

	float fovX = *(float*)(uPtr + 0x370);
	float fovY = *(float*)(uPtr + 0x374);

	INT nWidth = GetSystemMetrics(SM_CXSCREEN);
	INT nHeight = GetSystemMetrics(SM_CYSCREEN);

	Vector2<> v2ScreenPos = {
		(nWidth / 2.f) * (1.f + x / -fovX / z),
		(nHeight / 2.f) * (1.f - y / -fovY / z)
	};

	BOOL bOnScreen;
	if (v2ScreenPos.X() < 0.0f || v2ScreenPos.X() > nWidth) { bOnScreen = FALSE; }
	else bOnScreen = (z >= 1.0f);

	if (lpScreenPos) *lpScreenPos = v2ScreenPos;

	return bOnScreen;
}

BOOL BoneAndESPDraw::GetBones(
	_In_ ADDRESS uEntity,
	_Out_opt_ UINT8* lpBoneIds,
	_Out_opt_ Vector3<>* lpBonePositions
) {
	Artemis::DrawManager* pDraw = &Midnight::GetInst()->ESPDrawManager;
	Memory* pm = &Midnight::GetInst()->Mem;

	ADDRESS uPtr = *(ADDRESS*)(uEntity + 0x50);
	uPtr = ((_rotl64(uPtr, 0x1)) - 0x52) ^ 0x845C1A3CE5D1D3B7;

	if (!uPtr) {
		pDraw->UnregisterDraw(this);
		return FALSE;
	}

	uPtr = *(ADDRESS*)(uPtr + 0x18);

	INT8 nSkeletonIndex = pm->Read<INT8>(uPtr + 0x1CC);

	uPtr = *(ADDRESS*)(uPtr + 0xD8);
	uPtr = *(ADDRESS*)(uPtr + (nSkeletonIndex * 0x8));

	ADDRESS uBoneIds = *(ADDRESS*)(uPtr + 0xC8);
	BYTE uBoneCount = *(BYTE*)(uPtr + 0xD0);
	ADDRESS uBones = *(ADDRESS*)(uPtr + 0x270);
	ADDRESS uBoneData = *(ADDRESS*)(uBones + 0x58);

	for (INT i = 0; i < uBoneCount; i++) {
		UINT8 uBoneId = *(UINT8*)(uBoneIds + (i * 0x4));

		__m128 xmmBonePos;
		__m128 xmmBoneInfo = *(__m128*)(uBoneData + 0x20 * uBoneId);
		TransformsCalculation(uBones, &xmmBonePos, &xmmBoneInfo);

		if (lpBoneIds) lpBoneIds[i] = uBoneId;
		if (lpBonePositions) lpBonePositions[i] = { xmmBonePos.m128_f32[0], xmmBonePos.m128_f32[1], xmmBonePos.m128_f32[2] };
	}

	return TRUE;
}

BoneAndESPDraw::BoneAndESPDraw(_In_ ADDRESS uEntity) : IDraw(*(DWORD*)&uEntity), uEntity(uEntity) {}

Vector4<> v4Color = { 1 };

Vector4<>& BoneAndESPDraw::Color() { return v4Color; }

void BoneAndESPDraw::Draw() {
	Vector3<> szBones[27];
	if (GetBones(uEntity, nullptr, szBones)) {
		if (WorldToScreen(szBones[5])) {
			Vector2<> szRightLeg[4], szLeftLeg[4], szRightArm[4], szLeftArm[4], szSpine[4];
			Vector2<>* lpszArrays[5] = { szRightLeg, szLeftLeg, szRightArm, szLeftArm, szSpine };
			const INT nszIndexes[20] = { 20, 19, 18, 2, 24, 23, 22, -2, 10, 13, 14, 15, -10, 7, 8, 9, 11, -10, 5, -2 };

			for (INT i = 0; i < 20; i++) {
				if (nszIndexes[i] == -2)
					lpszArrays[i / 4][i % 4] = lpszArrays[0][3];
				else if (nszIndexes[i] == -10)
					lpszArrays[i / 4][i % 4] = lpszArrays[2][0];
				else
					WorldToScreen(
						szBones[nszIndexes[i]],
						&lpszArrays[i / 4][i % 4]
					);
			}

			for (INT i = 0; i < 5; i++) {
				List<Vector2<>> list(lpszArrays[i], 4);
				DrawPolyline(
					list,
					v4Color * 255,
					FALSE
				);
			}
		}
	}
	else delete this;
}