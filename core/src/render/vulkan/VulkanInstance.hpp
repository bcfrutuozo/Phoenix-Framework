#pragma once

#include <vulkan/vulkan.h>

#include "collections/List.hpp"

VkInstance CreateVulkanInstance(const List<const char*>& extensions);