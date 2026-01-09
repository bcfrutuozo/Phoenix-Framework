#pragma once

#include "Application.hpp"

#include "GUI/Window/Window.hpp"
#include "GUI/Window/WindowBackend.hpp"
#include "GUI/Events/UIEvents.hpp"
#include "GUI/Window/WindowHandle.hpp"
#include "System/Events/EventDispatcher.hpp"

#include "GUI/Rendering/Vulkan/VulkanContext.hpp"

struct WindowEntry
{
    Window* window;
    UIHandle handle;
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
        entry.window->attach_event_queue(&_events);
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

    void OnEvent(Event& e) override
    {
        HandleWindowEvent(e);

        for (uint32_t i = 0; i < _count; ++i)
        {
            _windows[i].window->Dispatch(e);
        }

        // Política global de fechamento
        EventDispatcher d(e);

        d.Dispatch<UICloseEvent>(
            EventCategory::UI,
            UIEventType::Close,
            [&](const UICloseEvent& ev)
            {
                if (Window* w = FindWindow(ev.Handle))
                {
                    w->Close(); // 🔑 fecha SÓ a janela certa
                }
            }
        );

        d.Dispatch<UIDestroyEvent>(
            EventCategory::UI,
            UIEventType::Destroy,
            [&](const UIDestroyEvent& ev)
            {
                Detach(ev.Handle);
                if (_count == 0)
                    Exit(); // agora sim
            }
        );

    }

    Window* FindWindow(const UIHandle& h)
    {
        for (uint32_t i = 0; i < _count; ++i)
        {
            if (_windows[i].handle == h)
                return _windows[i].window;
        }
        return nullptr;
    }

private:

    void Detach(const UIHandle& handle)
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
        switch (e.Type<UIEventType>())
        {
        case UIEventType::Resize:
        {
            auto& ev = e.As<UIResizeEvent>();
            if (Window* w = FindWindow(ev.Handle))
            {
                if (auto* vk = w->GetRenderContext())
                    vk->OnResize(ev.Width, ev.Height);
            }
            break;
        }
        case UIEventType::Destroy:
        {
            auto& ev = e.As<UIDestroyEvent>();
            if (Window* w = FindWindow(ev.Handle))
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