#ifndef __ARTEMIS_DRAW_MANAGER_H__
#define __ARTEMIS_DRAW_MANAGER_H__

#include <Aurora/Shapes.h>
#include <ImGui/imgui.h>

#include "Definitions.h"
#include "Manager.h"

namespace Artemis {
	namespace Helpers {
		ImVec2 PointToImVec2(_In_ const Aurora::Point& refPoint) noexcept;
		constexpr ImU32 RGBAToU32(_In_ const Aurora::RGBA& refColor) noexcept;
	}

	class IDraw {
		ImDrawList* pDrawList;
		bool bForeground;

	protected:
		void AddDraw(_In_ const Aurora::Line& refLine);
		void AddDraw(_In_ const Aurora::Rectangle& refRectangle);
		void AddDraw(_In_ const Aurora::Quad& refQuad);
		void AddDraw(_In_ const Aurora::Triangle& refTriangle);
		void AddDraw(_In_ const Aurora::Circle& refCircle);
		void AddDraw(_In_ const Aurora::PolyLine<>& refPolyLine);
		
		template<int nPointCount>
		inline void AddDraw(_In_ const Aurora::PolyLine<nPointCount>& refPolyLine) {
			ImVec2 v[nPointCount];
			
			for (int i = 0; i < nPointCount; i++)
				v[i] = Helpers::PointToImVec2(refPolyLine.szptPoints[i]);

			pDrawList->AddPolyline(
				v,
				nPointCount,
				Helpers::RGBAToU32(refPolyLine.Color),
				false,
				refPolyLine.fThickness
			);
		}

	public:
		constexpr IDraw(_In_ bool bForeground = true) noexcept;

		constexpr bool IsForeground() const noexcept;

		virtual void Draw() = 0;

		void Present(_Inout_ ImDrawList* pDrawList);
	};

	using DrawIndex = int;

#ifdef _ARTEMIS_EXPORT
	extern template class Manager<IDraw, DrawIndex>;
#else
	template class ARTEMIS_IMPORT Manager<IDraw, DrawIndex>;
#endif // _ARTEMIS_EXPORT

	class DrawManager : public Manager<IDraw, DrawIndex> {
	public:
		void PresentAll(_Inout_ ImDrawList* pForegroundDrawList, _Inout_ ImDrawList* pBackgroundDrawList);
	};

	using DrawManagerIndex = int;

	class DrawManagerCollection {
		DrawManager* DrawManagerArray[MAX_INVOKE];

	public:
		DrawManagerCollection();
		~DrawManagerCollection();

		DrawManagerIndex AddNew();

		void Release(_In_range_(INVALID_INDEX, MAX_INVOKE) DrawManagerIndex nIndex = INVALID_INDEX);

		_Ret_maybenull_ DrawManager* Get(_In_range_(0, MAX_INVOKE) DrawManagerIndex nIndex);

		void PresentAll(_Inout_ ImDrawList* pForegroundDrawList, _Inout_ ImDrawList* pBackgroundDrawList);
	};
}

#endif // !__ARTEMIS_DRAW_MANAGER_H__