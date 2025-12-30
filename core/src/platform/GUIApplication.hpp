#pragma once

#include "Application.hpp"

#include "window/Window.hpp"
#include "window/WindowBackend.hpp"
#include "events/WindowEvents.hpp"
#include "window/WindowHandle.hpp"
#include "events/EventDispatcher.hpp"


#include "render/vulkan/VulkanContext.hpp"

struct WindowEntry
{
    Window* window;
    WindowHandle handle;
};

class GUIApplication final : public Application
{
private:

public:

    GUIApplication() = default;

    ~GUIApplication() override
    {
        for (uint32_t i = 0; i < _count; ++i)
        {
            delete _windows[i].window;
        }
    }

    // 🔑 API PRINCIPAL
    void Attach(Window* window)
    {
        if (!window || _count >= MaxWindows)
            return;

        WindowEntry entry;
        entry.window = window;
        entry.handle = window->GetNativeHandle();
        _windows[_count++] = entry;
    }

protected:

    void OnInit() override
    {
        // Mostrar todas as janelas anexadas
        for (uint32_t i = 0; i < _count; ++i)
            _windows[i].window->Show();
    }

    void Tick() override
    {
        // Poll GLOBAL do backend
        PollWindowBackendEvents(nullptr);

        // 2️⃣ Render frame (somente janelas com Vulkan)
        for (uint32_t i = 0; i < _count; ++i)
        {
            Window* w = _windows[i].window;
            if (!w)
                continue;

            if (auto* vk = w->GetRenderContext())
            {
                if (!vk->IsRenderable())
                    continue;

                vk->RenderFrame();
            }
        }
    }

    void OnEvent(const Event& e) override
    {
        if (!e.Is(EventCategory::Window))
            return;

        HandleWindowEvent(e);

        for (uint32_t i = 0; i < _count; ++i)
        {
            _windows[i].window->Dispatch(e);
        }

        // Política global de fechamento
        EventDispatcher d(e);

        d.Dispatch<WindowCloseEvent>(
            EventCategory::Window,
            WindowEventType::Close,
            [&](const WindowCloseEvent& ev)
            {
                if (Window* w = FindWindow(ev.Window))
                {
                    w->Close(); // 🔑 fecha SÓ a janela certa
                }
            }
        );

        d.Dispatch<WindowDestroyEvent>(
            EventCategory::Window,
            WindowEventType::Destroy,
            [&](const WindowDestroyEvent& ev)
            {
                Detach(ev.Window);
                if (_count == 0)
                    Exit(); // agora sim
            }
        );

    }

    Window* FindWindow(const WindowHandle& h)
    {
        for (uint32_t i = 0; i < _count; ++i)
        {
            if (_windows[i].handle == h)
                return _windows[i].window;
        }
        return nullptr;
    }

private:

    void Detach(const WindowHandle& handle)
    {
        for (uint32_t i = 0; i < _count; ++i)
        {
            if (_windows[i].handle == handle)
            {
                // Move o último para a posição removida
                _windows[i] = _windows[_count - 1];
                _windows[_count - 1] = {};
                --_count;
                return;
            }
        }
    }

    void HandleWindowEvent(const Event& e)
    {
        switch (e.Type<WindowEventType>())
        {
        case WindowEventType::Resize:
        {
            auto& ev = e.As<WindowResizeEvent>();
            if (Window* w = FindWindow(ev.Window))
            {
                if (auto* vk = w->GetRenderContext())
                    vk->OnResize(ev.Width, ev.Height);
            }
            break;
        }
        case WindowEventType::Destroy:
        {
            auto& ev = e.As<WindowDestroyEvent>();
            if (Window* w = FindWindow(ev.Window))
            {
                if (auto* vk = w->GetRenderContext())
                    vk->OnWindowDestroyed();
            }
            break;
        }
        }
    }

    static constexpr uint32_t MaxWindows = 16;

    WindowEntry _windows[MaxWindows]{};
    uint32_t _count = 0;
};