//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __DRAW_MANAGER_H__
#define __DRAW_MANAGER_H__

#include "..\Definitions.h"
#include "..\Interfaces.h"
#include "..\Exceptions.h"
#include "..\Vector.h"
#include "..\Array.h"
#include "..\ImGui\imgui.h"

namespace Artemis {
	/// <summary>
	/// An abstract class for easily drawing to the screen using ImGui.
	/// </summary>
	class ARTEMIS_API IDraw : public IRegisterable {
		ImDrawList* pDrawList;

	public:
		/// <summary>
		/// Sets the data fields.
		/// </summary>
		/// <param name="dwDrawId">- The draw action id. Has to be an unique id used by no other draw action. Id 0-39 are reserved.</param>
		inline IDraw(_In_ DWORD dwDrawId) : IRegisterable(), dwDrawId(dwDrawId), pDrawList(nullptr) {}

		const DWORD dwDrawId; // The draw action id.

		/// <summary>
		/// Draws a line to the screen.
		/// </summary>
		/// <param name="v2Point1">- The x and y coordinates of the first point.</param>
		/// <param name="v2Point2">- The x and y coordinates of the second point.</param>
		/// <param name="v4Color">- RGBA values ranging from 0 to 255 describing the color of the object drawn.</param>
		/// <param name="fThickness">- The object thickness.</param>
		void DrawLine(
			_In_ Vector2<> v2Point1,
			_In_ Vector2<> v2Point2,
			_In_ Vector4<> v4Color, 
			_In_ float fThickness = 1.0f
		);

		/// <summary>
		/// Draws a rectangle on the screen.
		/// </summary>
		/// <param name="v2UpperLeftPoint">- The x and y coordinates of the upper left point.</param>
		/// <param name="v2LowerRightPoint">- The x and y coordinates of the lower right point.</param>
		/// <param name="v4Color">- RGBA values ranging from 0 to 255 describing the color of the object drawn.</param>
		/// <param name="fRounding">- The corner rounding radius.</param>
		/// <param name="nCornerRounding">- The corner rounding mode. See ImDrawCornerFlags.</param>
		/// <param name="fThickness">- The object thickness.</param>
		void DrawRect(
			_In_ Vector2<> v2UpperLeftPoint,
			_In_ Vector2<> v2LowerRightPoint,
			_In_ Vector4<> v4Color,
			_In_ float fRounding = 0.0f,
			_In_ ImDrawCornerFlags nCornerRounding = ImDrawCornerFlags_All,
			_In_ float fThickness = 0.0f
		);

		/// <summary>
		/// Draws a filled rectangle on the screen.
		/// </summary>
		/// <param name="v2UpperLeftPoint">- The x and y coordinates of the upper left point.</param>
		/// <param name="v2LowerRightPoint">- The x and y coordinates of the lower right point.</param>
		/// <param name="v4Color">- RGBA values ranging from 0 to 255 describing the color of the object drawn.</param>
		/// <param name="fRounding">- The corner rounding radius.</param>
		/// <param name="nCornerRounding">- The corner rounding mode. See ImDrawCornerFlags.</param>
		void DrawRectFilled(
			_In_ Vector2<> v2UpperLeftPoint,
			_In_ Vector2<> v2LowerRightPoint,
			_In_ Vector4<> v4Color,
			_In_ float fRounding = 0.0f,
			_In_ ImDrawCornerFlags nCornerRounding = ImDrawCornerFlags_All
		);

		/// <summary>
		/// Draws a quadrangle on the screen.
		/// </summary>
		/// <param name="v2Point1">- The x and y coordinates of the first point.</param>
		/// <param name="v2Point2">- The x and y coordinates of the second point.</param>
		/// <param name="v2Point3">- The x and y coordinates of the third point.</param>
		/// <param name="v2Point4">- The x and y coordinates of the fourth point.</param>
		/// <param name="v4Color">- RGBA values ranging from 0 to 255 describing the color of the object drawn.</param>
		/// <param name="fThickness">- The object thickness.</param>
		void DrawQuad(
			_In_ Vector2<> v2Point1,
			_In_ Vector2<> v2Point2,
			_In_ Vector2<> v2Point3,
			_In_ Vector2<> v2Point4,
			_In_ Vector4<> v4Color,
			_In_ float fThickness = 1.0f
		);

		/// <summary>
		/// Draws a filled quadrangle on the screen.
		/// </summary>
		/// <param name="v2Point1">- The x and y coordinates of the first point.</param>
		/// <param name="v2Point2">- The x and y coordinates of the second point.</param>
		/// <param name="v2Point3">- The x and y coordinates of the third point.</param>
		/// <param name="v2Point4">- The x and y coordinates of the fourth point.</param>
		/// <param name="v4Color">- RGBA values ranging from 0 to 255 describing the color of the object drawn.</param>
		void DrawQuadFilled(
			_In_ Vector2<> v2Point1,
			_In_ Vector2<> v2Point2,
			_In_ Vector2<> v2Point3,
			_In_ Vector2<> v2Point4,
			_In_ Vector4<> v4Color
		);

		/// <summary>
		/// Draws a triangle on the screen.
		/// </summary>
		/// <param name="v2Point1">- The x and y coordinates of the first point.</param>
		/// <param name="v2Point2">- The x and y coordinates of the second point.</param>
		/// <param name="v2Point3">- The x and y coordinates of the third point.</param>
		/// <param name="v4Color">- RGBA values ranging from 0 to 255 describing the color of the object drawn.</param>
		/// <param name="fThickness">- The object thickness.</param>
		void DrawTriangle(
			_In_ Vector2<> v2Point1,
			_In_ Vector2<> v2Point2,
			_In_ Vector2<> v2Point3,
			_In_ Vector4<> v4Color,
			_In_ float fThickness = 1.0f
		);

		/// <summary>
		/// Draws a filled triangle on the screen.
		/// </summary>
		/// <param name="v2Point1">- The x and y coordinates of the first point.</param>
		/// <param name="v2Point2">- The x and y coordinates of the second point.</param>
		/// <param name="v2Point3">- The x and y coordinates of the third point.</param>
		/// <param name="v4Color">- RGBA values ranging from 0 to 255 describing the color of the object drawn.</param>
		void DrawTriangleFilled(
			_In_ Vector2<> v2Point1,
			_In_ Vector2<> v2Point2,
			_In_ Vector2<> v2Point3,
			_In_ Vector4<> v4Color
		);

		/// <summary>
		/// Draws a circle on the screen.
		/// </summary>
		/// <param name="v2CenterPoint">- The x and y coordinates of the center point.</param>
		/// <param name="fRadius">- The radius of the object.</param>
		/// <param name="v4Color">- RGBA values ranging from 0 to 255 describing the color of the object drawn.</param>
		/// <param name="nSegments">- The number of segments to give the object.</param>
		/// <param name="fThickness">- The object thickness.</param>
		void DrawCircle(
			_In_ Vector2<> v2CenterPoint,
			_In_ float fRadius,
			_In_ Vector4<> v4Color,
			_In_ int nSegments = 12,
			_In_ float fThickness = 1.0f
		);

		/// <summary>
		/// Draws a filled circle on the screen.
		/// </summary>
		/// <param name="v2CenterPoint">- The x and y coordinates of the center point.</param>
		/// <param name="fRadius">- The radius of the object.</param>
		/// <param name="v4Color">- RGBA values ranging from 0 to 255 describing the color of the object drawn.</param>
		/// <param name="nSegments">- The number of segments to give the object.</param>
		void DrawCircleFilled(
			_In_ Vector2<> v2CenterPoint,
			_In_ float fRadius,
			_In_ Vector4<> v4Color,
			_In_ int nSegments = 12
		);

		/// <summary>
		/// Draws a ngon on the screen.
		/// </summary>
		/// <param name="v2CenterPoint">- The x and y coordinates of the center point.</param>
		/// <param name="fRadius">- The radius of the object.</param>
		/// <param name="v4Color">- RGBA values ranging from 0 to 255 describing the color of the object drawn.</param>
		/// <param name="nSegments">- The number of segments to give the object.</param>
		/// <param name="fThickness">- The object thickness.</param>
		void DrawNgon(
			_In_ Vector2<> v2CenterPoint,
			_In_ float fRadius,
			_In_ Vector4<> v4Color,
			_In_ int nSegments,
			_In_ float fThickness = 1.0f
		);

		/// <summary>
		/// Draws a filled ngon on the screen.
		/// </summary>
		/// <param name="v2CenterPoint">- The x and y coordinates of the center point.</param>
		/// <param name="fRadius">- The radius of the object.</param>
		/// <param name="v4Color">- RGBA values ranging from 0 to 255 describing the color of the object drawn.</param>
		/// <param name="nSegments">- The number of segments to give the object.</param>
		void DrawNgonFilled(
			_In_ Vector2<> v2CenterPoint,
			_In_ float fRadius,
			_In_ Vector4<> v4Color,
			_In_ int nSegments
		);

		/// <summary>
		/// Draws a line with multiple points.
		/// </summary>
		/// <param name="PointsList">- A list of the x and y coordinates of every point.</param>
		/// <param name="v4Color">- RGBA values ranging from 0 to 255 describing the color of the object drawn.</param>
		/// <param name="bClosed">- Defines wether the polyline shall be open or closed.</param>
		/// <param name="fThickness">- The object thickness.</param>
		void DrawPolyline(
			_In_ List<Vector2<>>& PointsList,
			_In_ Vector4<> v4Color,
			_In_ BOOL bClosed,
			_In_ float fThickness = 1.0f
		);

		/// <summary>
		/// A purely virtual function that handles queuing the draw commands.
		/// </summary>
		virtual void Draw() = 0;

		/// <summary>
		/// Presents the queued objects from the Draw function.
		/// </summary>
		/// <param name="pDrawList">- A pointer to the foreground draw list.</param>
		void Present(_In_ ImDrawList* pDrawList);
	};

	/// <summary>
	/// A class for adding draw requests to the ImGui foreground draw list.
	/// </summary>
	class ARTEMIS_API DrawManager : public IDisposable {
		IDraw* lpszDrawArray[MAX_INVOKE];

	public:
		DrawManager();

		/// <summary>
		/// Registers a draw operation to be drawn on the screen.
		/// <para>The pointer MUST be to memory created with the "new" keyword.</para>
		/// </summary>
		/// <param name="lpDrawInst">- A pointer to the derived class.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="AttributeException"/>
		void RegisterDraw(_In_ IDraw* lpDrawInst);

		/// <summary>
		/// Unregisters a previously registered draw class instance.
		/// <para>The pointer MUST be to memory created with the "new" keyword.</para>
		/// </summary>
		/// <param name="lpDrawInst">- A pointer to the derived class.</param>
		/// <exception cref="ObjectNotFoundException"/>
		void UnregisterDraw(_In_ IDraw* lpDrawInst);

		/// <summary>
		/// Deletes all memory pointed to by the registered pointers.
		/// </summary>
		void Release();

		/// <summary>
		/// Invokes all registered draw operations.
		/// </summary>
		/// <param name="pDrawList>- The ImGui draw list to pass to the draw operations.</param>
		void InvokeDraw(_In_ ImDrawList* pDrawList) const;

		/// <summary>
		/// Gets the pointer to a draw instance by its unique identifier.
		/// </summary>
		/// <param name="dwDrawId">- The identifier of the draw instance to get.</param>
		/// <returns>A pointer to the associated object.</returns>
		/// <exception cref="ObjectNotFoundException"/>
		_Check_return_ _Ret_maybenull_ IDraw* GetDrawById(_In_ DWORD dwDrawId) const;

		/// <summary>
		/// Checks if a Draw Id is present in the list of currently registered objects.
		/// </summary>
		/// <param name="dwDrawId">- The identifier of the draw instance to check.</param>
		/// <returns>TRUE if the associated object could be found, otherwise FALSE.</returns>
		_Check_return_ BOOL IsIdPresent(_In_ DWORD dwDrawId) const;
	};
}

#endif // !__DRAW_MANAGER_H__