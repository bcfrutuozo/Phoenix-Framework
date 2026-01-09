#include "Boolean.hpp"
#include "UInt32.hpp"
#include "String.hpp"

Boolean Boolean::Equals(const Boolean& other) const noexcept 
{
	return Value == other.Value;
}

UInt32 Boolean::GetHashCode() const noexcept
{
	return Value ? 1u : 0u;
}

String Boolean::ToString() const noexcept
{
	return Value ? String("True") : String("False");
}