#pragma once

#include "System/Framework.hpp"
#include "Events/Event.hpp"
#include "Events/IEventSink.hpp"
#include "Events/EventQueue.hpp"
#include "System/Types.hpp"
#include "GUI/System/MessageBox.hpp"

class Application : public IEventSink
{
public:

    virtual ~Application() = default;

    void Run()
    {
        OnInit();

        while (!_exitRequested)
        {
            try
            {
                PumpEvents();
                Tick();
            }
            catch (const Exception& e)
            {
                MessageBox::Show("Error", e.Message(), MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        OnShutdown();
    }

    void Exit() noexcept
    {
        _exitRequested = true;
    }

    void Dispatch(Event& e) override
    {
        OnEvent(e);
    }
    
protected:

    Application()
    {
        InitFramework();
        CloseFramework();
    }

    // lifecycle hooks
    virtual void OnInit() {}
    virtual void OnShutdown() {}

    // per-frame / per-tick
    virtual void Tick() {}

    // event entry point
    virtual void OnEvent(Event& e) {}

    void PumpEvents()
    {
        Event* e = nullptr;
        while (_events.Poll(e))
        {
            OnEvent(*e);
            delete e;
        }
    }

protected:
    EventQueue _events;

private:
    Boolean _exitRequested = false;
};