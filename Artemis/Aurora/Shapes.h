//------------------------------------------------------------------------>
// MIT License
// 
// Copyright (c) 2023 Artemis Group
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------>
// Aurora: https://github.com/ArtemisDevGroup/Aurora
// This file was authored by:
// @Astrea0014: https://github.com/Astrea0014
//------------------------------------------------------------------------>

#ifndef __AURORA_SHAPES_H__
#define __AURORA_SHAPES_H__

#include "Definitions.h"
#include "Vector.h"
#include "Color.h"
#include "ConstexprMath.h"
#include "Array.h"

namespace Aurora {
	/// <summary>
	/// A point on a screen.
	/// </summary>
	struct Point {
		A_I32 x; // The x coordinate of the point.
		A_I32 y; // The y coordinate of the point.

		constexpr Point() noexcept : x(0), y(0) {}
		constexpr Point(_In_ const A_I32 x, _In_ const A_I32 y) noexcept : x(x), y(y) {}

		/// <summary>
		/// Gets the distance between the current point and the referenced point.
		/// </summary>
		/// <param name="refPoint">- A constant reference to a point.</param>
		/// <returns>The distance between the two points.</returns>
		AURORA_NDWR_PURE("Distance") constexpr A_FL64 Distance(_In_ const Point& refPoint) const noexcept { return SquareRoot(Squared(x - refPoint.x) + Squared(y - refPoint.y)); }
	};

	/// <summary>
	/// A straight line between two points.
	/// </summary>
	struct Line {
		Array<Point, 2> szptPoints; // The beginning and end points of the line.
		RGBA Color; // The color of the line.
		A_FL32 fThickness; // The thickness of the line.

		constexpr Line() noexcept : szptPoints(), Color(), fThickness(0) {}

		/// <summary>
		/// Constructs a Line.
		/// </summary>
		/// <param name="ptPoint1">- A constant reference to the beginning point of the line.</param>
		/// <param name="ptPoint2">- A constant reference to the end point of the line.</param>
		/// <param name="Color">- The color of the line.</param>
		/// <param name="fThickness">- The thickness of the line.</param>
		constexpr Line(
			_In_ const Point& ptPoint1,
			_In_ const Point& ptPoint2,
			_In_ const RGBA& Color,
			_In_ const A_FL32 fThickness = 1.0f
		) noexcept : szptPoints({ ptPoint1, ptPoint2 }), Color(Color), fThickness(fThickness) {}

		/// <summary>
		/// Gets the cofficient of the slope.
		/// </summary>
		/// <returns>The cofficient of the slope.</returns>
		AURORA_NDWR_PURE("SlopeCofficient") constexpr A_FL64 SlopeCofficient() const noexcept { return Delta(szptPoints[0].y, szptPoints[1].y) / Delta(szptPoints[0].x, szptPoints[1].x); }

		/// <summary>
		/// Checks if the line is parallel with another referenced line.
		/// </summary>
		/// <param name="refLine">- A constant reference to a line.</param>
		/// <returns>True if the lines are parallel, otherwise false.</returns>
		AURORA_NDWR_PURE("IsParallel") constexpr A_BOOL IsParallel(const Line& refLine) const noexcept { return this->SlopeCofficient() - refLine.SlopeCofficient() == 0.0; }
	};

	// Static polyline struct. Number of points decided on compile time. Fully constexpr compatible.

	/// <summary>
	/// A line between multiple points.
	/// </summary>
	/// <typeparam name="nPointCount">- The number of points.</typeparam>
	template<A_I32 nPointCount = 0>
		requires(nPointCount > 2 || nPointCount == 0)
	struct PolyLine {
		Array<Point, nPointCount> szptPoints; // An array of points to draw the line between.
		RGBA Color; // The color of the line.
		A_FL32 fThickness; // The thickness of the line.

		constexpr PolyLine() noexcept : szptPoints(), Color(), fThickness(0) {}

		/// <summary>
		/// Constructs a Polyline.
		/// </summary>
		/// <param name="szptPoints">- The points to draw the line between.</param>
		/// <param name="Color">- The color of the line.</param>
		/// <param name="fThickness">- The thickness of the line.</param>
		constexpr PolyLine(
			_In_ const Array<Point, nPointCount>& szptPoints,
			_In_ const RGBA& Color,
			_In_ const A_FL32 fThickness = 1.0f
		) noexcept : szptPoints(szptPoints), Color(Color), fThickness(fThickness) {}
	};

	// Dynamic polyline struct. Number of points decided on runtime. Not constexpr compatible.

	/// <summary>
	/// A line between multiple points.
	/// </summary>
	template<>
	struct PolyLine<0> {
		List<Point> szptPoints; // A list of points to draw the line between.
		RGBA Color; // The color of the line.
		A_FL32 fThickness; // The thickness of the line.

		inline PolyLine() noexcept : szptPoints(), Color(), fThickness(0) {}

		/// <summary>
		/// Constructs a Polyline.
		/// </summary>
		/// <param name="szptPoints">- The points to draw the line between.</param>
		/// <param name="Color">- The color of the line.</param>
		/// <param name="fThickness">- The thickness of the line.</param>
		inline PolyLine(
			_In_ const List<Point>& szptPoints,
			_In_ const RGBA& Color,
			_In_ const A_FL32 fThickness = 1.0f
		) noexcept : szptPoints(szptPoints), Color(Color), fThickness(fThickness) {}
	};

	/// <summary>
	/// A rectangle between two points.
	/// </summary>
	struct Rectangle {
		Point ptTopLeftPoint; // The top-left point in the rectangle.
		Point ptBottomRightPoint; // The bottom-right point in the rectangle.
		RGBA Color; // The color of the rectangle.
		A_BOOL bFilled; // A bool that determines whether the rectangle shall be drawn as a solid or as an outline.
		A_FL32 fThickness; // The thickness of the rectangle borders.

		constexpr Rectangle() noexcept : ptTopLeftPoint(), ptBottomRightPoint(), Color(), bFilled(false), fThickness(0) {}

		/// <summary>
		/// Constructs a Rectangle.
		/// </summary>
		/// <param name="ptTopLeftPoint">- The top-left corner of the rectangle.</param>
		/// <param name="ptBottomRightPoint">- The bottom-right corner of the rectangle.</param>
		/// <param name="Color">- The color of the rectangle.</param>
		/// <param name="bFilled">- A bool that determines whether the rectangle shall be drawn as a solid or as an outline.</param>
		/// <param name="fThickness">- The thickness of the rectangle borders.</param>
		constexpr Rectangle(
			_In_ const Point& ptTopLeftPoint,
			_In_ const Point& ptBottomRightPoint,
			_In_ const RGBA& Color,
			_In_ const A_BOOL bFilled = false,
			_In_ const A_FL32 fThickness = 1.0f
		) noexcept : ptTopLeftPoint(ptTopLeftPoint),
			ptBottomRightPoint(ptBottomRightPoint),
			Color(Color),
			bFilled(bFilled),
			fThickness(fThickness) {}

		/// <summary>
		/// Calculates the area of the rectangle.
		/// </summary>
		/// <returns>The rectangle area.</returns>
		AURORA_NDWR_PURE("Area") constexpr A_I64 Area() const noexcept { return AbsoluteN(ptTopLeftPoint.x - ptBottomRightPoint.x) * AbsoluteN(ptTopLeftPoint.y - ptBottomRightPoint.y); }
	};

	/// <summary>
	/// A quad polygon.
	/// </summary>
	struct Quad {
		Array<Point, 4> szptPoints; // An array of the points in the quad.
		RGBA Color; // The color of the quad.
		A_BOOL bFilled; // A bool that determines whether the quad shall be drawn as a solid or as an outline.
		A_FL32 fThickness; // The thickness of the quad borders.

		constexpr Quad() noexcept : szptPoints(), Color(), bFilled(false), fThickness(0) {}

		/// <summary>
		/// Constructs a Quad.
		/// </summary>
		/// <param name="ptPoint1">- The first point in the quad.</param>
		/// <param name="ptPoint2">- The second point in the quad.</param>
		/// <param name="ptPoint3">- The third point in the quad.</param>
		/// <param name="ptPoint4">- The fourth point in the quad.</param>
		/// <param name="Color">- The color of the quad.</param>
		/// <param name="bFilled">- A bool that determines whether the quad shall be drawn as a solid or as an outline.</param>
		/// <param name="fThickness">- The thickness of the quad borders.</param>
		constexpr Quad(
			_In_ const Point& ptPoint1,
			_In_ const Point& ptPoint2,
			_In_ const Point& ptPoint3,
			_In_ const Point& ptPoint4,
			_In_ const RGBA& Color,
			_In_ const A_BOOL bFilled = false,
			_In_ const A_FL32 fThickness = 1.0f
		) noexcept : szptPoints({ ptPoint1, ptPoint2, ptPoint3, ptPoint4 }),
			Color(Color),
			bFilled(bFilled),
			fThickness(fThickness) {}
	};

	/// <summary>
	/// A triangle between three points.
	/// </summary>
	struct Triangle {
		Array<Point, 3> szptPoints; // An array of the points in the triangle.
		RGBA Color; // The color of the triangle.
		A_BOOL bFilled; // A bool that determines whether the triangle shall be drawn as a solid or as an outline.
		A_FL32 fThickness; // The thickness of the triangle borders.

		constexpr Triangle() noexcept : szptPoints(), Color(), bFilled(false), fThickness(0) {}

		/// <summary>
		/// Constructs a Triangle.
		/// </summary>
		/// <param name="ptPoint1">- The first point in the triangle.</param>
		/// <param name="ptPoint2">- The second point in the triangle.</param>
		/// <param name="ptPoint3">- The third point in the triangle.</param>
		/// <param name="ptPoint4">- The fourth point in the triangle.</param>
		/// <param name="Color">- The color of the triangle.</param>
		/// <param name="bFilled">- A bool that determines whether the triangle shall be drawn as a solid or as an outline.</param>
		/// <param name="fThickness">- The thickness of the triangle borders.</param>
		constexpr Triangle(
			_In_ const Point& ptPoint1,
			_In_ const Point& ptPoint2,
			_In_ const Point& ptPoint3,
			_In_ const RGBA& Color,
			_In_ const A_BOOL bFilled = false,
			_In_ const A_FL32 fThickness = 1.0f
		) noexcept : szptPoints({ ptPoint1, ptPoint2, ptPoint3 }),
			Color(Color),
			bFilled(bFilled),
			fThickness(fThickness) {}

		/// <summary>
		/// Calculates whether the triangle has a right angle or not.
		/// </summary>
		/// <returns>True if the triangle is right angled, otherwise false.</returns>
		AURORA_NDWR_PURE("IsRightAngled") constexpr A_BOOL IsRightAngled() const noexcept {
			A_I32 nL12Sq = SquaredN(szptPoints[0].x - szptPoints[1].x) +  SquaredN(szptPoints[0].y - szptPoints[1].y);
			A_I32 nL13Sq = SquaredN(szptPoints[0].x - szptPoints[2].x) +  SquaredN(szptPoints[0].y - szptPoints[2].y);
			A_I32 nL23Sq = SquaredN(szptPoints[1].x - szptPoints[2].x) +  SquaredN(szptPoints[1].y - szptPoints[2].y);

			if (nL12Sq > nL13Sq && nL12Sq > nL23Sq && nL13Sq + nL23Sq == nL12Sq) return true;
			else if (nL13Sq > nL12Sq && nL13Sq > nL23Sq && nL12Sq + nL23Sq == nL13Sq) return true;
			else if (nL23Sq > nL12Sq && nL23Sq > nL13Sq && nL12Sq + nL13Sq == nL23Sq) return true;
			else return false;
		}

		/// <summary>
		/// Gets the length of a side in the triangle.
		/// </summary>
		/// <param name="nFirstPointIndex">- The index of the first point of the line.</param>
		/// <param name="nSecondPointIndex">- The index of the second point of the line.</param>
		/// <returns>The length of the side.</returns>
		AURORA_NDWR_PURE("GetSideLength") constexpr A_FL64 GetSideLength(_In_range_(0, 2) A_I32 nFirstPointIndex, _In_range_(0, 2) A_I32 nSecondPointIndex) const noexcept { return szptPoints[nFirstPointIndex].Distance(szptPoints[nSecondPointIndex]); }
	};

	struct Circle {
		Point ptCenterPoint; // The center point of the circle.
		A_I32 nRadius; // The radius of the circle.
		A_I32 nSegments; // The segment (slice) count of the circle.
		RGBA Color; // The color of the circle.
		A_BOOL bFilled; // A bool that determines whether the circle shall be drawn as a solid or as an outline.
		A_FL32 fThickness; // The thickness of the circle border.

		constexpr Circle() noexcept : ptCenterPoint(), nRadius(0), nSegments(0), Color(), bFilled(false), fThickness(0) {}

		/// <summary>
		/// Constructs a Circle.
		/// </summary>
		/// <param name="ptCenterPoint">- The center point of the circle.</param>
		/// <param name="nRadius">- The radius of the circle.</param>
		/// <param name="nSegments">- The segment (slice) count of the circle.</param>
		/// <param name="Color">- The color of the circle.</param>
		/// <param name="bFilled">- A bool that determines whether the circle shall be drawn as a solid or as an outline.</param>
		/// <param name="fThickness">- The thickness of the circle border.</param>
		constexpr Circle(
			_In_ const Point& ptCenterPoint,
			_In_ const A_I32 nRadius,
			_In_ const A_I32 nSegments,
			_In_ const RGBA& Color,
			_In_ const A_BOOL bFilled = false,
			_In_ const A_FL32 fThickness = 1.0f
		) noexcept : ptCenterPoint(ptCenterPoint),
			nRadius(nRadius),
			nSegments(nSegments),
			Color(Color),
			bFilled(bFilled),
			fThickness(fThickness) {}

		/// <summary>
		/// Calculates the area of the circle.
		/// </summary>
		/// <returns>The circle area.</returns>
		AURORA_NDWR_PURE("Area") constexpr A_FL64 Area() const noexcept { return Pi * Squared(nRadius); }

		/// <summary>
		/// Calculates the diameter of the circle.
		/// </summary>
		/// <returns>The circle diameter.</returns>
		AURORA_NDWR_PURE("Diameter") constexpr A_I32 Diameter() const noexcept { return nRadius * 2; }
	};
}

#endif // !__AURORA_SHAPES_H__