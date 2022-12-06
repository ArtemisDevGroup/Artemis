#include "DrawManager.h"
#include "..\SafeMemory.h"

#include <stdio.h>

namespace Artemis {
	void IDraw::DrawLine(
		_In_ Vector2<> v2Point1,
		_In_ Vector2<> v2Point2,
		_In_ Vector4<> v4Color,
		_In_ float fThickness
	) {
		if (this->pDrawList) {
			this->pDrawList->AddLine(
				{ v2Point1.X(), v2Point1.Y() },
				{ v2Point2.X(), v2Point2.Y() },
				ImGui::ColorConvertFloat4ToU32({
						v4Color.X() / 255.0f,
						v4Color.Y() / 255.0f,
						v4Color.Z() / 255.0f,
						v4Color.W() / 255.0f
					}),
				fThickness
			);
		}
	}

	void IDraw::DrawRect(
		_In_ Vector2<> v2UpperLeftPoint,
		_In_ Vector2<> v2LowerRightPoint,
		_In_ Vector4<> v4Color,
		_In_ float fRounding,
		_In_ ImDrawCornerFlags nCornerRounding,
		_In_ float fThickness
	) {
		if (this->pDrawList) {
			this->pDrawList->AddRect(
				{ v2UpperLeftPoint.X(), v2UpperLeftPoint.Y() },
				{ v2LowerRightPoint.X(), v2LowerRightPoint.Y() },
				ImGui::ColorConvertFloat4ToU32({
						v4Color.X() / 255.0f,
						v4Color.Y() / 255.0f,
						v4Color.Z() / 255.0f,
						v4Color.W() / 255.0f
					}),
				fThickness
			);
		}
	}

	void IDraw::DrawRectFilled(
		_In_ Vector2<> v2UpperLeftPoint,
		_In_ Vector2<> v2LowerRightPoint,
		_In_ Vector4<> v4Color,
		_In_ float fRounding,
		_In_ ImDrawCornerFlags nCornerRounding
	) {
		if (this->pDrawList) {
			this->pDrawList->AddRectFilled(
				{ v2UpperLeftPoint.X(), v2UpperLeftPoint.Y() },
				{ v2LowerRightPoint.X(), v2LowerRightPoint.Y() },
				ImGui::ColorConvertFloat4ToU32({
						v4Color.X() / 255.0f,
						v4Color.Y() / 255.0f,
						v4Color.Z() / 255.0f,
						v4Color.W() / 255.0f
					}));
		}
	}

	void IDraw::DrawQuad(
		_In_ Vector2<> v2Point1,
		_In_ Vector2<> v2Point2,
		_In_ Vector2<> v2Point3,
		_In_ Vector2<> v2Point4,
		_In_ Vector4<> v4Color,
		_In_ float fThickness
	) {
		if (this->pDrawList) {
			this->pDrawList->AddQuad(
				{ v2Point1.X(), v2Point1.Y() },
				{ v2Point2.X(), v2Point2.Y() },
				{ v2Point3.X(), v2Point3.Y() },
				{ v2Point4.X(), v2Point4.Y() },
				ImGui::ColorConvertFloat4ToU32({
					v4Color.X() / 255.0f,
					v4Color.Y() / 255.0f,
					v4Color.Z() / 255.0f,
					v4Color.W() / 255.0f
					}),
				fThickness
			);
		}
	}

	void IDraw::DrawQuadFilled(
		_In_ Vector2<> v2Point1,
		_In_ Vector2<> v2Point2,
		_In_ Vector2<> v2Point3,
		_In_ Vector2<> v2Point4,
		_In_ Vector4<> v4Color
	) {
		if (this->pDrawList) {
			this->pDrawList->AddQuadFilled(
				{ v2Point1.X(), v2Point1.Y() },
				{ v2Point2.X(), v2Point2.Y() },
				{ v2Point3.X(), v2Point3.Y() },
				{ v2Point4.X(), v2Point4.Y() },
				ImGui::ColorConvertFloat4ToU32({
					v4Color.X() / 255.0f,
					v4Color.Y() / 255.0f,
					v4Color.Z() / 255.0f,
					v4Color.W() / 255.0f
					}));
		}
	}

	void IDraw::DrawTriangle(
		_In_ Vector2<> v2Point1,
		_In_ Vector2<> v2Point2,
		_In_ Vector2<> v2Point3,
		_In_ Vector4<> v4Color,
		_In_ float fThickness
	) {
		if (this->pDrawList) {
			this->pDrawList->AddTriangle(
				{ v2Point1.X(), v2Point1.Y() },
				{ v2Point2.X(), v2Point2.Y() },
				{ v2Point3.X(), v2Point3.Y() },
				ImGui::ColorConvertFloat4ToU32({
					v4Color.X() / 255.0f,
					v4Color.Y() / 255.0f,
					v4Color.Z() / 255.0f,
					v4Color.W() / 255.0f
					}),
				fThickness
			);
		}
	}

	void IDraw::DrawTriangleFilled(
		_In_ Vector2<> v2Point1,
		_In_ Vector2<> v2Point2,
		_In_ Vector2<> v2Point3,
		_In_ Vector4<> v4Color
	) {
		if (this->pDrawList) {
			this->pDrawList->AddTriangleFilled(
				{ v2Point1.X(), v2Point1.Y() },
				{ v2Point2.X(), v2Point2.Y() },
				{ v2Point3.X(), v2Point3.Y() },
				ImGui::ColorConvertFloat4ToU32({
					v4Color.X() / 255.0f,
					v4Color.Y() / 255.0f,
					v4Color.Z() / 255.0f,
					v4Color.W() / 255.0f
					}));
		}
	}

	void IDraw::DrawCircle(
		_In_ Vector2<> v2CenterPoint,
		_In_ float fRadius,
		_In_ Vector4<> v4Color,
		_In_ int nSegments,
		_In_ float fThickness
	) {
		if (this->pDrawList) {
			this->pDrawList->AddCircle(
				{ v2CenterPoint.X(), v2CenterPoint.Y() },
				fRadius,
				ImGui::ColorConvertFloat4ToU32({
					v4Color.X() / 255.0f,
					v4Color.Y() / 255.0f,
					v4Color.Z() / 255.0f,
					v4Color.W() / 255.0f
					}),
				nSegments,
				fThickness
			);
		}
	}

	void IDraw::DrawCircleFilled(
		_In_ Vector2<> v2CenterPoint,
		_In_ float fRadius,
		_In_ Vector4<> v4Color,
		_In_ int nSegments
	) {
		if (this->pDrawList) {
			this->pDrawList->AddCircleFilled(
				{ v2CenterPoint.X(), v2CenterPoint.Y() },
				fRadius,
				ImGui::ColorConvertFloat4ToU32({
					v4Color.X() / 255.0f,
					v4Color.Y() / 255.0f,
					v4Color.Z() / 255.0f,
					v4Color.W() / 255.0f
					}),
				nSegments
			);
		}
	}

	void IDraw::DrawNgon(
		_In_ Vector2<> v2CenterPoint,
		_In_ float fRadius,
		_In_ Vector4<> v4Color,
		_In_ int nSegments,
		_In_ float fThickness
	) {
		if (this->pDrawList) {
			this->pDrawList->AddNgon(
				{ v2CenterPoint.X(), v2CenterPoint.Y() },
				fRadius,
				ImGui::ColorConvertFloat4ToU32({
					v4Color.X() / 255.0f,
					v4Color.Y() / 255.0f,
					v4Color.Z() / 255.0f,
					v4Color.W() / 255.0f
					}),
				nSegments,
				fThickness
			);
		}
	}

	void IDraw::DrawNgonFilled(
		_In_ Vector2<> v2CenterPoint,
		_In_ float fRadius,
		_In_ Vector4<> v4Color,
		_In_ int nSegments
	) {
		if (this->pDrawList) {
			this->pDrawList->AddNgonFilled(
				{ v2CenterPoint.X(), v2CenterPoint.Y() },
				fRadius,
				ImGui::ColorConvertFloat4ToU32({
					v4Color.X() / 255.0f,
					v4Color.Y() / 255.0f,
					v4Color.Z() / 255.0f,
					v4Color.W() / 255.0f
					}),
				nSegments
			);
		}
	}

	void IDraw::DrawPolyline(
		_In_ List<Vector2<>>& PointsList,
		_In_ Vector4<> v4Color,
		_In_ BOOL bClosed,
		_In_ float fThickness
	) {
		if (this->pDrawList) {
			List<ImVec2> ImPointsList;
			for (INT i = 0; i < PointsList.GetCount(); i++)
				ImPointsList.Add(ImVec2(PointsList[i].X(), PointsList[i].Y()));

			this->pDrawList->AddPolyline(
				ImPointsList.GetPtr(),
				ImPointsList.GetCount(),
				ImGui::ColorConvertFloat4ToU32({
					v4Color.X() / 255.0f,
					v4Color.Y() / 255.0f,
					v4Color.Z() / 255.0f,
					v4Color.W() / 255.0f
					}),
				bClosed,
				fThickness
			);

			ImPointsList.Release();
		}
	}

	void IDraw::Present(_In_ ImDrawList* pDrawList) {
		this->pDrawList = pDrawList;
		Draw();
		this->pDrawList = nullptr;
	}

	DrawManager::DrawManager() { memset(lpszDrawArray, 0x00, sizeof(lpszDrawArray)); }

	void DrawManager::RegisterDraw(_In_ IDraw* lpDrawInst) {
		CONTEXT_BEGIN;

		if (!lpDrawInst) throw ParameterException("lpDrawInst");

		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (lpszDrawArray[i])
				if (lpszDrawArray[i]->dwDrawId == lpDrawInst->dwDrawId)
					throw AttributeException("dwDrawId");
		}

		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (!lpszDrawArray[i]) {
				lpszDrawArray[i] = lpDrawInst;
				lpDrawInst->dwRegisteredCount++;
				break;
			}
		}

		CONTEXT_END;
	}

	void DrawManager::UnregisterDraw(_In_ IDraw* lpDrawInst) {
		CONTEXT_BEGIN;

		BOOL bUnregistered = FALSE;

		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (lpszDrawArray[i] == lpDrawInst) {
				lpszDrawArray[i]->dwRegisteredCount--;
				lpszDrawArray[i] = nullptr;
				bUnregistered = TRUE;
				break;
			}
		}

		if (!bUnregistered) throw ObjectNotFoundException("lpDrawInst", "IDraw*");

		CONTEXT_END;
	}

	void DrawManager::Release() {
		for (INT i = 0; i < MAX_INVOKE; i++) {
			if (lpszDrawArray[i]) {
				if (lpszDrawArray[i]->dwRegisteredCount <= 1) SafeDelete(lpszDrawArray[i]);
				else lpszDrawArray[i]->dwRegisteredCount--;
				lpszDrawArray[i] = nullptr;
			}
		}
	}

	void DrawManager::InvokeDraw(_In_ ImDrawList* pDrawList) {
		for (INT i = 0; i < MAX_INVOKE; i++)
			if (lpszDrawArray[i])
				lpszDrawArray[i]->Present(pDrawList);
	}

	_Check_return_ _Ret_maybenull_ IDraw* DrawManager::GetDrawById(_In_ DWORD dwDrawId) {
		CONTEXT_BEGIN;

		IDraw* lpReturn = nullptr;

		for (INT i = 0; i < MAX_INVOKE; i++)
			if (lpszDrawArray[i])
				if (lpszDrawArray[i]->dwDrawId == dwDrawId)
					lpReturn = lpszDrawArray[i];

		if (!lpReturn) {
			CHAR szBuffer[MAX_NAME];
			sprintf_s(szBuffer, "Draw instance belonging to id %lu", dwDrawId);
			throw ObjectNotFoundException(szBuffer, "IDraw*");
		}

		CONTEXT_END;

		return lpReturn;
	}
}