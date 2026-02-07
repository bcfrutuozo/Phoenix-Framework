#pragma once

#include <cstdint>

enum class KnownColor : uint8_t
{
	Transparent,
	AliceBlue,
	AntiqueWhite,
	Aqua,
	Aquamarine,
	Azure,
	Beige,
	Bisque,
	Black,
	BlanchedAlmond,
	Blue,
	BlueViolet,
	Brown,
	BurlyWood,
	CadetBlue,
	Chartreuse,
	Chocolate,
	Coral,
	CornflowerBlue,
	Cornsilk,
	Crimson,
	Cyan,
	DarkBlue,
	DarkCyan,
	DarkGoldenrod,
	DarkGray,
	DarkGreen,
	DarkKhaki,
	DarkMagenta,
	DarkOliveGreen,
	DarkOrange,
	DarkOrchid,
	DarkRed,
	DarkSalmon,
	DarkSeaGreen,
	DarkSlateBlue,
	DarkSlateGray,
	DarkTurquoise,
	DarkViolet,
	DeepPink,
	DeepSkyBlue,
	DimGray,
	DodgerBlue,
	Firebrick,
	FloralWhite,
	ForestGreen,
	Fuchsia,
	Gainsboro,
	GhostWhite,
	Gold,
	Goldenrod,
	Gray,
	Green,
	GreenYellow,
	Honeydew,
	HotPink,
	IndianRed,
	Indigo,
	Ivory,
	Khaki,
	Lavender,
	LavenderBlush,
	LawnGreen,
	LemonChiffon,
	LightBlue,
	LightCoral,
	LightCyan,
	LightGoldenrodYellow,
	LightGray,
	LightGreen,
	LightPink,
	LightSalmon,
	LightSeaGreen,
	LightSkyBlue,
	LightSlateGray,
	LightSteelBlue,
	LightYellow,
	Lime,
	LimeGreen,
	Linen,
	Magenta,
	Maroon,
	MediumAquamarine,
	MediumBlue,
	MediumOrchid,
	MediumPurple,
	MediumSeaGreen,
	MediumSlateBlue,
	MediumSpringGreen,
	MediumTurquoise,
	MediumVioletRed,
	MidnightBlue,
	MintCream,
	MistyRose,
	Moccasin,
	NavajoWhite,
	Navy,
	OldLace,
	Olive,
	OliveDrab,
	Orange,
	OrangeRed,
	Orchid,
	PaleGoldenrod,
	PaleGreen,
	PaleTurquoise,
	PaleVioletRed,
	PapayaWhip,
	PeachPuff,
	Peru,
	Pink,
	Plum,
	PowderBlue,
	Purple,
	Red,
	RosyBrown,
	RoyalBlue,
	SaddleBrown,
	Salmon,
	SandyBrown,
	SeaGreen,
	SeaShell,
	Sienna,
	Silver,
	SkyBlue,
	SlateBlue,
	SlateGray,
	Snow,
	SpringGreen,
	SteelBlue,
	Tan,
	Teal,
	Thistle,
	Tomato,
	Turquoise,
	Violet,
	Wheat,
	White,
	WhiteSmoke,
	Yellow,
	YellowGreen,
};

static constexpr const char* Names[]
{
	"Transparent",
	"AliceBlue",
	"AntiqueWhite",
	"Aqua",
	"Aquamarine",
	"Azure",
	"Beige",
	"Bisque",
	"Black",
	"BlanchedAlmond",
	"Blue",
	"BlueViolet",
	"Brown",
	"BurlyWood",
	"CadetBlue",
	"Chartreuse",
	"Chocolate",
	"Coral",
	"CornflowerBlue",
	"Cornsilk",
	"Crimson",
	"Cyan",
	"DarkBlue",
	"DarkCyan",
	"DarkGoldenrod",
	"DarkGray",
	"DarkGreen",
	"DarkKhaki",
	"DarkMagenta",
	"DarkOliveGreen",
	"DarkOrange",
	"DarkOrchid",
	"DarkRed",
	"DarkSalmon",
	"DarkSeaGreen",
	"DarkSlateBlue",
	"DarkSlateGray",
	"DarkTurquoise",
	"DarkViolet",
	"DeepPink",
	"DeepSkyBlue",
	"DimGray",
	"DodgerBlue",
	"Firebrick",
	"FloralWhite",
	"ForestGreen",
	"Fuchsia",
	"Gainsboro",
	"GhostWhite",
	"Gold",
	"Goldenrod",
	"Gray",
	"Green",
	"GreenYellow",
	"Honeydew",
	"HotPink",
	"IndianRed",
	"Indigo",
	"Ivory",
	"Khaki",
	"Lavender",
	"LavenderBlush",
	"LawnGreen",
	"LemonChiffon",
	"LightBlue",
	"LightCoral",
	"LightCyan",
	"LightGoldenrodYellow",
	"LightGray",
	"LightGreen",
	"LightPink",
	"LightSalmon",
	"LightSeaGreen",
	"LightSkyBlue",
	"LightSlateGray",
	"LightSteelBlue",
	"LightYellow",
	"Lime",
	"LimeGreen",
	"Linen",
	"Magenta",
	"Maroon",
	"MediumAquamarine",
	"MediumBlue",
	"MediumOrchid",
	"MediumPurple",
	"MediumSeaGreen",
	"MediumSlateBlue",
	"MediumSpringGreen",
	"MediumTurquoise",
	"MediumVioletRed",
	"MidnightBlue",
	"MintCream",
	"MistyRose",
	"Moccasin",
	"NavajoWhite",
	"Navy",
	"OldLace",
	"Olive",
	"OliveDrab",
	"Orange",
	"OrangeRed",
	"Orchid",
	"PaleGoldenrod",
	"PaleGreen",
	"PaleTurquoise",
	"PaleVioletRed",
	"PapayaWhip",
	"PeachPuff",
	"Peru",
	"Pink",
	"Plum",
	"PowderBlue",
	"Purple",
	"Red",
	"RosyBrown",
	"RoyalBlue",
	"SaddleBrown",
	"Salmon",
	"SandyBrown",
	"SeaGreen",
	"SeaShell",
	"Sienna",
	"Silver",
	"SkyBlue",
	"SlateBlue",
	"SlateGray",
	"Snow",
	"SpringGreen",
	"SteelBlue",
	"Tan",
	"Teal",
	"Thistle",
	"Tomato",
	"Turquoise",
	"Violet",
	"Wheat",
	"White",
	"WhiteSmoke",
	"Yellow",
	"YellowGreen",
};

static constexpr uint32_t ColorValueTable[]
{
	0xFFFFFF00,     // Transparent
	0xF0F8FFFF,     // AliceBlue
	0xFAEBD7FF,     // AntiqueWhite
	0x00FFFFFF,     // Aqua
	0x7FFFD4FF,     // Aquamarine
	0xF0FFFFFF,     // Azure
	0xF5F5DCFF,     // Beige
	0xFFE4C4FF,     // Bisque
	0x000000FF,     // Black
	0xFFEBCDFF,     // BlanchedAlmond
	0x0000FFFF,     // Blue
	0x8A2BE2FF,     // BlueViolet
	0xA52A2AFF,     // Brown
	0xDEB887FF,     // BurlyWood
	0x5F9EA0FF,     // CadetBlue
	0x7FFF00FF,     // Chartreuse
	0xD2691EFF,     // Chocolate
	0xFF7F50FF,     // Coral
	0x6495EDFF,     // CornflowerBlue
	0xFFF8DCFF,     // Cornsilk
	0xDC143CFF,     // Crimson
	0x00FFFFFF,     // Cyan
	0x00008BFF,     // DarkBlue
	0x008B8BFF,     // DarkCyan
	0xB8860BFF,     // DarkGoldenrod
	0xA9A9A9FF,     // DarkGray
	0x006400FF,     // DarkGreen
	0xBDB76BFF,     // DarkKhaki
	0x8B008BFF,     // DarkMagenta
	0x556B2FFF,     // DarkOliveGreen
	0xFF8C00FF,     // DarkOrange
	0x9932CCFF,     // DarkOrchid
	0x8B0000FF,     // DarkRed
	0xE9967AFF,     // DarkSalmon
	0x8FBC8FFF,     // DarkSeaGreen
	0x483D8BFF,     // DarkSlateBlue
	0x2F4F4FFF,     // DarkSlateGray
	0x00CED1FF,     // DarkTurquoise
	0x9400D3FF,     // DarkViolet
	0xFF1493FF,     // DeepPink
	0x00BFFFFF,     // DeepSkyBlue
	0x696969FF,     // DimGray
	0x1E90FFFF,     // DodgerBlue
	0xB22222FF,     // Firebrick
	0xFFFAF0FF,     // FloralWhite
	0x228B22FF,     // ForestGreen
	0xFF00FFFF,     // Fuchsia
	0xDCDCDCFF,     // Gainsboro
	0xF8F8FFFF,     // GhostWhite
	0xFFD700FF,     // Gold
	0xDAA520FF,     // Goldenrod
	0x808080FF,     // Gray
	0x008000FF,     // Green
	0xADFF2FFF,     // GreenYellow
	0xF0FFF0FF,     // Honeydew
	0xFF69B4FF,     // HotPink
	0xCD5C5CFF,     // IndianRed
	0x4B0082FF,     // Indigo
	0xFFFFF0FF,     // Ivory
	0xF0E68CFF,     // Khaki
	0xE6E6FAFF,     // Lavender
	0xFFF0F5FF,     // LavenderBlush
	0x7CFC00FF,     // LawnGreen
	0xFFFACDFF,     // LemonChiffon
	0xADD8E6FF,     // LightBlue
	0xF08080FF,     // LightCoral
	0xE0FFFFFF,     // LightCyan
	0xFAFAD2FF,     // LightGoldenrodYellow
	0xD3D3D3FF,     // LightGray
	0x90EE90FF,     // LightGreen
	0xFFB6C1FF,     // LightPink
	0xFFA07AFF,     // LightSalmon
	0x20B2AAFF,     // LightSeaGreen
	0x87CEFAFF,     // LightSkyBlue
	0x778899FF,     // LightSlateGray
	0xB0C4DEFF,     // LightSteelBlue
	0xFFFFE0FF,     // LightYellow
	0x00FF00FF,     // Lime
	0x32CD32FF,     // LimeGreen
	0xFAF0E6FF,     // Linen
	0xFF00FFFF,     // Magenta
	0x800000FF,     // Maroon
	0x66CDAAFF,     // MediumAquamarine
	0x0000CDFF,     // MediumBlue
	0xBA55D3FF,     // MediumOrchid
	0x9370DBFF,     // MediumPurple
	0x3CB371FF,     // MediumSeaGreen
	0x7B68EEFF,     // MediumSlateBlue
	0x00FA9AFF,     // MediumSpringGreen
	0x48D1CCFF,     // MediumTurquoise
	0xC71585FF,     // MediumVioletRed
	0x191970FF,     // MidnightBlue
	0xF5FFFAFF,     // MintCream
	0xFFE4E1FF,     // MistyRose
	0xFFE4B5FF,     // Moccasin
	0xFFDEADFF,     // NavajoWhite
	0x000080FF,     // Navy
	0xFDF5E6FF,     // OldLace
	0x808000FF,     // Olive
	0x6B8E23FF,     // OliveDrab
	0xFFA500FF,     // Orange
	0xFF4500FF,     // OrangeRed
	0xDA70D6FF,     // Orchid
	0xEEE8AAFF,     // PaleGoldenrod
	0x98FB98FF,     // PaleGreen
	0xAFEEEEFF,     // PaleTurquoise
	0xDB7093FF,     // PaleVioletRed
	0xFFEFD5FF,     // PapayaWhip
	0xFFDAB9FF,     // PeachPuff
	0xCD853FFF,     // Peru
	0xFFC0CBFF,     // Pink
	0xDDA0DDFF,     // Plum
	0xB0E0E6FF,     // PowderBlue
	0x800080FF,     // Purple
	0xFF0000FF,     // Red
	0xBC8F8FFF,     // RosyBrown
	0x4169E1FF,     // RoyalBlue
	0x8B4513FF,     // SaddleBrown
	0xFA8072FF,     // Salmon
	0xF4A460FF,     // SandyBrown
	0x2E8B57FF,     // SeaGreen
	0xFFF5EEFF,     // SeaShell
	0xA0522DFF,     // Sienna
	0xC0C0C0FF,     // Silver
	0x87CEEBFF,     // SkyBlue
	0x6A5ACDFF,     // SlateBlue
	0x708090FF,     // SlateGray
	0xFFFAFAFF,     // Snow
	0x00FF7FFF,     // SpringGreen
	0x4682B4FF,     // SteelBlue
	0xD2B48CFF,     // Tan
	0x008080FF,     // Teal
	0xD8BFD8FF,     // Thistle
	0xFF6347FF,     // Tomato
	0x40E0D0FF,     // Turquoise
	0xEE82EEFF,     // Violet
	0xF5DEB3FF,     // Wheat
	0xFFFFFFFF,     // White
	0xF5F5F5FF,     // WhiteSmoke
	0xFFFF00FF,     // Yellow
	0x9ACD32FF,     // YellowGreen
};