#include "System/Events/EventQueue.hpp"


// ------------------------------------------------------------
// Configuration
// ------------------------------------------------------------

static constexpr uint32_t EVENT_QUEUE_CAPACITY = 1024;

// ------------------------------------------------------------
// Internal storage (ST, pointer-based, no slicing)
// ------------------------------------------------------------

static Event* g_buffer[EVENT_QUEUE_CAPACITY];
static uint32_t g_head = 0;
static uint32_t g_tail = 0;
static uint32_t g_count = 0;

// ------------------------------------------------------------
// Public API
// ------------------------------------------------------------

void EventQueue::Push(Event* e) noexcept
{
    if (!e)
        return;

    // Drop event if full (engine-style behavior)
    if (g_count == EVENT_QUEUE_CAPACITY)
    {
        delete e; // ownership da fila
        return;
    }

    g_buffer[g_tail] = e;
    g_tail = (g_tail + 1) % EVENT_QUEUE_CAPACITY;
    ++g_count;
}

Boolean EventQueue::Poll(Event*& out) noexcept
{
    if (g_count == 0)
        return false;

    out = g_buffer[g_head];
    g_head = (g_head + 1) % EVENT_QUEUE_CAPACITY;
    --g_count;
    return true;
}

void EventQueue::Clear() noexcept
{
    while (g_count > 0)
    {
        delete g_buffer[g_head];
        g_head = (g_head + 1) % EVENT_QUEUE_CAPACITY;
        --g_count;
    }

    g_head = g_tail = 0;
}