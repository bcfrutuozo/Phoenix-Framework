#include "Font.hpp"

// Conversão clássica: pontos tipográficos → pixels
static inline Single PointsToPixels(Single pt, Single dpi) noexcept
{
    return (pt * dpi) / 72.0f;
}

Font::Font(const String& name) noexcept
    : Font(name, 10.0, 96.0)
{
}

Font::Font(const String& name, Single size) noexcept
    : Font(name, size, 96.0)
{
}

Font::Font(const String& name, Single size, Single dpi) noexcept
    : _name(name)
    , _size(size)
    , _dpi(dpi)
{

}

String Font::GetName() const noexcept
{
    return _name;
}

Single Font::GetSize() const noexcept
{
    return _size;
}

Single Font::GetDPI() const noexcept
{
    return _dpi;
}

Single Font::GetPixelSize() const noexcept
{
    return PointsToPixels(_size, _dpi);
}

// Esses valores são *proporcionais* e independentes do backend.
// O renderer real (DirectWrite / FreeType / etc) pode refiná-los.
Single Font::GetLineHeight() const noexcept
{
    return GetPixelSize() * 1.25f;
}

Single Font::GetAscent() const noexcept
{
    return GetPixelSize() * 0.80f;
}

Single Font::GetDescent() const noexcept
{
    return GetPixelSize() * 0.20f;
}

Boolean Font::IsValid() const noexcept
{
    return _size > 0.0f;
}

String Font::ToString() const noexcept
{
    return String::Concat("Font: ", _name, " | Size: ", _size.ToString(), " | DPI: ", _dpi.ToString());
}

Boolean Font::Equals(const Font& other) const noexcept
{
    return _name == other.GetName() && _size == other.GetSize() && _dpi == other.GetDPI();
}

u32 Font::GetHashCode() const noexcept
{
    return ToString().GetHashCode();
}