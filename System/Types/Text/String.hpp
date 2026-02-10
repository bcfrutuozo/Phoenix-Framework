#pragma once

#include "System/Types/Fundamentals/Object.hpp"
#include "System/Types/Primitives/Boolean.hpp"
#include "System/Types/Primitives/Char.hpp"
#include "System/Types/Primitives/CodePoint.hpp"
#include "System/Text/Encoding.hpp"
#include "System/Types/Primitives/Int32.hpp"
#include "System/Types/Primitives/Int64.hpp"
#include "System/Types/Primitives/UInt32.hpp"
#include "System/Types/Primitives/UInt64.hpp"
#include "System/Collections/List.hpp"
#include "System/Meta/Forward.hpp"

enum class StringSplitOptions : uint8_t
{
	None = 0,
	RemoveEmptyEntries = 1 << 0,
	TrimEntries = 1 << 1
};

enum class StringComparison
{
	Ordinal,
	OrdinalIgnoreCase
};

class Locale;
class StringArg;

class String final : public Object<String>
{
public:

	using value_type = Char;
	using size_type = u32;
	using refcount_type = u32;

	String();
	String(char c) noexcept;
	String(Char c) noexcept;
	String(CodePoint cp) noexcept;
	String(const List<CodePoint>& cps) noexcept;

	// ⚠️ WARNING:
	// Array constructor assumes FULLY VALID CONTENT (string literal / static buffer).
	// Do NOT use with snprintf/local buffers.
	// Use (ptr, length) instead.
	template<size_t N, typename T, enable_if_t<is_single_char<T>::value, bool> = true>
	String(const T(&p)[N]) noexcept : String(p, N - 1) {}

	String(const char* p) noexcept;
	String(const wchar_t* p) noexcept;
	String(const char8_t* p) noexcept;
	String(const char16_t* p) noexcept;
	String(const char32_t* p) noexcept;
	String(const Byte* p) noexcept;
	String(const Char* p) noexcept;

	String(const char* p, u32 length) noexcept;
	String(const wchar_t* p, u32 length) noexcept;
	String(const char8_t* p, u32 length) noexcept;
	String(const char16_t* p, u32 length) noexcept;
	String(const char32_t* p, u32 length) noexcept;
	String(const Byte* bytes, u32 len) noexcept;
	String(const Char* bytes, u32 len) noexcept;

	String(const String& other) noexcept;
	String(String&& other) noexcept;
	String& operator=(const String& other) noexcept;
	String& operator=(String&& other) noexcept;
	~String() noexcept;

	inline friend String operator+(const String& a, const String& b) { return String::Concat(a, b); };
	inline String& operator+=(const String& other) { *this = String::Concat(*this, other); return *this; }
	inline friend Boolean operator==(const String& a, const String& b) noexcept { return a.Equals(b); }
	inline friend Boolean operator!=(const String& a, const String& b) noexcept { return !(a == b); }

	inline const Char& operator[](u32 i) const noexcept { return data()[i]; }

	explicit inline operator const Char* () const noexcept { return data(); }
	explicit inline operator const char* () const noexcept { return reinterpret_cast<const char*>(data()); }
	explicit inline operator const wchar_t* () const noexcept { return ToWideCharArray().GetData(); }
	explicit inline operator const unsigned char* () const noexcept { return reinterpret_cast<const unsigned char*>(data()); }

	template<typename... Args>
	static String Concat(Args&&... args)
	{
		// materializa StringArg para cada argumento
		StringArg packed[] = { StringArg(Forward<Args>(args))... };

		uint64_t totalBytes = 0;
		for (const auto& a : packed)
			totalBytes += a.Get().GetByteCount();

		if (totalBytes == 0)
			return String::Empty();

		if (totalBytes <= SSO_CAPACITY)
		{
			String out;
			out._flags = FLAG_SSO;
			out._byteLength = totalBytes;

			uint32_t offset = 0;
			for (const auto& a : packed)
				copy_bytes(out._sso, offset, a.Get());

			out._sso[totalBytes] = Char(0);
			return out;
		}

		unsigned char* block = allocate_block(totalBytes);
		char* dst = reinterpret_cast<char*>(block + sizeof(refcount_type) + sizeof(u32));

		uint32_t offset = 0;
		for (const auto& a : packed)
			copy_bytes(dst, offset, a.Get());

		String result(block);
		result._byteLength = totalBytes;
		return result;
	}

	Boolean Contains(const String& sub) const noexcept;
	Boolean Contains(CodePoint cp) const noexcept;
	Boolean Contains(const String& sub, Boolean ignoreCase) const noexcept;
	Boolean Contains(const String& sub, Boolean ignoreCase, const Locale& locale) const noexcept;
	Boolean Contains(CodePoint cp, Boolean ignoreCase) const noexcept;
	Boolean Contains(CodePoint cp, Boolean ignoreCase, const Locale& locale) const noexcept;

	static Boolean Compare(const String& a, const String& b, const Locale& locale) noexcept;
	static Int32 Compare(const String& A, const String& B, Boolean ignoreCase, const Locale& locale) noexcept;

	static List<CodePoint> DecodeToCodePoints(const String& s);

	static const String& Empty() { static const String emptyInstance; return emptyInstance; }

	Boolean EndsWith(const String& compare) const noexcept;
	Boolean EndsWith(const String& compare, Boolean ignoreCase) const noexcept;
	Boolean EndsWith(const String& compare, Boolean ignoreCase, const Locale& locale) const noexcept;

	Boolean Equals(const String& other, Boolean ignoreCase) const noexcept;
	Boolean Equals(const String& other, Boolean ignoreCase, const Locale& locale) const noexcept;
	inline static Boolean Equals(const String& a, const String& b) noexcept { return a.Equals(b); }
	static Boolean Equals(const String& a, const String& b, Boolean ignoreCase) noexcept;
	static Boolean Equals(const String& a, const String& b, Boolean ignoreCase, const Locale& locale) noexcept;

	inline static String FromChar(const Char ch) { return String(ch); }

	static String FromCodePoint(CodePoint cp)
	{
		Byte buf[4];
		uint32_t len = UTF8Encoding::Encode(cp, reinterpret_cast<Char*>(buf));
		return String(buf, len);
	}

	static String FromCodePoints(const List<CodePoint>& cps)
	{
		if (cps.IsEmpty())
			return String::Empty();

		// 1) calcular bytes totais (UTF-8)
		uint32_t totalBytes = 0;
		for (CodePoint cp : cps)
			totalBytes += (uint32_t)cp.ByteCount();

		unsigned char* block = allocate_block(totalBytes);
		if (!block)
			return String();

		Char* dst = reinterpret_cast<Char*>(
			block + sizeof(refcount_type) + sizeof(u32)
			);

		// 2) encode via Encoding
		uint32_t w = 0;
		for (CodePoint cp : cps)
		{
			Char buf[4];
			uint32_t len = UTF8Encoding::Encode(cp, buf);

			for (uint32_t i = 0; i < len; ++i)
				dst[w++] = Char(buf[i]);
		}

		String r(block);
		r._byteOffset = 0;
		r._byteLength = totalBytes;
		return r;
	}

	inline constexpr u64 GetByteCount() const noexcept { return _byteLength; }

	CodePoint GetCodePointAt(uint32_t cpIndex) const noexcept;

	inline u32 GetCodePointsCount() const noexcept
	{
		return DecodeToCodePoints(*this).Count();
	}

	u32 GetLength() const noexcept;
	inline u32 GetReferenceCount() const noexcept
	{
		if (IsSSO()) return 1;
		if (!_ptr) return 0;
		return refcount_ref();
	};

	i64 IndexOf(const String& value, u32 startIndex) const;
	i64 IndexOf(const String& value) const noexcept;
	i64 IndexOf(const String& value, u32 startIndex, u32 count) const noexcept;
	i64 IndexOf(const String& value, StringComparison comp) const noexcept;
	i64 IndexOf(const String& value, u32 startIndex, StringComparison comp) const noexcept;
	i64 IndexOf(const String& value, u32 startIndex, u32 count, StringComparison comp) const noexcept;
	i64 IndexOf(Char c) const noexcept;
	i64 IndexOf(Char c, u32 startIndex) const noexcept;
	i64 IndexOf(Char c, u32 startIndex, u32 count) const noexcept;
	i64 IndexOf(Char c, StringComparison comp) const noexcept;
	i64 IndexOfAny(const List<Char>& chars, u32 startIndex, u32 count) const noexcept;
	i64 IndexOfAny(const List<Char>& chars, u32 startIndex) const noexcept;
	i64 IndexOfAny(const List<Char>& chars) const noexcept;

	String Insert(u32 cpIndex, const String& value) const noexcept;

	inline constexpr Boolean IsEmpty() const noexcept { return _byteLength == 0; }

	static inline Boolean IsEmpty(const String& s) { return s == String::Empty(); }

	inline Boolean IsNormalized(NormalizationForm form = NormalizationForm::NFC) const noexcept
	{
		return Equals(Normalize(form));
	}

	inline constexpr Boolean IsSSO() const noexcept { return (_flags & FLAG_SSO) != 0; }

	static inline Boolean IsWhiteSpace(const String& s) { return s == String::WhiteSpace(); }

	static String Join(const String& separator, const List<String>& values);

	template<typename T>
	static String Join(const String& separator, const List<T>& values)
	{
		List<String> temp;
		temp.Reserve(values.Count());

		for (uint32_t i = 0; i < values.Count(); ++i)
			temp.Add(values[i].ToString());

		return Join(separator, temp);
	}

	static String Join(Char separator, const List<String>& values);

	template<typename T>
	static String Join(Char separator, const List<T>& values)
	{
		return Join(String(separator), values);
	}

	static String Join(CodePoint separator, const List<String>& values);

	template<typename T>
	static String Join(CodePoint separator, const List<T>& values)
	{
		return Join(String::FromCodePoint(separator), values);
	}

	static String Join(const String& separator, const List<String>& values, int start, int count);
	static String Join(Char separator, const List<String>& values, int start, int count);
	static String Join(CodePoint separator, const List<String>& values, int start, int count);

	i64 LastIndexOf(const String& value, u32 startIndex) const noexcept;
	i64 LastIndexOf(const String& value) const noexcept;
	i64 LastIndexOf(const String& value, u32 startIndex, u32 count) const noexcept;
	i64 LastIndexOf(const String& value, StringComparison comp) const noexcept;
	i64 LastIndexOf(const String& value, u32 startIndex, StringComparison comp) const noexcept;
	i64 LastIndexOf(const String& value, u32 startIndex, u32 count, StringComparison comp) const noexcept;
	i64 LastIndexOf(Char c) const noexcept;
	i64 LastIndexOf(Char c, u32 startIndex) const noexcept;
	i64 LastIndexOf(Char c, u32 startIndex, u32 count) const noexcept;
	i64 LastIndexOfAny(const List<Char>& chars, u32 startIndex, u32 count) const noexcept;
	i64 LastIndexOfAny(const List<Char>& chars, u32 startIndex) const noexcept;
	i64 LastIndexOfAny(const List<Char>& chars) const noexcept;

	String Normalize(NormalizationForm form = NormalizationForm::NFC) const noexcept;

	template<typename T>
	String PadLeft(u32 width, T ch) const requires(is_single_char_v<T>)
	{
		uint64_t currentLen = GetLength();

		if (currentLen >= width)
			return *this;

		uint32_t missing = width - (uint32_t)currentLen;

		// converter caractere para uma string de 1 codepoint
		String pad = ToSingleCharString(ch);

		// criar padding repetido
		uint32_t padBytes = pad.GetByteCount();

		uint32_t totalPadBytes = padBytes * missing;
		uint32_t totalBytes = totalPadBytes + _byteLength;

		// alocar novo bloco
		unsigned char* block = allocate_block(totalBytes);
		if (!block) return String();

		Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(u32));

		// repetir caractere na esquerda
		const Char* psrc = pad.data();
		uint32_t k = 0;
		for (uint32_t i = 0; i < missing; ++i) {
			for (uint32_t j = 0; j < padBytes; ++j)
				dst[k++] = psrc[j];
		}

		// copiar string original
		const Char* ssrc = data();
		for (uint32_t j = 0; j < _byteLength; ++j)
			dst[k++] = ssrc[j];

		String result(block);
		result._byteLength = totalBytes;
		return result;
	}

	inline String PadLeft(u32 width) const { return PadLeft(width, ' '); }

	template<typename T>
		requires(is_single_char_v<T>)
	String PadRight(u32 width, T ch) const
	{
		uint64_t currentLen = GetLength();

		if (currentLen >= width)
			return *this;

		uint32_t missing = width - (uint32_t)currentLen;

		// converter caractere para uma string de 1 codepoint
		String pad = ToSingleCharString(ch);

		uint32_t padBytes = pad.GetByteCount();
		uint32_t totalPadBytes = padBytes * missing;
		uint32_t totalBytes = _byteLength + totalPadBytes;

		// alocar novo bloco
		unsigned char* block = allocate_block(totalBytes);
		if (!block) return String();

		Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(u32));

		uint32_t k = 0;

		// copiar string original primeiro (diferente do PadLeft)
		const Char* ssrc = data();
		for (uint32_t j = 0; j < _byteLength; ++j)
			dst[k++] = ssrc[j];

		// repetir caractere na direita
		const Char* psrc = pad.data();
		for (uint32_t i = 0; i < missing; ++i) {
			for (uint32_t j = 0; j < padBytes; ++j)
				dst[k++] = psrc[j];
		}

		String result(block);
		result._byteLength = totalBytes;
		return result;
	}

	inline String PadRight(u32 width) const { return PadRight(width, ' '); }

	String Remove(i32 start) const noexcept;
	String Remove(i32 start, i32 count) const noexcept;

	// Replace byte-by-byte (ASCII only)
	String Replace(Char oldChar, Char newChar) const noexcept;
	String Replace(CodePoint oldCp, CodePoint newCp) const noexcept;
	String Replace(const String& oldValue, const String& newValue) const noexcept;
	String Replace(const String& oldValue, const String& newValue, StringComparison comp) const noexcept;
	String Replace(const String& oldValue, const String& newValue, Boolean ignoreCase, const Locale& locale) const noexcept;

	String ReplaceLineEndings() const noexcept;
	String ReplaceLineEndings(const String& replacement) const noexcept;

	List<String> Split(const String& separator, i32 maxCount, StringSplitOptions options) const;
	List<String> Split(const String& separator, StringSplitOptions options) const;
	List<String> Split(const List<String>& separators, i32 maxCount, StringSplitOptions options) const;
	List<String> Split(const List<String>& separators, StringSplitOptions options) const;
	List<String> Split(const List<Char>& separators, i32 maxCount, StringSplitOptions options) const;
	List<String> Split(const List<Char>& separators, StringSplitOptions options) const;
	List<String> Split(const List<Char>& separators) const;
	List<String> Split(const String& separator) const;
	List<String> Split(const List<String>& separators) const;

	inline String Substring(u32 gcStart) const noexcept { return Substring(gcStart, this->GetLength() - gcStart); }
	String Substring(u32 gcStart, u32 gcCount) const noexcept;
	Boolean StartsWith(const String& compare, Boolean ignoreCase, const Locale& locale) const noexcept;
	Boolean StartsWith(const String& compare, Boolean ignoreCase) const noexcept;
	Boolean StartsWith(const String& compare) const noexcept;

	String ToHex(Encoding enc = Encoding::UTF8) const noexcept;

	String ToLower(const Locale& locale) const noexcept;
	String ToLower() const noexcept;
	String ToUpper(const Locale& locale) const noexcept;
	String ToUpper() const noexcept;

	template<typename T>
	static String ToSingleCharString(T ch) requires(is_single_char_v<T>)
	{
		return FromCodePoint(CodePoint(ch));
	}

	String Trim() const;
	String Trim(Char c) const;
	String Trim(const List<Char>& chars) const;
	String TrimEnd() const;
	String TrimEnd(Char c) const;
	String TrimEnd(const List<Char>& chars) const;
	String TrimStart() const;
	String TrimStart(Char c) const;
	String TrimStart(const List<Char>& chars) const;

	static const String& WhiteSpace() { static const String whiteSpaceInstance(" "); return whiteSpaceInstance; }

	Array<wchar_t> ToWideCharArray() const noexcept;

	Boolean Equals(const String& other) const noexcept;
	UInt32 GetHashCode() const noexcept;
	String ToString() const noexcept;

private:

	static constexpr uint32_t SSO_CAPACITY = 22;
	static constexpr uint32_t FLAG_SSO = 1 << 0;
	static constexpr uint32_t FLAG_ASCII_KNOWN = 1 << 1;
	static constexpr uint32_t FLAG_IS_ASCII = 1 << 2;

	union
	{
		unsigned char* _ptr = nullptr;   // Block start (heap)
		char _sso[SSO_CAPACITY + 1];
	};

	uint32_t _byteOffset = 0;        // offset em bytes dentro do bloco
	uint32_t _byteLength = 0;        // comprimento em bytes desta string
	mutable uint32_t _flags = 0;
	mutable uint32_t _gcLength = UInt32::MaxValue; // Grapheme cluster lazy cache

	explicit String(unsigned char* block) noexcept;

	const Char* data() const noexcept;
	inline refcount_type& refcount_ref() const { return *reinterpret_cast<refcount_type*>(_ptr); }
	uint32_t& length_ref() const { return *reinterpret_cast<uint32_t*>(_ptr + sizeof(refcount_type)); }
	inline Char* bytes_ptr() const { return reinterpret_cast<Char*>(_ptr + sizeof(refcount_type) + sizeof(u32)); }

	void add_ref() noexcept;
	void release() noexcept;
	static unsigned char* allocate_block(uint32_t lenChars) noexcept;
	void init_from_bytes(const unsigned char* bytes, uint32_t len) noexcept;
	uint32_t find_byte_offset_of_code_point(uint32_t cpIndex) const;

	static void copy_bytes(char* dst, uint32_t& offset, const String& s) noexcept;
	static void copy_bytes(char* dst, uint32_t& offset, const char* cstr) noexcept;

	static bool is_ascii(const String& s) noexcept;

	bool contain_byte(const String& sub) const noexcept;
	String substring_by_bytes(uint32_t byteStart, uint32_t byteLen) const noexcept;

	bool impl_EndsWith(const String& needle, bool ignoreCase, const Locale& locale) const noexcept;
	bool impl_StartsWith(const String& needle, bool ignoreCase, const Locale& locale) const noexcept;

	static void remove_combining_dot_above(String& s);

	List<String> impl_Split(const List<String>& stringSeps, const List<Char>& charSeps, int maxCount, StringSplitOptions options) const;

	// Verifica se TODOS os Strings em uma lista são ASCII
	static bool are_all_strings_ascii(const List<String>& list) {
		for (u64 i = 0; i < list.Count(); ++i)
			if (!String::is_ascii(list[i]))
				return false;
		return true;
	}

	// Soma os bytes totais de todos os elementos + separadores
	static uint32_t compute_join_ascii_byte_count(const List<String>& list, uint32_t sepLen)
	{
		uint64_t total = 0;
		uint64_t count = list.Count();

		for (uint64_t i = 0; i < count; ++i)
			total += list[i].GetByteCount();

		if (count > 1)
			total += (count - 1) * sepLen;

		return total;
	}

	template<typename T,
	enable_if_t<is_single_char<T>::value, bool> = true>
	static inline uint32_t get_pointer_length(const T* p) noexcept
	{
		uint32_t len = 0;
		while (p[len]) ++len;
		return len;
	}

	inline constexpr void set_sso_flag() const noexcept {
		_flags |= FLAG_SSO;
	}

	inline constexpr void clear_sso_flag() const noexcept {
		_flags &= ~FLAG_SSO;
	}

	inline constexpr void reset_flags_keep_sso() const noexcept {
		_flags &= FLAG_SSO;
	}

	inline constexpr void reset_all_flags() const noexcept {
		_flags = 0;
	}

	inline constexpr bool is_ascii_known() const noexcept {
		return (_flags & FLAG_ASCII_KNOWN) != 0;
	}

	inline constexpr bool is_ascii_cached() const noexcept {
		return (_flags & FLAG_IS_ASCII) != 0;
	}

	inline constexpr void set_string_as_ascii(bool isAscii) const noexcept {
		_flags |= FLAG_ASCII_KNOWN;
		if (isAscii)
			_flags |= FLAG_IS_ASCII;
		else
			_flags &= ~FLAG_IS_ASCII;
	}

	inline constexpr void invalidate_ascii_flag() const noexcept {
		_flags &= ~(FLAG_ASCII_KNOWN | FLAG_IS_ASCII);
	}

};

inline StringSplitOptions operator|(StringSplitOptions a, StringSplitOptions b)
{
	return static_cast<StringSplitOptions>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline Boolean HasFlag(StringSplitOptions opt, StringSplitOptions flag)
{
	return (static_cast<uint8_t>(opt) & static_cast<uint8_t>(flag)) != 0;
}

class StringArg
{
public:

	StringArg(const String& s) noexcept : _str(&s), _owns(false) {}

	template<size_t N>
	StringArg(const char(&p)[N]) noexcept
		: _temp(p), _str(&_temp), _owns(true) {
	}

	StringArg(const char* p, u32 length) noexcept
		: _temp(p, length), _str(&_temp), _owns(true) {
	}

	template<size_t N>
	StringArg(const wchar_t(&p)[N]) noexcept
		: _temp(p), _str(&_temp), _owns(true) {
	}

	StringArg(const wchar_t* p, u32 length) noexcept
		: _temp(p, length), _str(&_temp), _owns(true) {
	}

	template<size_t N>
	StringArg(const char8_t(&p)[N]) noexcept
		: _temp(p), _str(&_temp), _owns(true) {
	}

	StringArg(const char8_t* p, u32 length) noexcept
		: _temp(p, length), _str(&_temp), _owns(true) {
	}

	template<size_t N>
	StringArg(const char16_t(&p)[N]) noexcept
		: _temp(p), _str(&_temp), _owns(true) {
	}

	StringArg(const char16_t* p, u32 length) noexcept
		: _temp(p, length), _str(&_temp), _owns(true) {
	}

	template<size_t N>
	StringArg(const char32_t(&p)[N]) noexcept
		: _temp(p), _str(&_temp), _owns(true) {
	}

	StringArg(const char32_t* p, u32 length) noexcept
		: _temp(p, length), _str(&_temp), _owns(true) {
	}

	const String& Get() const noexcept
	{
		return *_str;
	}

private:

	String        _temp;
	const String* _str;
	bool          _owns;
};