#include "VulkanSurface.hpp"

#define WIN32_LEAN_AND_MEAN
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <windows.h>

#include "GUI/Window/Window.hpp"

VkSurfaceKHR VulkanSurface::Create(VkInstance instance, Window* window) noexcept
{
    auto native = window->GetSurfaceHandle();

    VkWin32SurfaceCreateInfoKHR info{};
    info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    info.hinstance = static_cast<HINSTANCE>(native.display.value.Get());
    info.hwnd = static_cast<HWND>(native.window.value.Get());

    VkSurfaceKHR surface = VK_NULL_HANDLE;

    // MOVER
    const char* extensions[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };

    VkInstanceCreateInfo ci{};
    ci.enabledExtensionCount = 2;
    ci.ppEnabledExtensionNames = extensions;

    VkResult r = vkCreateWin32SurfaceKHR(
        instance,
        &info,
        nullptr,
        &surface
    );

    if (r != VK_SUCCESS)
        return VK_NULL_HANDLE;

    return surface;
}