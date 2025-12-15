#include "StringBuilder.hpp"

#include "StringBuilder.hpp"
#include "text/UTF8.hpp"

StringBuilder::StringBuilder() noexcept
{

}

StringBuilder::StringBuilder(size_type capacity) noexcept
{
	_buffer.Reserve(capacity);
}

void StringBuilder::Clear() noexcept
{
	_buffer.FastClear();
}

void StringBuilder::Reserve(size_type capacity) noexcept
{
	_buffer.Reserve(capacity);
}

StringBuilder& StringBuilder::Append(Char c) noexcept
{
	_buffer.Add(c);
	return *this;
}

StringBuilder& StringBuilder::Append(CodePoint cp) noexcept
{
	if (!cp.IsValid())
		return *this;

	UTF8::UTF8EncodeResult enc = UTF8::encode_utf8(cp);
	for (auto i = 0; i < enc.Length; ++i)
		_buffer.Add(Char(enc.Bytes[i]));

	return *this;
}

StringBuilder& StringBuilder::Append(const char* cstr) noexcept
{
	if (!cstr)
		return *this;

	while (*cstr)
	{
		_buffer.Add(Char(static_cast<unsigned char>(*cstr)));
		++cstr;
	}
	return *this;
}

StringBuilder& StringBuilder::Append(const String& s) noexcept
{
	UInt32 len = s.GetByteCount();
	if (len == 0)
		return *this;

	const Char* p = static_cast<const Char*>(s);

	_buffer.EnsureCapacity(_buffer.Count() + len);
	_buffer.AddRange(p, len);

	return *this;
}

StringBuilder& StringBuilder::Append(const unsigned char* bytes, size_type len) noexcept
{
	if (!bytes || len == 0)
		return *this;

	_buffer.EnsureCapacity(_buffer.Count() + len);

	for (auto i = 0; i < len; ++i)
		_buffer.Add(Char(bytes[i]));

	return *this;
}

StringBuilder& StringBuilder::AppendLine() noexcept
{
	_buffer.Add(Char('\n'));
	return *this;
}

StringBuilder& StringBuilder::AppendLine(const String& s) noexcept
{
	Append(s);
	_buffer.Add(Char('\n'));
	return *this;
}

StringBuilder& StringBuilder::AppendLine(const char* cstr) noexcept
{
	Append(cstr);
	_buffer.Add(Char('\n'));
	return *this;
}

String StringBuilder::ToString() const noexcept
{
	if (_buffer.IsEmpty())
		return String::Empty();

	return String(_buffer.Data(), _buffer.Count());
}