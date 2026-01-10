#pragma once

#include "WindowDescriptor.hpp"
#include "WindowHandle.hpp"
#include "Events/EventQueue.hpp"

struct WindowBackend;
class VulkanContext;
class Control;

using ResizeFn = void(*)(u32, u32);
using CloseFn = void(*)();

class Window
{
    friend class GUIApplication;

public:

    explicit Window(const WindowDesc& desc);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void Show();
    void Close();
    void Update();
    void PollEvents();
    UIHandle GetNativeHandle() const noexcept;
    SurfaceHandle GetSurfaceHandle() const noexcept;

    void AttachRenderContext(VulkanContext* ctx);
    VulkanContext* GetRenderContext() const;

    void Dispatch(Event& e);
    void AddControl(Control* c);

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
    void (*OnResize)(u32 width, u32 height) = nullptr;
    void (*OnMove)(i32 x, i32 y) = nullptr;
    void (*OnMinimize)() = nullptr;
    void (*OnMaximize)() = nullptr;

    // ------------------------------------------------------------
    // DPI
    // ------------------------------------------------------------
    void (*OnDpiChanged)(u32 dpi) = nullptr;

    // ------------------------------------------------------------
    // Mouse enter / leave (window boundary)
    // ------------------------------------------------------------
    void (*OnMouseEnter)() = nullptr;
    void (*OnMouseLeave)() = nullptr;

private:

    void attach_event_queue(EventQueue* queue);
    void dispatch_to_controls(Event& e);
    
    VulkanContext* _vk = nullptr;
    WindowBackend* _impl = nullptr;
    Boolean _backendDestroyed = false;
    List<Control*> _controls;
};