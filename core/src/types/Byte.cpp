#include "Byte.hpp"
#include "String.hpp"

Boolean Byte::Equals(const Byte& other) const noexcept
{
	return Value == other.Value;
}

UInt32 Byte::GetHashCode() const noexcept
{
	return Value;
}

String Byte::ToString() const noexcept
{
	char buf[4];
	int n = snprintf(buf, sizeof(buf), "%u", (unsigned)Value);
	return String(buf);
}