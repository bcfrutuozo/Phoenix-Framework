#include "Pointer.hpp"
#include "String.hpp"

Boolean Pointer::Equals(const Pointer& other) const noexcept
{
	return _value == other._value;
}

UInt32 Pointer::GetHashCode() const noexcept
{
	return _value.GetHashCode();
}

String Pointer::ToString() const noexcept
{
	return _value.ToString();
}