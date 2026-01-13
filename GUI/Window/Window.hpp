#pragma once

#include "WindowDescriptor.hpp"
#include "GUI/Core/UIHandle.hpp"
#include "Events/EventQueue.hpp"
#include "GUI/Drawing/Point.hpp"
#include "GUI/Drawing/Size.hpp"

struct WindowBackend;
struct ControlBackend;
class VulkanContext;
class Control;

using ResizeFn = void(*)(u32, u32);
using CloseFn = void(*)();

class Window : public Object<Window>
{
public:

    friend class MessageBox;

    struct InitializationContext
    {
        EventQueue* queue;
    };

    explicit Window(const String& title, i32 width, i32 height, Boolean isResizable = true);
    explicit Window(const String& title, i32 width, i32 height, i32 x, i32 y, Boolean isResizable = true);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void Show();
    void Close();
    void Update();
    void PollEvents();

    SurfaceHandle GetSurfaceHandle() const noexcept;

    void AttachRenderContext(VulkanContext* ctx);
    VulkanContext* GetRenderContext() const;

    void Initialize(InitializationContext ctx);

    void Dispatch(Event& e);
    void AddControl(Control* c);

    void SetText(const String& text) const;
    void SetSize(i32 width, i32 height) const;
    void SetLocation(i32 x, i32 y) const;

    inline String GetText() const noexcept { return _desc->Title; }
    inline constexpr Size GetSize() const noexcept { return _desc->Size; }
    inline constexpr Point GetLocation() const noexcept { return _desc->Location; }

    inline constexpr Boolean IsResizable() const noexcept { return _desc->Resizable; }
    void EnableResize();
    void DisableResize();

    // ------------------------------------------------------------
    // Lifecycle
    // ------------------------------------------------------------
    void (*OnClose)() = nullptr;
    void (*OnDestroy)() = nullptr;

    // ------------------------------------------------------------
    // Visibility / focus
    // ------------------------------------------------------------
    void (*OnRestore)() = nullptr;
    void (*OnShow)() = nullptr;
    void (*OnHide)() = nullptr;
    void (*OnFocusGained)() = nullptr;
    void (*OnFocusLost)() = nullptr;

    // ------------------------------------------------------------
    // Geometry
    // ------------------------------------------------------------
    void (*OnResize)(Window* window, u32 width, u32 height) = nullptr;
    void (*OnMove)(i32 x, i32 y) = nullptr;
    void (*OnMinimize)() = nullptr;
    void (*OnMaximize)() = nullptr;

    // ------------------------------------------------------------
    // DPI
    // ------------------------------------------------------------
    void (*OnDPIChanged)(u32 dpi) = nullptr;

    // ------------------------------------------------------------
    // Mouse enter / leave (window boundary)
    // ------------------------------------------------------------
    void (*OnMouseEnter)() = nullptr;
    void (*OnMouseLeave)() = nullptr;

private:

    explicit Window() noexcept;

    void dispatch_to_controls(Event& e);
    
    VulkanContext* _vk = nullptr;
    WindowDescriptor* _desc;
    WindowBackend* _impl = nullptr;
    Boolean _backendDestroyed = false;
    List<Control*> _controls = List<Control*>(16);
};