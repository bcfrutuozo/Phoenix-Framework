#include "Window.hpp"
#include "WindowBackend.hpp"

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