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

#ifndef __AURORA_COLOR_H__
#define __AURORA_COLOR_H__

#include "Definitions.h"
#include "String.h"
#include "Vector.h"
#include "ConstexprMath.h"

#undef RGB

namespace Aurora {
	struct AURORA_API RGBA;
	struct AURORA_API HSL;
	struct AURORA_API HEX;

	/// <summary>
	/// Represents a color in amounts of red, green and blue in the ranges of 0 to 255.
	/// </summary>
	struct AURORA_API RGB {
		A_FL32 fR, fG, fB;

		constexpr RGB() noexcept : fR(0), fG(0), fB(0) {}

		/// <summary>
		/// Sets all fields to the value of fX.
		/// </summary>
		/// <param name="fX">- The value to set R, G and B to, ranging from 0 to 255.</param>
		constexpr RGB(_In_ A_FL32 fX) noexcept : fR(fX), fG(fX), fB(fX) {}

		/// <summary>
		/// Sets the values of the fields fR, fG and fB.
		/// </summary>
		/// <param name="fR">- The amount of red, ranging from 0 to 255.</param>
		/// <param name="fG">- The amount of green, ranging from 0 to 255.</param>
		/// <param name="fB">- The amount of blue, ranging from 0 to 255.</param>
		constexpr RGB(_In_ A_FL32 fR, _In_ A_FL32 fG, _In_ A_FL32 fB) noexcept : fR(fR), fG(fG), fB(fB) {}

		/// <summary>
		/// Returns a string representation of the color in a conventional format, e.g: 'rgb(r, g, b)'
		/// </summary>
		/// <param name="lpString">- A pointer to a buffer to receive the returned string.</param>
		/// <param name="dwSize">- The size of the buffer in characters.</param>
		A_VOID ToString(_Out_writes_z_(dwSize) A_LPSTR lpString, _In_ A_DWORD dwSize) const;

		/// <summary>
		/// Converts the current RGB instance into an equivalent RGBA instance.
		/// </summary>
		/// <returns>The current color in RGBA.</returns>
		AURORA_NDWR_PURE("ToRGBA") constexpr RGBA ToRGBA() const noexcept;

		/// <summary>
		/// Converts the current RGB instance into an equivalent HSL instance.
		/// </summary>
		/// <returns>The current color in HSL.</returns>
		AURORA_NDWR_PURE("ToHSL") constexpr HSL ToHSL() const noexcept;

		/// <summary>
		/// Converts the current RGB instance into an equivalent HEX instance.
		/// </summary>
		/// <returns>The current color in HEX.</returns>
		AURORA_NDWR_PURE("ToHex") constexpr HEX ToHex() const noexcept;

		AURORA_NDWR_PURE("operator RGBA") constexpr operator RGBA() const noexcept;
		AURORA_NDWR_PURE("operator HSL") constexpr operator HSL() const noexcept;
		AURORA_NDWR_PURE("operator HEX") constexpr operator HEX() const noexcept;
	};

	/// <summary>
	/// Represents a color in amouns of red, green and blue aswell as an opacity value (alpha) in ranges of 0 to 255.
	/// </summary>
	struct AURORA_API RGBA {
		A_FL32 fR, fG, fB, fA;

		constexpr RGBA() noexcept : fR(0), fG(0), fB(0), fA(0) {}

		/// <summary>
		/// Sets R, G and B to the value of fX and A to the value of fA.
		/// </summary>
		/// <param name="fX">- The value to set R, G and B to, ranging from 0 to 255.</param>
		/// <param name="fA">- The value to set A to, ranging from 0 to 255.</param>
		constexpr RGBA(_In_ A_FL32 fX, _In_ A_FL32 fA) noexcept : fR(fX), fG(fX), fB(fX), fA(fA) {}

		/// <summary>
		/// Sets the values of the fields fR, fG and fB.
		/// </summary>
		/// <param name="fR">- The amount of red, ranging from 0 to 255.</param>
		/// <param name="fG">- The amount of green, ranging from 0 to 255.</param>
		/// <param name="fB">- The amount of blue, ranging from 0 to 255.</param>
		/// <param name="fA">- The value to set A to, ranging from 0 to 255.</param>
		constexpr RGBA(_In_ A_FL32 fR, _In_ A_FL32 fG, _In_ A_FL32 fB, _In_ A_FL32 fA) noexcept : fR(fR), fG(fG), fB(fB), fA(fA) {}

		/// <summary>
		/// Returns a string representation of the color in a conventional format, e.g: 'rgba(r, g, b, a)'
		/// </summary>
		/// <param name="lpString">- A pointer to a buffer to receive the returned string.</param>
		/// <param name="dwSize">- The size of the buffer in characters.</param>
		A_VOID ToString(_Out_writes_z_(dwSize) A_LPSTR lpString, _In_ A_DWORD dwSize) const;

		/// <summary>
		/// Converts the current RGBA instance into an equivalent RGB instance.
		/// </summary>
		/// <returns>The current color in RGB.</returns>
		AURORA_NDWR_PURE("ToRGB") constexpr RGB ToRGB() const noexcept;

		/// <summary>
		/// Converts the current RGBA instance into an equivalent HSL instance.
		/// </summary>
		/// <returns>The current color in HSL.</returns>
		AURORA_NDWR_PURE("ToHSL") constexpr HSL ToHSL() const noexcept;

		/// <summary>
		/// Converts the current RGBA instance into an equivalent HEX instance.
		/// </summary>
		/// <returns>The current color in HEX.</returns>
		AURORA_NDWR_PURE("ToHex") constexpr HEX ToHex() const noexcept;

		AURORA_NDWR_PURE("operator RGB") constexpr operator RGB() const noexcept;
		AURORA_NDWR_PURE("operator HSL") constexpr operator HSL() const noexcept;
		AURORA_NDWR_PURE("operator HEX") constexpr operator HEX() const noexcept;
	};

	/// <summary>
	/// Represents a color in a hue measured in degrees (0 &#x2264; x &#60; 360) aswell as saturation, lightness and opacity (alpha) measured in percent (0 &#x2264; x &#x2264; 1).
	/// </summary>
	struct AURORA_API HSL {
		A_FL32 fHue, fSaturation, fLightness, fAlpha;

		constexpr HSL() noexcept : fHue(0.0f), fSaturation(0.0f), fLightness(0.0f), fAlpha(0.0f) {}

		/// <summary>
		/// Sets the hue, saturation, lightness and alpha.
		/// </summary>
		/// <param name="fHue">- The hue of the color. (0 &#x2264; x &#60; 360)</param>
		/// <param name="fSaturation">
		/// <para>- The saturation of the color. (0 &#x2264; x &#x2264; 1)</para>
		/// <para>A higher saturation produces a more vibrant color whilst a lower saturation creates a more washed out color.</para>
		/// </param>
		/// <param name="fLightness">- The lightness of the color. (0 &#x2264; x &#x2264; 1)</param>
		/// <param name="fAlpha">- The opacity of the color. (0 &#x2264; x &#x2264; 1)</param>
		constexpr HSL(_In_ A_FL32 fHue, _In_ A_FL32 fSaturation, _In_ A_FL32 fLightness, _In_ A_FL32 fAlpha = 1.f) noexcept : fHue(fHue), fSaturation(fSaturation), fLightness(fLightness), fAlpha(fAlpha) {}

		/// <summary>
		/// Returns a string representation of the color in a conventional format, e.g: 'hsl(h, s, l, a)'
		/// </summary>
		/// <param name="lpString">- A pointer to a buffer to receive the returned string.</param>
		/// <param name="dwSize">- The size of the buffer in characters.</param>
		A_VOID ToString(_Out_writes_z_(dwSize) A_LPSTR lpString, _In_ A_DWORD dwSize) const;

		/// <summary>
		/// Converts the current HSL instance into an equivalent RGB instance.
		/// </summary>
		/// <returns>The current color in RGB.</returns>
		AURORA_NDWR_PURE("ToRGB") constexpr RGB ToRGB() const noexcept;

		/// <summary>
		/// Converts the current HSL instance into an equivalent RGBA instance.
		/// </summary>
		/// <returns>The current color in RGBA.</returns>
		AURORA_NDWR_PURE("ToRGBA") constexpr RGBA ToRGBA() const noexcept;

		/// <summary>
		/// Converts the current HSL instance into an equivalent HEX instance.
		/// </summary>
		/// <returns>The current color in HEX.</returns>
		AURORA_NDWR_PURE("ToHex") constexpr HEX ToHex() const noexcept;

		AURORA_NDWR_PURE("operator RGB") constexpr operator RGB() const noexcept;
		AURORA_NDWR_PURE("operator RGBA") constexpr operator RGBA() const noexcept;
		AURORA_NDWR_PURE("operator HEX") constexpr operator HEX() const noexcept;
	};
	
	/// <summary>
	/// <para>Represents a color in amouns of red, green and blue aswell as an opacity value (alpha) in ranges of 0 to 255.</para>
	/// <para>Accepts and returns hexadecimal representations of color, e.g: '#FFF' for pure white, '#0000' for transparency, '#CECECE' for gray, '#FF00FFA0' for moderately opaque purple, et cetera.</para>
	/// </summary>
	struct AURORA_API HEX {
		A_FL32 fR, fG, fB, fA;

		constexpr HEX() noexcept : fR(0), fG(0), fB(0), fA(0) {}

		/// <summary>
		/// Initializes the R, G, B and A fields with values from the provided string.
		/// </summary>
		/// <param name="lpHexString">- The hex string to process.</param>
		HEX(_In_z_ A_LPCSTR lpHexString);

		/// <summary>
		/// <para>Returns a string representation of the color in a conventional format, e.g:</para>
		/// <para>- '#RGB' for colors where the alpha is 'FF' and cases where both digits of R, G and B are the same.</para>
		/// <para>* For example: '#FFF' for pure white, or '#000' for pure black.</para>
		/// <para>- '#RGBA' for colors where both digits of R, G, B and A are the same.</para>
		/// <para>* For example: '#FFF9' for moderately opaque white, or '#0000' for complete transparency.</para>
		/// <para>- '#RRGGBB' for colors where the alpha is 'FF'.</para>
		/// <para>- '#RRGGBBAA' for colors that do not fall under any of the previous cases.</para>
		/// </summary>
		/// <param name="lpString">- A pointer to a buffer to receive the returned string.</param>
		/// <param name="dwSize">- The size of the buffer in characters.</param>
		A_VOID ToString(_Out_writes_z_(dwSize) A_LPSTR lpString, _In_ A_DWORD dwSize) const;

		/// <summary>
		/// Converts the current HEX instance into an equivalent RGB instance.
		/// </summary>
		/// <returns>The current color in RGB.</returns>
		AURORA_NDWR_PURE("ToRGB") constexpr RGB ToRGB() const noexcept { return RGB(fR, fG, fB); }

		/// <summary>
		/// Converts the current HEX instance into an equivalent RGBA instance.
		/// </summary>
		/// <returns>The current color in RGBA.</returns>
		AURORA_NDWR_PURE("ToRGBA") constexpr RGBA ToRGBA() const noexcept { return RGBA(fR, fG, fB, fA); }

		/// <summary>
		/// Converts the current HEX instance into an equivalent HSL instance.
		/// </summary>
		/// <returns>The current color in HSL.</returns>
		AURORA_NDWR_PURE("ToHSL") constexpr HSL ToHSL() const noexcept { return RGBA(fR, fG, fB, fA).ToHSL(); }

		AURORA_NDWR_PURE("operator RGB") constexpr operator RGB() const noexcept { return this->ToRGB(); }
		AURORA_NDWR_PURE("operator RGBA") constexpr operator RGBA() const noexcept { return this->ToRGBA(); }
		AURORA_NDWR_PURE("operator HSL") constexpr operator HSL() const noexcept { return this->ToHSL(); }
	};

	// RGB constexpr definitions.

	constexpr RGBA RGB::ToRGBA() const noexcept { return RGBA(fR, fG, fB, 255.f); }

	constexpr HSL RGB::ToHSL() const noexcept {
		A_FL32 fMax = (A_FL32)Max(Max(fR, fG), fB);
		A_FL32 fMin = (A_FL32)Min(Min(fR, fG), fB);
		A_FL32 fDelta = (fMax - fMin) / 255.0f;

		A_FL32 fLightness = (fMax + fMin) / 510.0f;
		A_FL32 fSaturation = (0.0f < fLightness && 1.0f > fLightness) ? RoundF((fDelta / (1.0f - AbsoluteF(2.0f * fLightness - 1.0f))) * 100.0f) / 100.0f : 0.0f;

		A_FL32 fHue = fR == fG && fR == fB && fG == fB ? 0 : RoundF(RadiansToDegreesF(InverseCosineF((fR - HalfF(fG) - HalfF(fB)) / SquareRootF(SquaredF(fR) + SquaredF(fG) + SquaredF(fB) - fR * fG - fR * fB - fG * fB))));

		return HSL(fB > fG ? (360.0f - fHue) : fHue, fSaturation, fLightness);
	}

	constexpr HEX RGB::ToHex() const noexcept {
		HEX hex;
		hex.fR = fR;
		hex.fG = fG;
		hex.fB = fB;
		hex.fA = 0.f;
		return hex;
	}

	constexpr RGB::operator RGBA() const noexcept { return this->ToRGBA(); }
	constexpr RGB::operator HSL() const noexcept { return this->ToHSL(); }
	constexpr RGB::operator HEX() const noexcept { return this->ToHex(); }

	// RGBA constexpr definitions.

	constexpr RGB RGBA::ToRGB() const noexcept { return RGB(fR, fG, fB); }

	constexpr HSL RGBA::ToHSL() const noexcept {
		HSL hsl = ToRGB().ToHSL();
		hsl.fAlpha = fA / 255.f;
		return hsl;
	}

	constexpr HEX RGBA::ToHex() const noexcept {
		HEX hex;
		hex.fR = fR;
		hex.fG = fG;
		hex.fB = fB;
		hex.fA = fA;
		return hex;
	}

	constexpr RGBA::operator RGB() const noexcept { return this->ToRGB(); }
	constexpr RGBA::operator HSL() const noexcept { return this->ToHSL(); }
	constexpr RGBA::operator HEX() const noexcept { return this->ToHex(); }

	// HSL constexpr definitions.

	constexpr RGB HSL::ToRGB() const noexcept {
		A_FL32 fC = (1.0f - AbsoluteF(2.0f * fLightness - 1.0f)) * fSaturation;
		A_FL32 fX = fC * (1.0f - AbsoluteF(ModulusF(fHue / 60.0f, 2.0f) - 1.0f));
		A_FL32 fm = fLightness - fC / 2.f;

		A_FL32 fR, fG, fB;

		if (0.f <= fHue && fHue < 60.f) { fR = fC; fG = fX; fB = 0.f; }
		else if (60.f <= fHue && fHue < 120.f) { fR = fX; fG = fC; fB = 0.f; }
		else if (120.f <= fHue && fHue < 180.f) { fR = 0.f; fG = fC; fB = fX; }
		else if (180.f <= fHue && fHue < 240.f) { fR = 0.f; fG = fX; fB = fC; }
		else if (240.f <= fHue && fHue < 300.f) { fR = fX; fG = 0.f; fB = fC; }
		else if (300.f <= fHue && fHue < 360.f) { fR = fC; fG = 0.f; fB = fX; }
		else { fR = 0.f; fG = 0.f; fB = 0.f; }

		return RGB(
			RoundF((fR + fm) * 255.f),
			RoundF((fG + fm) * 255.f),
			RoundF((fB + fm) * 255.f)
		);
	}

	constexpr RGBA HSL::ToRGBA() const noexcept {
		RGB rgb = ToRGB();
		return RGBA(rgb.fR, rgb.fG, rgb.fR, fAlpha * 255.f);
	}

	constexpr HEX HSL::ToHex() const noexcept {
		return this->ToRGBA().ToHex();
	}

	constexpr HSL::operator RGB() const noexcept { return this->ToRGB(); }
	constexpr HSL::operator RGBA() const noexcept { return this->ToRGBA(); }
	constexpr HSL::operator HEX() const noexcept { return this->ToHex(); }

	namespace Colors {
		constexpr RGB AliceBlue(240, 248, 255);
		constexpr RGB AntiqueWhite(250, 235, 215);
		constexpr RGB Aqua(0, 255, 255);
		constexpr RGB Aquamarine(127, 255, 212);
		constexpr RGB Azure(240, 255, 255);
		constexpr RGB Beige(245, 245, 220);
		constexpr RGB Bisque(255, 228, 196);
		constexpr RGB Black(0, 0, 0);
		constexpr RGB BlancedAlmond(255, 235, 205);
		constexpr RGB Blue(0, 0, 255);
		constexpr RGB BlueViolet(138, 43, 226);
		constexpr RGB Brown = RGB(165, 42, 42);
		constexpr RGB BurlyWood = RGB(222, 184, 135);
		constexpr RGB CadetBlue = RGB(95, 158, 160);
		constexpr RGB Chartreuse = RGB(127, 255, 0);
		constexpr RGB Chocolate = RGB(210, 105, 30);
		constexpr RGB Coral = RGB(255, 127, 80);
		constexpr RGB CornflowerBlue = RGB(100, 149, 237);
		constexpr RGB Cornsilk = RGB(255, 248, 220);
		constexpr RGB Crimson = RGB(220, 20, 60);
		constexpr RGB Cyan = RGB(0, 255, 255);
		constexpr RGB DarkBlue = RGB(0, 0, 139);
		constexpr RGB DarkCyan = RGB(0, 139, 139);
		constexpr RGB DarkGoldenrod = RGB(184, 134, 11);
		constexpr RGB DarkGray = RGB(169, 169, 169);
		constexpr RGB DarkGreen = RGB(0, 100, 0);
		constexpr RGB DarkKhaki = RGB(189, 183, 107);
		constexpr RGB DarkMagenta = RGB(139, 0, 139);
		constexpr RGB DarkOliveGreen = RGB(85, 107, 47);
		constexpr RGB DarkOrange = RGB(255, 140, 0);
		constexpr RGB DarkRed = RGB(139, 0, 0);
		constexpr RGB DarkSalmon = RGB(233, 150, 122);
		constexpr RGB DarkSeaGreen = RGB(143, 188, 143);
		constexpr RGB DarkSlateBlue = RGB(72, 61, 139);
		constexpr RGB DarkSlateGray = RGB(47, 79, 79);
		constexpr RGB DarkTurquoise = RGB(0, 206, 209);
		constexpr RGB DarkViolet = RGB(148, 0, 211);
		constexpr RGB DeepPink = RGB(255, 20, 147);
		constexpr RGB DeepSkyBlue = RGB(0, 191, 255);
		constexpr RGB DimGray = RGB(105, 105, 105);
		constexpr RGB DodgerBlue = RGB(30, 144, 255);
		constexpr RGB Firebrick = RGB(178, 34, 34);
		constexpr RGB FloralWhite = RGB(255, 250, 240);
		constexpr RGB FloralGreen = RGB(181, 230, 190);
		constexpr RGB Fuchsia = RGB(255, 0, 255);
		constexpr RGB Gainsboro = RGB(220, 220, 220);
		constexpr RGB GhostWhite = RGB(248, 248, 255);
		constexpr RGB Gold = RGB(255, 215, 0);
		constexpr RGB Goldenrod = RGB(218, 165, 32);
		constexpr RGB Gray = RGB(128, 128, 128);
		constexpr RGB Green = RGB(0, 255, 0);
		constexpr RGB GreenYellow = RGB(173, 255, 47);
		constexpr RGB Honeydew = RGB(240, 255, 240);
		constexpr RGB HotPink = RGB(255, 105, 180);
		constexpr RGB IndianRed = RGB(205, 92, 92);
		constexpr RGB Indigo = RGB(75, 0, 130);
		constexpr RGB Ivory = RGB(255, 255, 240);
		constexpr RGB Lavender = RGB(230, 230, 250);
		constexpr RGB LavenderBlush = RGB(255, 240, 245);
		constexpr RGB LawnGreen = RGB(124, 252, 0);
		constexpr RGB LemonChiffon = RGB(255, 250, 205);
		constexpr RGB LightBlue = RGB(173, 216, 230);
		constexpr RGB LightCoral = RGB(240, 128, 128);
		constexpr RGB LightCyan = RGB(224, 255, 255);
		constexpr RGB LightGoldenrodYellow = RGB(250, 250, 210);
		constexpr RGB LightGray = RGB(211, 211, 211);
		constexpr RGB LightPink = RGB(255, 182, 193);
		constexpr RGB LightSalmon = RGB(255, 160, 122);
		constexpr RGB LightSeaGreen = RGB(32, 178, 170);
		constexpr RGB LightSkyBlue = RGB(135, 206, 250);
		constexpr RGB LightSlateGray = RGB(119, 136, 153);
		constexpr RGB LightSteelBlue = RGB(176, 196, 222);
		constexpr RGB LightYellow = RGB(255, 255, 224);
		constexpr RGB Lime = RGB(191, 255, 0);
		constexpr RGB LimeGreen = RGB(50, 205, 50);
		constexpr RGB Linen = RGB(250, 240, 230);
		constexpr RGB Magenta = RGB(255, 0, 255);
		constexpr RGB Maroon = RGB(128, 0, 0);
		constexpr RGB MediumAquamarine = RGB(102, 205, 170);
		constexpr RGB MediumBlue = RGB(0, 0, 205);
		constexpr RGB MediumOrchid = RGB(186, 85, 211);
		constexpr RGB MediumPurple = RGB(147, 112, 219);
		constexpr RGB MediumSeaGreen = RGB(60, 179, 113);
		constexpr RGB MediumSlateBlue = RGB(123, 104, 238);
		constexpr RGB MediumSpringGreen = RGB(0, 250, 154);
		constexpr RGB MediumTurquoise = RGB(72, 209, 204);
		constexpr RGB MediumVioletRed = RGB(199, 21, 133);
		constexpr RGB MidnightBlue = RGB(25, 25, 112);
		constexpr RGB MintCream = RGB(245, 255, 250);
		constexpr RGB MistyRose = RGB(255, 228, 225);
		constexpr RGB Moccasin = RGB(255, 228, 181);
		constexpr RGB NajavoWhite = RGB(255, 222, 173);
		constexpr RGB Navy = RGB(0, 0, 128);
		constexpr RGB OldLace = RGB(253, 245, 230);
		constexpr RGB Olive = RGB(128, 128, 0);
		constexpr RGB OliveDrab = RGB(107, 142, 35);
		constexpr RGB Orange = RGB(255, 165, 0);
		constexpr RGB OrangeRed = RGB(255, 69, 0);
		constexpr RGB Orchid = RGB(218, 112, 214);
		constexpr RGB PaleGoldenrod = RGB(238, 232, 170);
		constexpr RGB PaleGreen = RGB(152, 251, 152);
		constexpr RGB PaleTurquoise = RGB(175, 238, 238);
		constexpr RGB PaleVioletRed = RGB(219, 112, 147);
		constexpr RGB PapayaWhip = RGB(255, 239, 213);
		constexpr RGB PeachPuff = RGB(255, 218, 185);
		constexpr RGB Peru = RGB(205, 133, 63);
		constexpr RGB Pink = RGB(255, 192, 203);
		constexpr RGB Plum = RGB(221, 160, 221);
		constexpr RGB PowderBlue = RGB(176, 224, 230);
		constexpr RGB Purple = RGB(128, 0, 128);
		constexpr RGB Red = RGB(255, 0, 0);
		constexpr RGB RosyBrown = RGB(188, 143, 143);
		constexpr RGB RoyalBlue = RGB(65, 105, 225);
		constexpr RGB SaddleBrown = RGB(139, 69, 19);
		constexpr RGB Salmon = RGB(255, 140, 105);
		constexpr RGB SandyBrown = RGB(244, 164, 96);
		constexpr RGB SeaGreen = RGB(46, 139, 87);
		constexpr RGB SeaShell = RGB(255, 245, 238);
		constexpr RGB Sienna = RGB(136, 45, 23);
		constexpr RGB Silver = RGB(192, 192, 192);
		constexpr RGB SkyBlue = RGB(135, 206, 235);
		constexpr RGB SlateBlue = RGB(106, 90, 205);
		constexpr RGB SlateGray = RGB(112, 128, 144);
		constexpr RGB Snow = RGB(255, 250, 250);
		constexpr RGB SpringGreen = RGB(0, 255, 127);
		constexpr RGB SteelBlue = RGB(70, 130, 180);
		constexpr RGB Tan = RGB(210, 180, 140);
		constexpr RGB Teal = RGB(0, 128, 128);
		constexpr RGB Thistle = RGB(216, 191, 216);
		constexpr RGB Tomato = RGB(255, 99, 71);
		constexpr RGBA Transparent = RGBA(0, 0, 0, 0);
		constexpr RGB Turquoise = RGB(64, 224, 208);
		constexpr RGB Violet = RGB(238, 130, 238);
		constexpr RGB Wheat = RGB(245, 222, 179);
		constexpr RGB White = RGB(255, 255, 255);
		constexpr RGB WhiteSmoke = RGB(245, 245, 245);
		constexpr RGB Yellow = RGB(255, 255, 0);
		constexpr RGB YellowGreen = RGB(154, 205, 50);
	}
}

#endif // !__AURORA_COLOR_H__