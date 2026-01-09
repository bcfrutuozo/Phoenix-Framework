#include "CodePoint.hpp"
#include "String.hpp"
#include "UInt32.hpp"

Boolean CodePoint::Equals(const CodePoint& other) const noexcept
{
	return Value == other.Value;
}

UInt32 CodePoint::GetHashCode() const noexcept
{
	return UInt32(static_cast<uint32_t>(Value));
}

String CodePoint::ToString() const noexcept
{
	return String::FromCodePoint(Value);
}