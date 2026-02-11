#include "Size.hpp"

Boolean Size::Equals(const Size& other) const noexcept
{
	return _width == other._width && _height == other._height;
}

u32 Size::GetHashCode() const noexcept
{
	// HASHCODE Combine
	return 0;
}

String Size::ToString() const noexcept
{
	return String::Concat("Width: ", _width.ToString(), ", Height: ", _height.ToString());
}