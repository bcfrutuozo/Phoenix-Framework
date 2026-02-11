#include "Padding.hpp"

Boolean Padding::Equals(const Padding& other) const noexcept
{
	return _left == other._left && _top == other._top && _right == other._right && _bottom == other._bottom;
}

u32 Padding::GetHashCode() const noexcept
{
	// HASHCODE Combine
	return 0;
}

String Padding::ToString() const noexcept
{
	return String::Concat("Left: ", _left.ToString(), ", Top: ", _top.ToString(), ", Right: ", _right.ToString(), ", Bottom: ", _bottom.ToString());
}