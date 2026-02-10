#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"
#include "KnownColors.hpp"

class Color final : public Object<Color>
{
public:

	inline friend Boolean operator==(const Color& a, const Color& b) noexcept { return a.Equals(b); }
	inline friend Boolean operator!=(const Color& a, const Color& b) noexcept { return !(a == b); }

	inline constexpr u8 GetR() const noexcept { return _r; }
	inline constexpr u8 GetG() const noexcept { return _g; }
	inline constexpr u8 GetB() const noexcept { return _b; }
	inline constexpr u8 GetA() const noexcept { return _a; }

	inline constexpr void SetR(u8 r) noexcept { _r = r; }
	inline constexpr void SetG(u8 g) noexcept { _g = g; }
	inline constexpr void SetB(u8 b) noexcept { _b = b; }
	inline constexpr void SetA(u8 a) noexcept { _a = a; }


	constexpr u32 PackRGBA() const noexcept
	{
		return
			(uint32_t(_r) << 24) |
			(uint32_t(_g) << 16) |
			(uint32_t(_b) << 8) |
			uint32_t(_a);
	}

	constexpr u32 PackARGB() const noexcept
	{
		return
			(uint32_t(_a) << 24) |
			(uint32_t(_r) << 16) |
			(uint32_t(_g) << 8) |
			uint32_t(_b);
	}

	static constexpr Color FromRGB(u8 r, u8 g, u8 b)
	{
		return { r,g,b, 255 };
	}

	static constexpr Color FromRGBA(u32 v) noexcept
	{
		return {
			uint8_t((v >> 24) & 0xFF),
			uint8_t((v >> 16) & 0xFF),
			uint8_t((v >> 8) & 0xFF),
			uint8_t(v & 0xFF)
		};
	}

	static constexpr Color FromRGBA(u8 r, u8 g, u8 b, u8 a = 255)
	{
		return{ r, g, b, a };
	}

	static constexpr Color FromARGB(u32 v) noexcept
	{
		return {
			uint8_t(v & 0xFF),
			uint8_t((v >> 16) & 0xFF),
			uint8_t((v >> 8) & 0xFF),
			uint8_t((v >> 24) & 0xFF)
		};
	}

	static constexpr Color FromARGB(u8 a, u8 r, u8 g, u8 b)
	{
		return { r, g, b, a };
	}

	String GetName() const noexcept;

	static constexpr Boolean IsKnownColor(Color c) noexcept
	{
		const uint32_t argb = c.PackARGB();

		for (uint32_t i = 0; i < CountOf(ColorValueTable); ++i)
		{
			if (ColorValueTable[i] == argb)
				return true;
		}

		return false;
	}

#define DECLARE_KNOWN_COLOR(name) static const Color name;
	DECLARE_KNOWN_COLOR(Transparent)
	DECLARE_KNOWN_COLOR(AliceBlue)
	DECLARE_KNOWN_COLOR(AntiqueWhite)
	DECLARE_KNOWN_COLOR(Aqua)
	DECLARE_KNOWN_COLOR(Aquamarine)
	DECLARE_KNOWN_COLOR(Azure)
	DECLARE_KNOWN_COLOR(Beige)
	DECLARE_KNOWN_COLOR(Bisque)
	DECLARE_KNOWN_COLOR(Black)
	DECLARE_KNOWN_COLOR(BlanchedAlmond)
	DECLARE_KNOWN_COLOR(Blue)
	DECLARE_KNOWN_COLOR(BlueViolet)
	DECLARE_KNOWN_COLOR(Brown)
	DECLARE_KNOWN_COLOR(BurlyWood)
	DECLARE_KNOWN_COLOR(CadetBlue)
	DECLARE_KNOWN_COLOR(Chartreuse)
	DECLARE_KNOWN_COLOR(Chocolate)
	DECLARE_KNOWN_COLOR(Coral)
	DECLARE_KNOWN_COLOR(CornflowerBlue)
	DECLARE_KNOWN_COLOR(Cornsilk)
	DECLARE_KNOWN_COLOR(Crimson)
	DECLARE_KNOWN_COLOR(Cyan)
	DECLARE_KNOWN_COLOR(DarkBlue)
	DECLARE_KNOWN_COLOR(DarkCyan)
	DECLARE_KNOWN_COLOR(DarkGoldenrod)
	DECLARE_KNOWN_COLOR(DarkGray)
	DECLARE_KNOWN_COLOR(DarkGreen)
	DECLARE_KNOWN_COLOR(DarkKhaki)
	DECLARE_KNOWN_COLOR(DarkMagenta)
	DECLARE_KNOWN_COLOR(DarkOliveGreen)
	DECLARE_KNOWN_COLOR(DarkOrange)
	DECLARE_KNOWN_COLOR(DarkOrchid)
	DECLARE_KNOWN_COLOR(DarkRed)
	DECLARE_KNOWN_COLOR(DarkSalmon)
	DECLARE_KNOWN_COLOR(DarkSeaGreen)
	DECLARE_KNOWN_COLOR(DarkSlateBlue)
	DECLARE_KNOWN_COLOR(DarkSlateGray)
	DECLARE_KNOWN_COLOR(DarkTurquoise)
	DECLARE_KNOWN_COLOR(DarkViolet)
	DECLARE_KNOWN_COLOR(DeepPink)
	DECLARE_KNOWN_COLOR(DeepSkyBlue)
	DECLARE_KNOWN_COLOR(DimGray)
	DECLARE_KNOWN_COLOR(DodgerBlue)
	DECLARE_KNOWN_COLOR(Firebrick)
	DECLARE_KNOWN_COLOR(FloralWhite)
	DECLARE_KNOWN_COLOR(ForestGreen)
	DECLARE_KNOWN_COLOR(Fuchsia)
	DECLARE_KNOWN_COLOR(Gainsboro)
	DECLARE_KNOWN_COLOR(GhostWhite)
	DECLARE_KNOWN_COLOR(Gold)
	DECLARE_KNOWN_COLOR(Goldenrod)
	DECLARE_KNOWN_COLOR(Gray)
	DECLARE_KNOWN_COLOR(Green)
	DECLARE_KNOWN_COLOR(GreenYellow)
	DECLARE_KNOWN_COLOR(Honeydew)
	DECLARE_KNOWN_COLOR(HotPink)
	DECLARE_KNOWN_COLOR(IndianRed)
	DECLARE_KNOWN_COLOR(Indigo)
	DECLARE_KNOWN_COLOR(Ivory)
	DECLARE_KNOWN_COLOR(Khaki)
	DECLARE_KNOWN_COLOR(Lavender)
	DECLARE_KNOWN_COLOR(LavenderBlush)
	DECLARE_KNOWN_COLOR(LawnGreen)
	DECLARE_KNOWN_COLOR(LemonChiffon)
	DECLARE_KNOWN_COLOR(LightBlue)
	DECLARE_KNOWN_COLOR(LightCoral)
	DECLARE_KNOWN_COLOR(LightCyan)
	DECLARE_KNOWN_COLOR(LightGoldenrodYellow)
	DECLARE_KNOWN_COLOR(LightGray)
	DECLARE_KNOWN_COLOR(LightGreen)
	DECLARE_KNOWN_COLOR(LightPink)
	DECLARE_KNOWN_COLOR(LightSalmon)
	DECLARE_KNOWN_COLOR(LightSeaGreen)
	DECLARE_KNOWN_COLOR(LightSkyBlue)
	DECLARE_KNOWN_COLOR(LightSlateGray)
	DECLARE_KNOWN_COLOR(LightSteelBlue)
	DECLARE_KNOWN_COLOR(LightYellow)
	DECLARE_KNOWN_COLOR(Lime)
	DECLARE_KNOWN_COLOR(LimeGreen)
	DECLARE_KNOWN_COLOR(Linen)
	DECLARE_KNOWN_COLOR(Magenta)
	DECLARE_KNOWN_COLOR(Maroon)
	DECLARE_KNOWN_COLOR(MediumAquamarine)
	DECLARE_KNOWN_COLOR(MediumBlue)
	DECLARE_KNOWN_COLOR(MediumOrchid)
	DECLARE_KNOWN_COLOR(MediumPurple)
	DECLARE_KNOWN_COLOR(MediumSeaGreen)
	DECLARE_KNOWN_COLOR(MediumSlateBlue)
	DECLARE_KNOWN_COLOR(MediumSpringGreen)
	DECLARE_KNOWN_COLOR(MediumTurquoise)
	DECLARE_KNOWN_COLOR(MediumVioletRed)
	DECLARE_KNOWN_COLOR(MidnightBlue)
	DECLARE_KNOWN_COLOR(MintCream)
	DECLARE_KNOWN_COLOR(MistyRose)
	DECLARE_KNOWN_COLOR(Moccasin)
	DECLARE_KNOWN_COLOR(NavajoWhite)
	DECLARE_KNOWN_COLOR(Navy)
	DECLARE_KNOWN_COLOR(OldLace)
	DECLARE_KNOWN_COLOR(Olive)
	DECLARE_KNOWN_COLOR(OliveDrab)
	DECLARE_KNOWN_COLOR(Orange)
	DECLARE_KNOWN_COLOR(OrangeRed)
	DECLARE_KNOWN_COLOR(Orchid)
	DECLARE_KNOWN_COLOR(PaleGoldenrod)
	DECLARE_KNOWN_COLOR(PaleGreen)
	DECLARE_KNOWN_COLOR(PaleTurquoise)
	DECLARE_KNOWN_COLOR(PaleVioletRed)
	DECLARE_KNOWN_COLOR(PapayaWhip)
	DECLARE_KNOWN_COLOR(PeachPuff)
	DECLARE_KNOWN_COLOR(Peru)
	DECLARE_KNOWN_COLOR(Pink)
	DECLARE_KNOWN_COLOR(Plum)
	DECLARE_KNOWN_COLOR(PowderBlue)
	DECLARE_KNOWN_COLOR(Purple)
	DECLARE_KNOWN_COLOR(Red)
	DECLARE_KNOWN_COLOR(RosyBrown)
	DECLARE_KNOWN_COLOR(RoyalBlue)
	DECLARE_KNOWN_COLOR(SaddleBrown)
	DECLARE_KNOWN_COLOR(Salmon)
	DECLARE_KNOWN_COLOR(SandyBrown)
	DECLARE_KNOWN_COLOR(SeaGreen)
	DECLARE_KNOWN_COLOR(SeaShell)
	DECLARE_KNOWN_COLOR(Sienna)
	DECLARE_KNOWN_COLOR(Silver)
	DECLARE_KNOWN_COLOR(SkyBlue)
	DECLARE_KNOWN_COLOR(SlateBlue)
	DECLARE_KNOWN_COLOR(SlateGray)
	DECLARE_KNOWN_COLOR(Snow)
	DECLARE_KNOWN_COLOR(SpringGreen)
	DECLARE_KNOWN_COLOR(SteelBlue)
	DECLARE_KNOWN_COLOR(Tan)
	DECLARE_KNOWN_COLOR(Teal)
	DECLARE_KNOWN_COLOR(Thistle)
	DECLARE_KNOWN_COLOR(Tomato)
	DECLARE_KNOWN_COLOR(Turquoise)
	DECLARE_KNOWN_COLOR(Violet)
	DECLARE_KNOWN_COLOR(Wheat)
	DECLARE_KNOWN_COLOR(White)
	DECLARE_KNOWN_COLOR(WhiteSmoke)
	DECLARE_KNOWN_COLOR(Yellow)
	DECLARE_KNOWN_COLOR(YellowGreen)
#undef DECLARE_KNOWN_COLOR

	Boolean Equals(const Color& other) const noexcept;
	UInt32 GetHashCode() const noexcept;
	String ToString() const noexcept;

private:

	static constexpr Color FromKnownColor(KnownColor kc) noexcept
	{
		uint32_t index = (uint32_t)kc;
		return FromRGBA(ColorValueTable[index]);
	}

	constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
		:
		_r(r),
		_g(g),
		_b(b),
		_a(a)
	{
	}

	u8 _r;
	u8 _g;
	u8 _b;
	u8 _a;
};

#define DEFINE_KNOWN_COLOR(name) inline const Color Color::name = Color::FromKnownColor(KnownColor::name);
	DEFINE_KNOWN_COLOR(Transparent)
	DEFINE_KNOWN_COLOR(AliceBlue)
	DEFINE_KNOWN_COLOR(AntiqueWhite)
	DEFINE_KNOWN_COLOR(Aqua)
	DEFINE_KNOWN_COLOR(Aquamarine)
	DEFINE_KNOWN_COLOR(Azure)
	DEFINE_KNOWN_COLOR(Beige)
	DEFINE_KNOWN_COLOR(Bisque)
	DEFINE_KNOWN_COLOR(Black)
	DEFINE_KNOWN_COLOR(BlanchedAlmond)
	DEFINE_KNOWN_COLOR(Blue)
	DEFINE_KNOWN_COLOR(BlueViolet)
	DEFINE_KNOWN_COLOR(Brown)
	DEFINE_KNOWN_COLOR(BurlyWood)
	DEFINE_KNOWN_COLOR(CadetBlue)
	DEFINE_KNOWN_COLOR(Chartreuse)
	DEFINE_KNOWN_COLOR(Chocolate)
	DEFINE_KNOWN_COLOR(Coral)
	DEFINE_KNOWN_COLOR(CornflowerBlue)
	DEFINE_KNOWN_COLOR(Cornsilk)
	DEFINE_KNOWN_COLOR(Crimson)
	DEFINE_KNOWN_COLOR(Cyan)
	DEFINE_KNOWN_COLOR(DarkBlue)
	DEFINE_KNOWN_COLOR(DarkCyan)
	DEFINE_KNOWN_COLOR(DarkGoldenrod)
	DEFINE_KNOWN_COLOR(DarkGray)
	DEFINE_KNOWN_COLOR(DarkGreen)
	DEFINE_KNOWN_COLOR(DarkKhaki)
	DEFINE_KNOWN_COLOR(DarkMagenta)
	DEFINE_KNOWN_COLOR(DarkOliveGreen)
	DEFINE_KNOWN_COLOR(DarkOrange)
	DEFINE_KNOWN_COLOR(DarkOrchid)
	DEFINE_KNOWN_COLOR(DarkRed)
	DEFINE_KNOWN_COLOR(DarkSalmon)
	DEFINE_KNOWN_COLOR(DarkSeaGreen)
	DEFINE_KNOWN_COLOR(DarkSlateBlue)
	DEFINE_KNOWN_COLOR(DarkSlateGray)
	DEFINE_KNOWN_COLOR(DarkTurquoise)
	DEFINE_KNOWN_COLOR(DarkViolet)
	DEFINE_KNOWN_COLOR(DeepPink)
	DEFINE_KNOWN_COLOR(DeepSkyBlue)
	DEFINE_KNOWN_COLOR(DimGray)
	DEFINE_KNOWN_COLOR(DodgerBlue)
	DEFINE_KNOWN_COLOR(Firebrick)
	DEFINE_KNOWN_COLOR(FloralWhite)
	DEFINE_KNOWN_COLOR(ForestGreen)
	DEFINE_KNOWN_COLOR(Fuchsia)
	DEFINE_KNOWN_COLOR(Gainsboro)
	DEFINE_KNOWN_COLOR(GhostWhite)
	DEFINE_KNOWN_COLOR(Gold)
	DEFINE_KNOWN_COLOR(Goldenrod)
	DEFINE_KNOWN_COLOR(Gray)
	DEFINE_KNOWN_COLOR(Green)
	DEFINE_KNOWN_COLOR(GreenYellow)
	DEFINE_KNOWN_COLOR(Honeydew)
	DEFINE_KNOWN_COLOR(HotPink)
	DEFINE_KNOWN_COLOR(IndianRed)
	DEFINE_KNOWN_COLOR(Indigo)
	DEFINE_KNOWN_COLOR(Ivory)
	DEFINE_KNOWN_COLOR(Khaki)
	DEFINE_KNOWN_COLOR(Lavender)
	DEFINE_KNOWN_COLOR(LavenderBlush)
	DEFINE_KNOWN_COLOR(LawnGreen)
	DEFINE_KNOWN_COLOR(LemonChiffon)
	DEFINE_KNOWN_COLOR(LightBlue)
	DEFINE_KNOWN_COLOR(LightCoral)
	DEFINE_KNOWN_COLOR(LightCyan)
	DEFINE_KNOWN_COLOR(LightGoldenrodYellow)
	DEFINE_KNOWN_COLOR(LightGray)
	DEFINE_KNOWN_COLOR(LightGreen)
	DEFINE_KNOWN_COLOR(LightPink)
	DEFINE_KNOWN_COLOR(LightSalmon)
	DEFINE_KNOWN_COLOR(LightSeaGreen)
	DEFINE_KNOWN_COLOR(LightSkyBlue)
	DEFINE_KNOWN_COLOR(LightSlateGray)
	DEFINE_KNOWN_COLOR(LightSteelBlue)
	DEFINE_KNOWN_COLOR(LightYellow)
	DEFINE_KNOWN_COLOR(Lime)
	DEFINE_KNOWN_COLOR(LimeGreen)
	DEFINE_KNOWN_COLOR(Linen)
	DEFINE_KNOWN_COLOR(Magenta)
	DEFINE_KNOWN_COLOR(Maroon)
	DEFINE_KNOWN_COLOR(MediumAquamarine)
	DEFINE_KNOWN_COLOR(MediumBlue)
	DEFINE_KNOWN_COLOR(MediumOrchid)
	DEFINE_KNOWN_COLOR(MediumPurple)
	DEFINE_KNOWN_COLOR(MediumSeaGreen)
	DEFINE_KNOWN_COLOR(MediumSlateBlue)
	DEFINE_KNOWN_COLOR(MediumSpringGreen)
	DEFINE_KNOWN_COLOR(MediumTurquoise)
	DEFINE_KNOWN_COLOR(MediumVioletRed)
	DEFINE_KNOWN_COLOR(MidnightBlue)
	DEFINE_KNOWN_COLOR(MintCream)
	DEFINE_KNOWN_COLOR(MistyRose)
	DEFINE_KNOWN_COLOR(Moccasin)
	DEFINE_KNOWN_COLOR(NavajoWhite)
	DEFINE_KNOWN_COLOR(Navy)
	DEFINE_KNOWN_COLOR(OldLace)
	DEFINE_KNOWN_COLOR(Olive)
	DEFINE_KNOWN_COLOR(OliveDrab)
	DEFINE_KNOWN_COLOR(Orange)
	DEFINE_KNOWN_COLOR(OrangeRed)
	DEFINE_KNOWN_COLOR(Orchid)
	DEFINE_KNOWN_COLOR(PaleGoldenrod)
	DEFINE_KNOWN_COLOR(PaleGreen)
	DEFINE_KNOWN_COLOR(PaleTurquoise)
	DEFINE_KNOWN_COLOR(PaleVioletRed)
	DEFINE_KNOWN_COLOR(PapayaWhip)
	DEFINE_KNOWN_COLOR(PeachPuff)
	DEFINE_KNOWN_COLOR(Peru)
	DEFINE_KNOWN_COLOR(Pink)
	DEFINE_KNOWN_COLOR(Plum)
	DEFINE_KNOWN_COLOR(PowderBlue)
	DEFINE_KNOWN_COLOR(Purple)
	DEFINE_KNOWN_COLOR(Red)
	DEFINE_KNOWN_COLOR(RosyBrown)
	DEFINE_KNOWN_COLOR(RoyalBlue)
	DEFINE_KNOWN_COLOR(SaddleBrown)
	DEFINE_KNOWN_COLOR(Salmon)
	DEFINE_KNOWN_COLOR(SandyBrown)
	DEFINE_KNOWN_COLOR(SeaGreen)
	DEFINE_KNOWN_COLOR(SeaShell)
	DEFINE_KNOWN_COLOR(Sienna)
	DEFINE_KNOWN_COLOR(Silver)
	DEFINE_KNOWN_COLOR(SkyBlue)
	DEFINE_KNOWN_COLOR(SlateBlue)
	DEFINE_KNOWN_COLOR(SlateGray)
	DEFINE_KNOWN_COLOR(Snow)
	DEFINE_KNOWN_COLOR(SpringGreen)
	DEFINE_KNOWN_COLOR(SteelBlue)
	DEFINE_KNOWN_COLOR(Tan)
	DEFINE_KNOWN_COLOR(Teal)
	DEFINE_KNOWN_COLOR(Thistle)
	DEFINE_KNOWN_COLOR(Tomato)
	DEFINE_KNOWN_COLOR(Turquoise)
	DEFINE_KNOWN_COLOR(Violet)
	DEFINE_KNOWN_COLOR(Wheat)
	DEFINE_KNOWN_COLOR(White)
	DEFINE_KNOWN_COLOR(WhiteSmoke)
	DEFINE_KNOWN_COLOR(Yellow)
	DEFINE_KNOWN_COLOR(YellowGreen)
#undef DEFINE_KNOWN_COLOR