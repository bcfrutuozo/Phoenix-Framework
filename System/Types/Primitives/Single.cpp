#include "Single.hpp"
#include "System/String.hpp"
#include "UInt32.hpp"

Boolean Single::Equals(const Single& other) const noexcept
{
	return _value == other._value;
}

UInt32 Single::GetHashCode() const noexcept
{
	return *(uint32_t*)&_value;
}

String Single::ToString() const noexcept
{
	char buf[32];
	int n = snprintf(buf, sizeof(buf), "%g", (double)_value);
	return String(buf, n);
}