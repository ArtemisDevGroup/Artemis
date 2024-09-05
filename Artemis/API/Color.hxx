#ifndef __ARTEMIS_API_COLOR_HXX__
#define __ARTEMIS_API_COLOR_HXX__

#include "Definitions.hxx"

#include "gcem/gcem.hpp"

#include <string>
#include <format>

namespace Artemis::API {
	struct rgba;
	struct hsl;

	struct rgb {
		/// <summary>
		/// <para>The color's red component.</para>
		/// <para>0 &#x2264; R &#x2264; 255</para>
		/// </summary>
		float R;

		/// <summary>
		/// <para>The color's green component.</para>
		/// <para>0 &#x2264; G &#x2264; 255</para>
		/// </summary>
		float G;

		/// <summary>
		/// <para>The color's blue component.</para>
		/// <para>0 &#x2264; B &#x2264; 255</para>
		/// </summary>
		float B;

		constexpr rgb() noexcept : R(0), G(0), B(0) {}

		constexpr rgb(float _X) : R(_X), G(_X), B(_X) {}

		constexpr rgb(float _R, float _G, float _B) : R(_R), G(_G), B(_B) {}

		constexpr rgba to_rgba() const noexcept;

		constexpr hsl to_hsl() const noexcept;

		inline std::string to_hex_string() const {
			int iR = static_cast<int>(this->R);
			int iG = static_cast<int>(this->G);
			int iB = static_cast<int>(this->B);

			if (iR / 0x10 == iR % 0x10 && iG / 0x10 == iG % 0x10 && iB / 0x10 == iB % 0x10)
				return std::format("#{:X}{:X}{:X}", iR % 0x10, iG % 0x10, iB % 0x10);
			return std::format("#{:02X}{:02X}{:02X}", iR, iG, iB);
		}

		inline std::string to_string() const { return std::format("rgb({:.2}, {:.2}, {:.2})", this->R, this->G, this->B); }

		constexpr operator rgba () const noexcept;
		constexpr operator hsl () const noexcept;
	};

	struct rgba {
		// <summary>
		/// <para>The color's red component.</para>
		/// <para>0 &#x2264; R &#x2264; 255</para>
		/// </summary>
		float R;

		/// <summary>
		/// <para>The color's green component.</para>
		/// <para>0 &#x2264; G &#x2264; 255</para>
		/// </summary>
		float G;

		/// <summary>
		/// <para>The color's blue component.</para>
		/// <para>0 &#x2264; B &#x2264; 255</para>
		/// </summary>
		float B;

		/// <summary>
		/// <para>The color's alpha component.</para>
		/// <para>The alpha component controls the opacity of the color. The lower the value, the more opaque the color.</para>
		/// <para>0 &#x2264; A &#x2264; 1</para>
		/// </summary>
		float A;

		constexpr rgba() noexcept : R(0), G(0), B(0), A(0) {}

		constexpr rgba(float _X, float _A) noexcept : R(_X), G(_X), B(_X), A(_A) {}

		constexpr rgba(float _R, float _G, float _B, float _A) noexcept : R(_R), G(_G), B(_B), A(_A) {}

		constexpr rgb to_rgb() const noexcept {	return rgb(this->R, this->G, this->B); }

		constexpr hsl to_hsl() const noexcept;

		inline std::string to_hex_string() const {
			int iR = static_cast<int>(this->R);
			int iG = static_cast<int>(this->G);
			int iB = static_cast<int>(this->B);
			int iA = static_cast<int>(this->A);

			if (iR / 0x10 == iR % 0x10 && iG / 0x10 == iG % 0x10 && iB / 0x10 == iB % 0x10 && iA / 0x10 == iA % 0x10)
				return std::format("#{:X}{:X}{:X}{:X}", iR % 0x10, iG % 0x10, iB % 0x10, iA % 0x10);
			return std::format("#{:02X}{:02X}{:02X}{:02X}", iR, iG, iB, iA);
		}

		inline std::string to_string() const { return std::format("rgba({:.2}{:.2}{:.2}{:.2}", this->R, this->G, this->B, this->A); }

		constexpr operator rgb () const noexcept { return this->to_rgb(); }
		constexpr operator hsl () const noexcept;
	};

	struct hsl {
		/// <summary>
		/// <para>The color's hue component</para>
		/// <para>The hue component controls how many degrees the color shall be shifted on the color spectrum.</para>
		/// <para>0 &#x2264; Hue &lt; 360</para>
		/// </summary>
		float Hue;

		/// <summary>
		/// <para>The color's saturation component.</para>
		/// <para>0 &#x2264; Saturation &#x2264; 1</para>
		/// </summary>
		float Saturation;

		/// <summary>
		/// <para>The color's lightness component.</para>
		/// <para>0 &#x2264; Lightness &#x2264; 1</para>
		/// </summary>
		float Lightness;

		/// <summary>
		/// <para>The color's alpha component.</para>
		/// <para>The alpha component controls the opacity of the color. The lower the value, the more opaque the color.</para>
		/// <para>0 &#x2264; Alpha &#x2264; 1</para>
		/// </summary>
		float Alpha;

		constexpr hsl() noexcept : Hue(0), Saturation(0), Lightness(0), Alpha(0) {}

		constexpr hsl(float _Hue, float _Saturation, float _Lightness, float _Alpha = 1.0f) noexcept : Hue(_Hue), Saturation(_Saturation), Lightness(_Lightness), Alpha(_Alpha) {}

		constexpr rgb to_rgb() const noexcept {
			float c = (1.0f - gcem::abs(2.0f * Lightness - 1.0f)) * Saturation;
			float x = c * (1.0f - gcem::abs(gcem::fmod(Hue / 60.0f, 2.0f) - 1.0f));
			float m = Lightness - c / 2.0f;

			float r, g, b;

			if (0.0f <= Hue && Hue < 60.0f) { r = c; g = x; b = 0.0f; }
			else if (60.0f <= Hue && Hue < 120.0f) { r = x; g = c; b = 0.0f; }
			else if (120.0f <= Hue && Hue < 180.0f) { r = 0.0f; g = c; b = x; }
			else if (180.0f <= Hue && Hue < 240.0f) { r = 0.0f; g = x; b = c; }
			else if (240.0f <= Hue && Hue < 300.0f) { r = x; g = 0.0f; b = c; }
			else if (300.0f <= Hue && Hue < 360.0f) { r = c; g = 0.0f; b = x; }
			else { r = 0.f; g = 0.f; b = 0.f; }

			return rgb(
				gcem::round((r + m) * 255.0f),
				gcem::round((g + m) * 255.0f),
				gcem::round((b + m) * 255.0f)
			);
		}

		constexpr rgba to_rgba() const noexcept {
			rgba rgba = this->to_rgb().to_rgba();
			rgba.A = Alpha;
			return rgba;
		}

		inline std::string to_hex_string() const {

		}

		inline std::string to_string() const;

		constexpr operator rgb () const noexcept { return this->to_rgb(); }
		constexpr operator rgba () const noexcept { return this->to_rgba(); }
	};

	constexpr rgba rgb::to_rgba() const noexcept {
		return rgba(this->R, this->G, this->B, 1.0f);
	}

	constexpr rgb::operator rgba () const noexcept { return this->to_rgba(); }

	constexpr hsl rgb::to_hsl() const noexcept {
		float max = gcem::max(gcem::max(R, G), B);
		float min = gcem::min(gcem::min(R, G), B);
		float delta = (max - min) / 255.0f;

		float lightness = (max + min) / 510.0f;
		float saturation = (0.0f < lightness && 1.0f > lightness) ? gcem::round((delta / (1.0f - gcem::abs(2.0f * lightness - 1.0f))) * 100.0f) / 100.0f : 0.0f;
		float hue =
			R == G &&
			R == B &&
			G == B ? 0
			: gcem::round((gcem::acos((R - (G / 2.0f) - (B / 2.0f)) / gcem::sqrt((R * R) + (G * G) + (B * B) - R * G - R * B - G * B))) * 180.0f / GCEM_PI);

		return hsl(B > G ? (360.0f - hue) : hue, saturation, lightness);
	}

	constexpr rgb::operator hsl () const noexcept { return this->to_hsl(); }

	constexpr hsl rgba::to_hsl() const noexcept {
		hsl hsl = this->to_rgb().to_hsl();
		hsl.Alpha = A;
		return hsl;
	}

	constexpr rgba::operator hsl () const noexcept { return this->to_hsl(); }
}

#endif // !__ARTEMIS_API_COLOR_HXX__