#pragma once

#include "Application.hpp"

class ConsoleApplication : public Application
{
protected:

    void OnInit() override
    {
        // init logging, jobs, scripting, etc
    }

    void Tick() override
    {
        // stdin, tasks, workers, etc
    }

    void OnEvent(Event& e) override
    {
        // console apps normalmente ignoram window/input
        (void)e;
    }
};