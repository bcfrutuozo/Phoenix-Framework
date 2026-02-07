#pragma once

#include <cstdint>

enum class TextFormat : uint8_t
{
	None = 0,
	SingleLine = 1 << 0,
	WordWrap = 1 << 1,
	Ellipsis = 1 << 2,
	NoClip = 1 << 3,
};


inline TextFormat operator|(TextFormat a, TextFormat b)
{
	return static_cast<TextFormat>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline TextFormat operator&(TextFormat a, TextFormat b)
{
	return static_cast<TextFormat>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline TextFormat operator~(TextFormat a)
{
	return static_cast<TextFormat>(~static_cast<uint8_t>(a));
}

inline TextFormat& operator|=(TextFormat& a, TextFormat b)
{
	a = a | b;
	return a;
}

inline TextFormat& operator&=(TextFormat& a, TextFormat b)
{
	a = a & b;
	return a;
}