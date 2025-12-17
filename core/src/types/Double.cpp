#include "Double.hpp"
#include "String.hpp"

Boolean Double::Equals(const Double& other) const noexcept
{
	return Value == other.Value;
}

UInt32 Double::GetHashCode() const noexcept
{
	uint64_t bits = *(uint64_t*)&Value;
	return (int)(bits ^ (bits >> 32));
}

String Double::ToString() const noexcept
{
	char buf[64];
	int n = snprintf(buf, sizeof(buf), "%g", Value);
	return String(buf, n);
}