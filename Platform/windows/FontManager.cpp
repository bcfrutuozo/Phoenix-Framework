#include "GUI/Context/FontManager.hpp"
#include "GUI/Drawing/Font.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

Pointer FontManager::create_native_object(Font* fi, Boolean shouldCache) noexcept
{
	// Font size em pixels (negativo = altura do glyph)
	const Single pixelSize = fi->GetPixelSize();

	LOGFONTW lf{};
	lf.lfHeight = -static_cast<LONG>((float)pixelSize);
	lf.lfWeight = FW_NORMAL;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfQuality = CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

	auto wdName = fi->GetName().ToWideCharArray();
	const wchar_t* src = wdName.GetData();
	wcsncpy_s(lf.lfFaceName, LF_FACESIZE, src, _TRUNCATE);

	Pointer createdFont = Pointer(CreateFontIndirectW(&lf));
	if (shouldCache) _cache.Insert(fi, createdFont);
	return createdFont;
}

void FontManager::destroy_native_object(Pointer ptr) noexcept
{
	DeleteObject(ptr.Get());
}