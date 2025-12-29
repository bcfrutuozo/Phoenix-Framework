#pragma once

#include "types/Boolean.hpp"
#include "types/String.hpp"
#include "types/Int32.hpp"

struct WindowDescriptor
{
	String title;
	Int32 width;
	Int32 height;
	Boolean resizable = true;
};

typedef WindowDescriptor WindowDesc;