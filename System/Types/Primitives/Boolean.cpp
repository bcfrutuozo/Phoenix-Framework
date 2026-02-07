#include "Boolean.hpp"
#include "UInt32.hpp"
#include "System/String.hpp"

Boolean Boolean::Equals(const Boolean& other) const noexcept 
{
	return _value == other._value;
}

UInt32 Boolean::GetHashCode() const noexcept
{
	return _value ? 1u : 0u;
}

String Boolean::ToString() const noexcept
{
	return _value ? String("True") : String("False");
}