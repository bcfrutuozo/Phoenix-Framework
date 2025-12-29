#pragma once

#include "Application.hpp"

#include "window/Window.hpp"
#include "window/WindowBackend.hpp"
#include "events/WindowEvents.hpp"
#include "window/WindowHandle.hpp"

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
                vk->RenderFrame();
            }
        }
    }

    void OnEvent(const Event& e) override
    {
        if (!e.Is(EventCategory::Window))
            return;

        HandleWindowEvent(e);
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

    void HandleWindowEvent(const Event& e)
    {
        switch (e.Type<WindowEventType>())
        {
        case WindowEventType::Close:
            // política simples: fechar app se qualquer janela fechar
            Exit();
            break;

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
        }
    }

    static constexpr uint32_t MaxWindows = 16;

    WindowEntry _windows[MaxWindows]{};
    uint32_t _count = 0;
};