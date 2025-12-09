#include "Int64.hpp"
#include "UInt32.hpp"
#include "String.hpp"

Boolean Int64::Equals(const Int64& other) const noexcept
{
	return Value == other.Value;
}

UInt32 Int64::GetHashCode() const noexcept
{
	uint64_t v = static_cast<uint64_t>(Value);
	return UInt32(static_cast<uint32_t>(v ^ (v >> 32)));
}

String Int64::ToString() const noexcept
{
	char buf[32];
	int n = snprintf(buf, sizeof(buf), "%lld", (long long)Value);
	return String(buf);
}