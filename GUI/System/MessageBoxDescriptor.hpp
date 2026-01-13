#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"

enum class MessageBoxButtons
{
    OK,
    OKCancel,
    YesNo,
    YesNoCancel,
    RetryCancel,
    AbortRetryIgnore
};

enum class MessageBoxIcon
{
    None,
    Info,
    Warning,
    Error,
    Question
};

enum class MessageBoxDefaultButton
{
    Button1,
    Button2,
    Button3
};

enum class MessageBoxResult
{
    OK,
    Cancel,
    Yes,
    No,
    Retry,
    Abort,
    Ignore,
    Close,
    Timeout
};

struct MessageBoxDescriptor : public Object<MessageBoxDescriptor>
{
	String Title;
	String Text;
	MessageBoxIcon Icon;
	MessageBoxButtons Buttons;
    MessageBoxDefaultButton DefaultButton;
};