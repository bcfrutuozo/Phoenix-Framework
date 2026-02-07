#pragma once

#include "System/Types/Fundamentals/Pointer.hpp"

struct WindowNativeHandle
{
    Pointer value = nullptr;
};

struct DisplayNativeHandle
{
    Pointer value = nullptr;
};

class Window;
class Control;

struct UIHandle
{
    enum class Type : uint8_t
    {
        None,
        Window,
        Control
    };

    Type Type = Type::None;
    Pointer Handle;

    // -------------------------
    // Fábricas
    // -------------------------
    static UIHandle None()
    {
        return {};
    }

    static UIHandle FromWindow(const Window* w)
    {
        UIHandle h;
        h.Type = Type::Window;
        h.Handle = Pointer(w);
        return h;
    }

    static UIHandle FromControl(const Control* c)
    {
        UIHandle h;
        h.Type = Type::Control;
        h.Handle = Pointer(c);
        return h;
    }

    // -------------------------
    // Queries
    // -------------------------
    bool IsValid() const
    {
        return Handle != nullptr;
    }

    bool IsWindow() const
    {
        return Type == Type::Window;
    }

    bool IsControl() const
    {
        return Type == Type::Control;
    }

    // -------------------------
    // Cast helpers
    // -------------------------
    Window* AsWindow() const
    {
        return (Type == Type::Window)
            ? static_cast<Window*>(Handle.Get())
            : nullptr;
    }

    Control* AsControl() const
    {
        return (Type == Type::Control)
            ? static_cast<Control*>(Handle.Get())
            : nullptr;
    }

    // -------------------------
    // Comparação
    // -------------------------
    Boolean operator==(const UIHandle& other) const
    {
        return Type == other.Type && Handle == other.Handle;
    }

    Boolean operator!=(const UIHandle& other) const
    {
        return !(*this == other);
    }
};

struct SurfaceHandle
{
    WindowNativeHandle  window;
    DisplayNativeHandle display;
};