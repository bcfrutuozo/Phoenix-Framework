#pragma once

#include "System/String.hpp"
#include "System/Types.hpp"
#include "GUI/Core/Control.hpp"

class Label final : public Control
{
public:

    Label(const String& text, i32 x, i32 y);
    ~Label() = default;

    String ToString() const noexcept;

    void Initialize(InitializationContext ctx) override;

protected:

};