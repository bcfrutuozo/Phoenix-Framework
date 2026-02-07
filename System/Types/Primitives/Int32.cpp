#include "Int32.hpp"
#include "System/String.hpp"
#include "UInt32.hpp"

Boolean Int32::Equals(const Int32& other) const noexcept
{
	return _value == other._value;
}

UInt32 Int32::GetHashCode() const noexcept
{
	return UInt32(static_cast<uint32_t>(_value));
}

String Int32::ToString() const noexcept
{
	char buf[16];
	int n = snprintf(buf, sizeof(buf), "%d", (int32_t)_value);
	return String(buf, n);
}