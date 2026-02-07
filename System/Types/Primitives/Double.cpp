#include "Double.hpp"
#include "System/String.hpp"

Boolean Double::Equals(const Double& other) const noexcept
{
	return _value == other._value;
}

UInt32 Double::GetHashCode() const noexcept
{
	uint64_t bits = *(uint64_t*)&_value;
	return (int)(bits ^ (bits >> 32));
}

String Double::ToString() const noexcept
{
	char buf[64];
	int n = snprintf(buf, sizeof(buf), "%g", _value);
	return String(buf, n);
}