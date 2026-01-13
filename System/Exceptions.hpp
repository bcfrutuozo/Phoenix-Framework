#pragma once

#include "System/Types/String.hpp"
#include "System/Types.hpp"

class Exception
{
public:

	// =============================
	// Constructors
	// =============================

	// Default: mensagem vazia
	Exception() noexcept
		: _message(String::Empty()) {
	}

	// From String
	explicit Exception(const String& msg) noexcept
		: _message(msg) {
	}

	// Copy constructor
	Exception(const Exception& other) noexcept
		: _message(other._message) {
	}

	// Move constructor
	Exception(Exception&& other) noexcept
		: _message(static_cast<String&&>(other._message)) {
	}

	// Copy assignment
	Exception& operator=(const Exception& other) noexcept
	{
		if (this != &other)
			_message = other._message;
		return *this;
	}

	// Move assignment
	Exception& operator=(Exception&& other) noexcept
	{
		if (this != &other)
			_message = static_cast<String&&>(other._message);
		return *this;
	}

	// Destructor
	virtual ~Exception() noexcept = default;

	// =============================
	// Accessor
	// =============================

	constexpr const String& Message() const noexcept { return _message; }

protected:
	String _message;
};

// ================================================
// ArgumentException
// ================================================
class ArgumentException : public Exception
{
public:
	using Exception::Exception;
};


// ================================================
// ArgumentNullException
// ================================================
class ArgumentNullException : public Exception
{
public:
	explicit ArgumentNullException(const String& paramName) noexcept
		: Exception(String("ArgumentNullException: ") + paramName) {
	}
};


// ================================================
// ArgumentOutOfRangeException
// ================================================
class ArgumentOutOfRangeException : public Exception
{
public:
	explicit ArgumentOutOfRangeException(const String& param) noexcept
		: Exception(String("ArgumentOutOfRange: ") + param) {
	}
};


// ================================================
// IndexOutOfRangeException
// ================================================
class IndexOutOfRangeException : public Exception
{
public:
	IndexOutOfRangeException() noexcept
		: Exception("IndexOutOfRange") {
	}

	explicit IndexOutOfRangeException(const String& msg) noexcept
		: Exception(msg) {
	}
};


// ================================================
// InvalidOperationException
// ================================================
class InvalidOperationException : public Exception
{
public:
	using Exception::Exception;
};


// ================================================
// FormatException
// ================================================
class FormatException : public Exception
{
public:
	using Exception::Exception;
};


// ================================================
// NotSupportedException
// ================================================
class NotSupportedException : public Exception
{
public:
	using Exception::Exception;
};


// ================================================
// NotImplementedException
// ================================================
class NotImplementedException : public Exception
{
public:
	NotImplementedException() noexcept
		: Exception("NotImplementedException") {
	}

	explicit NotImplementedException(const String& msg) noexcept
		: Exception(msg) {
	}
};


// ================================================
// DivideByZeroException
// ================================================
class DivideByZeroException : public Exception
{
public:
	DivideByZeroException() noexcept
		: Exception("DivideByZeroException") {
	}
};


// ================================================
// IOException
// ================================================
class IOException : public Exception
{
public:
	using Exception::Exception;
};


// ================================================
// FileNotFoundException
// ================================================
class FileNotFoundException : public IOException
{
public:
	explicit FileNotFoundException(const String& file) noexcept
		: IOException(String("File not found: ") + file) {
	}
};

#ifdef _WIN32

// ================================================
// SystemException
// ================================================
class SystemException : public Exception
{
public:
	explicit SystemException() noexcept
		: Exception("SystemException") {
	}

	explicit SystemException(const String& message) noexcept
		: Exception(message) {
	}

	explicit SystemException(i32 HRESULT) noexcept;
};

// ================================================
// Win32Exception
// ================================================
class Win32Exception : public SystemException
{
public:

	explicit Win32Exception() noexcept
		: SystemException("Win32Exception") {
	}

	explicit Win32Exception(i32 HRESULT) noexcept
		: SystemException(HRESULT)
	{

	}
};

#endif