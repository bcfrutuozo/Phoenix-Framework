#pragma once

#include "System/String.hpp"
#include "System/Types.hpp"
#include "GUI/Core/Control.hpp"

class Label final : public Control
{
public:

    Label(const String& text, i32 x, i32 y);
    ~Label() = default;
    
    Label(const Label&) = delete;
    Label& operator=(const Label&) = delete;

    void Initialize(InitializationContext ctx) override;

    String ToString() const noexcept;
};