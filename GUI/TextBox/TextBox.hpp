#pragma once

#include "GUI/Core/Control.hpp"
#include "GUI/Core/InitializationContext.hpp"
#include "System/Types.hpp"
#include "System/String.hpp"

class TextBox : public Control<TextBox>
{
	friend class Control<TextBox>;

public:

	TextBox(const String& text, i32 x, i32 y);
	~TextBox();

	TextBox(const TextBox&) = delete;
	TextBox& operator=(const TextBox&) = delete;

	String ToString() const noexcept;

private:

	void InitializeImpl(InitializationContext& ctx);
};

