#include "UIHandle.hpp"

Boolean UIHandle::Equals(const UIHandle& other) const noexcept
{
	return Handle == other.Handle;
}

u32 UIHandle::GetHashCode() const noexcept
{ 
	return Handle.GetHashCode(); 
}

String UIHandle::ToString() const noexcept
{
	return Handle.ToString();
}