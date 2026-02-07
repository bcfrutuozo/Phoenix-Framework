#include "SByte.hpp"
#include "UInt32.hpp"
#include "System/String.hpp"

Boolean SByte::Equals(const SByte& other) const noexcept
{
	return _value == other._value;
}

UInt32 SByte::GetHashCode() const noexcept
{
	return UInt32(static_cast<uint32_t>(_value));
}

String SByte::ToString() const noexcept
{
	char buf[4];
	int n = snprintf(buf, sizeof(buf), "%i", (signed)_value);
	return String(buf, n);
}