#include "UInt32.hpp"
#include "String.hpp"

Boolean UInt32::Equals(const UInt32& other) const noexcept
{
	return Value == other.Value;
}

UInt32 UInt32::GetHashCode() const noexcept
{
	return Value;
}

String UInt32::ToString() const noexcept
{
	char buf[16];
	int n = snprintf(buf, sizeof(buf), "%u", (uint32_t)Value);
	return String(buf, n);
}