#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

#include "VulkanContext.hpp"
#include "VulkanSurface.hpp"
#include "VulkanInstance.hpp"
#include "window/Window.hpp"
#include "collections/List.hpp"

#include <cassert>


VulkanContext::VulkanContext(const Window& window)
{
    List<const char*> lst;
    lst.Add(VK_KHR_SURFACE_EXTENSION_NAME);
    lst.Add(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

    _instance = CreateVulkanInstance(lst);
    _surface = VulkanSurface::Create(_instance, window);

    pickPhysicalDevice();
    createDevice();
    createSwapchain();

    createRenderPass();
    createImageViews();
    createFramebuffers();
    createCommandPool();
    allocateCommandBuffers();
    recordCommandBuffers();
    createSyncObjects();
}

VulkanContext::~VulkanContext()
{
    if (_device)
        vkDeviceWaitIdle(_device);

    if (_imageAvailable)
        vkDestroySemaphore(_device, _imageAvailable, nullptr);
    if (_renderFinished)
        vkDestroySemaphore(_device, _renderFinished, nullptr);

    if (_commandPool)
        vkDestroyCommandPool(_device, _commandPool, nullptr);

    for (uint32_t i = 0; i < _framebuffers.Count(); ++i)
        vkDestroyFramebuffer(_device, _framebuffers[i], nullptr);

    for (uint32_t i = 0; i < _imageViews.Count(); ++i)
        vkDestroyImageView(_device, _imageViews[i], nullptr);

    if (_renderPass)
        vkDestroyRenderPass(_device, _renderPass, nullptr);

    if (_swapchain)
        vkDestroySwapchainKHR(_device, _swapchain, nullptr);

    if (_device)
        vkDestroyDevice(_device, nullptr);

    if (_surface)
        vkDestroySurfaceKHR(_instance, _surface, nullptr);

    if (_instance)
        vkDestroyInstance(_instance, nullptr);
}

void VulkanContext::RenderFrame()
{
    uint32_t imageIndex = 0;

    VkResult res = vkAcquireNextImageKHR(
        _device,
        _swapchain,
        UINT64_MAX,
        _imageAvailable,
        VK_NULL_HANDLE,
        &imageIndex
    );

    // 🔁 Resize necessário
    if (res == VK_ERROR_OUT_OF_DATE_KHR || _framebufferResized)
    {
        _framebufferResized = false;
        RecreateSwapchain();
        return;
    }

    if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR)
        return;

    VkPipelineStageFlags waitStage =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submit{};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.waitSemaphoreCount = 1;
    submit.pWaitSemaphores = &_imageAvailable;
    submit.pWaitDstStageMask = &waitStage;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &_commandBuffers[imageIndex];
    submit.signalSemaphoreCount = 1;
    submit.pSignalSemaphores = &_renderFinished;

    vkQueueSubmit(_graphicsQueue, 1, &submit, VK_NULL_HANDLE);

    VkPresentInfoKHR present{};
    present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present.waitSemaphoreCount = 1;
    present.pWaitSemaphores = &_renderFinished;
    present.swapchainCount = 1;
    present.pSwapchains = &_swapchain;
    present.pImageIndices = &imageIndex;

    VkResult pres = vkQueuePresentKHR(_presentQueue, &present);

    if (pres == VK_ERROR_OUT_OF_DATE_KHR || pres == VK_SUBOPTIMAL_KHR)
    {
        _framebufferResized = false;
        RecreateSwapchain();
    }
}

void VulkanContext::OnResize(uint32_t w, uint32_t h)
{
    if (w == 0 || h == 0)
        return; // janela minimizada

    _width = w;
    _height = h;
    _framebufferResized = true;
}

void VulkanContext::RecreateSwapchain()
{
    if (_width == 0 || _height == 0)
        return;

    vkDeviceWaitIdle(_device);

    // cleanup dependente do swapchain
    for (uint32_t i = 0; i < _framebuffers.Count(); ++i)
        vkDestroyFramebuffer(_device, _framebuffers[i], nullptr);

    for (uint32_t i = 0; i < _imageViews.Count(); ++i)
        vkDestroyImageView(_device, _imageViews[i], nullptr);

    vkDestroySwapchainKHR(_device, _swapchain, nullptr);

    _framebuffers.Clear();
    _imageViews.Clear();
    _swapchainImages.Clear();

    createSwapchain();
    createImageViews();
    createFramebuffers();

    recordCommandBuffers();
}

void VulkanContext::pickPhysicalDevice()
{
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(_instance, &count, nullptr);
    assert(count > 0);

    List<VkPhysicalDevice> devices;
    devices.Resize(count);
    vkEnumeratePhysicalDevices(_instance, &count, devices.Data());

    _physical = devices[0]; // simples por enquanto
}

void VulkanContext::createDevice()
{
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
        _physical, &count, nullptr);

    List<VkQueueFamilyProperties> families;
    families.Resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(
        _physical, &count, families.Data());

    uint32_t graphicsIndex = UINT32_MAX;
    for (uint32_t i = 0; i < count; ++i)
    {
        if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphicsIndex = i;
            break;
        }
    }
    assert(graphicsIndex != UINT32_MAX);

    float priority = 1.0f;
    VkDeviceQueueCreateInfo qci{};
    qci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    qci.queueFamilyIndex = graphicsIndex;
    qci.queueCount = 1;
    qci.pQueuePriorities = &priority;

    const char* extensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo dci{};
    dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    dci.queueCreateInfoCount = 1;
    dci.pQueueCreateInfos = &qci;
    dci.enabledExtensionCount = 1;
    dci.ppEnabledExtensionNames = extensions;

    VkResult r = vkCreateDevice(
        _physical, &dci, nullptr, &_device);
    assert(r == VK_SUCCESS);

    vkGetDeviceQueue(_device, graphicsIndex, 0, &_graphicsQueue);
    vkGetDeviceQueue(_device, graphicsIndex, 0, &_presentQueue);

    _graphicsQueueIndex = graphicsIndex;
    _presentQueueIndex = graphicsIndex;
}

void VulkanContext::createSwapchain()
{
    VkSurfaceCapabilitiesKHR caps{};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        _physical,
        _surface,
        &caps
    );

    // ------------------------------------------------------------
    // Escolha correta do extent (OBRIGATÓRIO no Vulkan)
    // ------------------------------------------------------------
    VkExtent2D extent{};

    if (caps.currentExtent.width != UINT32_MAX)
    {
        // O driver já define o tamanho (caso comum no Win32)
        extent = caps.currentExtent;
    }
    else
    {
        // Usar tamanho vindo do resize da janela
        extent.width = _width;
        extent.height = _height;

        // Clamp dentro dos limites permitidos
        if (extent.width < caps.minImageExtent.width)
            extent.width = caps.minImageExtent.width;
        if (extent.width > caps.maxImageExtent.width)
            extent.width = caps.maxImageExtent.width;

        if (extent.height < caps.minImageExtent.height)
            extent.height = caps.minImageExtent.height;
        if (extent.height > caps.maxImageExtent.height)
            extent.height = caps.maxImageExtent.height;
    }

    // Salva o extent final usado
    _extent = Extent2D{ extent.width, extent.height };

    // ------------------------------------------------------------
    // Número de imagens (double/triple buffering)
    // ------------------------------------------------------------
    uint32_t imageCount = caps.minImageCount + 1;
    if (caps.maxImageCount > 0 && imageCount > caps.maxImageCount)
        imageCount = caps.maxImageCount;

    // ------------------------------------------------------------
    // Criação do swapchain
    // ------------------------------------------------------------
    VkSwapchainCreateInfoKHR sci{};
    sci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    sci.surface = _surface;
    sci.minImageCount = imageCount;
    sci.imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
    sci.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    sci.imageExtent = extent;
    sci.imageArrayLayers = 1;
    sci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    sci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    sci.preTransform = caps.currentTransform;
    sci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    sci.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    sci.clipped = VK_TRUE;
    sci.oldSwapchain = VK_NULL_HANDLE;

    if (!_windowAlive || !_surfaceValid)
        return;

    VkResult res = vkCreateSwapchainKHR(
        _device,
        &sci,
        nullptr,
        &_swapchain
    );
    assert(res == VK_SUCCESS);

    if (res == VK_ERROR_SURFACE_LOST_KHR ||
        res == VK_ERROR_NATIVE_WINDOW_IN_USE_KHR)
    {
        _surfaceValid = false;
        return;
    }

    if (res != VK_SUCCESS)
    {
        // loga erro, mas NÃO crasha
        return;
    }

    // ------------------------------------------------------------
    // Recupera imagens do swapchain
    // ------------------------------------------------------------
    uint32_t actualCount = 0;
    vkGetSwapchainImagesKHR(
        _device,
        _swapchain,
        &actualCount,
        nullptr
    );

    _swapchainImages.Resize(actualCount);

    vkGetSwapchainImagesKHR(
        _device,
        _swapchain,
        &actualCount,
        _swapchainImages.Data()
    );
}

void VulkanContext::createRenderPass()
{
    VkAttachmentDescription color{};
    color.format = VK_FORMAT_B8G8R8A8_UNORM;
    color.samples = VK_SAMPLE_COUNT_1_BIT;
    color.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference ref{};
    ref.attachment = 0;
    ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription sub{};
    sub.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    sub.colorAttachmentCount = 1;
    sub.pColorAttachments = &ref;

    VkRenderPassCreateInfo rp{};
    rp.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rp.attachmentCount = 1;
    rp.pAttachments = &color;
    rp.subpassCount = 1;
    rp.pSubpasses = &sub;

    VkResult r = vkCreateRenderPass(_device, &rp, nullptr, &_renderPass);
    assert(r == VK_SUCCESS);
}

void VulkanContext::createImageViews()
{
    _imageViews.Resize(_swapchainImages.Count());

    for (uint32_t i = 0; i < _swapchainImages.Count(); ++i)
    {
        VkImageViewCreateInfo iv{};
        iv.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        iv.image = _swapchainImages[i];
        iv.viewType = VK_IMAGE_VIEW_TYPE_2D;
        iv.format = VK_FORMAT_B8G8R8A8_UNORM;
        iv.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        iv.subresourceRange.levelCount = 1;
        iv.subresourceRange.layerCount = 1;

        vkCreateImageView(_device, &iv, nullptr, &_imageViews[i]);
    }
}

void VulkanContext::createFramebuffers()
{
    _framebuffers.Resize(_imageViews.Count());

    for (uint32_t i = 0; i < _imageViews.Count(); ++i)
    {
        VkImageView attachments[] = { _imageViews[i] };

        VkFramebufferCreateInfo fb{};
        fb.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fb.renderPass = _renderPass;
        fb.attachmentCount = 1;
        fb.pAttachments = attachments;
        fb.width = _extent.width;
        fb.height = _extent.height;
        fb.layers = 1;

        vkCreateFramebuffer(_device, &fb, nullptr, &_framebuffers[i]);
    }
}

void VulkanContext::createCommandPool()
{
    VkCommandPoolCreateInfo cp{};
    cp.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cp.queueFamilyIndex = _graphicsQueueIndex;

    vkCreateCommandPool(_device, &cp, nullptr, &_commandPool);
}

void VulkanContext::allocateCommandBuffers()
{
    _commandBuffers.Resize(_framebuffers.Count());

    VkCommandBufferAllocateInfo ai{};
    ai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    ai.commandPool = _commandPool;
    ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    ai.commandBufferCount = _commandBuffers.Count();

    vkAllocateCommandBuffers(_device, &ai, _commandBuffers.Data());
}

void VulkanContext::recordCommandBuffers()
{
    for (uint32_t i = 0; i < _commandBuffers.Count(); ++i)
    {
        VkCommandBufferBeginInfo bi{};
        bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        vkBeginCommandBuffer(_commandBuffers[i], &bi);

        VkClearValue clear{};
        clear.color = { { 0.2f, 0.4f, 0.9f, 1.0f } };

        VkRenderPassBeginInfo rp{};
        rp.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rp.renderPass = _renderPass;
        rp.framebuffer = _framebuffers[i];
        rp.renderArea.extent = VkExtent2D{ _extent.width, _extent.height } ;
        rp.clearValueCount = 1;
        rp.pClearValues = &clear;

        vkCmdBeginRenderPass(_commandBuffers[i], &rp, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdEndRenderPass(_commandBuffers[i]);

        vkEndCommandBuffer(_commandBuffers[i]);
    }
}

void VulkanContext::createSyncObjects()
{
    VkSemaphoreCreateInfo si{};
    si.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    vkCreateSemaphore(_device, &si, nullptr, &_imageAvailable);
    vkCreateSemaphore(_device, &si, nullptr, &_renderFinished);
}