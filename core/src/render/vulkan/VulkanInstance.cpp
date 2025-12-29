#include "VulkanInstance.hpp"

#include <cstring>

VkInstance CreateVulkanInstance(const List<const char*>& extensions)
{
    VkApplicationInfo app{};
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pApplicationName = "Phoenix";
    app.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app.pEngineName = "PhoenixEngine";
    app.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo ci{};
    ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ci.pApplicationInfo = &app;
    ci.enabledExtensionCount =
        static_cast<uint32_t>(extensions.Count());
    ci.ppEnabledExtensionNames = extensions.Data();

#ifdef _DEBUG
    const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
    ci.enabledLayerCount = 1;
    ci.ppEnabledLayerNames = layers;
#endif

    VkInstance instance = VK_NULL_HANDLE;
    VkResult r = vkCreateInstance(&ci, nullptr, &instance);
    if (r != VK_SUCCESS)
        return VK_NULL_HANDLE;

    return instance;
}