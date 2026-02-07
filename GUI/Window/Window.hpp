#pragma once

#include "WindowDescriptor.hpp"
#include "GUI/Core/UIHandle.hpp"
#include "Events/EventQueue.hpp"
#include "System/Types/Drawing/Point.hpp"
#include "System/Types/Drawing/Size.hpp"
#include "GUI/Drawing/Font.hpp"
#include "System/Types/Drawing/Color.hpp"
#include "GUI/Core/HorizontalAlignment.hpp"
#include "GUI/Core/VerticalAlignment.hpp"
#include "GUI/Core/TextFormat.hpp"
#include "GUI/Context/UIContext.hpp"

struct WindowBackend;
struct ControlBackend;
class VulkanContext;
class Control;

// Events forward declare
class CloseEvent;
class DestroyEvent;
class RestoreEvent;
class ShowEvent;
class HideEvent;
class FocusGainedEvent;
class FocusLostEvent;
class ResizedEvent;
class ResizingEvent;
class MoveEvent;
class MinimizeEvent;
class MaximizeEvent;
class DPIChangedEvent;

class Window : public Object<Window>
{
public:

    friend class MessageBox;

    struct InitializationContext
    {
        EventQueue* Queue;
        UIContext* UIContext;
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

    inline constexpr Font* GetFont() const noexcept { return _desc->Font; }
    void SetFont(Font* f) noexcept;

    void SetText(const String& text) const;
    void SetSize(i32 width, i32 height) const;
    void SetLocation(i32 x, i32 y) const;

    inline String GetText() const noexcept { return _desc->Title; }
    inline constexpr Size GetSize() const noexcept { return _desc->Size; }
    inline constexpr Point GetLocation() const noexcept { return _desc->Location; }
    inline constexpr Color GetBackgroundColor() const noexcept { return _desc->BackgroundColor; }
    inline constexpr Color GetForeColor() const noexcept { return _desc->ForeColor; }
    inline constexpr void SetBackgroundColor(Color c) noexcept { _desc->BackgroundColor = Color::White; }
    inline constexpr void SetForeColor(Color c) noexcept { _desc->ForeColor = Color::Black; }

    inline constexpr HorizontalAlignment GetHorizontalAlignment() const noexcept { return _desc->HorizontalAlignment; }
    inline constexpr VerticalAlignment GetVerticalAlignment() const noexcept { return _desc->VerticalAlignment; }
    inline constexpr TextFormat GetTextFormat() const noexcept { return _desc->TextFormat; }
    inline constexpr void SetHorizontalAlignment(HorizontalAlignment ha) { _desc->HorizontalAlignment = ha; }
    inline constexpr void SetVerticalAlignment(VerticalAlignment va) { _desc->VerticalAlignment = va; }
    inline constexpr void SetTextFormat(TextFormat tf) { _desc->TextFormat = tf; }

    inline constexpr Boolean IsResizable() const noexcept { return _desc->Resizable; }
    void EnableResize();
    void DisableResize();

    // ------------------------------------------------------------
    // Lifecycle
    // ------------------------------------------------------------
    void (*OnClose)(CloseEvent*) = nullptr;
    void (*OnDestroy)(DestroyEvent*) = nullptr;

    // ------------------------------------------------------------
    // Visibility / focus
    // ------------------------------------------------------------
    void (*OnRestore)(RestoreEvent*) = nullptr;
    void (*OnShow)(ShowEvent*) = nullptr;
    void (*OnHide)(HideEvent*) = nullptr;
    void (*OnFocusGained)(FocusGainedEvent*) = nullptr;
    void (*OnFocusLost)(FocusLostEvent*) = nullptr;

    // ------------------------------------------------------------
    // Geometry
    // ------------------------------------------------------------
    void (*OnResized)(ResizedEvent*) = nullptr;
    void (*OnResizing)(ResizingEvent*) = nullptr;
    void (*OnMove)(MoveEvent*, i32 x, i32 y) = nullptr;
    void (*OnMinimize)(MinimizeEvent*) = nullptr;
    void (*OnMaximize)(MaximizeEvent*) = nullptr;

    // ------------------------------------------------------------
    // DPI
    // ------------------------------------------------------------
    void (*OnDPIChanged)(DPIChangedEvent*) = nullptr;

    // ------------------------------------------------------------
    // Mouse enter / leave (window boundary)
    // ------------------------------------------------------------
    //void (*OnMouseEnter)(MouseEnterEvent*) = nullptr;
    //void (*OnMouseLeave)(MouseLeaveEvent*) = nullptr;

private:

    explicit Window() noexcept;

    void dispatch_to_controls(Event& e);
    
    VulkanContext* _vk = nullptr;
    WindowDescriptor* _desc = nullptr;
    WindowBackend* _impl = nullptr;
    Boolean _backendDestroyed = false;
    List<Control*> _controls = List<Control*>(16);
};