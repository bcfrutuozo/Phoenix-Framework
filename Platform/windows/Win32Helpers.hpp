#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0601
#define WINVER       0x0601

#include <windows.h>

static COLORREF ToCOLORREF(Color c) noexcept
{
	return RGB(c.GetR(), c.GetG(), c.GetB());
}

static UINT Win32ConvertTextFlags(HorizontalAlignment ha, VerticalAlignment va, TextFormat format)
{
	UINT flags = 0;

	// Horizontal
	switch (ha)
	{
	case HorizontalAlignment::Left:   flags |= DT_LEFT;   break;
	case HorizontalAlignment::Center: flags |= DT_CENTER; break;
	case HorizontalAlignment::Right:  flags |= DT_RIGHT;  break;
	case HorizontalAlignment::Stretch:
		flags |= DT_LEFT; // Win32 não tem stretch real
		break;
	}

	// Vertical
	switch (va)
	{
	case VerticalAlignment::Top:    flags |= DT_TOP;     break;
	case VerticalAlignment::Center: flags |= DT_VCENTER; break;
	case VerticalAlignment::Bottom: flags |= DT_BOTTOM;  break;
	case VerticalAlignment::Stretch:
		flags |= DT_VCENTER;
		break;
	}

	// TextFormat flags
	if ((format & TextFormat::SingleLine) != TextFormat::None)
		flags |= DT_SINGLELINE;

	if ((format & TextFormat::WordWrap) != TextFormat::None)
		flags |= DT_WORDBREAK;

	if ((format & TextFormat::Ellipsis) != TextFormat::None)
		flags |= DT_END_ELLIPSIS;

	if ((format & TextFormat::NoClip) != TextFormat::None)
		flags |= DT_NOCLIP;

	// Defaults seguros
	if ((flags & DT_SINGLELINE) == 0 &&
		(flags & DT_WORDBREAK) == 0)
	{
		flags |= DT_SINGLELINE;
	}

	return flags;
}