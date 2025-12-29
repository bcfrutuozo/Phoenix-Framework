#include "Window.hpp"
#include "WindowBackend.hpp"
#include "render/vulkan/VulkanContext.hpp"

Window::Window(const WindowDesc& desc, EventQueue& queue)
{
    _impl = CreateWindowBackend(desc, queue);
}

Window::~Window()
{
    DestroyWindowBackend(_impl);
    _impl = nullptr;
}

void Window::Show()
{
    ShowWindowBackend(_impl);
}

void Window::PollEvents()
{
    PollWindowBackendEvents(_impl);
}

WindowHandle Window::GetNativeHandle() const noexcept
{
    return GetWindowBackendHandle(_impl);
}

void Window::AttachRenderContext(VulkanContext* ctx)
{
    _vk = ctx;
}

VulkanContext* Window::GetRenderContext() const
{
    return _vk;
}
