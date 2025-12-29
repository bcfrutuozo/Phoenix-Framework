#pragma once

struct VkInstance_T;
struct VkSurfaceKHR_T;
struct VkPhysicalDevice_T;
struct VkDevice_T;
struct VkQueue_T;
struct VkSwapchainKHR_T;
struct VkRenderPass_T;
struct VkFramebuffer_T;
struct VkCommandPool_T;
struct VkCommandBuffer_T;
struct VkSemaphore_T;
struct VkFence_T;
struct VkImage_T;
struct VkImageView_T;

using VkInstance = VkInstance_T*;
using VkSurfaceKHR = VkSurfaceKHR_T*;
using VkPhysicalDevice = VkPhysicalDevice_T*;
using VkDevice = VkDevice_T*;
using VkQueue = VkQueue_T*;
using VkSwapchainKHR = VkSwapchainKHR_T*;
using VkRenderPass = VkRenderPass_T*;
using VkFramebuffer = VkFramebuffer_T*;
using VkCommandPool = VkCommandPool_T*;
using VkCommandBuffer = VkCommandBuffer_T*;
using VkSemaphore = VkSemaphore_T*;
using VkFence = VkFence_T*;
using VkImage = VkImage_T*;
using VkImageView = VkImageView_T*;

#include "window/Window.hpp"

struct Extent2D 
{
    uint32_t width;
    uint32_t height;
};

class VulkanContext
{
public:
    VulkanContext(const Window& window);
    ~VulkanContext();

    void RenderFrame();

private:

    // --------------------------------------------------------
    // Initialization stages
    // --------------------------------------------------------
    void pickPhysicalDevice();
    void createDevice();
    void createSwapchain();
    void createRenderPass();
    void createImageViews();
    void createFramebuffers();
    void createCommandPool();
    void allocateCommandBuffers();
    void recordCommandBuffers();
    void createSyncObjects();

    // --------------------------------------------------------
    // Vulkan core objects
    // --------------------------------------------------------
    VkInstance       _instance = nullptr;
    VkSurfaceKHR     _surface = nullptr;
    VkPhysicalDevice _physical = nullptr;
    VkDevice         _device = nullptr;
    VkQueue          _graphicsQueue = nullptr;
    VkQueue          _presentQueue = nullptr;

    // --------------------------------------------------------
    // Swapchain
    // --------------------------------------------------------
    VkSwapchainKHR   _swapchain = nullptr;
    Extent2D       _extent = { 0, 0 };

    // --------------------------------------------------------
    // Rendering
    // --------------------------------------------------------
    VkRenderPass     _renderPass = nullptr;
    VkCommandPool    _commandPool = nullptr;

    // --------------------------------------------------------
    // Per-swapchain-image resources
    // --------------------------------------------------------
    List<VkImage>        _swapchainImages;
    List<VkImageView>    _imageViews;
    List<VkFramebuffer>  _framebuffers;
    List<VkCommandBuffer> _commandBuffers;

    // --------------------------------------------------------
    // Synchronization (minimal)
    // --------------------------------------------------------
    VkSemaphore _imageAvailable = nullptr;
    VkSemaphore _renderFinished = nullptr;

    // --------------------------------------------------------
    // Cached state
    // --------------------------------------------------------
    uint32_t _graphicsQueueIndex = 0;
    uint32_t _presentQueueIndex = 0;
};