#pragma once

#include "WindowDescriptor.hpp"
#include "WindowHandle.hpp"
#include "events/EventQueue.hpp"

struct WindowBackend;
class VulkanContext;

using ResizeFn = void(*)(uint32_t, uint32_t);
using CloseFn = void(*)();

class Window
{
public:

    explicit Window(const WindowDesc& desc, EventQueue& queue);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void Show();
    void Close();
    void PollEvents();
    WindowHandle GetNativeHandle() const noexcept;

    void AttachRenderContext(VulkanContext* ctx);
    VulkanContext* GetRenderContext() const;

    void Dispatch(const Event& e);

    // ------------------------------------------------------------
    // Lifecycle
    // ------------------------------------------------------------
    void (*OnClose)() = nullptr;
    void (*OnDestroy)() = nullptr;

    // ------------------------------------------------------------
    // Visibility / focus
    // ------------------------------------------------------------
    void (*OnShow)() = nullptr;
    void (*OnHide)() = nullptr;
    void (*OnFocusGained)() = nullptr;
    void (*OnFocusLost)() = nullptr;

    // ------------------------------------------------------------
    // Geometry
    // ------------------------------------------------------------
    void (*OnResize)(uint32_t width, uint32_t height) = nullptr;
    void (*OnMove)(int32_t x, int32_t y) = nullptr;
    void (*OnMinimize)() = nullptr;
    void (*OnMaximize)() = nullptr;

    // ------------------------------------------------------------
    // DPI
    // ------------------------------------------------------------
    void (*OnDpiChanged)(uint32_t dpi) = nullptr;

    // ------------------------------------------------------------
    // Mouse enter / leave (window boundary)
    // ------------------------------------------------------------
    void (*OnMouseEnter)() = nullptr;
    void (*OnMouseLeave)() = nullptr;

private:
    
    VulkanContext* _vk = nullptr;
    WindowBackend* _impl = nullptr;
    Boolean _backendDestroyed = false;
};