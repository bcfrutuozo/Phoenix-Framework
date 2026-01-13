#pragma once

struct VkInstance_T;
struct VkSurfaceKHR_T;

using VkInstance = VkInstance_T*;
using VkSurfaceKHR = VkSurfaceKHR_T*;

class Window; // forward

class VulkanSurface
{
public:
    static VkSurfaceKHR Create(
        VkInstance instance,
        Window* window
    ) noexcept;
};