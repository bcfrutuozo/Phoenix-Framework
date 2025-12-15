#pragma once

#include "Object.hpp"
#include "Boolean.hpp"
#include "Char.hpp"
#include "CodePoint.hpp"
#include "text/Encoding.hpp"
#include "Int32.hpp"
#include "Int64.hpp"
#include "UInt32.hpp"
#include "UInt64.hpp"
#include "collections/List.hpp"

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

class String final : public Object<String>
{
public:

	using value_type = Char;
	using size_type = unsigned int;
	using refcount_type = unsigned int;

	constexpr String() : _ptr(0), _byteOffset(0), _byteLength(0)
	{
		_sso[0] = Char(0);
	}

	String(char c) noexcept : String(Char(c)) {}
	String(Char c) noexcept;
	String(const char* cstr) noexcept;
	String(const Byte* bytes, size_type len) noexcept;
	String(const Char* bytes, size_t len) noexcept : String(reinterpret_cast<const Byte*>(bytes), len) {}
	String(const List<CodePoint>& cps) noexcept;

	template<size_t N>
	String(const wchar_t(&p)[N]) noexcept
	{
		static_assert(N > 0, "Invalid wide string literal");
		List<CodePoint> cps;
#if WCHAR_MAX == 0xFFFF
		// Windows → UTF-16
		UTF16Encoding::Decode(reinterpret_cast<const char16_t*>(p), N - 1, cps);
#elif WCHAR_MAX == 0x10FFFF
		// Unix → UTF-32
		UTF32Encoding::Decode(reinterpret_cast<const char32_t*>(p), N - 1, cps);
#else
		static_assert(false, "Unsupported wchar_t size");
#endif
		* this = FromCodePoints(cps);
	}

	String(const wchar_t* p, uint32_t length) noexcept
	{
		if (!p || length == 0)
		{
			*this = String::Empty();
			return;
		}

		List<CodePoint> cps;
#if WCHAR_MAX == 0xFFFF
		UTF16Encoding::Decode(reinterpret_cast<const char16_t*>(p), length, cps);
#elif WCHAR_MAX == 0x10FFFF
		UTF32Encoding::Decode(reinterpret_cast<const char32_t*>(p), length, cps);
#else
		static_assert(false, "Unsupported wchar_t size");
#endif

		* this = FromCodePoints(cps);
	}

	template<size_t N>
	String(const char8_t(&p)[N]) noexcept
	{
		static_assert(N > 0, "Invalid UTF-8 Literal");
		*this = String(reinterpret_cast<const Char*>(p), static_cast<uint32_t>(N - 1));
	}

	String(const char8_t* p, uint32_t length) noexcept
	{
		if (!p || length == 0)
		{
			*this = String::Empty();
			return;
		}

		*this = String(reinterpret_cast<const char8_t*>(p), length);
	}

	template<size_t N>
	String(const char16_t(&p)[N]) noexcept
	{
		static_assert(N > 0, "Invalid UTF-16 Literal");
		List<CodePoint> cps;
		UTF16Encoding::Decode(p, N - 1, cps);
		*this = FromCodePoints(cps);
	}

	String(const char16_t* p, uint32_t length) noexcept
	{
		if (!p || length == 0)
		{
			*this = String::Empty();
			return;
		}
		List<CodePoint> cps;
		UTF16Encoding::Decode(p, length, cps);
		*this = String::FromCodePoints(cps);
	}

	template<size_t N>
	String(const char32_t(&p)[N]) noexcept
	{
		static_assert(N > 0, "Invalid UTF-32 Literal");
		List<CodePoint> cps;
		UTF32Encoding::Decode(p, N - 1, cps);
		*this = FromCodePoints(cps);
	}

	String(const char32_t* p, uint32_t length) noexcept
	{
		if (!p || length == 0)
		{
			*this = String::Empty();
			return;
		}

		List<CodePoint> cps;
		UTF32Encoding::Decode(p, length, cps);
		*this = String::FromCodePoints(cps);
	}


	String(const String& other) noexcept;
	String(String&& other) noexcept;
	String& operator=(const String& other) noexcept;
	String& operator=(String&& other) noexcept;
	~String() noexcept;

	inline friend String operator+(const String& a, const String& b) { return String::Concat(a, b); }
	inline String& operator+=(const String& other) { *this = String::Concat(*this, other); return *this; }
	inline friend Boolean operator==(const String& a, const String& b) noexcept { return a.Equals(b); }
	inline friend Boolean operator!=(const String& a, const String& b) noexcept { return !(a == b); }

	inline const Char& operator[](size_type i) const noexcept { return data()[i]; }

	explicit inline operator const Char* () const noexcept { return data(); }
	explicit inline operator const char* () const noexcept { return reinterpret_cast<const char*>(data()); }
	explicit inline operator const unsigned char* () const noexcept { return reinterpret_cast<const unsigned char*>(data()); }

	template<typename... Args>
	static String Concat(const Args&... args)
	{
		uint32_t totalBytes = (0 + ... + GetByteLen(args));
		if (totalBytes == 0)
			return String::Empty();

		// SSO FIRST
		if (totalBytes <= SSO_CAPACITY)
		{
			String out;
			out._flags = FLAG_SSO;
			out._byteOffset = 0;
			out._byteLength = totalBytes;

			uint32_t offset = 0;
			(CopyBytes(out._sso, offset, args), ...);
			out._sso[totalBytes] = Char(0);
			return out;
		}

		// heap fallback (seu código atual)
		unsigned char* block = allocate_block(totalBytes);
		Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));

		uint32_t offset = 0;
		(CopyBytes(dst, offset, args), ...);

		String result(block);
		result._byteOffset = 0;
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
			block + sizeof(refcount_type) + sizeof(size_type)
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

	inline constexpr UInt64 GetByteCount() const noexcept { return _byteLength; }

	CodePoint GetCodePointAt(uint32_t cpIndex) const noexcept;

	inline UInt64 GetCodePointsCount() const noexcept
	{
		return DecodeToCodePoints(*this).Count();
	}

	UInt64 GetLength() const noexcept;
	inline UInt32 GetReferenceCount() const noexcept
	{
		if (IsSSO()) return 1;
		if (!_ptr) return 0;
		return refcount_ref();
	};

	Int64 IndexOf(const String& value, size_t startIndex) const;
	Int64 IndexOf(const String& value) const noexcept;
	Int64 IndexOf(const String& value, size_t startIndex, size_t count) const noexcept;
	Int64 IndexOf(const String& value, StringComparison comp) const noexcept;
	Int64 IndexOf(const String& value, size_t startIndex, StringComparison comp) const noexcept;
	Int64 IndexOf(const String& value, size_t startIndex, size_t count, StringComparison comp) const noexcept;
	Int64 IndexOf(Char c) const noexcept;
	Int64 IndexOf(Char c, size_t startIndex) const noexcept;
	Int64 IndexOf(Char c, size_t startIndex, size_t count) const noexcept;
	Int64 IndexOf(Char c, StringComparison comp) const noexcept;
	Int64 IndexOfAny(const List<Char>& chars, UInt64 startIndex, UInt64 count) const noexcept;
	Int64 IndexOfAny(const List<Char>& chars, UInt64 startIndex) const noexcept;
	Int64 IndexOfAny(const List<Char>& chars) const noexcept;

	String Insert(uint32_t cpIndex, const String& value) const noexcept;

	inline constexpr Boolean IsEmpty() const noexcept { return _byteLength == 0; }

	static inline Boolean IsEmpty(const String& s) { return s == String::Empty(); }

	inline Boolean IsNormalized(UnicodeNormalization::NormalizationForm form = UnicodeNormalization::NormalizationForm::NFC) const noexcept
	{
		return Equals(Normalize(form));
	}

	inline constexpr Boolean IsSSO() const noexcept { return (_flags & FLAG_SSO) != 0; }

	static inline Boolean IsWhiteSpace(const String& s) { return s == String::WhiteSpace(); }
	static inline Boolean IsWhiteSpace(const char* c) { return IsWhiteSpace(String(c)); }

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

	Int64 LastIndexOf(const String& value, size_t startIndex) const noexcept;
	Int64 LastIndexOf(const String& value) const noexcept;
	Int64 LastIndexOf(const String& value, size_t startIndex, size_t count) const noexcept;
	Int64 LastIndexOf(const String& value, StringComparison comp) const noexcept;
	Int64 LastIndexOf(const String& value, size_t startIndex, StringComparison comp) const noexcept;
	Int64 LastIndexOf(const String& value, size_t startIndex, size_t count, StringComparison comp) const noexcept;
	Int64 LastIndexOf(Char c) const noexcept;
	Int64 LastIndexOf(Char c, size_t startIndex) const noexcept;
	Int64 LastIndexOf(Char c, size_t startIndex, size_t count) const noexcept;
	Int64 LastIndexOfAny(const List<Char>& chars, UInt64 startIndex, UInt64 count) const noexcept;
	Int64 LastIndexOfAny(const List<Char>& chars, UInt64 startIndex) const noexcept;
	Int64 LastIndexOfAny(const List<Char>& chars) const noexcept;

	String Normalize(UnicodeNormalization::NormalizationForm form = UnicodeNormalization::NormalizationForm::NFC) const noexcept;

	template<typename T>
	String PadLeft(uint32_t width, T ch) const
		requires(is_single_char_v<T>)
	{
		UInt64 currentLen = GetLength();

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

		Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));

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

	inline String PadLeft(uint32_t width) const { return PadLeft(width, ' '); }

	template<typename T>
		requires(is_single_char_v<T>)
	String PadRight(uint32_t width, T ch) const
	{
		UInt64 currentLen = GetLength();

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

		Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));

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

	inline String PadRight(uint32_t width) const { return PadRight(width, ' '); }

	String Remove(int start) const noexcept;
	String Remove(int start, int count) const noexcept;

	// Replace byte-by-byte (ASCII only)
	String Replace(Char oldChar, Char newChar) const noexcept;
	String Replace(CodePoint oldCp, CodePoint newCp) const noexcept;
	String Replace(const String& oldValue, const String& newValue) const noexcept;
	String Replace(const String& oldValue, const String& newValue, StringComparison comp) const noexcept;
	String Replace(const String& oldValue, const String& newValue, bool ignoreCase, const String& locale) const noexcept;

	String ReplaceLineEndings() const noexcept;
	String ReplaceLineEndings(const String& replacement) const noexcept;

	List<String> Split(const String& separator, int maxCount, StringSplitOptions options) const;
	List<String> Split(const String& separator, StringSplitOptions options) const;
	List<String> Split(const List<String>& separators, int maxCount, StringSplitOptions options) const;
	List<String> Split(const List<String>& separators, StringSplitOptions options) const;
	List<String> Split(const List<Char>& separators, int maxCount, StringSplitOptions options) const;
	List<String> Split(const List<Char>& separators, StringSplitOptions options) const;
	List<String> Split(const List<Char>& separators) const;
	List<String> Split(const String& separator) const;
	List<String> Split(const List<String>& separators) const;

	inline String Substring(uint32_t gcStart) const noexcept { return Substring(gcStart, this->GetLength() - gcStart); }
	String Substring(uint32_t gcStart, uint32_t gcCount) const noexcept;
	Boolean StartsWith(const String& compare, Boolean ignoreCase, const Locale& locale) const noexcept;
	Boolean StartsWith(const String& compare, Boolean ignoreCase) const noexcept;
	Boolean StartsWith(const String& compare) const noexcept;

	inline Boolean StartsWith(char c) const noexcept
	{
		UInt64 len = GetLength();
		if (len == 0)
			return Boolean(false);

		CodePoint first = GetCodePointAt(0);
		CodePoint cp((uint32_t)(unsigned char)c);

		return Boolean(first == cp);
	}

	inline Boolean StartsWith(CodePoint cp) const noexcept
	{
		if (!cp.IsValid()) return Boolean(false);
		String s = FromCodePoint(cp);
		return StartsWith(s);
	}

	String ToHex(Encoding enc = Encoding::UTF8) const noexcept;

	String ToLower(const String& locale) const noexcept;
	inline String ToLower() const noexcept { return ToLower("en"); }
	String ToUpper(const String& locale) const noexcept;
	String ToUpper() const noexcept { return ToUpper("en"); }

	template<typename T>
	static String ToSingleCharString(T ch) requires(is_single_char_v<T>)
	{
		if constexpr (is_same_v<T, Char>)
			return FromChar(ch);
		else if constexpr (is_same_v<T, CodePoint>)
			return FromCodePoint(ch);
		else if constexpr (is_same_v<T, char>)
			return FromCodePoint(CodePoint((unsigned char)ch));
		else if constexpr (is_same_v<T, char32_t>)
			return FromCodePoint(CodePoint(ch));
		else
			static_assert(!sizeof(T), "Unsupported character type");
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
		Char _sso[SSO_CAPACITY + 1];
	};

	uint32_t _byteOffset = 0;        // offset em bytes dentro do bloco
	uint32_t _byteLength = 0;        // comprimento em bytes desta string
	mutable uint32_t _flags = 0;
	mutable uint32_t _gcLength = UInt32::MaxValue(); // Grapheme cluster lazy cache

	explicit String(unsigned char* block) noexcept;

	const Char* data() const noexcept;
	inline refcount_type& refcount_ref() const { return *reinterpret_cast<refcount_type*>(_ptr); }
	size_type& length_ref() const { return *reinterpret_cast<size_type*>(_ptr + sizeof(refcount_type)); }
	inline Char* bytes_ptr() const { return reinterpret_cast<Char*>(_ptr + sizeof(refcount_type) + sizeof(size_type)); }

	void add_ref() noexcept;
	void release() noexcept;
	static unsigned char* allocate_block(size_type lenChars) noexcept;
	void init_from_cstr(const char* cstr) noexcept;
	void init_from_bytes(const Byte* bytes, size_type len) noexcept;
	uint32_t FindByteOffsetOfCodePoint(uint32_t cpIndex) const;
	static int CompareOrdinalCP(CodePoint cpA, CodePoint cpB) noexcept;
	static int CompareCodePoints_CaseAware(CodePoint a, CodePoint b) noexcept;

	inline static uint32_t GetByteLen(const String& s) noexcept { return s.GetByteCount(); }
	static uint32_t GetByteLen(const char* cstr) noexcept;
	static void CopyBytes(Char* dst, uint32_t& offset, const String& s) noexcept;
	static void CopyBytes(Char* dst, uint32_t& offset, const char* cstr) noexcept;

	static bool IsASCII(const String& s) noexcept;

	Boolean ContainsBytes(const String& sub) const noexcept;
	String SubstringByBytes(uint32_t byteStart, uint32_t byteLen) const noexcept;

	static void CaseFoldUnicode(const List<CodePoint>& cps, const char* localeBytes, uint32_t localeLen, List<CodePoint>& output);
	static int CompareFolded(const List<CodePoint>& a, const List<CodePoint>& b);
	static Boolean ContainsFolded(const List<CodePoint>& hay, const List<CodePoint>& ned);
	Boolean EndsWithInternal(const String& needle, Boolean ignoreCase, const Locale& locale) const noexcept;
	Boolean StartsWithInternal(const String& needle, Boolean ignoreCase, const Locale& locale) const noexcept;

	static void RemoveCombiningDotAbove(String& s);

	List<String> SplitImpl(const List<String>& stringSeps, const List<Char>& charSeps, int maxCount, StringSplitOptions options) const;

	// Verifica se TODOS os Strings em uma lista são ASCII
	static bool AllASCII(const List<String>& list) {
		for (uint32_t i = 0; i < list.Count(); ++i)
			if (!String::IsASCII(list[i]))
				return false;
		return true;
	}

	// Soma os bytes totais de todos os elementos + separadores
	static uint32_t ComputeJoinASCIIByteCount(const List<String>& list, uint32_t sepLen)
	{
		uint32_t total = 0;
		uint32_t count = list.Count();

		for (uint32_t i = 0; i < count; ++i)
			total += list[i].GetByteCount();

		if (count > 1)
			total += (count - 1) * sepLen;

		return total;
	}

	inline constexpr void SetSSO() const noexcept {
		_flags |= FLAG_SSO;
	}

	inline constexpr void ClearSSO() const noexcept {
		_flags &= ~FLAG_SSO;
	}

	inline constexpr void ResetFlagsKeepSSO() const noexcept {
		_flags &= FLAG_SSO;
	}

	inline constexpr void ResetAllFlags() const noexcept {
		_flags = 0;
	}

	inline constexpr bool IsAsciiKnown() const noexcept {
		return (_flags & FLAG_ASCII_KNOWN) != 0;
	}

	inline constexpr bool IsAsciiCached() const noexcept {
		return (_flags & FLAG_IS_ASCII) != 0;
	}

	inline constexpr void SetAscii(bool isAscii) const noexcept {
		_flags |= FLAG_ASCII_KNOWN;
		if (isAscii)
			_flags |= FLAG_IS_ASCII;
		else
			_flags &= ~FLAG_IS_ASCII;
	}

	inline constexpr void InvalidateAscii() const noexcept {
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

typedef String string;