#include "Int16.hpp"
#include "String.hpp"
#include "UInt32.hpp"

Boolean Int16::Equals(const Int16& other) const noexcept
{
	return Value == other.Value;
}

UInt32 Int16::GetHashCode() const noexcept
{
	return UInt32(static_cast<uint32_t>(Value));
}

String Int16::ToString() const noexcept
{
	char buf[8];
	int n = snprintf(buf, sizeof(buf), "%d", (int16_t)Value);
	return String(buf, n);
}