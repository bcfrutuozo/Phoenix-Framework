#include "Int32.hpp"
#include "String.hpp"
#include "UInt32.hpp"

Boolean Int32::Equals(const Int32& other) const noexcept
{
	return Value == other.Value;
}

UInt32 Int32::GetHashCode() const noexcept
{
	return UInt32(static_cast<uint32_t>(Value));
}

String Int32::ToString() const noexcept
{
	char buf[16];
	int n = snprintf(buf, sizeof(buf), "%d", (int32_t)Value);
	return String(buf, n);
}