#include "CodePoint.hpp"
#include "System/String.hpp"
#include "UInt32.hpp"

Boolean CodePoint::Equals(const CodePoint& other) const noexcept
{
	return _value == other._value;
}

UInt32 CodePoint::GetHashCode() const noexcept
{
	return UInt32(static_cast<uint32_t>(_value));
}

String CodePoint::ToString() const noexcept
{
	return String::FromCodePoint(_value);
}