#pragma once

#include "GUI/Core/Control.hpp"
#include "System/Types.hpp"
#include "System/String.hpp"

class TextBox : public Control
{
public:

	TextBox(const String& text, i32 x, i32 y);
	~TextBox();

	TextBox(const TextBox&) = delete;
	TextBox& operator=(const TextBox&) = delete;

	void Initialize(InitializationContext ctx) override;

	String ToString() const noexcept;
};

