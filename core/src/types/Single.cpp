#include "Single.hpp"
#include "String.hpp"
#include "UInt32.hpp"

Boolean Single::Equals(const Single& other) const noexcept
{
	return Value == other.Value;
}

UInt32 Single::GetHashCode() const noexcept
{
	return *(uint32_t*)&Value;
}

String Single::ToString() const noexcept
{
	char buf[32];
	int n = snprintf(buf, sizeof(buf), "%g", (double)Value);
	return String(buf, n);
}