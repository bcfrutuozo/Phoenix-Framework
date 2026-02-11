#pragma once

#include "Application.hpp"

#include "GUI/Controls.hpp"
#include "GUI/Core/NativeBackend.hpp"
#include "Events/GUI/UIEvents.hpp"
#include "System/Collections/Dictionary.hpp"
#include "GUI/Core/UIHandle.hpp"
#include "GUI/Drawing/Font.hpp"
#include "Events/EventDispatcher.hpp"
#include "System/Types.hpp"
#include "GUI/Context/UIContext.hpp"

#include "GUI/Rendering/Vulkan/VulkanContext.hpp"

class GUIApplication final : public Application
{
private:

public:

    GUIApplication() :
        _windows(MaxWindows),
        _count(0)
    {
        _uiContext = new UIContext();
    }

    ~GUIApplication() override
    {
        for (auto i = 0; i < _count; ++i)
        {
            delete _windows[i];
        }

        delete _uiContext;
    }

    // 🔑 API PRINCIPAL
    void Attach(Window* window)
    {
        if (!window || _count >= MaxWindows)
            return;

        _windows[_count++] = window;
    }

protected:

    void OnInit() override
    {
        Window::InitializationContext ctx;
        ctx.Queue = &_events;
        ctx.UIContext = _uiContext;

        // Initialize all windows first
        for (auto i = 0; i < _count; ++i) 
        {
            _windows[i]->Initialize(ctx);
            _windows[i]->Show();
        }
    }

    void Tick() override
    {
        // Poll GLOBAL do backend
        PollBackendEvents();

        // 2️⃣ Render frame (somente janelas com Vulkan)
        for (auto i = 0; i < _count; ++i)
        {
            Window* w = _windows[i];
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

        for (auto i = 0; i < _count; ++i)
        {
            if (_windows[i]->Dispatch(e))
                break;
        }

        // Política global de fechamento
        EventDispatcher d(e);

        d.Dispatch<CloseEvent>(
            EventCategory::UI,
            UIEventType::Close,
            [&](const CloseEvent& ev)
            {
                if (Window* w = FindWindow(ev.Handle))
                {
                    w->Close(); // 🔑 fecha SÓ a janela certa
                }
            }
        );

        d.Dispatch<DestroyEvent>(
            EventCategory::UI,
            UIEventType::Destroy,
            [&](const DestroyEvent& ev)
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
            if (UIHandle::FromWindow(_windows[i]) == h)
                return _windows[i];
        }
        return nullptr;
    }

private:

    void Detach(const UIHandle& handle)
    {
        for (auto i = 0; i < _count; ++i)
        {
            if (UIHandle::FromWindow(_windows[i]) == handle)
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
        case UIEventType::Resized:
        {
            auto& ev = e.As<ResizedEvent>();
            if (Window* w = FindWindow(ev.Handle))
            {
                if (auto* vk = w->GetRenderContext())
                    vk->OnResize(ev.Width, ev.Height);
            }
            break;
        }
        case UIEventType::Destroy:
        {
            auto& ev = e.As<DestroyEvent>();
            if (Window* w = FindWindow(ev.Handle))
            {
                if (auto* vk = w->GetRenderContext())
                    vk->OnWindowDestroyed();
            }
            break;
        }
        }
    }

    static constexpr u32 MaxWindows = 16;

    Array<Window*>_windows;
    u32 _count;
    UIContext* _uiContext;
};