#include "Int64.hpp"
#include "UInt32.hpp"
#include "System/String.hpp"

Boolean Int64::Equals(const Int64& other) const noexcept
{
	return _value == other._value;
}

UInt32 Int64::GetHashCode() const noexcept
{
	uint64_t v = static_cast<uint64_t>(_value);
	return UInt32(static_cast<uint32_t>(v ^ (v >> 32)));
}

String Int64::ToString() const noexcept
{
	char buf[32];
	int n = snprintf(buf, sizeof(buf), "%lld", (long long)_value);
	return String(buf, n);
}