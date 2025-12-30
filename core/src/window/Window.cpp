#include "Window.hpp"
#include "WindowBackend.hpp"
#include "render/vulkan/VulkanContext.hpp"
#include "events/EventDispatcher.hpp"
#include "events/WindowEvents.hpp"

Window::Window(const WindowDesc& desc, EventQueue& queue)
{
    _impl = CreateWindowBackend(desc, queue);
}

Window::~Window()
{
    if (_impl && !_backendDestroyed)
    {
        DestroyWindowBackend(_impl);
    }

    _impl = nullptr;
}

void Window::Show()
{
    ShowWindowBackend(_impl);
}

void Window::Close()
{
    if (!_impl || _backendDestroyed)
        return;

    // Isso dispara WM_CLOSE → WM_DESTROY
    DestroyWindowBackend(_impl);
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

void Window::Dispatch(const Event& e)
{
    EventDispatcher d(e);

    // ------------------------------------------------------------
    // Close / destroy
    // ------------------------------------------------------------
    d.Dispatch<WindowCloseEvent>(
        EventCategory::Window,
        WindowEventType::Close,
        [&](const WindowCloseEvent& ev)
        {
            if (ev.Window != GetNativeHandle())
                return;

            // 🔔 Notifica o usuário
            if (OnClose)
                OnClose();

            // 🔑 DECISÃO LOCAL:
            // Por padrão, fecha a janela
            // (política simples e previsível)
            //Close(); // chama DestroyWindow(hwnd)
        }
    );

    d.Dispatch<WindowDestroyEvent>(
        EventCategory::Window,
        WindowEventType::Destroy,
        [&](const WindowDestroyEvent& ev)
        {
            if (ev.Window != GetNativeHandle())
                return;

            _backendDestroyed = true;

            // 🔔 Notifica
            if (OnDestroy)
                OnDestroy();

            // ❌ NÃO chama DestroyWindow aqui
            // ❌ NÃO decide política global
            // Só cleanup local
        }
    );

    // ------------------------------------------------------------
    // Resize / move
    // ------------------------------------------------------------
    d.Dispatch<WindowResizeEvent>(
        EventCategory::Window, WindowEventType::Resize,
        [&](const WindowResizeEvent& ev)
        {
            if (ev.Window == GetNativeHandle() && OnResize)
                OnResize(ev.Width, ev.Height);
        }
    );

    d.Dispatch<WindowMoveEvent>(
        EventCategory::Window, WindowEventType::Move,
        [&](const WindowMoveEvent& ev)
        {
            if (ev.Window == GetNativeHandle() && OnMove)
                OnMove(ev.X, ev.Y);
        }
    );

    // ------------------------------------------------------------
    // Min / max
    // ------------------------------------------------------------
    d.Dispatch<WindowMinimizeEvent>(
        EventCategory::Window, WindowEventType::Minimize,
        [&](const WindowMinimizeEvent& ev)
        {
            if (ev.Window == GetNativeHandle() && OnMinimize)
                OnMinimize();
        }
    );

    d.Dispatch<WindowMaximizeEvent>(
        EventCategory::Window, WindowEventType::Maximize,
        [&](const WindowMaximizeEvent& ev)
        {
            if (ev.Window == GetNativeHandle() && OnMaximize)
                OnMaximize();
        }
    );

    // ------------------------------------------------------------
    // Visibility / focus
    // ------------------------------------------------------------
    d.Dispatch<WindowShowEvent>(
        EventCategory::Window, WindowEventType::Show,
        [&](const WindowShowEvent& ev)
        {
            if (ev.Window == GetNativeHandle() && OnShow)
                OnShow();
        }
    );

    d.Dispatch<WindowHideEvent>(
        EventCategory::Window, WindowEventType::Hide,
        [&](const WindowHideEvent& ev)
        {
            if (ev.Window == GetNativeHandle() && OnHide)
                OnHide();
        }
    );

    d.Dispatch<WindowFocusGainedEvent>(
        EventCategory::Window, WindowEventType::FocusGained,
        [&](const WindowFocusGainedEvent& ev)
        {
            if (ev.Window == GetNativeHandle() && OnFocusGained)
                OnFocusGained();
        }
    );

    d.Dispatch<WindowFocusLostEvent>(
        EventCategory::Window, WindowEventType::FocusLost,
        [&](const WindowFocusLostEvent& ev)
        {
            if (ev.Window == GetNativeHandle() && OnFocusLost)
                OnFocusLost();
        }
    );

    // ------------------------------------------------------------
    // DPI
    // ------------------------------------------------------------
    d.Dispatch<WindowDPIChangedEvent>(
        EventCategory::Window, WindowEventType::DPIChanged,
        [&](const WindowDPIChangedEvent& ev)
        {
            if (ev.Window == GetNativeHandle() && OnDpiChanged)
                OnDpiChanged(ev.DPI);
        }
    );
}