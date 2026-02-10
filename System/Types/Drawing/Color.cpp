#include "Color.hpp"

String Color::GetName() const noexcept
{
	const uint32_t argb = PackARGB();

	for (uint32_t i = 0; i < CountOf(ColorValueTable); ++i)
	{
		if (ColorValueTable[i] == argb)
			return Names[i];
	}

	return "Unknown Color";
}

Boolean Color::Equals(const Color& other) const noexcept
{
	return _r == other._r && _g == other._g && _b == other._b && _a == other._a;
}

UInt32 Color::GetHashCode() const noexcept
{
	return PackRGBA().GetHashCode();
}

String Color::ToString() const noexcept
{
	return String::Concat("Name: ", GetName(), " | R: ", _r.ToString(), " | G: ", _g.ToString(), " | B : ", _b.ToString());
}