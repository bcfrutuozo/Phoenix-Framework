#include "Char.hpp"
#include "String.hpp"

Boolean Char::Equals(const Char& other) const noexcept
{
	return Value == other.Value;
}

UInt32 Char::GetHashCode() const noexcept
{
	return static_cast<int>(Value) | (static_cast<int>(Value << 16));
}

String Char::ToString() const noexcept
{
	return String::FromChar(Value);
}