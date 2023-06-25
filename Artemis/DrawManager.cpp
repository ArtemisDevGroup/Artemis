#include "pch.h"
#include "DrawManager.h"

namespace Artemis {
	namespace Helpers {
		ImVec2 PointToImVec2(_In_ const Aurora::Point& refPoint) noexcept {
			return ImVec2(static_cast<float>(refPoint.x), static_cast<float>(refPoint.y));
		}

		constexpr ImU32 RGBAToU32(_In_ const Aurora::RGBA& refColor) noexcept {
			ImU32 uRet = 0;
			ImU8* pBytes = (ImU8*)&uRet;
			pBytes[0] = static_cast<ImU8>(refColor.fR);
			pBytes[1] = static_cast<ImU8>(refColor.fG);
			pBytes[2] = static_cast<ImU8>(refColor.fB);
			pBytes[3] = static_cast<ImU8>(refColor.fA);

			return uRet;
		}
	}

	void IDraw::AddDraw(_In_ const Aurora::Line& refLine) {
		pDrawList->AddLine(
			Helpers::PointToImVec2(refLine.szptPoints[0]),
			Helpers::PointToImVec2(refLine.szptPoints[1]),
			Helpers::RGBAToU32(refLine.Color),
			refLine.fThickness
		);
	}

	void IDraw::AddDraw(_In_ const Aurora::Rectangle& refRectangle) {
		if (refRectangle.bFilled) {
			pDrawList->AddRectFilled(
				Helpers::PointToImVec2(refRectangle.ptTopLeftPoint),
				Helpers::PointToImVec2(refRectangle.ptBottomRightPoint),
				Helpers::RGBAToU32(refRectangle.Color),
				0.0F,
				ImDrawCornerFlags_All
			);
		}
		else {
			pDrawList->AddRect(
				Helpers::PointToImVec2(refRectangle.ptTopLeftPoint),
				Helpers::PointToImVec2(refRectangle.ptBottomRightPoint),
				Helpers::RGBAToU32(refRectangle.Color),
				0.0F,
				ImDrawCornerFlags_All,
				refRectangle.fThickness
			);
		}
	}

	void IDraw::AddDraw(_In_ const Aurora::Quad& refQuad) {
		if (refQuad.bFilled) {
			pDrawList->AddQuadFilled(
				Helpers::PointToImVec2(refQuad.szptPoints[0]),
				Helpers::PointToImVec2(refQuad.szptPoints[1]),
				Helpers::PointToImVec2(refQuad.szptPoints[2]),
				Helpers::PointToImVec2(refQuad.szptPoints[3]),
				Helpers::RGBAToU32(refQuad.Color)
			);
		}
		else {
			pDrawList->AddQuad(
				Helpers::PointToImVec2(refQuad.szptPoints[0]),
				Helpers::PointToImVec2(refQuad.szptPoints[1]),
				Helpers::PointToImVec2(refQuad.szptPoints[2]),
				Helpers::PointToImVec2(refQuad.szptPoints[3]),
				Helpers::RGBAToU32(refQuad.Color),
				refQuad.fThickness
			);
		}
	}

	void IDraw::AddDraw(_In_ const Aurora::Triangle& refTriangle) {
		if (refTriangle.bFilled) {
			pDrawList->AddTriangleFilled(
				Helpers::PointToImVec2(refTriangle.szptPoints[0]),
				Helpers::PointToImVec2(refTriangle.szptPoints[1]),
				Helpers::PointToImVec2(refTriangle.szptPoints[2]),
				Helpers::RGBAToU32(refTriangle.Color)
			);
		}
		else {
			pDrawList->AddTriangle(
				Helpers::PointToImVec2(refTriangle.szptPoints[0]),
				Helpers::PointToImVec2(refTriangle.szptPoints[1]),
				Helpers::PointToImVec2(refTriangle.szptPoints[2]),
				Helpers::RGBAToU32(refTriangle.Color),
				refTriangle.fThickness
			);
		}
	}

	void IDraw::AddDraw(_In_ const Aurora::Circle& refCircle) {
		if (refCircle.bFilled) {
			pDrawList->AddCircleFilled(
				Helpers::PointToImVec2(refCircle.ptCenterPoint),
				static_cast<float>(refCircle.nRadius),
				Helpers::RGBAToU32(refCircle.Color),
				refCircle.nSegments
			);
		}
		else {
			pDrawList->AddCircle(
				Helpers::PointToImVec2(refCircle.ptCenterPoint),
				static_cast<float>(refCircle.nRadius),
				Helpers::RGBAToU32(refCircle.Color),
				refCircle.nSegments,
				refCircle.fThickness
			);
		}
	}

	void IDraw::AddDraw(_In_ const Aurora::PolyLine<>& refPolyLine) {
		ImVec2* v = new ImVec2[refPolyLine.szptPoints.size()];

		for (int i = 0; i < refPolyLine.szptPoints.size(); i++)
			v[i] = Helpers::PointToImVec2(refPolyLine.szptPoints[i]);

		pDrawList->AddPolyline(
			v,
			refPolyLine.szptPoints.size(),
			Helpers::RGBAToU32(refPolyLine.Color),
			false,
			refPolyLine.fThickness
		);

		delete[] v;
	}

	constexpr IDraw::IDraw(bool bForeground) noexcept : bForeground(bForeground) {}

	constexpr bool IDraw::IsForeground() const noexcept { return bForeground; }

	void IDraw::Present(_Inout_ ImDrawList* pDrawList) {
		this->pDrawList = pDrawList;
		Draw();
	}

	void DrawManager::PresentAll(_Inout_ ImDrawList* pForegroundDrawList, _Inout_ ImDrawList* pBackgroundDrawList) {
		for (IDraw* pDraw : InvocableCollection)
			if (pDraw)
				pDraw->Present(pDraw->IsForeground() ? pForegroundDrawList : pBackgroundDrawList);
	}

	DrawManagerCollection::DrawManagerCollection() { ZeroMemory(DrawManagerArray, sizeof(DrawManagerArray)); }

	DrawManagerCollection::~DrawManagerCollection() { this->Release(); }

	DrawManagerIndex DrawManagerCollection::AddNew() {
		for (DrawManagerIndex i = 0; i < MAX_INVOKE; i++)
			if (!DrawManagerArray[i]) {
				DrawManagerArray[i] = new DrawManager();
				return i;
			}
		return INVALID_INDEX;
	}

	void DrawManagerCollection::Release(_In_range_(INVALID_INDEX, MAX_INVOKE) DrawManagerIndex nIndex) {
		if (nIndex == INVALID_INDEX) {
			for (DrawManager* pDrawManager : DrawManagerArray)
				if (pDrawManager)
					delete pDrawManager;
			memset(DrawManagerArray, 0, sizeof(DrawManagerArray));
		}
		else if (DrawManagerArray[nIndex]) {
			delete DrawManagerArray[nIndex];
			DrawManagerArray[nIndex] = nullptr;
		}
	}

	_Ret_maybenull_ DrawManager* DrawManagerCollection::Get(_In_range_(0, MAX_INVOKE) DrawManagerIndex nIndex) {
		if (nIndex > 64 || nIndex < 0) return nullptr;
		return DrawManagerArray[nIndex];
	}

	void DrawManagerCollection::PresentAll(_Inout_ ImDrawList* pForegroundDrawList, _Inout_ ImDrawList* pBackgroundDrawList) {
		for (DrawManager* pDrawManager : DrawManagerArray)
			if (pDrawManager)
				pDrawManager->PresentAll(pForegroundDrawList, pBackgroundDrawList);
	}
}