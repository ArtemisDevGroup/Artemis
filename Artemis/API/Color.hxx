#ifndef __ARTEMIS_API_COLOR_HXX__
#define __ARTEMIS_API_COLOR_HXX__

#include "Definitions.hxx"

#include "gcem/gcem.hpp"	// gcem::abs, gcem::fmod, gcem::round, gcem::max, gcem::min, gcem::acos, gcem::sqrt, GCEM_PI

#include <string>			// std::string
#include <format>			// std::format

namespace Artemis::API {
	struct rgba;
	struct hsl;

	/**
	 * @brief Represents a color composed of an amount of red, green and blue.
	 */
	struct rgb {
		/**
		 * @brief The color's red component. 0 -> 255
		 */
		float R;

		/**
		 * @brief The color's green component. 0 -> 255
		 */
		float G;

		/**
		 * @brief The color's blue component. 0 -> 255
		 */
		float B;

		/**
		 * @brief Initializes all amounts to zero. This will equal the color black.
		 */
		constexpr rgb() noexcept : R(0), G(0), B(0) {}

		/**
		 * @brief Initializes all color amounts to the provided value.
		 * @param[in] _X The value to set all color amounts to.
		 */
		constexpr rgb(float _X) noexcept : R(_X), G(_X), B(_X) {}

		/**
		 * @brief Initializes the respective color amounts.
		 * @param[in] _R The amount of red.
		 * @param[in] _G The amount of green.
		 * @param[in] _B The amount of blue.
		 */
		constexpr rgb(float _R, float _G, float _B) noexcept : R(_R), G(_G), B(_B) {}

		/**
		 * @brief Converts the represented color into an equivalent in the RGBA format.
		 * @return The converted color.
		 */
		constexpr rgba to_rgba() const noexcept;

		/**
		 * @brief Converts the represented color into an equivalent in the HSL (HSLA) format.
		 * @return The converted color.
		 */
		constexpr hsl to_hsl() const noexcept;

		/**
		 * @brief Converts the represented color into an equivalent hex string.
		 * @return The converted hex string.
		 */
		inline std::string to_hex_string() const {
			int iR = static_cast<int>(this->R);
			int iG = static_cast<int>(this->G);
			int iB = static_cast<int>(this->B);

			if (iR / 0x10 == iR % 0x10 && iG / 0x10 == iG % 0x10 && iB / 0x10 == iB % 0x10)
				return std::format("#{:X}{:X}{:X}", iR % 0x10, iG % 0x10, iB % 0x10);
			return std::format("#{:02X}{:02X}{:02X}", iR, iG, iB);
		}

		/**
		 * @brief Converts the represented color into a string representation.
		 * @return The converted string.
		 */
		inline std::string to_string() const { return std::format("rgb({:.2}, {:.2}, {:.2})", this->R, this->G, this->B); }
	};

	/**
	 * @brief Represents a color composed of an amount of red, green and blue including an alpha channel.
	 */
	struct rgba {
		/**
		 * @brief The color's red component. 0 -> 255
		 */
		float R;

		/**
		 * @brief The color's green component. 0 -> 255
		 */
		float G;

		/**
		 * @brief The color's blue component. 0 -> 255
		 */
		float B;

		/**
		 * @brief The color's alpha component. The alpha component controls the opacity of the color. The lower the value, the less opaque the color. 0 -> 1
		 */
		float A;

		/**
		 * @brief Initializes all amounts to zero. This will equal complete transparency.
		 */
		constexpr rgba() noexcept : R(0), G(0), B(0), A(0) {}

		/**
		 * @brief Initializes all color amounts to the provided value. Additionally initializes the alpha channel.
		 * @param[in] _X The value to set all color amounts to.
		 * @param[in] _A The value to set the alpha channel to.
		 */
		constexpr rgba(float _X, float _A) noexcept : R(_X), G(_X), B(_X), A(_A) {}

		/**
		 * @brief Initializes the respective color and alpha amounts.
		 * @param[in] _R The amount of red.
		 * @param[in] _G The amount of green.
		 * @param[in] _B The amount of blue.
		 * @param[in] _A The amount of alpha.
		 */
		constexpr rgba(float _R, float _G, float _B, float _A) noexcept : R(_R), G(_G), B(_B), A(_A) {}

		/**
		 * @brief Converts the represented color into an equivalent in the RGB format. Essentially preserves the red, green and blue amounts but strips the alpha from the color.
		 * @return The converted color.
		 */
		constexpr rgb to_rgb() const noexcept {	return rgb(this->R, this->G, this->B); }

		/**
		 * @brief Converts the represented color into an equivalent in the HSL (HSLA) format.
		 * @return The converted color.
		 */
		constexpr hsl to_hsl() const noexcept;

		/**
		 * @brief Converts the represented color into an equivalent hex string.
		 * @return The converted hex string.
		 */
		inline std::string to_hex_string() const {
			int iR = static_cast<int>(this->R);
			int iG = static_cast<int>(this->G);
			int iB = static_cast<int>(this->B);
			int iA = static_cast<int>(this->A);

			if (iR / 0x10 == iR % 0x10 && iG / 0x10 == iG % 0x10 && iB / 0x10 == iB % 0x10 && iA / 0x10 == iA % 0x10)
				return std::format("#{:X}{:X}{:X}{:X}", iR % 0x10, iG % 0x10, iB % 0x10, iA % 0x10);
			return std::format("#{:02X}{:02X}{:02X}{:02X}", iR, iG, iB, iA);
		}

		/**
		 * @brief Converts the represented color into a string representation.
		 * @return The converted string.
		 */
		inline std::string to_string() const { return std::format("rgba({:.2}{:.2}{:.2}{:.2}", this->R, this->G, this->B, this->A); }
	};

	/**
	 * @brief Represents a color composed of a hue shift, a saturation percentage, a lightness percentage and an alpha percentage.
	 */
	struct hsl {
		/**
		 * @brief The color's hue component. The hue component controls how many degrees the color shall be shifted on the color spectrum. 0 -> 360
		 */
		float Hue;

		/**
		 * @brief The color's saturation component. 0 -> 1
		 */
		float Saturation;

		/**
		 * @brief The color's lightness component. 0 -> 1
		 */
		float Lightness;

		/**
		 * @brief The color's alpha component. The alpha component controls the opacity of the color. The lower the value, the more opaque the color. 0 -> 1
		 */
		float Alpha;

		/**
		 * @brief Initializes all amount to zero. This will equal complete transparency.
		 */
		constexpr hsl() noexcept : Hue(0), Saturation(0), Lightness(0), Alpha(0) {}

		/**
		 * @brief Initializes all fields to the provided values.
		 * @param[in] _Hue The hue-shift in degrees.
		 * @param[in] _Saturation The saturation amount.
		 * @param[in] _Lightness The lightness amount.
		 * @param[in] _Alpha The alpha amount.
		 */
		constexpr hsl(float _Hue, float _Saturation, float _Lightness, float _Alpha = 1.0f) noexcept : Hue(_Hue), Saturation(_Saturation), Lightness(_Lightness), Alpha(_Alpha) {}

		/**
		 * @brief Converts the represented color into an equivalent in the RGB format. Essentially preserves the red, green and blue amounts but strips the alpha from the color.
		 * @return The converted color.
		 */
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

		/**
		 * @brief Converts the represented color into an equivalent in the RGBA format.
		 * @return The converted color.
		 */
		constexpr rgba to_rgba() const noexcept {
			rgba rgba = this->to_rgb().to_rgba();
			rgba.A = Alpha;
			return rgba;
		}

		/**
		 * @brief Converts the represented color into an equivalent hex string.
		 * @return The converted hex string.
		 */
		inline std::string to_hex_string() const { return this->to_rgba().to_hex_string(); }

		/**
		 * @brief Converts the represented color into a string representation.
		 * @return The converted string.
		 */
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

	namespace Color {
		constexpr rgba AliceBlue{ 240, 248, 255, 1 };
		constexpr rgba AntiqueWhite{ 250, 235, 215, 1 };
		constexpr rgba Aqua{ 0, 255, 255, 1 };
		constexpr rgba Aquamarine{ 127, 255, 212, 1 };
		constexpr rgba Azure{ 240, 255, 255, 1 };
		constexpr rgba Beige{ 245, 245, 220, 1 };
		constexpr rgba Bisque{ 255, 228, 196, 1 };
		constexpr rgba Black{ 0, 0, 0, 1 };
		constexpr rgba BlancedAlmond{ 255, 235, 205, 1 };
		constexpr rgba Blue{ 0, 0, 255, 1 };
		constexpr rgba BlueViolet{ 138, 43, 226, 1 };
		constexpr rgba Brown{ 165, 42, 42, 1 };
		constexpr rgba BurlyWood{ 222, 184, 135, 1 };
		constexpr rgba CadetBlue{ 95, 158, 160, 1 };
		constexpr rgba Chartreuse{ 127, 255, 0, 1 };
		constexpr rgba Chocolate{ 210, 105, 30, 1 };
		constexpr rgba Coral{ 255, 127, 80, 1 };
		constexpr rgba CornflowerBlue{ 100, 149, 237, 1 };
		constexpr rgba Cornsilk{ 255, 248, 220, 1 };
		constexpr rgba Crimson{ 220, 20, 60, 1 };
		constexpr rgba Cyan{ 0, 255, 255, 1 };
		constexpr rgba DarkBlue{ 0, 0, 139, 1 };
		constexpr rgba DarkCyan{ 0, 139, 139, 1 };
		constexpr rgba DarkGoldenrod{ 184, 134, 11, 1 };
		constexpr rgba DarkGray{ 169, 169, 169, 1 };
		constexpr rgba DarkGreen{ 0, 100, 0, 1 };
		constexpr rgba DarkKhaki{ 189, 183, 107, 1 };
		constexpr rgba DarkMagenta{ 139, 0, 139, 1 };
		constexpr rgba DarkOliveGreen{ 85, 107, 47, 1 };
		constexpr rgba DarkOrange{ 255, 140, 0, 1 };
		constexpr rgba DarkRed{ 139, 0, 0, 1 };
		constexpr rgba DarkSalmon{ 233, 150, 122, 1 };
		constexpr rgba DarkSeaGreen{ 143, 188, 143, 1 };
		constexpr rgba DarkSlateBlue{ 72, 61, 139, 1 };
		constexpr rgba DarkSlateGray{ 47, 79, 79, 1 };
		constexpr rgba DarkTurquoise{ 0, 206, 209, 1 };
		constexpr rgba DarkViolet{ 148, 0, 211, 1 };
		constexpr rgba DeepPink{ 255, 20, 147, 1 };
		constexpr rgba DeepSkyBlue{ 0, 191, 255, 1 };
		constexpr rgba DimGray{ 105, 105, 105, 1 };
		constexpr rgba DodgerBlue{ 30, 144, 255, 1 };
		constexpr rgba Firebrick{ 178, 34, 34, 1 };
		constexpr rgba FloralWhite{ 255, 250, 240, 1 };
		constexpr rgba FloralGreen{ 181, 230, 190, 1 };
		constexpr rgba Fuchsia{ 255, 0, 255, 1 };
		constexpr rgba Gainsboro{ 220, 220, 220, 1 };
		constexpr rgba GhostWhite{ 248, 248, 255, 1 };
		constexpr rgba Gold{ 255, 215, 0, 1 };
		constexpr rgba Goldenrod{ 218, 165, 32, 1 };
		constexpr rgba Gray{ 128, 128, 128, 1 };
		constexpr rgba Green{ 0, 255, 0, 1 };
		constexpr rgba GreenYellow{ 173, 255, 47, 1 };
		constexpr rgba Honeydew{ 240, 255, 240, 1 };
		constexpr rgba HotPink{ 255, 105, 180, 1 };
		constexpr rgba IndianRed{ 205, 92, 92, 1 };
		constexpr rgba Indigo{ 75, 0, 130, 1 };
		constexpr rgba Ivory{ 255, 255, 240, 1 };
		constexpr rgba Lavender{ 230, 230, 250, 1 };
		constexpr rgba LavenderBlush{ 255, 240, 245, 1 };
		constexpr rgba LawnGreen{ 124, 252, 0, 1 };
		constexpr rgba LemonChiffon{ 255, 250, 205, 1 };
		constexpr rgba LightBlue{ 173, 216, 230, 1 };
		constexpr rgba LightCoral{ 240, 128, 128, 1 };
		constexpr rgba LightCyan{ 224, 255, 255, 1 };
		constexpr rgba LightGoldenrodYellow{ 250, 250, 210, 1 };
		constexpr rgba LightGray{ 211, 211, 211, 1 };
		constexpr rgba LightPink{ 255, 182, 193, 1 };
		constexpr rgba LightSalmon{ 255, 160, 122, 1 };
		constexpr rgba LightSeaGreen{ 32, 178, 170, 1 };
		constexpr rgba LightSkyBlue{ 135, 206, 250, 1 };
		constexpr rgba LightSlateGray{ 119, 136, 153, 1 };
		constexpr rgba LightSteelBlue{ 176, 196, 222, 1 };
		constexpr rgba LightYellow{ 255, 255, 224, 1 };
		constexpr rgba Lime{ 191, 255, 0, 1 };
		constexpr rgba LimeGreen{ 50, 205, 50, 1 };
		constexpr rgba Linen{ 250, 240, 230, 1 };
		constexpr rgba Magenta{ 255, 0, 255, 1 };
		constexpr rgba Maroon{ 128, 0, 0, 1 };
		constexpr rgba MediumAquamarine{ 102, 205, 170, 1 };
		constexpr rgba MediumBlue{ 0, 0, 205, 1 };
		constexpr rgba MediumOrchid{ 186, 85, 211, 1 };
		constexpr rgba MediumPurple{ 147, 112, 219, 1 };
		constexpr rgba MediumSeaGreen{ 60, 179, 113, 1 };
		constexpr rgba MediumSlateBlue{ 123, 104, 238, 1 };
		constexpr rgba MediumSpringGreen{ 0, 250, 154, 1 };
		constexpr rgba MediumTurquoise{ 72, 209, 204, 1 };
		constexpr rgba MediumVioletRed{ 199, 21, 133, 1 };
		constexpr rgba MidnightBlue{ 25, 25, 112, 1 };
		constexpr rgba MintCream{ 245, 255, 250, 1 };
		constexpr rgba MistyRose{ 255, 228, 225, 1 };
		constexpr rgba Moccasin{ 255, 228, 181, 1 };
		constexpr rgba NajavoWhite{ 255, 222, 173, 1 };
		constexpr rgba Navy{ 0, 0, 128, 1 };
		constexpr rgba OldLace{ 253, 245, 230, 1 };
		constexpr rgba Olive{ 128, 128, 0, 1 };
		constexpr rgba OliveDrab{ 107, 142, 35, 1 };
		constexpr rgba Orange{ 255, 165, 0, 1 };
		constexpr rgba OrangeRed{ 255, 69, 0, 1 };
		constexpr rgba Orchid{ 218, 112, 214, 1 };
		constexpr rgba PaleGoldenrod{ 238, 232, 170, 1 };
		constexpr rgba PaleGreen{ 152, 251, 152, 1 };
		constexpr rgba PaleTurquoise{ 175, 238, 238, 1 };
		constexpr rgba PaleVioletRed{ 219, 112, 147, 1 };
		constexpr rgba PapayaWhip{ 255, 239, 213, 1 };
		constexpr rgba PeachPuff{ 255, 218, 185, 1 };
		constexpr rgba Peru{ 205, 133, 63, 1 };
		constexpr rgba Pink{ 255, 192, 203, 1 };
		constexpr rgba Plum{ 221, 160, 221, 1 };
		constexpr rgba Powder_blue{ 176, 224, 230, 1 };
		constexpr rgba Purple{ 128, 0, 128, 1 };
		constexpr rgba Red{ 255, 0, 0, 1 };
		constexpr rgba RosyBrown{ 188, 143, 143, 1 };
		constexpr rgba RoyalBlue{ 65, 105, 225, 1 };
		constexpr rgba SaddleBrown{ 139, 69, 19, 1 };
		constexpr rgba Salmon{ 255, 140, 105, 1 };
		constexpr rgba SandyBrown{ 244, 164, 96, 1 };
		constexpr rgba SeaGreen{ 46, 139, 87, 1 };
		constexpr rgba SeaShell{ 255, 245, 238, 1 };
		constexpr rgba Sienna{ 136, 45, 23, 1 };
		constexpr rgba Silver{ 192, 192, 192, 1 };
		constexpr rgba SkyBlue{ 135, 206, 235, 1 };
		constexpr rgba SlateBlue{ 106, 90, 205, 1 };
		constexpr rgba SlateGray{ 112, 128, 144, 1 };
		constexpr rgba Snow{ 255, 250, 250, 1 };
		constexpr rgba SpringGreen{ 0, 255, 127, 1 };
		constexpr rgba SteelBlue{ 70, 130, 180, 1 };
		constexpr rgba Tan{ 210, 180, 140, 1 };
		constexpr rgba Teal{ 0, 128, 128, 1 };
		constexpr rgba Thistle{ 216, 191, 216, 1 };
		constexpr rgba Tomato{ 255, 99, 71, 1 };
		constexpr rgba Transparent{ 0, 0, 0, 0 };
		constexpr rgba Turquoise{ 64, 224, 208, 1 };
		constexpr rgba Violet{ 238, 130, 238, 1 };
		constexpr rgba Wheat{ 245, 222, 179, 1 };
		constexpr rgba White{ 255, 255, 255, 1 };
		constexpr rgba WhiteSmoke{ 245, 245, 245, 1 };
		constexpr rgba Yellow{ 255, 255, 0, 1 };
		constexpr rgba YellowGreen{ 154, 205, 50, 1 };
	};
}

#endif // !__ARTEMIS_API_COLOR_HXX__