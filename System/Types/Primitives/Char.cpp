#include "Char.hpp"
#include "System/String.hpp"

Boolean Char::Equals(const Char& other) const noexcept
{
	return _value == other._value;
}

UInt32 Char::GetHashCode() const noexcept
{
	return static_cast<int>(_value) | (static_cast<int>(_value << 16));
}

String Char::ToString() const noexcept
{
	return String::FromChar(_value);
}