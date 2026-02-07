#include "Byte.hpp"
#include "System/String.hpp"

Boolean Byte::Equals(const Byte& other) const noexcept
{
	return _value == other._value;
}

UInt32 Byte::GetHashCode() const noexcept
{
	return _value;
}

String Byte::ToString() const noexcept
{
	char buf[4];
	int n = snprintf(buf, sizeof(buf), "%u", (unsigned)_value);
	return String(buf, n);
}