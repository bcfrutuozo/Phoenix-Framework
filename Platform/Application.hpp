#pragma once

#include "Events/Event.hpp"
#include "Events/EventQueue.hpp"

class Application
{
public:
    virtual ~Application() = default;

    void Run()
    {
        OnInit();

        while (!_exitRequested)
        {
            PumpEvents();
            Tick();
        }

        OnShutdown();
    }

    void Exit() noexcept
    {
        _exitRequested = true;
    }

protected:

    Application() = default;

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
    bool _exitRequested = false;
};