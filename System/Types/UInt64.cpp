#include "UInt64.hpp"
#include "UInt32.hpp"
#include "String.hpp"

Boolean UInt64::Equals(const UInt64& other) const noexcept
{
	return Value == other.Value;
}

UInt32 UInt64::GetHashCode() const noexcept
{
	uint64_t v = Value;
	return UInt32(static_cast<uint32_t>(v ^ (v >> 32)));
}

String UInt64::ToString() const noexcept
{
	char buf[32];
	int n = snprintf(buf, sizeof(buf), "%llu", (uint64_t)Value);
	return String(buf, n);
}