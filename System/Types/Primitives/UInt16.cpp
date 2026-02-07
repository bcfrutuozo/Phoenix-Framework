#include "UInt16.hpp"
#include "UInt32.hpp"
#include "System/String.hpp"

Boolean UInt16::Equals(const UInt16& other) const noexcept
{
	return _value == other._value;
}

UInt32 UInt16::GetHashCode() const noexcept
{
	return UInt32(static_cast<uint32_t>(static_cast<int32_t>(_value)));
}

String UInt16::ToString() const noexcept
{
	char buf[8];
	int n = snprintf(buf, sizeof(buf), "%d", (int16_t)_value);
	return String(buf, n);
}