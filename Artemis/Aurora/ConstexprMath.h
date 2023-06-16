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

#ifndef __AURORA_CONSTEXPR_MATH_H__
#define __AURORA_CONSTEXPR_MATH_H__

#include <limits>

#include "Definitions.h"

namespace Aurora {
	constexpr A_FL32 PiF = 3.1415927f;
	constexpr A_FL64 Pi = 3.141592653589793;

	namespace Helpers {
		AURORA_NDWR_PURE("SquareRootNewtonRaphson") constexpr A_FL64 SquareRootNewtonRaphson(A_FL64 x, A_FL64 fCurrent, A_FL64 fPrevious) noexcept { return fCurrent == fPrevious ? fCurrent : SquareRootNewtonRaphson(x, 0.5 * (fCurrent + x / fCurrent), fCurrent); }

        AURORA_NDWR_PURE("InverseTangentSeriesOrderCalculation") constexpr A_FL64 InverseTangentSeriesOrderCalculation(A_FL64 x, A_FL64 xPower, A_U32 uOrder) noexcept { return(1.0 / ((A_FL64)((uOrder - 1) * 4 - 1) * xPower) - 1.0 / ((A_FL64)((uOrder - 1) * 4 + 1) * xPower * x)); }
		AURORA_NDWR_PURE("InverseTangentSeriesOrder") constexpr A_FL64 InverseTangentSeriesOrder(A_FL64 x, A_FL64 xPower, A_U32 uOrder, A_U32 uMaxOrder) noexcept { return(uOrder == 1 ? (Pi / 2.L) - 1.L / x + InverseTangentSeriesOrder(x * x, x * x * x, uOrder + 1, uMaxOrder) : uOrder < uMaxOrder ? InverseTangentSeriesOrderCalculation(x, xPower, uOrder) + InverseTangentSeriesOrder(x, xPower * x * x, uOrder + 1, uMaxOrder) : InverseTangentSeriesOrderCalculation(x, xPower, uOrder)); }

		AURORA_NDWR_PURE("InverseTangentSeries") constexpr A_FL64 InverseTangentSeries(A_FL64 x) noexcept {
            return(x < 3.L ? InverseTangentSeriesOrder(x, x, 1U, 10U) :
                x < 4.L ? InverseTangentSeriesOrder(x, x, 1U, 9U) :
                x < 5.L ? InverseTangentSeriesOrder(x, x, 1U, 8U) :
                x < 7.L ? InverseTangentSeriesOrder(x, x, 1U, 7U) :
                x < 11.L ? InverseTangentSeriesOrder(x, x, 1U, 6U) :
                x < 25.L ? InverseTangentSeriesOrder(x, x, 1U, 5U) :
                x < 100.L ? InverseTangentSeriesOrder(x, x, 1U, 4U) :  
                x < 1000.L ? InverseTangentSeriesOrder(x, x, 1U, 3U) : 
                InverseTangentSeriesOrder(x, x, 1U, 2U));
        }

		AURORA_NDWR_PURE("InverseTangentCFRecursive") constexpr A_FL64 InverseTangentCFRecursive(A_FL64 xx, A_U32 uDepth, A_U32 uMaxDepth) noexcept { return(uDepth < uMaxDepth ? (A_FL64)(2 * uDepth - 1) + uDepth * uDepth * xx / InverseTangentCFRecursive(xx, uDepth + 1, uMaxDepth) : (A_FL64)(2 * uDepth - 1)); }
		AURORA_NDWR_PURE("InverseTangentCF") constexpr A_FL64 InverseTangentCF(A_FL64 x) noexcept { return(x < 0.5L ? x / InverseTangentCFRecursive(x * x, 1U, 15U) : x < 1.L ? x / InverseTangentCFRecursive(x * x, 1U, 25U) : x < 1.5L ? x / InverseTangentCFRecursive(x * x, 1U, 35U) : x < 2.L ? x / InverseTangentCFRecursive(x * x, 1U, 45U) : x / InverseTangentCFRecursive(x * x, 1U, 52U)); }

		AURORA_NDWR_PURE("InverseTangentCalculate") constexpr A_FL64 InverseTangentCalculate(A_FL64 x) noexcept { return(x > 2.5L ? InverseTangentSeries(x) : InverseTangentCF(x)); }
	}

	AURORA_NDWR_PURE("SquaredF") constexpr A_FL32 SquaredF(A_FL32 x) noexcept { return x * x; }
	AURORA_NDWR_PURE("Squared") constexpr A_FL64 Squared(A_FL64 x) noexcept { return x * x; }
	AURORA_NDWR_PURE("SquaredN") constexpr A_I32 SquaredN(A_I32 x) noexcept { return x * x; }
	AURORA_NDWR_PURE("SquaredL") constexpr A_I64 SquaredL(A_I64 x) noexcept { return x * x; }
	AURORA_NDWR_PURE("SquareRootF") constexpr A_FL32 SquareRootF(A_FL32 x) noexcept { return x >= 0 && x < std::numeric_limits<float>::infinity() ? static_cast<A_FL32>(Helpers::SquareRootNewtonRaphson(x, x, 0)) : std::numeric_limits<float>::quiet_NaN(); }
	AURORA_NDWR_PURE("SquareRoot") constexpr A_FL64 SquareRoot(A_FL64 x) noexcept { return x >= 0 && x < std::numeric_limits<double>::infinity() ? Helpers::SquareRootNewtonRaphson(x, x, 0) : std::numeric_limits<double>::quiet_NaN(); }
	AURORA_NDWR_PURE("Max") constexpr A_FL64 Max(A_FL64 x, A_FL64 y) noexcept { return x > y ? x : y; }
	AURORA_NDWR_PURE("Min") constexpr A_FL64 Min(A_FL64 x, A_FL64 y) noexcept { return x < y ? x : y; }
	AURORA_NDWR_PURE("AbsoluteF") constexpr A_FL32 AbsoluteF(A_FL32 x) noexcept { return x >= 0.0 ? x : -x; }
	AURORA_NDWR_PURE("Absolute") constexpr A_FL64 Absolute(A_FL64 x) noexcept { return x >= 0.0 ? x : -x; }
	AURORA_NDWR_PURE("AbsoluteN") constexpr A_I32 AbsoluteN(A_I32 x) noexcept { return x >= 0 ? x : -x; }
	AURORA_NDWR_PURE("AbsoluteL") constexpr A_I64 AbsoluteL(A_I64 x) noexcept { return x >= 0 ? x : -x; }
	AURORA_NDWR_PURE("HalfF") constexpr A_FL32 HalfF(A_FL32 x) noexcept { return x / 2.0f; }
	AURORA_NDWR_PURE("Half") constexpr A_FL64 Half(A_FL64 x) noexcept { return x / 2.0; }
	AURORA_NDWR_PURE("RadiansToDegreesF") constexpr A_FL32 RadiansToDegreesF(A_FL32 x) noexcept { return x * 180.0f / PiF; }
	AURORA_NDWR_PURE("RadiansToDegrees") constexpr A_FL64 RadiansToDegrees(A_FL64 x) noexcept { return x * 180.0 / Pi; }
	AURORA_NDWR_PURE("Sign") constexpr A_FL64 Sign(A_FL64 x) noexcept { return x == 0 ? 0 : x > 0 ? 1 : x < 0 ? -1 : 0; }
	AURORA_NDWR_PURE("RoundF") constexpr A_FL32 RoundF(A_FL32 x) noexcept { return x - static_cast<A_FL32>(static_cast<A_I32>(x)) >= 0.5f ? static_cast<A_FL32>(static_cast<A_I32>(x)) + 1.f : static_cast<A_FL32>(static_cast<A_I32>(x)); }
	AURORA_NDWR_PURE("Round") constexpr A_FL64 Round(A_FL64 x) noexcept { return x - static_cast<A_FL64>(static_cast<A_I64>(x)) >= 0.5f ? static_cast<A_FL64>(static_cast<A_I64>(x)) + 1.f : static_cast<A_FL64>(static_cast<A_I64>(x)); }
	AURORA_NDWR_PURE("ModulusF") constexpr A_FL32 ModulusF(A_FL32 x, A_FL32 y) noexcept { return x - static_cast<A_I64>(x / y) * y; }
	AURORA_NDWR_PURE("Modulus") constexpr A_FL64 Modulus(A_FL64 x, A_FL64 y) noexcept { return x - static_cast<A_I64>(x / y) * y; }
	AURORA_NDWR_PURE("InverseTangentF") constexpr A_FL32 InverseTangentF(A_FL32 x) noexcept { return FLT_MIN > AbsoluteF(x) ? 0.0f : x < 0.0f ? static_cast<A_FL32>(Helpers::InverseTangentCalculate(-x)) : static_cast<A_FL32>(Helpers::InverseTangentCalculate(x)); }
	AURORA_NDWR_PURE("InverseTangent") constexpr A_FL64 InverseTangent(A_FL64 x) noexcept { return DBL_MIN > Absolute(x) ? 0.L : x < 0.L ? Helpers::InverseTangentCalculate(-x) : Helpers::InverseTangentCalculate(x); }
	AURORA_NDWR_PURE("InverseCosineF") constexpr A_FL32 InverseCosineF(A_FL32 x) noexcept { return FLT_MIN > AbsoluteF(x - 1.0f) ? 0.0f : FLT_MIN > AbsoluteF(x) ? Pi / 2.0f : InverseTangentF(SquareRootF(1.0f - SquaredF(x)) / x); }
	AURORA_NDWR_PURE("InverseCosine") constexpr A_FL64 InverseCosine(A_FL64 x) noexcept { return DBL_MIN > Absolute(x - 1.L) ? 0.L : DBL_MIN > Absolute(x) ? Pi / 2.L : InverseTangent(SquareRoot(1.L - Squared(x)) / x); }
	AURORA_NDWR_PURE("Delta") constexpr A_FL64 Delta(A_FL64 x, A_FL64 y) noexcept { return x - y; }
}

#endif // !__AURORA_CONSTEXPR_MATH_H__