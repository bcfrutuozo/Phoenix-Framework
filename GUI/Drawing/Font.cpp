#include "Font.hpp"

Font::Font(u32 id, const String& name) noexcept
    : _id(id)
    , _name(name)
{
}

u32 Font::GetId() const noexcept
{
    return _id;
}

String Font::GetName() const noexcept
{
    return _name;
}

Boolean Font::IsValid() const noexcept
{
    return _id != 0;
}

// Conversão clássica: pontos tipográficos → pixels
static inline Single PointsToPixels(Single pt, Single dpi) noexcept
{
    return (pt * dpi) / 72.0f;
}

FontInstance::FontInstance(Font* font, Single size, Single dpi) noexcept
    : _font(font)
    , _size(size)
    , _dpi(dpi)
{
}

Font* FontInstance::GetFont() const noexcept
{
    return _font;
}

Single FontInstance::GetSize() const noexcept
{
    return _size;
}

Single FontInstance::GetDPI() const noexcept
{
    return _dpi;
}

Single FontInstance::GetPixelSize() const noexcept
{
    return PointsToPixels(_size, _dpi);
}

// Esses valores são *proporcionais* e independentes do backend.
// O renderer real (DirectWrite / FreeType / etc) pode refiná-los.
Single FontInstance::GetLineHeight() const noexcept
{
    return GetPixelSize() * 1.25f;
}

Single FontInstance::GetAscent() const noexcept
{
    return GetPixelSize() * 0.80f;
}

Single FontInstance::GetDescent() const noexcept
{
    return GetPixelSize() * 0.20f;
}

Boolean FontInstance::IsValid() const noexcept
{
    return _font != nullptr && _font->IsValid() && _size > 0.0f;
}