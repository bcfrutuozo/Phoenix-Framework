#include "events/EventQueue.hpp"

#include "types/UInt32.hpp"

// ------------------------------------------------------------
// Configuration
// ------------------------------------------------------------

static constexpr uint32_t EVENT_QUEUE_CAPACITY = 1024;

// ------------------------------------------------------------
// Internal storage (ST for now)
// ------------------------------------------------------------

static Event   g_buffer[EVENT_QUEUE_CAPACITY];
static uint32_t  g_head = 0;
static uint32_t  g_tail = 0;
static uint32_t  g_count = 0;

// ------------------------------------------------------------
// Public API
// ------------------------------------------------------------

void EventQueue::Push(const Event& e) noexcept
{
    // Drop event if full (engine-style behavior)
    if (g_count == EVENT_QUEUE_CAPACITY)
        return;

    g_buffer[g_tail] = e;
    g_tail = (g_tail + 1) % EVENT_QUEUE_CAPACITY;
    ++g_count;
}

Boolean EventQueue::Poll(Event& out) noexcept
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
    g_head = g_tail = g_count = 0;
}