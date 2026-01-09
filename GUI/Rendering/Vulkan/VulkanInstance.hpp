#pragma once

#include <vulkan/vulkan.h>

#include "System/Collections/List.hpp"

VkInstance CreateVulkanInstance(const List<const char*>& extensions);