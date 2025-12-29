#pragma once

#include "WindowDescriptor.hpp"
#include "WindowHandle.hpp"
#include "events/EventQueue.hpp"

struct WindowBackend;
class VulkanContext;

class Window
{
public:

    explicit Window(const WindowDesc& desc, EventQueue& queue);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void Show();
    void PollEvents();
    WindowHandle GetNativeHandle() const noexcept;

    void AttachRenderContext(VulkanContext* ctx);
    VulkanContext* GetRenderContext() const;

private:
    
    VulkanContext* _vk;
    WindowBackend* _impl;
};