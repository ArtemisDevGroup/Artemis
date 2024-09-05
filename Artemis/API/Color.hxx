#ifndef __ARTEMIS_API_COLOR_HXX__
#define __ARTEMIS_API_COLOR_HXX__

#include "Definitions.hxx"

#include "gcem/gcem.hpp"

#include <string>
#include <format>

namespace Artemis::API {
	struct rgba;
	struct hsl;

	/// <summary>
	/// Represents a color composed of an amount of red, green and blue.
	/// </summary>
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

		/// <summary>
		/// Initializes all amounts to zero. This will equal the color black.
		/// </summary>
		constexpr rgb() noexcept : R(0), G(0), B(0) {}

		/// <summary>
		/// Initializes all color amounts to the provided value.
		/// </summary>
		/// <param name="_X">- The value to set all color amounts to.</param>
		constexpr rgb(float _X) noexcept : R(_X), G(_X), B(_X) {}

		/// <summary>
		/// Initializes the respective color amounts.
		/// </summary>
		/// <param name="_R">- The amount of red.</param>
		/// <param name="_G">- The amount of green.</param>
		/// <param name="_B">- The amount of blue.</param>
		constexpr rgb(float _R, float _G, float _B) noexcept : R(_R), G(_G), B(_B) {}

		/// <summary>
		/// Converts the represented color into an equivalent in the RGBA format.
		/// </summary>
		/// <returns>The converted color.</returns>
		constexpr rgba to_rgba() const noexcept;

		/// <summary>
		/// Converts the represented color into an equivalent in the HSL (HSLA) format.
		/// </summary>
		/// <returns>The converted color.</returns>
		constexpr hsl to_hsl() const noexcept;

		/// <summary>
		/// Converts the represented color into an equivalent hex string.
		/// </summary>
		/// <returns>The converted hex string.</returns>
		inline std::string to_hex_string() const {
			int iR = static_cast<int>(this->R);
			int iG = static_cast<int>(this->G);
			int iB = static_cast<int>(this->B);

			if (iR / 0x10 == iR % 0x10 && iG / 0x10 == iG % 0x10 && iB / 0x10 == iB % 0x10)
				return std::format("#{:X}{:X}{:X}", iR % 0x10, iG % 0x10, iB % 0x10);
			return std::format("#{:02X}{:02X}{:02X}", iR, iG, iB);
		}

		/// <summary>
		/// Converts the represented color into a string representation.
		/// </summary>
		/// <returns>The converted string.</returns>
		inline std::string to_string() const { return std::format("rgb({:.2}, {:.2}, {:.2})", this->R, this->G, this->B); }
	};

	/// <summary>
	/// Represents a color composed of an amount of red, green and blue including an alpha channel.
	/// </summary>
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

		/// <summary>
		/// Initializes all amounts to zero. This will equal complete transparency.
		/// </summary>
		constexpr rgba() noexcept : R(0), G(0), B(0), A(0) {}

		/// <summary>
		/// Initializes all color amounts to the provided value. Additionally initializes the alpha channel.
		/// </summary>
		/// <param name="_X">- The value to set all color amounts to.</param>
		/// <param name="_A">- The value to set the alpha channel to.</param>
		constexpr rgba(float _X, float _A) noexcept : R(_X), G(_X), B(_X), A(_A) {}

		/// <summary>
		/// Initializes the respective color and alpha amounts.
		/// </summary>
		/// <param name="_R">- The amount of red.</param>
		/// <param name="_G">- The amount of green.</param>
		/// <param name="_B">- The amount of blue.</param>
		/// <param name="_A">- The amount of alpha.</param>
		constexpr rgba(float _R, float _G, float _B, float _A) noexcept : R(_R), G(_G), B(_B), A(_A) {}

		/// <summary>
		/// <para>Converts the represented color into an equivalent in the RGB format.</para>
		/// <para>Essentially preserves the red, green and blue amounts but strips the alpha from the color.</para>
		/// </summary>
		/// <returns>The converted color.</returns>
		constexpr rgb to_rgb() const noexcept {	return rgb(this->R, this->G, this->B); }

		/// <summary>
		/// Converts the represented color into an equivalent in the HSL (HSLA) format.
		/// </summary>
		/// <returns>The converted color.</returns>
		constexpr hsl to_hsl() const noexcept;

		/// <summary>
		/// Converts the represented color into an equivalent hex string.
		/// </summary>
		/// <returns>The converted hex string.</returns>
		inline std::string to_hex_string() const {
			int iR = static_cast<int>(this->R);
			int iG = static_cast<int>(this->G);
			int iB = static_cast<int>(this->B);
			int iA = static_cast<int>(this->A);

			if (iR / 0x10 == iR % 0x10 && iG / 0x10 == iG % 0x10 && iB / 0x10 == iB % 0x10 && iA / 0x10 == iA % 0x10)
				return std::format("#{:X}{:X}{:X}{:X}", iR % 0x10, iG % 0x10, iB % 0x10, iA % 0x10);
			return std::format("#{:02X}{:02X}{:02X}{:02X}", iR, iG, iB, iA);
		}

		/// <summary>
		/// Converts the represented color into a string representation.
		/// </summary>
		/// <returns>The converted string.</returns>
		inline std::string to_string() const { return std::format("rgba({:.2}{:.2}{:.2}{:.2}", this->R, this->G, this->B, this->A); }
	};

	/// <summary>
	/// Represents a color composed of a hue shift aswell as a saturation, lightness and alpha percentage.
	/// </summary>
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

		/// <summary>
		/// Initializes all amount to zero. This will equal complete transparency.
		/// </summary>
		constexpr hsl() noexcept : Hue(0), Saturation(0), Lightness(0), Alpha(0) {}

		/// <summary>
		/// Initializes all fields to the provided values.
		/// </summary>
		/// <param name="_Hue">- The hue-shift in degrees.</param>
		/// <param name="_Saturation">- The saturation amount.</param>
		/// <param name="_Lightness">- The lightness amount.</param>
		/// <param name="_Alpha">- The alpha amount.</param>
		constexpr hsl(float _Hue, float _Saturation, float _Lightness, float _Alpha = 1.0f) noexcept : Hue(_Hue), Saturation(_Saturation), Lightness(_Lightness), Alpha(_Alpha) {}

		/// <summary>
		/// <para>Converts the represented color into an equivalent in the RGB format.</para>
		/// <para>Essentially preserves the red, green and blue amounts but strips the alpha from the color.</para>
		/// </summary>
		/// <returns>The converted color.</returns>
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

		/// <summary>
		/// Converts the represented color into an equivalent in the RGBA format.
		/// </summary>
		/// <returns>The converted color.</returns>
		constexpr rgba to_rgba() const noexcept {
			rgba rgba = this->to_rgb().to_rgba();
			rgba.A = Alpha;
			return rgba;
		}

		/// <summary>
		/// Converts the represented color into an equivalent hex string.
		/// </summary>
		/// <returns>The converted hex string.</returns>
		inline std::string to_hex_string() const { return this->to_rgba().to_hex_string(); }

		/// <summary>
		/// Converts the represented color into a string representation.
		/// </summary>
		/// <returns>The converted string.</returns>
		inline std::string to_string() const { return std::format("hsl({:.2}, {:.2}, {:.2}, {:.2})", this->Hue, this->Saturation, this->Lightness, this->Alpha); }
	};

	constexpr rgba rgb::to_rgba() const noexcept { return rgba(this->R, this->G, this->B, 1.0f); }

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

	constexpr hsl rgba::to_hsl() const noexcept {
		hsl hsl = this->to_rgb().to_hsl();
		hsl.Alpha = A;
		return hsl;
	}
}

#endif // !__ARTEMIS_API_COLOR_HXX__