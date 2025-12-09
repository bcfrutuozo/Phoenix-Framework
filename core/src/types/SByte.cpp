#include "SByte.hpp"
#include "UInt32.hpp"
#include "String.hpp"

Boolean SByte::Equals(const SByte& other) const noexcept
{
	return Value == other.Value;
}

UInt32 SByte::GetHashCode() const noexcept
{
	return UInt32(static_cast<uint32_t>(Value));
}

String SByte::ToString() const noexcept
{
	char buf[4];
	int n = snprintf(buf, sizeof(buf), "%i", (signed)Value);
	return String(buf);
}