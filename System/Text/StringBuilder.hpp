#pragma once

#include "System/Types.hpp"
#include "System/Collections/List.hpp"

class StringBuilder : public Object<StringBuilder>
{
public:
	using size_type = u32;

	StringBuilder() noexcept;
	explicit StringBuilder(size_type capacity) noexcept;

	void Clear() noexcept;
	void Reserve(size_type capacity) noexcept;

	inline size_type GetByteCount() const noexcept { return _buffer.Count(); }
	inline Boolean IsEmpty() const noexcept { return _buffer.IsEmpty(); }

	StringBuilder& Append(Char c) noexcept;
	StringBuilder& Append(CodePoint cp) noexcept;
	StringBuilder& Append(const char* cstr) noexcept;
	StringBuilder& Append(const String& s) noexcept;
	StringBuilder& Append(const unsigned char* bytes, size_type len) noexcept;
	StringBuilder& AppendLine() noexcept;
	StringBuilder& AppendLine(const String& s) noexcept;
	StringBuilder& AppendLine(const char* cstr) noexcept;

	String ToString() const noexcept;

	friend inline StringBuilder& operator<<(StringBuilder& sb, const String& s) noexcept
	{
		sb.Append(s);
		return sb;
	}

	friend inline StringBuilder& operator<<(StringBuilder& sb, const char* s) noexcept
	{
		sb.Append(s);
		return sb;
	}

	friend inline StringBuilder& operator<<(StringBuilder& sb, Char c) noexcept
	{
		sb.Append(c);
		return sb;
	}

	friend inline StringBuilder& operator<<(StringBuilder& sb, CodePoint cp) noexcept
	{
		sb.Append(cp);
		return sb;
	}

	template <typename T>
	friend inline enable_if_t<is_base_of_v<Object<T>, T>, StringBuilder&> operator<<(StringBuilder& sb, const T& obj) noexcept
	{
		sb.Append(obj.ToString());
		return sb;
	}

private:

	List<Char> _buffer;
};