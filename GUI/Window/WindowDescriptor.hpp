#pragma once

#include "System/Types/Boolean.hpp"
#include "System/Types/Int32.hpp"
#include "System/String.hpp"

struct WindowDescriptor
{
	String title;
	Int32 width;
	Int32 height;
	Boolean resizable = true;
};

using WindowDesc = WindowDescriptor;