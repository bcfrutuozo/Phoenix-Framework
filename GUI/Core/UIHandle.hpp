#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"

struct WindowNativeHandle
{
    Pointer value = nullptr;
};

struct DisplayNativeHandle
{
    Pointer value = nullptr;
};

class ControlBase;

struct UIHandle : public Object<UIHandle>
{
    Pointer Handle;

    inline constexpr UIHandle(const ControlBase* c)
        :
        Handle(c)
    { }

    // -------------------------
    // Queries
    // -------------------------
    inline constexpr Boolean IsValid() const
    {
        return Handle != nullptr;
    }

    inline operator ControlBase* () const noexcept
    {
        return static_cast<ControlBase*>(Handle.Get());
    }

    // -------------------------
    // Comparação
    // -------------------------
    inline constexpr Boolean operator==(const UIHandle& other) const
    {
        return Handle == other.Handle;
    }

    inline constexpr Boolean operator!=(const UIHandle& other) const
    {
        return !(*this == other);
    }

    Boolean Equals(const UIHandle& other) const noexcept;
    u32 GetHashCode() const noexcept;
    String ToString() const noexcept;
};

struct SurfaceHandle
{
    WindowNativeHandle  window;
    DisplayNativeHandle display;
};