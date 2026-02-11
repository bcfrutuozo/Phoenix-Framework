#include "Point.hpp"

Boolean Point::Equals(const Point& other) const noexcept
{
	return _x == other._x && _y == other._y;
}

u32 Point::GetHashCode() const noexcept
{
	// HASHCODE Combine
	return 0;
}

String Point::ToString() const noexcept
{
	return String::Concat("X: ", _x.ToString(), ", Y: ", _y.ToString());
}