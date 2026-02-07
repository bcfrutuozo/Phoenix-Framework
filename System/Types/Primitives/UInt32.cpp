#include "UInt32.hpp"
#include "System/String.hpp"

Boolean UInt32::Equals(const UInt32& other) const noexcept
{
	return _value == other._value;
}

UInt32 UInt32::GetHashCode() const noexcept
{
	return _value;
}

String UInt32::ToString() const noexcept
{
	char buf[16];
	int n = snprintf(buf, sizeof(buf), "%u", (uint32_t)_value);
	return String(buf, n);
}