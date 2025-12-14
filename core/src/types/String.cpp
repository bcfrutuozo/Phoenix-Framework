#include "String.hpp"

#include "system/Globals.hpp"
#include "system/Marvin32.hpp"

#include <stdlib.h>
#include <cstring>

String::String(const char* cstr) noexcept
{
	init_from_cstr(cstr);
}

String::String(Char c) noexcept
{
	_flags = FLAG_SSO;
	_byteOffset = 0;
	_byteLength = 1;
	_sso[0] = c;
	_sso[1] = Char(0);

	SetAscii((unsigned char)c.Value < 0x80);

	_gcLength = 1;
}

String::String(const unsigned char* bytes, size_type len) noexcept
{
	init_from_bytes(bytes, len);
}

String::String(const List<CodePoint>& cps) noexcept
{
	uint32_t count = cps.Count();
	if (count == 0) {
		_ptr = nullptr;
		_byteOffset = 0;
		_byteLength = 0;
		return;
	}

	// calcula bytes
	uint32_t totalBytes = 0;
	for (uint32_t cp : cps)
		totalBytes += (cp <= 0x7F ? 1 :
			cp <= 0x7FF ? 2 :
			cp <= 0xFFFF ? 3 : 4);

	unsigned char* block = allocate_block(totalBytes);
	if (!block) {
		_ptr = nullptr;
		_byteOffset = 0;
		_byteLength = 0;
		return;
	}

	Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));
	uint32_t w = 0;

	for (uint32_t cp : cps) {
		UTF8::UTF8EncodeResult enc = UTF8::encode_utf8(CodePoint(cp));
		for (uint32_t i = 0; i < enc.Length; ++i)
			dst[w++] = enc.Bytes[i];
	}

	_ptr = block;
	_byteOffset = 0;
	_byteLength = totalBytes;
}

String::String(const String& other) noexcept 
{
	if (other.IsSSO())
	{
		_flags = FLAG_SSO;
		_byteOffset = 0;
		_byteLength = other._byteLength;

		for (uint32_t i = 0; i < _byteLength; ++i)
			_sso[i] = other._sso[i];

		_sso[_byteLength] = Char(0);
	}
	else
	{
		_flags = 0;
		_ptr = other._ptr;
		_byteOffset = other._byteOffset;
		_byteLength = other._byteLength;
		add_ref();
	}

	// reset caches
	InvalidateCache();
	_gcLength = UInt32::MaxValue();
	ReleaseNormalizationCache();
}

String::String(String&& other) noexcept 
{
	if (other.IsSSO())
	{
		_flags = FLAG_SSO;
		_byteOffset = 0;
		_byteLength = other._byteLength;

		for (uint32_t i = 0; i < _byteLength; ++i)
			_sso[i] = other._sso[i];

		_sso[_byteLength] = Char(0);

		// Esvaziar origem (estado válido)
		other._byteLength = 0;
		other._byteOffset = 0;
		other._sso[0] = Char(0);
		other._flags = FLAG_SSO;
	}
	else
	{
		_flags = 0;
		_ptr = other._ptr;
		_byteOffset = other._byteOffset;
		_byteLength = other._byteLength;

		other._ptr = nullptr;
		other._byteLength = 0;
		other._byteOffset = 0;
		other._flags = FLAG_SSO;
	}

	// transfere caches
	_flags |= (other._flags & (FLAG_ASCII_KNOWN | FLAG_IS_ASCII |
		FLAG_HAS_NFC | FLAG_HAS_NFKC));

	// transfere ponteiros
	_nfc = other._nfc;
	_nfkc = other._nfkc;

	// limpa origem
	other._nfc = nullptr;
	other._nfkc = nullptr;
	other._flags = FLAG_SSO;

	_gcLength = UInt32::MaxValue();
	InvalidateCache();
	other.InvalidateCache();
}

String& String::operator=(const String& other) noexcept
{
	if (this == &other)
		return *this;

	release();

	if (other.IsSSO())
	{
		_flags = FLAG_SSO;
		_byteOffset = 0;
		_byteLength = other._byteLength;

		for (uint32_t i = 0; i < _byteLength; ++i)
			_sso[i] = other._sso[i];

		_sso[_byteLength] = Char(0);
	}
	else
	{
		_flags = 0;
		_ptr = other._ptr;
		_byteOffset = other._byteOffset;
		_byteLength = other._byteLength;
		add_ref();
	}

	// reset caches
	InvalidateCache();
	_gcLength = UInt32::MaxValue();

	return *this;
}

String& String::operator=(String&& other) noexcept
{
	if (this == &other)
		return *this;

	release();

	if (other.IsSSO())
	{
		_flags = FLAG_SSO;
		_byteOffset = 0;
		_byteLength = other._byteLength;

		for (uint32_t i = 0; i < _byteLength; ++i)
			_sso[i] = other._sso[i];

		_sso[_byteLength] = Char(0);
	}
	else
	{
		_flags = 0;
		_ptr = other._ptr;
		_byteOffset = other._byteOffset;
		_byteLength = other._byteLength;

		other._ptr = nullptr;
		other._byteOffset = 0;
		other._byteLength = 0;
		other._flags = FLAG_SSO;
	}

	_gcLength = UInt32::MaxValue();
	_flags |= (other._flags & (FLAG_ASCII_KNOWN | FLAG_IS_ASCII | FLAG_HAS_NFC | FLAG_HAS_NFKC));

	other.ReleaseNormalizationCache();
	other.InvalidateCache();

	return *this;
}

String::~String() noexcept
{
	release();
}

Boolean operator==(const String& a, const String& b) noexcept
{
	if (a._byteLength == b._byteLength)
	{
		if (a.IsSSO() && b.IsSSO())
		{
			return memcmp(a._sso, b._sso, a._byteLength * sizeof(Char)) == 0;
		}

		if (!a.IsSSO() && !b.IsSSO() &&
			a._ptr == b._ptr &&
			a._byteOffset == b._byteOffset)
			return true;
	}

	// 2) Comprimentos diferentes → diferente
	if (a._byteLength != b._byteLength)
		return false;

	// 3) Se ambas são vazias → iguais
	if (a._byteLength == 0)
		return true;

	// 4) Comparação de bytes (mais rápida possível)
	const Char* pa = a.data();
	const Char* pb = b.data();

	// Comparar byte a byte
	for (uint32_t i = 0; i < a._byteLength; ++i)
	{
		if (pa[i] != pb[i])
			return false;
	}

	return true;
}

UInt32 String::GetHashCode() const noexcept
{
	if (_byteLength == 0)
		return 0;

	return Marvin32::Compute(
		reinterpret_cast<const uint8_t*>(data()),
		_byteLength,
		GLOBAL_HASH_SEED
	);

	return 0;
}

UInt64 String::GetLength() const noexcept
{
	if (_gcLength != UInt32::MaxValue())
		return _gcLength;

	if (_byteLength == 0)
	{
		_gcLength = 0;
		return 0;
	}

	const Char* p = data();

	if (ASCII::IsAllASCII(p, _byteLength))
	{
		_gcLength = _byteLength;
		return _gcLength;
	}

	// Slow path: decode + grapheme cluster detection

	List<CodePoint> cps;
	cps.EnsureCapacity(_byteLength);

	uint32_t len = _byteLength;
	uint32_t pos = 0;

	UTF8::Decode(*this, GetByteCount(), cps);

	uint32_t cpCount = cps.Count();
	if (cpCount == 0)
	{
		_gcLength = 0;
		return 0;
	}

	List<CodePoint> boundaries;
	boundaries.EnsureCapacity(cpCount);

	uint32_t gcTotal = 0;
	UnicodeCase::ComputeGraphemeBoundaries(cps, cps.Count(), boundaries, gcTotal);

	_gcLength = gcTotal;
	return gcTotal;
}

Boolean String::Contains(const String& sub) const noexcept {
	if (sub.IsEmpty()) return true;

	// If identical region → contains
	if (_ptr == sub._ptr &&
		_byteOffset == sub._byteOffset &&
		_byteLength == sub._byteLength)
		return true;

	return ContainsBytes(sub);
}

Boolean String::Contains(const char* cstr) const noexcept {
	if (!cstr) return false;
	if (cstr[0] == '\0') return true;
	return Contains(String(cstr));
}

Boolean String::Contains(CodePoint cp) const noexcept {
	if (!cp.IsValid()) return false;
	String s = FromCodePoint(cp);
	return Contains(s);
}

Boolean String::Contains(const String& sub, bool ignoreCase) const noexcept
{
	if (!ignoreCase)
		return Contains(sub);

	return Contains(sub, true, "en");
}

Boolean String::Contains(const String& sub, bool ignoreCase, const String& locale) const noexcept
{
	// Se substring vazia → true
	if (sub.IsEmpty())
		return true;

	const Char* hayBytes = data();
	const Char* subBytes = sub.data();
	uint32_t H = _byteLength;
	uint32_t N = sub._byteLength;

	if (N == 0) return true;

	// Detectar locale
	const char* locBytes = reinterpret_cast<const char*>(locale.data());
	uint32_t locLen = locale.GetByteCount();
	bool isTurkic = UnicodeCase::locale_is_turkic(locBytes, locLen);

	bool asciiHay = ASCII::IsAllASCII(hayBytes, H);
	bool asciiSub = ASCII::IsAllASCII(subBytes, N);

	// ======================================
	//  FAST-PATH 1: ordinal ASCII
	// ======================================
	if (!ignoreCase && asciiHay && asciiSub)
		return ASCII::Find(hayBytes, H, subBytes, N) != nullptr;

	// ======================================
	//  FAST-PATH 2: ignoreCase ASCII, locale NÃO turkic
	// ======================================
	if (ignoreCase && !isTurkic && asciiHay && asciiSub)
		return ASCII::FindIgnoreCase(hayBytes, H, subBytes, N) != nullptr;

	// ======================================
	//  SLOW PATH: Unicode-aware casefold + normalization
	// ======================================
	List<CodePoint> hayFold, subFold;
	UTF8::FoldAndNormalize(*this, GetByteCount(), locBytes, hayFold);
	UTF8::FoldAndNormalize(sub, sub.GetByteCount(), locBytes, subFold);

	return UTF8::Contains(hayFold, subFold);
}

Boolean String::Contains(const char* cstr, bool ignoreCase) const noexcept
{
	return Contains(String(cstr), ignoreCase);
}

Boolean String::Contains(const char* cstr, bool ignoreCase, const String& locale) const noexcept
{
	return Contains(String(cstr), ignoreCase, locale);
}

Boolean String::Contains(CodePoint cp, bool ignoreCase) const noexcept
{
	if (!ignoreCase)
		return Contains(cp);

	String s = FromCodePoint(cp);
	return Contains(s, true);
}

Boolean String::Contains(CodePoint cp, bool ignoreCase, const String& locale) const noexcept
{
	if (!ignoreCase)
		return Contains(cp);

	String s = FromCodePoint(cp);
	return Contains(s, true, locale);
}

Int32 String::Compare(const String& A, const String& B, bool ignoreCase, const String& locale) noexcept
{
	// Mesma região? iguais.
	if (A._ptr == B._ptr &&
		A._byteOffset == B._byteOffset &&
		A._byteLength == B._byteLength)
		return 0;

	const Char* aBytes = A.data();
	const Char* bBytes = B.data();

	uint32_t lenA = A._byteLength;
	uint32_t lenB = B._byteLength;

	const char* loc = reinterpret_cast<const char*>(locale.data());
	uint32_t locLen = locale.GetByteCount();
	bool isTurkic = UnicodeCase::locale_is_turkic(loc, locLen);

	bool asciiA = ASCII::IsAllASCII(aBytes, lenA);
	bool asciiB = ASCII::IsAllASCII(bBytes, lenB);

	// ====================================
	// FAST-PATH 1: ordinal ASCII
	// ====================================
	if (!ignoreCase && asciiA && asciiB)
		return ASCII::Compare(aBytes, lenA, bBytes, lenB);

	// ====================================
	// FAST-PATH 2: ignoreCase ASCII (não-Turkic)
	// ====================================
	if (ignoreCase && !isTurkic && asciiA && asciiB)
		return ASCII::CompareIgnoreCase(aBytes, lenA, bBytes, lenB);

	// FAST equality shortcut: identical bytes (ordinal, not ignoreCase)
	if (!ignoreCase && lenA == lenB && lenA > 0)
	{
		if (memcmp(aBytes, bBytes, lenA * sizeof(Char)) == 0)
			return 0;
		// else fallthrough to lexicographic compare below
	}

	// ====================================
	// SLOW-PATH Unicode: casefold + normalize
	// ====================================
	List<CodePoint> Ac, Bc;
	UTF8::FoldAndNormalize(A, A.GetByteCount(), locale, Ac);
	UTF8::FoldAndNormalize(B, B.GetByteCount(), locale, Bc);
	return UTF8::Compare(Ac, Bc);
}

Boolean String::Compare(const String& a, const String& b, const char* locale) noexcept
{
	List<CodePoint> A, B;
	UTF8::FoldAndNormalize(a, a.GetByteCount(), locale, A);
	UTF8::FoldAndNormalize(b, b.GetByteCount(), locale, B);

	if (A.Count() != B.Count()) return false;
	for (uint32_t i = 0; i < A.Count(); ++i)
		if (A[i] != B[i]) return false;
	return true;
}

Boolean String::Equals(const String& other) const noexcept
{
	// Fast path: mesma região de memória
	if (_ptr == other._ptr &&
		_byteOffset == other._byteOffset &&
		_byteLength == other._byteLength)
		return true;

	// Tamanho diferente -> falso
	if (_byteLength != other._byteLength)
		return false;

	// Compare bytes
	const Char* a = data();
	const Char* b = other.data();
	for (uint32_t i = 0; i < _byteLength; ++i)
		if (a[i] != b[i])
			return false;

	return true;
}

Boolean String::Equals(const String& other, bool ignoreCase) const noexcept
{
	if (!ignoreCase)
		return Equals(other);

	return String::Compare(*this, other, true, "en") == 0;
}

Boolean String::Equals(const String& other,
	bool ignoreCase,
	const String& locale) const noexcept
{
	if (!ignoreCase)
		return Equals(other);

	return String::Compare(*this, other, true, locale) == 0;
}

Boolean String::Equals(const String& a,
	const String& b,
	bool ignoreCase) noexcept
{
	if (!ignoreCase)
		return a.Equals(b);

	return String::Compare(a, b, true, "en") == 0;
}

Boolean String::Equals(const String& a,
	const String& b,
	bool ignoreCase,
	const String& locale) noexcept
{
	if (!ignoreCase)
		return a.Equals(b);

	return String::Compare(a, b, true, locale) == 0;
}

Int64 String::IndexOf(const String& value, size_t startIndex) const
{
	// -----------------------------
	// 1) Fast-path: ambas ASCII puras
	// -----------------------------
	if (IsASCII(*this) && IsASCII(value))
	{
		UInt64 H = _byteLength;
		UInt64 N = value._byteLength;

		// ASCII::IndexOf já trata N==0 e bounds
		int64_t res = ASCII::IndexOf(data(), H, value.data(), N, (uint32_t)startIndex);
		return static_cast<Int64>(res);
	}

	// Decode both strings to codepoints
	List<CodePoint> H, N;
	UTF8::Decode(data(), _byteLength, H);
	UTF8::Decode(value.data(), value._byteLength, N);

	return UTF8::IndexOf(H, N, startIndex);
}

Int64 String::IndexOf(const String& value) const noexcept
{
	return IndexOf(value, 0);
}

Int64 String::IndexOf(const String& value, size_t startIndex, size_t count) const noexcept
{
	UInt64 len = GetLength();
	if (startIndex >= len) return -1;

	if (count > len - startIndex)
		count = len - startIndex;

	// substring by grapheme cluster
	String sub = this->Substring((uint32_t)startIndex, (uint32_t)count);

	Int64 idx = sub.IndexOf(value);
	if (idx == -1)
		return -1;

	return startIndex + idx;
}

Int64 String::IndexOf(const String& value, StringComparison comp) const noexcept
{
	return IndexOf(value, 0, comp);
}

Int64 String::IndexOf(const String& value, size_t startIndex, StringComparison comp) const noexcept
{
	if (comp == StringComparison::Ordinal)
		return IndexOf(value, startIndex);

	// OrdinalIgnoreCase – .NET-compatible mode
	String LHS = this->ToLower("en");
	String RHS = value.ToLower("en");

	// remove U+0307 — required for .NET compatibility
	RemoveCombiningDotAbove(LHS);
	RemoveCombiningDotAbove(RHS);

	return LHS.IndexOf(RHS, startIndex);
}

Int64 String::IndexOf(const String& value, size_t startIndex, size_t count, StringComparison comp) const noexcept
{
	// ORDINAL → já estava correto
	if (comp == StringComparison::Ordinal)
		return IndexOf(value, startIndex, count);

	// --------------- OrdinalIgnoreCase (com clamp) -----------------

	Int64 len = (Int64)this->GetLength();
	if (len == 0) return -1;

	// Clamp startIndex
	Int64 sIndex = (Int64)startIndex;
	if (sIndex >= len)
		sIndex = len - 1;

	// Clamp count
	Int64 c = (Int64)count;
	if (c > len - sIndex)
		c = len - sIndex;
	if (c < 0)
		return -1;

	// Safe substring window
	String LHS = this->ToLower("en").Substring((uint32_t)sIndex, (uint32_t)c);
	String RHS = value.ToLower("en");

	RemoveCombiningDotAbove(LHS);
	RemoveCombiningDotAbove(RHS);

	Int64 idx = LHS.IndexOf(RHS);
	if (idx < 0) return -1;

	return sIndex + idx;
}

Int64 String::IndexOf(Char c) const noexcept
{
	String s = String::FromChar(c);
	return IndexOf(s, 0);
}

Int64 String::IndexOf(Char c, size_t startIndex) const noexcept
{
	String s = String::FromChar(c);
	return IndexOf(s, startIndex);
}

Int64 String::IndexOf(Char c, size_t startIndex, size_t count) const noexcept
{
	String s = String::FromChar(c);
	return IndexOf(s, startIndex, count);
}

Int64 String::IndexOf(Char c, StringComparison comp) const noexcept
{
	String s = String::FromChar(c);
	return IndexOf(s, comp);
}

Int64 String::IndexOfAny(const List<Char>& chars, UInt64 startIndex, UInt64 count) const noexcept
{
	UInt64 len = GetLength();
	if (startIndex >= len || chars.IsEmpty())
		return -1;

	if (count > len - startIndex)
		count = len - startIndex;

	const Char* bytes = data();
	UInt64 end = startIndex + count;

	for (UInt64 g = startIndex; g < end; ++g)
	{
		// Grapheme index → byte offset do primeiro byte UTF-8
		uint32_t byteOffset = FindByteOffsetOfCodePoint((uint32_t)g);
		if (byteOffset >= _byteLength)
			continue;

		unsigned char lead = (unsigned char)bytes[byteOffset].Value;

		for (UInt64 k = 0; k < chars.Count(); ++k)
		{
			if ((unsigned char)chars[k].Value == lead)
				return (Int64)g;
		}
	}

	return -1;
}

Int64 String::IndexOfAny(const List<Char>& chars, UInt64 startIndex) const noexcept
{
	UInt64 len = GetLength();
	if (startIndex >= len)
		return -1;

	return IndexOfAny(chars, startIndex, len - startIndex);
}

Int64 String::IndexOfAny(const List<Char>& chars) const noexcept
{
	return IndexOfAny(chars, 0, GetLength());
}

String String::Insert(uint32_t cpIndex, const String& value) const noexcept
{
	// Se value estiver vazio → retorno rápido
	if (value.IsEmpty()) return *this;

	// Se string original estiver vazia → apenas copia value
	if (IsEmpty()) return value;

	// Converter cpIndex → offset em bytes
	uint32_t insertBytePos = FindByteOffsetOfCodePoint(cpIndex);
	if (insertBytePos > _byteLength)
		insertBytePos = _byteLength; // insere no fim se passou

	// Nova string = parte antes + value + parte depois
	uint32_t newByteLen = _byteLength + value._byteLength;
	unsigned char* block = allocate_block(newByteLen);

	if (!block) return String(); // sem memória

	Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));
	const Char* src = data();

	// copiar parte antes do insert
	for (uint32_t i = 0; i < insertBytePos; ++i)
		dst[i] = src[i];

	// copiar value inteiro
	const Char* vsrc = value.data();
	for (uint32_t i = 0; i < value._byteLength; ++i)
		dst[insertBytePos + i] = vsrc[i];

	// copiar parte depois
	for (uint32_t i = insertBytePos; i < _byteLength; ++i)
		dst[value._byteLength + i] = src[i];

	// construir String final
	String result(block);
	result._byteOffset = 0;
	result._byteLength = newByteLen;
	return result;
}

String String::Join(const String& separator, const List<String>& values)
{
	uint32_t count = values.Count();
	if (count == 0)
		return String::Empty();
	if (count == 1)
		return values[0];

	bool sepASCII = String::IsASCII(separator);
	bool allASCII = sepASCII && AllASCII(values);

	// ============================================
	// FAST ASCII PATH
	// ============================================
	if (allASCII)
	{
		uint32_t sepLen = separator.GetByteCount();
		uint32_t total = ComputeJoinASCIIByteCount(values, sepLen);

		if (total <= SSO_CAPACITY)
		{
			String out;
			out._flags = FLAG_SSO;
			out._byteLength = total;

			uint32_t w = 0;
			for (uint32_t i = 0; i < count; ++i)
			{
				const String& s = values[i];
				for (uint32_t k = 0; k < s.GetByteCount(); ++k)
					out._sso[w++] = s.data()[k];

				if (i + 1 < count)
					for (uint32_t k = 0; k < sepLen; ++k)
						out._sso[w++] = separator.data()[k];
			}

			out._sso[total] = Char(0);
			return out;
		}

		unsigned char* block = allocate_block(total);
		if (!block) return String();

		Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));
		uint32_t w = 0;

		for (uint32_t i = 0; i < count; ++i)
		{
			const String& s = values[i];
			uint32_t slen = s.GetByteCount();
			const Char* sb = s.data();

			for (uint32_t k = 0; k < slen; ++k)
				dst[w++] = sb[k];

			if (i + 1 < count)
			{
				const Char* sp = separator.data();
				for (uint32_t k = 0; k < sepLen; ++k)
					dst[w++] = sp[k];
			}
		}

		String out(block);
		out._byteLength = total;
		return out;
	}

	// ============================================
	// GENERAL UNICODE PATH
	// ============================================
	String result = values[0];

	for (uint32_t i = 1; i < count; ++i)
		result = result + separator + values[i];

	return result;
}

String String::Join(Char separator, const List<String>& values)
{
	return Join(String(separator), values);
}

String String::Join(CodePoint separator, const List<String>& values)
{
	return Join(String::FromCodePoint(separator), values);
}

String String::Join(const String& separator, const List<String>& values, int start, int count)
{
	if (count <= 0 || start < 0 || (uint32_t)start >= values.Count())
		return String::Empty();

	uint32_t end = start + count;
	if (end > values.Count())
		end = values.Count();

	List<String> slice;
	slice.Reserve(end - start);

	for (uint32_t i = start; i < end; ++i)
		slice.Add(values[i]);

	return Join(separator, slice);
}

String String::Join(Char separator, const List<String>& values, int start, int count)
{
	return Join(String(separator), values, start, count);
}

String String::Join(CodePoint separator, const List<String>& values, int start, int count)
{
	return Join(String::FromCodePoint(separator), values, start, count);
}

Int64 String::LastIndexOf(const String& value, size_t startIndex) const noexcept
{
	// -----------------------------
	// 1) Fast-path: ambas ASCII puras
	// -----------------------------
	if (IsASCII(*this) && IsASCII(value))
	{
		Int64 H = _byteLength;
		Int64 N = value._byteLength;

		int64_t res = ASCII::LastIndexOf(data(), H, value.data(), N, (uint32_t)startIndex);
		return static_cast<Int64>(res);
	}

	// Decode both strings to codepoints
	List<CodePoint> H, N;
	UTF8::Decode(data(), _byteLength, H);
	UTF8::Decode(value.data(), value._byteLength, N);

	return UTF8::LastIndexOf(H, N, startIndex);
}

Int64 String::LastIndexOf(const String& value) const noexcept
{
	UInt64 len = GetLength();
	if (len == 0) return -1;

	return LastIndexOf(value, len - 1);
}

Int64 String::LastIndexOf(const String& value, size_t startIndex, size_t count) const noexcept
{
	Int64 len = (Int64)this->GetLength();
	if (len == 0) return -1;

	// Clamp startIndex
	Int64 sIndex = (Int64)startIndex;
	if (sIndex >= len)
		sIndex = len - 1;

	if (count == 0) return -1;

	// Clamp count
	Int64 c = (Int64)count;
	if (c > sIndex + 1)
		c = sIndex + 1;

	// Safe window: backward search range [first .. sIndex]
	Int64 first = sIndex - c + 1;

	// Slice string for scanning
	String sub = this->Substring((uint32_t)first, (uint32_t)c);

	Int64 idx = sub.LastIndexOf(value);
	if (idx < 0) return -1;

	return first + idx;
}

Int64 String::LastIndexOf(const String& value, StringComparison comp) const noexcept
{
	UInt64 len = GetLength();
	if (len == 0) return -1;

	return LastIndexOf(value, len - 1, comp);
}

Int64 String::LastIndexOf(const String& value, size_t startIndex, StringComparison comp) const noexcept
{
	if (comp == StringComparison::Ordinal)
		return LastIndexOf(value, startIndex);

	String LHS = this->ToLower("en");
	String RHS = value.ToLower("en");

	RemoveCombiningDotAbove(LHS);
	RemoveCombiningDotAbove(RHS);

	return LHS.LastIndexOf(RHS, startIndex);
}

Int64 String::LastIndexOf(const String& value, size_t startIndex, size_t count, StringComparison comp) const noexcept
{
	Int64 len = (Int64)this->GetLength();
	if (len == 0) return -1;

	// Clamp startIndex
	Int64 sIndex = (Int64)startIndex;
	if (sIndex >= len)
		sIndex = len - 1;

	if (count == 0)
		return -1;

	// Clamp count
	Int64 c = (Int64)count;
	if (c > sIndex + 1)
		c = sIndex + 1;

	// Safe backward window [first .. sIndex]
	Int64 first = sIndex - c + 1;

	// --------------- ORDINAL -----------------
	if (comp == StringComparison::Ordinal)
	{
		String sub = this->Substring((uint32_t)first, (uint32_t)c);
		Int64 idx = sub.LastIndexOf(value);
		if (idx < 0) return -1;
		return first + idx;
	}

	// --------------- ORDINAL IGNORE CASE -----------------

	String LHS = this->ToLower("en").Substring((uint32_t)first, (uint32_t)c);
	String RHS = value.ToLower("en");

	RemoveCombiningDotAbove(LHS);
	RemoveCombiningDotAbove(RHS);

	Int64 idx = LHS.LastIndexOf(RHS);
	if (idx < 0)
		return -1;

	return first + idx;
}

Int64 String::LastIndexOf(Char c) const noexcept
{
	String s = String::FromChar(c);
	return LastIndexOf(s);
}

Int64 String::LastIndexOf(Char c, size_t startIndex) const noexcept
{
	String s = String::FromChar(c);
	return LastIndexOf(s, startIndex);
}

Int64 String::LastIndexOf(Char c,
	size_t startIndex,
	size_t count) const noexcept
{
	String s = String::FromChar(c);
	return LastIndexOf(s, startIndex, count);
}

Int64 String::LastIndexOfAny(const List<Char>& chars, UInt64 startIndex, UInt64 count) const noexcept
{
	UInt64 len = GetLength();
	if (len == 0 || chars.IsEmpty())
		return -1;

	Int64 sIndex = (Int64)startIndex;
	if (sIndex >= (Int64)len)
		sIndex = (Int64)len - 1;

	if (count == 0)
		return -1;

	Int64 c = (Int64)count;
	if (c > sIndex + 1)
		c = sIndex + 1;

	Int64 first = sIndex - c + 1;

	const Char* bytes = data();

	for (Int64 g = sIndex; g >= first; --g)
	{
		uint32_t byteOffset = FindByteOffsetOfCodePoint((uint32_t)g);
		if (byteOffset >= _byteLength)
			continue;

		unsigned char lead = (unsigned char)bytes[byteOffset].Value;

		for (UInt64 k = 0; k < chars.Count(); ++k)
		{
			if ((unsigned char)chars[k].Value == lead)
				return g;
		}
	}

	return -1;
}

Int64 String::LastIndexOfAny(const List<Char>& chars, UInt64 startIndex) const noexcept
{
	UInt64 len = GetLength();
	if (len == 0)
		return -1;

	if (startIndex >= len)
		startIndex = len - 1;

	return LastIndexOfAny(chars, startIndex, startIndex + 1);
}

Int64 String::LastIndexOfAny(const List<Char>& chars) const noexcept
{
	UInt64 len = GetLength();
	if (len == 0)
		return -1;

	return LastIndexOfAny(chars, len - 1, len);
}

String String::Normalize(UnicodeNormalization::NormalizationForm form) const noexcept
{
	if (form == UnicodeNormalization::NormalizationForm::NFC)
	{
		if (HasNFC())
			return *_nfc;

		String tmp = GetNormalized(form);
		_nfc = new String(tmp);
		SetHasNFC();
		return *_nfc;
	}

	if (form == UnicodeNormalization::NormalizationForm::NFKC)
	{
		if (HasNFKC())
			return *_nfkc;

		String tmp = GetNormalized(form);
		_nfkc = new String(tmp);
		SetHasNFKC();
		return *_nfkc;
	}

	// Formas não cacheadas (NFD/NFKD)
	return this->GetNormalized(form);
}

String String::Remove(int start) const noexcept
{
	// Normalize negative start to 0 to keep behavior consistent with the rest of the codebase/tests
	if (start < 0) start = 0;

	uint64_t totalG = GetLength();

	// Out of range → return the original string unchanged
	if ((UInt64)start >= totalG)
		return *this;

	// Equivalent to .NET: Remove(start, Length - start)
	UInt64 count = totalG - (UInt64)start;

	// Delegate to the (start, count) overload which already implements:
	//  - ASCII fast-path
	//  - grapheme boundary handling for UTF-8
	//  - clamping and out-of-range semantics
	return Remove(start, count);
}

String String::Remove(int start, int count) const noexcept
{
	if (start < 0 || count < 0)
		return *this;

	UInt64 totalG = GetLength();

	if ((UInt64)start > totalG)  // strictly '>' like .NET 
		return *this;

	if (count == 0)
		return *this;

	if ((UInt64)start == totalG)
		return *this;

	// Clamp count to available graphemes
	UInt64 rem = (UInt64)count;
	UInt64 maxRem = totalG - (UInt64)start;
	if (rem > maxRem) rem = maxRem;

	// ===========================
	// ASCII FAST-PATH
	// ===========================
	if (IsASCII(*this))
	{
		uint32_t byteStart = (uint32_t)start;
		uint32_t byteCount = (uint32_t)rem;

		uint32_t newLen = _byteLength;
		if (byteStart >= newLen)
			return *this;

		uint32_t outLen = newLen - byteCount;

		if (outLen == 0)
			return String::Empty();

		unsigned char* block = allocate_block(outLen);
		if (!block)
			return String::Empty();

		Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));
		const Char* src = data();

		// copy before
		for (uint32_t i = 0; i < byteStart; ++i)
			dst[i] = src[i];

		// copy after
		for (uint32_t i = byteStart; i < outLen; ++i)
			dst[i] = src[i + byteCount];

		String res(block);
		res._byteOffset = 0;
		res._byteLength = outLen;
		return res;
	}

	// ===========================
	// UNICODE SLOW PATH
	// ===========================
	List<CodePoint> cps = DecodeToCodePoints(*this);
	uint32_t cpCount = cps.Count();
	if (cpCount == 0)
		return String::Empty();

	List<CodePoint> boundaries;
	boundaries.Reserve(cpCount);
	uint32_t gcTotal = 0;
	UnicodeCase::ComputeGraphemeBoundaries(cps, cpCount, boundaries, gcTotal);

	uint32_t s = (uint32_t)start;
	uint32_t c = (uint32_t)rem;

	if (s >= gcTotal)
		return *this;

	uint32_t startCpIndex = boundaries[s];
	uint32_t endCpIndex = (s + c < gcTotal) ? (uint32_t)boundaries[s + c] : cpCount;

	uint32_t byteStart = FindByteOffsetOfCodePoint(startCpIndex);
	uint32_t byteEnd = FindByteOffsetOfCodePoint(endCpIndex);

	if (byteStart > _byteLength) byteStart = _byteLength;
	if (byteEnd > _byteLength) byteEnd = _byteLength;

	uint32_t outLen = _byteLength - (byteEnd - byteStart);

	if (outLen == 0)
		return String::Empty();

	unsigned char* block = allocate_block(outLen);
	if (!block)
		return String::Empty();

	Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));
	const Char* src = data();

	// copy before removed segment
	for (uint32_t i = 0; i < byteStart; ++i)
		dst[i] = src[i];

	// copy after removed segment
	uint32_t pos = byteStart;
	for (uint32_t i = byteEnd; i < _byteLength; ++i)
		dst[pos++] = src[i];

	String res(block);
	res._byteOffset = 0;
	res._byteLength = outLen;
	return res;
}

String String::Replace(Char oldChar, Char newChar) const noexcept
{
	if (IsEmpty()) return *this;

	const Char* src = data();
	uint32_t len = _byteLength;

	bool hasAny = false;
	for (uint32_t i = 0; i < len; ++i)
		if (src[i].Value == oldChar.Value)
			hasAny = true;

	if (!hasAny) return *this;

	if (len <= SSO_CAPACITY)
	{
		String out;
		out._flags = FLAG_SSO;
		out._byteLength = len;

		for (uint32_t i = 0; i < len; ++i)
			out._sso[i] = (src[i].Value == oldChar.Value ? newChar : src[i]);

		out._sso[len] = Char(0);
		return out;
	}

	// heap fallback
	unsigned char* block = allocate_block(len);
	Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));

	for (uint32_t i = 0; i < len; ++i)
		dst[i] = (src[i].Value == oldChar.Value ? newChar : src[i]);

	String out(block);
	out._byteLength = len;
	return out;
}

String String::Replace(CodePoint oldCp, CodePoint newCp) const noexcept
{
	if (IsEmpty()) return *this;

	// -----------------------
	// FAST-PATH ASCII CP replace
	// -----------------------
	if (oldCp.Value <= 0x7F && newCp.Value <= 0x7F && IsASCII(*this))
	{
		unsigned char oldb = (unsigned char)oldCp.Value;
		unsigned char newb = (unsigned char)newCp.Value;

		const unsigned char* src = reinterpret_cast<const unsigned char*>(data());
		uint32_t len = _byteLength;

		bool has = false;
		for (uint32_t i = 0; i < len; ++i)
			if (src[i] == oldb) has = true;

		if (!has) return *this;

		unsigned char* block = allocate_block(len);
		if (!block) return String();

		Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));

		for (uint32_t i = 0; i < len; ++i)
			dst[i] = Char(src[i] == oldb ? newb : src[i]);

		String out(block);
		out._byteLength = len;
		return out;
	}

	// -----------------------
	// UNICODE slow-path
	// -----------------------
	List<CodePoint> cps = DecodeToCodePoints(*this);
	bool changed = false;

	for (uint32_t i = 0; i < cps.Count(); ++i)
	{
		if ((uint32_t)cps[i] == (uint32_t)oldCp)
		{
			cps[i] = newCp;
			changed = true;
		}
	}

	if (!changed) return *this;

	return String::FromCodePoints(cps);
}

String String::Replace(const String& oldValue, const String& newValue) const noexcept
{
	if (oldValue.IsEmpty()) return *this;
	if (this->IsEmpty()) return *this;

	const uint32_t H = _byteLength;
	const uint32_t O = oldValue._byteLength;
	const uint32_t N = newValue._byteLength;

	// -----------------------------------------
	// FAST-PATH: ASCII ordinal replace
	// -----------------------------------------
	if (IsASCII(*this) && IsASCII(oldValue) && IsASCII(newValue))
	{
		const Char* h = data();
		const Char* o = oldValue.data();
		const Char* r = newValue.data();

		if (O == 0) return *this;
		if (O > H) return *this;

		// 1) Count occurrences
		uint32_t count = ASCII::CountOccurrences(h, H, o, O);

		if (count == 0) return *this;

		uint32_t newByteLength = H + count * (N - O);
		unsigned char* block = allocate_block(newByteLength);
		if (!block) return String();

		Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));
		uint32_t writePos = 0;

		// 2) Build result
		ASCII::ReplaceOrdinal(h, H, o, O, r, N, dst);

		String out(block);
		out._byteLength = newByteLength;
		return out;
	}

	// -----------------------------------------
	// FALLBACK: original safe Unicode logic
	// -----------------------------------------
	String result = *this;
	Int64 pos = 0;

	while (true)
	{
		pos = result.IndexOf(oldValue, (size_t)pos);
		if (pos < 0) break;

		result = result.Remove((int)pos, (int)oldValue.GetLength())
			.Insert((uint32_t)pos, newValue);

		pos += newValue.GetLength();
	}

	return result;
}

String String::Replace(const String& oldValue, const String& newValue, StringComparison comp) const noexcept
{
	if (comp == StringComparison::Ordinal)
		return Replace(oldValue, newValue);

	// ---------------------------------------
	// OrdinalIgnoreCase ASCII fast-path
	// ---------------------------------------
	if (IsASCII(*this) && IsASCII(oldValue) && IsASCII(newValue))
	{
		const Char* h = data();
		const Char* o = oldValue.data();
		const Char* r = newValue.data();

		uint32_t H = _byteLength;
		uint32_t O = oldValue._byteLength;
		uint32_t N = newValue._byteLength;

		if (O == 0) return *this;

		// Count occurrences using ASCII::CountOccurrencesIgnoreCase
		uint32_t count = ASCII::CountOccurrencesIgnoreCase(h, H, o, O);
		if (count == 0) return *this;

		uint32_t newByteLength = H + count * (N - O);
		unsigned char* block = allocate_block(newByteLength);
		if (!block) return String();

		Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));

		ASCII::ReplaceIgnoreCase(h, H, o, O, r, N, dst);

		String out(block);
		out._byteLength = newByteLength;
		return out;
	}

	// ---------------------------------------
	// fallback → full Unicode Replace(ignoreCase,"en")
	// ---------------------------------------
	return Replace(oldValue, newValue, true, "en");
}

String String::Replace(const String& oldValue, const String& newValue,
	bool ignoreCase, const String& locale) const noexcept
{
	if (oldValue.IsEmpty()) return *this;

	bool asciiAll =
		IsASCII(*this) &&
		IsASCII(oldValue) &&
		IsASCII(newValue);

	bool isTurkic = UnicodeCase::locale_is_turkic(
		(const char*)locale.data(),
		locale.GetByteCount()
	);

	// ---------------------------------------------------------------
	// FAST PATH: ASCII ignore-case + NON-TURKIC locale
	// ---------------------------------------------------------------
	if (ignoreCase && asciiAll && !isTurkic)
	{
		const Char* h = data();
		const Char* o = oldValue.data();
		const Char* r = newValue.data();

		uint32_t H = _byteLength;
		uint32_t O = oldValue._byteLength;
		uint32_t N = newValue._byteLength;

		if (O == 0) return *this;

		// Count matches
		uint32_t count = ASCII::CountOccurrencesIgnoreCase(h, H, o, O);
		if (count == 0) return *this;

		uint32_t newByteLength = H + count * (N - O);
		unsigned char* block = allocate_block(newByteLength);
		if (!block) return String();

		Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));

		ASCII::ReplaceIgnoreCase(h, H, o, O, r, N, dst);

		String out(block);
		out._byteLength = newByteLength;
		return out;
	}

	// ------------------------------
	// 1. Normalize + casefold haystack and needle
	// ------------------------------
	List<CodePoint> hayFold, oldFold;
	UTF8::FoldAndNormalize(*this, GetByteCount(), locale, hayFold);
	UTF8::FoldAndNormalize(oldValue, oldValue.GetByteCount(), locale, oldFold);

	// decode newValue to pure CPs (NO folding)
	List<CodePoint> newCP = DecodeToCodePoints(newValue);

	// build result in CP domain
	List<CodePoint> out;
	UTF8::Replace(hayFold, oldFold, newCP, out);

	return String::FromCodePoints(out);
}

String String::ReplaceLineEndings() const noexcept
{
	// Se vazio — retorno rápido
	if (IsEmpty())
		return *this;

	const Char* src = data();
	uint32_t len = _byteLength;

	// Primeiro pass: contar bytes finais
	uint32_t outLen = 0;
	for (uint32_t i = 0; i < len; )
	{
		unsigned char c = (unsigned char)src[i].Value;

		if (c == '\r')
		{
			// CRLF?
			if (i + 1 < len && (unsigned char)src[i + 1].Value == '\n')
			{
				outLen += 1;   // becomes '\n'
				i += 2;
			}
			else
			{
				outLen += 1;   // '\r' → '\n'
				i += 1;
			}
		}
		else if (c == '\n')
		{
			outLen += 1;       // '\n' stays '\n'
			i += 1;
		}
		else
		{
			outLen += 1;
			i += 1;
		}
	}

	unsigned char* block = allocate_block(outLen);
	if (!block)
		return String();

	Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));
	uint32_t w = 0;

	// Second pass: write transformed string
	for (uint32_t i = 0; i < len; )
	{
		unsigned char c = (unsigned char)src[i].Value;

		if (c == '\r')
		{
			if (i + 1 < len && (unsigned char)src[i + 1].Value == '\n')
			{
				dst[w++] = Char('\n');
				i += 2;
			}
			else
			{
				dst[w++] = Char('\n');
				i += 1;
			}
		}
		else if (c == '\n')
		{
			dst[w++] = Char('\n');
			i += 1;
		}
		else
		{
			dst[w++] = Char(c);
			i += 1;
		}
	}

	String out(block);
	out._byteLength = outLen;
	return out;
}

String String::ReplaceLineEndings(const String& replacement) const noexcept
{
	if (IsEmpty())
		return *this;

	// Replacement pode ser vazio → remove line endings
	const Char* rep = replacement.data();
	uint32_t repLen = replacement.GetByteCount();

	const Char* src = data();
	uint32_t len = _byteLength;

	// Primeiro pass: calcular novo tamanho final
	uint32_t outLen = 0;

	for (uint32_t i = 0; i < len; )
	{
		unsigned char c = (unsigned char)src[i].Value;

		if (c == '\r')
		{
			if (i + 1 < len && (unsigned char)src[i + 1].Value == '\n')
			{
				outLen += repLen;
				i += 2;
			}
			else
			{
				outLen += repLen;
				i += 1;
			}
		}
		else if (c == '\n')
		{
			outLen += repLen;
			i += 1;
		}
		else
		{
			outLen += 1;
			i += 1;
		}
	}

	unsigned char* block = allocate_block(outLen);
	if (!block)
		return String();

	Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));
	uint32_t w = 0;

	// Second pass: escrever saída
	for (uint32_t i = 0; i < len; )
	{
		unsigned char c = (unsigned char)src[i].Value;

		if (c == '\r')
		{
			if (i + 1 < len && (unsigned char)src[i + 1].Value == '\n')
			{
				// write replacement
				for (uint32_t k = 0; k < repLen; ++k)
					dst[w++] = rep[k];
				i += 2;
			}
			else
			{
				for (uint32_t k = 0; k < repLen; ++k)
					dst[w++] = rep[k];
				i += 1;
			}
		}
		else if (c == '\n')
		{
			for (uint32_t k = 0; k < repLen; ++k)
				dst[w++] = rep[k];
			i += 1;
		}
		else
		{
			dst[w++] = Char(c);
			i += 1;
		}
	}

	String out(block);
	out._byteLength = outLen;
	return out;
}

List<String> String::Split(const String& sep, int max, StringSplitOptions opt) const
{
	List<String> s;
	s.Add(sep);
	return SplitImpl(s, {}, max, opt);
}

List<String> String::Split(const String& separator, StringSplitOptions options) const
{
	List<String> s;
	s.Add(separator);
	return SplitImpl(s, {}, Int32::MaxValue(), options);
}

List<String> String::Split(Char sep, int max, StringSplitOptions opt) const
{
	List<Char> c;
	c.Add(sep);
	return SplitImpl({}, c, max, opt);
}

List<String> String::Split(Char separator, StringSplitOptions options) const
{
	List<Char> c;
	c.Add(separator);
	return SplitImpl({}, c, Int32::MaxValue(), options);
}

List<String> String::Split(const List<String>& seps, StringSplitOptions opt) const
{
	return SplitImpl(seps, {}, Int32::MaxValue(), opt);
}

List<String> String::Split(const List<String>& separators, int maxCount, StringSplitOptions options) const
{
	return SplitImpl(separators, {}, maxCount, options);
}

List<String> String::Split(const List<Char>& separators, int maxCount, StringSplitOptions options) const
{
	return SplitImpl({}, separators, maxCount, options);
}

List<String> String::Split(const List<Char>& separators, StringSplitOptions options) const
{
	return SplitImpl({}, separators, Int32::MaxValue(), options);
}

List<String> String::Split(Char separator) const
{
	List<Char> c;
	c.Add(separator);
	return SplitImpl({}, c, Int32::MaxValue(), StringSplitOptions::None);
}

List<String> String::Split(const List<Char>& separators) const
{
	return SplitImpl({}, separators, Int32::MaxValue(), StringSplitOptions::None);
}

List<String> String::Split(const String& separator) const
{
	List<String> s;
	s.Add(separator);
	return SplitImpl(s, {}, Int32::MaxValue(), StringSplitOptions::None);
}

List<String> String::Split(const List<String>& separators) const
{
	return SplitImpl(separators, {}, Int32::MaxValue(), StringSplitOptions::None);
}

String String::Substring(uint32_t gcStart, uint32_t gcCount) const noexcept
{
	if (IsEmpty() || gcCount == 0)
		return String::Empty();

	// FAST ASCII PATH
	if (IsASCII(*this))
	{
		uint32_t start = gcStart;
		uint32_t end = gcStart + gcCount;

		if (start >= _byteLength)
			return String::Empty();

		if (end > _byteLength)
			end = _byteLength;

		uint32_t len = end - start;

		if (IsSSO())
		{
			// COPY
			return String(reinterpret_cast<const unsigned char*>(data() + start), len);
		}
		else
		{
			// SLICE
			String sub(*this);
			sub._byteOffset += start;
			sub._byteLength = len;

			sub.InvalidateAscii();
			sub._gcLength = UInt32::MaxValue();
			sub.ReleaseNormalizationCache();

			return sub;
		}
	}

	// -------- Unicode slow-path (como você já tinha) --------

	List<CodePoint> cps = DecodeToCodePoints(*this);
	uint32_t cpCount = cps.Count();
	if (cpCount == 0)
		return String::Empty();

	List<CodePoint> boundaries;
	uint32_t gcTotal = 0;
	UnicodeCase::ComputeGraphemeBoundaries(cps, cpCount, boundaries, gcTotal);

	if (gcStart >= gcTotal)
		return String::Empty();

	uint32_t gcEnd = gcStart + gcCount;
	if (gcEnd > gcTotal)
		gcEnd = gcTotal;

	uint32_t startCp = boundaries[gcStart];
	uint32_t endCp = (gcEnd < gcTotal ? (uint32_t)boundaries[gcEnd] : cpCount);

	uint32_t byteStart = FindByteOffsetOfCodePoint(startCp);
	uint32_t byteEnd = FindByteOffsetOfCodePoint(endCp);
	if (byteEnd > _byteLength) byteEnd = _byteLength;

	uint32_t len = byteEnd - byteStart;

	if (IsSSO())
	{
		return String(reinterpret_cast<const unsigned char*>(data() + byteStart), len);
	}

	String sub(*this);
	sub._byteOffset += byteStart;
	sub._byteLength = len;

	sub.InvalidateAscii();
	sub._gcLength = UInt32::MaxValue();
	sub.ReleaseNormalizationCache();

	return sub;
}

String String::ToLower(const String& locale) const noexcept
{
	const char* localeBytes = reinterpret_cast<const char*>(locale.data());
	uint32_t localeLen = locale.GetByteCount();
	bool isTurkic = UnicodeCase::locale_is_turkic(localeBytes, localeLen);

	// FAST ASCII path (not turkic)
	if (!isTurkic && IsASCII(*this))
	{
		uint32_t len = _byteLength;
		if (len == 0) return String();

		unsigned char* block = allocate_block(len);
		if (!block) return String();

		Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));
		const Char* src = data();

		for (uint32_t i = 0; i < len; ++i)
			dst[i] = Char(ASCII::ToLower((unsigned char)src[i].Value));

		String result(block);
		result._byteOffset = 0;
		result._byteLength = len;
		return result;
	}

	// =====================================================
	// 1) Decode UTF-8 → List<CodePoint>
	// =====================================================
	List<CodePoint> cps;
	cps.EnsureCapacity(_byteLength);

	const Char* ptr = data();
	uint32_t pos = 0;

	UTF8::Decode(*this, GetByteCount(), cps);

	if (cps.IsEmpty())
		return String();

	// =====================================================
	// 2) Locale-aware lowercase expansion → List<CodePoint>
	// =====================================================
	List<CodePoint> lowered;
	lowered.EnsureCapacity(cps.Count() * 3);

	for (uint32_t i = 0; i < cps.Count(); ++i)
	{
		List<CodePoint> seq;
		seq.EnsureCapacity(4);

		UnicodeCase::map_to_lower_sequence_nostd(
			cps[i],
			localeBytes, localeLen,
			cps,        // contexto completo
			i,          // index atual
			seq         // saída
		);

		for (uint32_t k = 0; k < seq.Count(); ++k)
			lowered.Add(seq[k]);
	}

	// =====================================================
	// 3) Re-encode lowered → UTF-8
	// =====================================================
	uint32_t totalBytes = 0;
	for (CodePoint cp : lowered)
		totalBytes += cp.ByteCount();

	unsigned char* block = allocate_block(totalBytes);
	if (!block) return String();

	Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));
	uint32_t w = 0;

	for (CodePoint cp : lowered)
	{
		auto enc = UTF8::encode_utf8(cp);
		for (uint32_t k = 0; k < enc.Length; ++k)
			dst[w++] = Char(enc.Bytes[k]);
	}

	String result(block);
	result._byteOffset = 0;
	result._byteLength = totalBytes;
	return result;
}

String String::ToUpper(const String& locale) const noexcept
{
	const char* localeBytes = reinterpret_cast<const char*>(locale.data());
	uint32_t localeLen = locale.GetByteCount();
	bool isTurkic = UnicodeCase::locale_is_turkic(localeBytes, localeLen);

	// ASCII fast-path (not turkic)
	if (!isTurkic && IsASCII(*this))
	{
		uint32_t len = _byteLength;
		if (len == 0) return String();

		unsigned char* block = allocate_block(len);
		if (!block) return String();

		Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));
		const Char* src = data();

		for (uint32_t i = 0; i < len; ++i)
			dst[i] = Char(ASCII::ToUpper((unsigned char)src[i].Value));

		String result(block);
		result._byteOffset = 0;
		result._byteLength = len;
		return result;
	}

	// =====================================================
	// 1) Decode UTF-8 → List<CodePoint>
	// =====================================================
	List<CodePoint> cps;
	cps.EnsureCapacity(_byteLength);

	const Char* ptr = data();
	uint32_t pos = 0;

	UTF8::Decode(*this, GetByteCount(), cps);

	if (cps.IsEmpty())
		return String();

	// =====================================================
	// 2) Uppercase mapping → List<CodePoint>
	// =====================================================
	List<CodePoint> upper;
	upper.EnsureCapacity(cps.Count() * 3);

	for (uint32_t i = 0; i < cps.Count(); ++i)
	{
		List<CodePoint> seq;
		seq.EnsureCapacity(4);

		UnicodeCase::map_to_upper_sequence_nostd(
			cps[i],
			localeBytes, localeLen,
			seq
		);

		for (uint32_t k = 0; k < seq.Count(); ++k)
			upper.Add(seq[k]);
	}

	// =====================================================
	// 3) Encode UTF-8
	// =====================================================
	uint32_t totalBytes = 0;
	for (CodePoint cp : upper)
		totalBytes += cp.ByteCount();

	unsigned char* block = allocate_block(totalBytes);
	if (!block) return String();

	Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));
	uint32_t w = 0;

	for (CodePoint cp : upper)
	{
		auto enc = UTF8::encode_utf8(cp);
		for (uint32_t k = 0; k < enc.Length; ++k)
			dst[w++] = Char(enc.Bytes[k]);
	}

	String result(block);
	result._byteOffset = 0;
	result._byteLength = totalBytes;
	return result;
}

String String::Trim() const
{
	// Decode to codepoints and compute grapheme cluster boundaries.
	// We trim by whole grapheme clusters: a cluster is trimmed if its first
	// codepoint is a whitespace (so combining marks attached to non-space
	// bases won't be stripped).
	List<CodePoint> cps = DecodeToCodePoints(*this);
	uint32_t cpCount = cps.Count();
	if (cpCount == 0) return String::Empty();

	List<CodePoint> boundaries;
	uint32_t gcTotal = 0;
	UnicodeCase::ComputeGraphemeBoundaries(cps, cpCount, boundaries, gcTotal);
	if (gcTotal == 0) return String::Empty();

	// find first non-whitespace cluster from left
	uint32_t startGc = 0;
	while (startGc < gcTotal) {
		uint32_t cpIndex = (uint32_t)boundaries[startGc];
		if (!cps[cpIndex].IsWhiteSpace()) break;
		++startGc;
	}

	// find first non-whitespace cluster from right
	int32_t endGc = (int32_t)gcTotal - 1;
	while (endGc >= (int32_t)startGc) {
		uint32_t cpIndex = (uint32_t)boundaries[(uint32_t)endGc];
		if (!cps[cpIndex].IsWhiteSpace()) break;
		--endGc;
	}

	if (startGc == 0 && endGc == (int32_t)gcTotal - 1) {
		// nothing trimmed
		return *this;
	}

	if (startGc > endGc) {
		// all trimmed -> empty
		return String::Empty();
	}

	uint32_t newCount = (uint32_t)endGc - (uint32_t)startGc + 1;
	return this->Substring(startGc, newCount);
}

String String::Trim(Char c) const
{
	// trim by grapheme clusters whose first codepoint == c
	List<CodePoint> cps = DecodeToCodePoints(*this);
	uint32_t cpCount = cps.Count();
	if (cpCount == 0) return String::Empty();

	List<CodePoint> boundaries;
	uint32_t gcTotal = 0;
	UnicodeCase::ComputeGraphemeBoundaries(cps, cpCount, boundaries, gcTotal);
	if (gcTotal == 0) return String::Empty();

	uint32_t startGc = 0;
	while (startGc < gcTotal) {
		uint32_t cpIndex = (uint32_t)boundaries[startGc];
		if (cps[cpIndex] != (uint32_t)c) break;
		++startGc;

	}

	int32_t endGc = (int32_t)gcTotal - 1;
	while (endGc >= (int32_t)startGc) {
		uint32_t cpIndex = (uint32_t)boundaries[(uint32_t)endGc];
		if (cps[cpIndex] != (uint32_t)c) break;
		--endGc;

	}

	if (startGc > (uint32_t)endGc) return String::Empty();
	return this->Substring(startGc, (uint32_t)endGc - startGc + 1);
}

String String::Trim(const List<Char>& chars) const
{
	if (chars.IsEmpty()) return *this;

	List<CodePoint> codeSet;
	codeSet.Reserve(chars.Count());
	for (uint32_t i = 0; i < chars.Count(); ++i)
		codeSet.Add((uint32_t)chars[i].Value);

	List<CodePoint> cps = DecodeToCodePoints(*this);
	uint32_t cpCount = cps.Count();
	if (cpCount == 0) return String::Empty();

	List<CodePoint> boundaries;
	uint32_t gcTotal = 0;
	UnicodeCase::ComputeGraphemeBoundaries(cps, cpCount, boundaries, gcTotal);

	uint32_t startGc = 0;
	while (startGc < gcTotal)
	{
		uint32_t cpIndex = boundaries[startGc];
		if (!codeSet.Contains(cps[cpIndex]))
			break;
		++startGc;
	}

	int32_t endGc = gcTotal - 1;
	while (endGc >= (int32_t)startGc)
	{
		uint32_t cpIndex = boundaries[endGc];
		if (!codeSet.Contains(cps[cpIndex]))
			break;
		--endGc;
	}

	if (startGc > endGc) return String::Empty();
	return this->Substring(startGc, endGc - startGc + 1);
}

String String::TrimEnd() const
{
	if (this->IsEmpty()) return *this;

	List<CodePoint> cps = DecodeToCodePoints(*this);
	uint32_t cpCount = cps.Count();
	if (cpCount == 0) return String::Empty();

	List<CodePoint> boundaries;
	uint32_t gcTotal = 0;
	UnicodeCase::ComputeGraphemeBoundaries(cps, cpCount, boundaries, gcTotal);
	if (gcTotal == 0) return String::Empty();

	int32_t endGc = (int32_t)gcTotal - 1;
	while (endGc >= 0) {
		uint32_t cpIndex = (uint32_t)boundaries[(uint32_t)endGc];
		if (!cps[cpIndex].IsWhiteSpace()) break;
		--endGc;
	}

	if (endGc == (int32_t)gcTotal - 1) return *this; // nothing trimmed
	if (endGc < 0) return String::Empty();
	return this->Substring(0, (uint32_t)endGc + 1);
}

String String::TrimEnd(Char c) const
{
	uint32_t target = (uint32_t)c;

	size_t end = this->GetLength();
	while (end > 0 && (uint32_t)GetCodePointAt(end - 1) == target)
		--end;

	return this->Substring(0, end);
}

String String::TrimEnd(const List<Char>& chars) const
{
	if (chars.IsEmpty()) return *this;

	size_t end = this->GetLength();

	while (end > 0)
	{
		CodePoint cp = (uint32_t)GetCodePointAt(end - 1);

		bool match = false;
		for (uint32_t k = 0; k < chars.Count(); ++k)
			if ((uint32_t)chars[k] == (uint32_t)cp.Value)
				match = true;

		if (!match) break;
		--end;
	}

	return this->Substring(0, end);
}

String String::TrimStart() const
{
	// decode + boundaries
	List<CodePoint> cps = DecodeToCodePoints(*this);
	uint32_t cpCount = cps.Count();
	if (cpCount == 0) return String::Empty();

	List<CodePoint> boundaries;
	uint32_t gcTotal = 0;
	UnicodeCase::ComputeGraphemeBoundaries(cps, cpCount, boundaries, gcTotal);
	if (gcTotal == 0) return String::Empty();

	uint32_t startGc = 0;
	while (startGc < gcTotal) {
		uint32_t cpIndex = (uint32_t)boundaries[startGc];
		if (!cps[cpIndex].IsWhiteSpace()) break;
		++startGc;
	}

	if (startGc == 0) return *this;
	if (startGc >= gcTotal) return String::Empty();
	return this->Substring(startGc);
}

String String::TrimStart(Char c) const
{
	// convert Char → CodePoint (ASCII safe)
	uint32_t target = (uint32_t)c;

	size_t i = 0;

	// compare codepoints, not raw bytes
	while (i < this->GetLength() && (uint32_t)GetCodePointAt(i) == target)
		++i;

	return this->Substring(i);
}

String String::TrimStart(const List<Char>& chars) const
{
	if (chars.IsEmpty()) return *this;

	size_t i = 0;

	while (i < this->GetLength())
	{
		uint32_t cp = (uint32_t)GetCodePointAt(i);

		bool match = false;
		for (uint32_t k = 0; k < chars.Count(); ++k)
			if ((uint32_t)chars[k] == (uint32_t)cp)
				match = true;

		if (!match) break;
		++i;
	}

	return this->Substring(i);
}

String String::ToString() const noexcept
{
	return *this;
}


String::String(unsigned char* block) noexcept
{
	if (!block)
	{
		_flags = FLAG_SSO;
		_byteOffset = 0;
		_byteLength = 0;
		_sso[0] = Char(0);
		return;
	}

	_flags = 0;
	_ptr = block;
	_byteOffset = 0;
	_byteLength = length_ref();
}

const Char* String::data() const noexcept
{
	if (IsSSO())
		return _sso;

	if (!_ptr) {
		static const Char emptyChar = Char(0);
		return &emptyChar;
	}
	return bytes_ptr() + _byteOffset;
}

void String::add_ref() noexcept
{
	if (!IsSSO() && _ptr) refcount_ref()++;
}

void String::release() noexcept
{
	if (!IsSSO() && _ptr)
	{
		refcount_type& rc = refcount_ref();
		if (--rc == 0)
			delete[] _ptr;
	}

	_ptr = nullptr;
	_byteOffset = 0;
	_byteLength = 0;
	_flags = FLAG_SSO;

	// invalidate caches
	InvalidateAscii();
	_gcLength = UInt32::MaxValue();
	ReleaseNormalizationCache();
}

unsigned char* String::allocate_block(String::size_type lenChars) noexcept
{
	if (lenChars == 0) return nullptr;

	const unsigned int headerSize = sizeof(refcount_type) + sizeof(size_type);
	unsigned long total = headerSize + lenChars * sizeof(Char) + sizeof(Char);

	unsigned char* p = new unsigned char[total];
	*(refcount_type*)p = 1;
	*(size_type*)(p + sizeof(refcount_type)) = lenChars;

	Char* buf = reinterpret_cast<Char*>(p + headerSize);
	buf[lenChars] = Char(0);

	return p;
}

void String::init_from_cstr(const char* cstr) noexcept
{
	if (!cstr || cstr[0] == '\0')
	{
		_flags = FLAG_SSO;
		_byteOffset = 0;
		_byteLength = 0;
		_sso[0] = Char(0);
		return;
	}
	size_type len = 0; while (cstr[len] != '\0') ++len;
	init_from_bytes(reinterpret_cast<const unsigned char*>(cstr), len);
}

void String::init_from_bytes(const unsigned char* bytes, size_type len) noexcept
{
	static_assert(sizeof(Char) == sizeof(Byte));

	if (!bytes || len == 0)
	{
		_flags = FLAG_SSO;
		_byteLength = 0;
		_byteOffset = 0;
		_sso[0] = Char(0);
		SetAscii(true);
		return;
	}

	if (len <= SSO_CAPACITY)
	{
		_flags = FLAG_SSO;
		_byteOffset = 0;
		_byteLength = len;

		bool ascii = true;
		for (uint32_t i = 0; i < len; ++i)
		{
			_sso[i] = Char(bytes[i]);
			if (bytes[i] & 0x80) ascii = false;
		}

		_sso[len] = Char(0);
		SetAscii(ascii);
		return;
	}

	// fallback heap
	_flags = 0;
	_ptr = allocate_block(len);
	_byteOffset = 0;
	_byteLength = len;

	bool ascii = true;
	Char* dst = bytes_ptr();
	for (size_type i = 0; i < len; ++i)
	{
		dst[i] = Char(bytes[i]);
		if (bytes[i] & 0x80) ascii = false;
	}

	SetAscii(ascii);
}

uint32_t String::FindByteOffsetOfCodePoint(uint32_t cpIndex) const
{
	if (cpIndex == 0) return 0;

	const Char* p = data();
	uint32_t len = _byteLength;

	uint32_t pos = 0;
	uint32_t current = 0;

	while (pos < len)
	{
		if (current == cpIndex)
			return pos;

		auto r = UTF8::decode_utf8((p + pos), len - pos);
		if (!r.IsValid || r.Length == 0)
			return len;

		pos += r.Length;
		current++;
	}

	return pos;
}

int String::CompareOrdinalCP(CodePoint cpA, CodePoint cpB) noexcept
{
	if (cpA < cpB) return -1;
	if (cpA > cpB) return 1;
	return 0;
}

int String::CompareCodePoints_CaseAware(CodePoint a, CodePoint b) noexcept
{
	if (a < b) return -1;
	if (a > b) return 1;
	return 0;
}

CodePoint String::GetCodePointAt(uint32_t cpIndex) const noexcept
{
	uint32_t offset = FindByteOffsetOfCodePoint(cpIndex);
	if (offset >= _byteLength) return CodePoint(0);
	auto r = UTF8::decode_utf8(data() + offset, _byteLength - offset);
	if (!r.IsValid) return CodePoint(0);
	return r.cp;
}

uint32_t String::GetByteLen(const char* cstr) noexcept {
	if (!cstr) return 0;
	uint32_t len = 0;
	while (cstr[len] != '\0') ++len;   // UTF-8 byte count
	return len;
}

void String::CopyBytes(Char* dst, uint32_t& offset, const String& s) noexcept {
	uint32_t len = s.GetByteCount();
	const Char* src = s.data();
	for (uint32_t i = 0; i < len; ++i)
		dst[offset++] = src[i];
}

void String::CopyBytes(Char* dst, uint32_t& offset, const char* cstr) noexcept {
	if (!cstr) return;
	while (*cstr)
		dst[offset++] = Char(static_cast<unsigned char>(*cstr++));
}

bool String::IsASCII(const String& s) noexcept
{
	// if known, return quickly
	if (s.IsAsciiKnown()) return s.IsAsciiCached();

	const Char* p = s.data();
	uint32_t len = s._byteLength;

	for (uint32_t i = 0; i < len; ++i)
	{
		if ((unsigned char)p[i].Value >= 0x80)
		{
			// cache negative
			s.SetAscii(false);
			return false;
		}
	}

	// all ascii
	s.SetAscii(true);
	return true;
}

Boolean String::ContainsBytes(const String& sub) const noexcept
{
	const Char* hay = data();
	const Char* needle = sub.data();

	uint32_t hlen = _byteLength;
	uint32_t nlen = sub._byteLength;

	if (nlen == 0) return true;
	if (nlen > hlen) return false;

	for (uint32_t i = 0; i + nlen <= hlen; ++i)
	{
		if (memcmp(hay + i, needle, nlen * sizeof(Char)) == 0)
			return true;
	}
	return false;
}

List<CodePoint> String::DecodeToCodePoints(const String& s)
{
	List<CodePoint> result;

	const Char* p = s.data();
	uint32_t len = s.GetByteCount();
	if (len == 0)
		return result;

	// Reserva capacidade aproximada: pior caso 1 byte = 1 CP
	result.EnsureCapacity(len);

	UTF8::Decode(s, s.GetByteCount(), result);

	return result;
}

void String::CaseFoldUnicode(
	const List<CodePoint>& cps,        // input sequence
	const char* localeBytes,        // ex: "tr", "en"
	uint32_t localeLen,
	List<CodePoint>& output)         // output dinâmico
{
	output.Clear();
	output.EnsureCapacity(cps.Count() * 4);

	uint32_t cpCount = cps.Count();

	for (uint32_t i = 0; i < cpCount; ++i)
	{
		List<CodePoint> seq;
		seq.EnsureCapacity(4);

		UnicodeCase::map_to_casefold_sequence_nostd(
			cps[i],
			localeBytes,
			localeLen,
			cps,     // contexto
			i,       // index
			seq      // saída
		);

		for (uint32_t k = 0; k < seq.Count(); ++k)
			output.Add(seq[k]);
	}

	// Turkish rule: remove U+0307 after folding
	bool isTurkic = UnicodeCase::locale_is_turkic(localeBytes, localeLen);

	if (isTurkic)
	{
		for (int i = (int)output.Count() - 1; i >= 0; --i)
			if ((uint32_t)output[i] == 0x0307) // combining dot above
				output.RemoveAt(i);
	}
}

int String::CompareFolded(const List<CodePoint>& a, const List<CodePoint>& b)
{
	uint32_t lenA = a.Count();
	uint32_t lenB = b.Count();

	uint32_t m = lenA < lenB ? lenA : lenB;

	for (uint32_t i = 0; i < m; ++i)
	{
		if (a[i] < b[i]) return -1;
		if (a[i] > b[i]) return 1;
	}

	if (lenA < lenB) return -1;
	if (lenA > lenB) return 1;

	return 0;
}

Boolean String::ContainsFolded(const List<CodePoint>& hay, const List<CodePoint>& ned)
{
	uint32_t hayLen = hay.Count();
	uint32_t nedLen = ned.Count();

	if (nedLen == 0) return true;
	if (hayLen < nedLen) return false;

	for (uint32_t i = 0; i + nedLen <= hayLen; ++i)
	{
		bool match = true;
		for (uint32_t j = 0; j < nedLen; ++j)
		{
			if (hay[i + j] != ned[j])
			{
				match = false;
				break;
			}
		}
		if (match)
			return true;
	}
	return false;
}

String String::GetNormalized(UnicodeNormalization::NormalizationForm form) const noexcept
{
	if (IsEmpty()) return *this;

	// 0) Decode to codepoints
	List<CodePoint> cps = DecodeToCodePoints(*this);
	uint32_t cpCount = cps.Count();
	if (cpCount == 0) return *this;

	// 1) ZWJ → skip normalization
	for (uint32_t i = 0; i < cpCount; ++i)
		if ((uint32_t)cps[i] == 0x200D)
			return *this;

	// 2) Decompose dynamically
	const bool compat = (form == UnicodeNormalization::NormalizationForm::NFKC ||
		form == UnicodeNormalization::NormalizationForm::NFKD);

	List<CodePoint> temp;
	temp.EnsureCapacity(cpCount * 3);

	for (uint32_t i = 0; i < cpCount; ++i)
	{
		CodePoint local[32];
		uint32_t localLen = 0;

		UnicodeNormalization::DecomposeCodePoint(cps[i], compat, local, localLen);

		for (uint32_t k = 0; k < localLen; ++k)
			temp.Add(local[k]);
	}

	// 3) If nothing was produced, fallback to original cps
	if (temp.Count() == 0 && cpCount > 0)
	{
		temp.Clear();
		for (uint32_t i = 0; i < cpCount; ++i)
			temp.Add(cps[i]);
	}

	// 4) Reorder
	UnicodeNormalization::ReorderByCCC(temp);

	// 5) Compose
	if (form == UnicodeNormalization::NormalizationForm::NFC ||
		form == UnicodeNormalization::NormalizationForm::NFKC)
	{
		UnicodeNormalization::Compose(temp);
	}

	// 6) Re-encode UTF-8
	uint32_t totalBytes = 0;
	for (uint32_t i = 0; i < temp.Count(); ++i)
	{
		uint32_t cpv = temp[i];
		totalBytes += (cpv <= 0x7F ? 1 :
			cpv <= 0x7FF ? 2 :
			cpv <= 0xFFFF ? 3 : 4);
	}

	unsigned char* block = allocate_block(totalBytes);
	if (!block) return String();

	Char* dst = reinterpret_cast<Char*>(block + sizeof(refcount_type) + sizeof(size_type));

	uint32_t w = 0;
	for (uint32_t i = 0; i < temp.Count(); ++i)
	{
		UTF8::UTF8EncodeResult enc = UTF8::encode_utf8(temp[i]);
		for (uint32_t j = 0; j < enc.Length; ++j)
			dst[w++] = enc.Bytes[j];
	}

	String out(block);
	out._byteLength = totalBytes;
	return out;
}

Boolean String::EndsWithInternal(const String& needle, bool ignoreCase, const char* locale) const noexcept
{
	if (needle.IsEmpty())
		return true;

	const Char* hayBytes = data();
	const Char* neeBytes = needle.data();
	uint32_t H = _byteLength;
	uint32_t N = needle._byteLength;

	if (H < N)
		return false;

	uint32_t start = H - N;

	const char* loc = (locale ? locale : "en");
	uint32_t locLen = (uint32_t)strlen(loc);
	bool isTurkic = UnicodeCase::locale_is_turkic(loc, locLen);

	bool asciiHay = ASCII::IsAllASCII(hayBytes, H);
	bool asciiNee = ASCII::IsAllASCII(neeBytes, N);

	// ====================================
	// FAST-PATH 1: Ordinal ASCII
	// ====================================
	if (!ignoreCase && asciiHay && asciiNee)
		return ASCII::EndsWith(hayBytes + start, N, neeBytes, N);

	// ====================================
	// FAST-PATH 2: ignoreCase ASCII (não-Turkic)
	// ====================================
	if (ignoreCase && !isTurkic && asciiHay && asciiNee)
		return ASCII::EndsWithIgnoreCase(hayBytes + start, N, neeBytes, N);

	// ====================================
	// SLOW-PATH: Unicode Casefold + Normalize (Full Unicode Standard)
	// ====================================
	List<CodePoint> hayFold, neeFold;
	UTF8::FoldAndNormalize(*this, GetByteCount(), loc, hayFold);
	UTF8::FoldAndNormalize(needle, needle.GetByteCount(), loc, neeFold);

	if (ignoreCase)
		return UTF8::EndsWithIgnoreCase(hayFold, neeFold, loc);
	else
		return UTF8::EndsWith(hayFold, neeFold);
}

Boolean String::StartsWithInternal(const String& needle, bool ignoreCase, const char* locale) const noexcept
{
	if (needle.IsEmpty())
		return true;

	const Char* hayBytes = data();
	const Char* neeBytes = needle.data();
	uint32_t H = _byteLength;
	uint32_t N = needle._byteLength;

	if (H < N)
		return false;

	// Locale detection
	const char* loc = (locale ? locale : "en");
	uint32_t locLen = (uint32_t)strlen(loc);
	bool isTurkic = UnicodeCase::locale_is_turkic(loc, locLen);

	bool asciiHay = ASCII::IsAllASCII(hayBytes, H);
	bool asciiNee = ASCII::IsAllASCII(neeBytes, N);

	// ====================================
	// FAST-PATH 1: Ordinal ASCII
	// ====================================
	if (!ignoreCase && asciiHay && asciiNee)
		return ASCII::StartsWith(hayBytes, N, neeBytes, N);

	// ====================================
	// FAST-PATH 2: ignoreCase ASCII (não-Turkic)
	// ====================================
	if (ignoreCase && !isTurkic && asciiHay && asciiNee)
		return ASCII::StartsWithIgnoreCase(hayBytes, N, neeBytes, N);

	// ====================================
	// SLOW-PATH: Unicode Casefold + Normalize (Full Unicode Standard)
	// ====================================
	List<CodePoint> hayFold, neeFold;
	UTF8::FoldAndNormalize(*this, GetByteCount(), loc, hayFold);
	UTF8::FoldAndNormalize(needle, needle.GetByteCount(), loc, neeFold);

	if (ignoreCase)
		return UTF8::StartsWithIgnoreCase(hayFold, neeFold, loc);
	else		   
		return UTF8::StartsWith(hayFold, neeFold);
}

List<String> String::SplitImpl(
	const List<String>& stringSeps,
	const List<Char>& charSeps,
	int maxCount,
	StringSplitOptions options
) const
{
	List<String> result;

	const bool useString = !stringSeps.IsEmpty();
	const bool useChars = !charSeps.IsEmpty();

	if (!useString && !useChars)
	{
		result.Add(*this);
		return result;
	}

	const int limit = (maxCount <= 0 ? Int32::MaxValue() : (Int32)maxCount);

	const Char* bytes = this->data();
	uint32_t H = this->GetByteCount();
	uint32_t pos = 0;

	while (pos < H && (int)result.Count() < (limit - 1))
	{
		uint32_t cutPos = UINT32_MAX;
		uint32_t cutLen = 0;

		// ---------------------------------------------------
		// 1) Separadores de string (byte-based)
		// ---------------------------------------------------
		if (useString)
		{
			for (const auto& sep : stringSeps)
			{
				uint32_t N = sep.GetByteCount();
				if (N == 0) continue;

				// IndexOf retorna byte offset → perfeito
				Int64 idx = this->IndexOf(sep, pos);
				if (idx >= 0)
				{
					uint32_t p = (uint32_t)idx;
					if (p < cutPos || (p == cutPos && N > cutLen))
					{
						cutPos = p;
						cutLen = N;
					}
				}
			}
		}

		// ---------------------------------------------------
		// 2) Separadores de Char (byte-based)
		// ---------------------------------------------------
		if (useChars)
		{
			for (uint32_t i = pos; i < H; ++i)
			{
				unsigned char b = (unsigned char)(Char)bytes[i];
				if (charSeps.Contains(Char(b)))
				{
					if (i < cutPos)
					{
						cutPos = i;
						cutLen = 1;
					}
					break;
				}
			}
		}

		// Nenhum separador encontrado → fim
		if (cutPos == UInt32::MaxValue())
			break;

		// Extrai token (byte-based)
		uint32_t tokLen = cutPos - pos;
		String token = this->SubstringByBytes(pos, tokLen);

		if (HasFlag(options, StringSplitOptions::TrimEntries))
			token = token.Trim();

		if (!HasFlag(options, StringSplitOptions::RemoveEmptyEntries) || !token.IsEmpty())
			result.Add(token);

		pos = cutPos + cutLen;
	}

	// Último token
	if (pos <= H)
	{
		uint32_t tokLen = H - pos;
		String token = this->SubstringByBytes(pos, tokLen);

		if (HasFlag(options, StringSplitOptions::TrimEntries))
			token = token.Trim();

		if (!HasFlag(options, StringSplitOptions::RemoveEmptyEntries) || !token.IsEmpty())
			result.Add(token);
	}

	return result;
}

void String::RemoveCombiningDotAbove(String& s)
{
	List<CodePoint> cps = String::DecodeToCodePoints(s);
	for (int i = (int)cps.Count() - 1; i >= 0; --i)
		if (cps[i] == 0x0307) // COMBINING DOT ABOVE
			cps.RemoveAt(i);
	s = String::FromCodePoints(cps);
}

String String::SubstringByBytes(uint32_t byteStart, uint32_t byteLen) const noexcept
{
	if (byteStart >= _byteLength || byteLen == 0)
		return String::Empty();

	if (byteStart + byteLen > _byteLength)
		byteLen = _byteLength - byteStart;

	if (IsSSO())
	{
		// COPY — nunca slice em SSO
		return String(reinterpret_cast<const unsigned char*>(data() + byteStart), byteLen);
	}

	String sub(*this);
	sub._byteOffset += byteStart;
	sub._byteLength = byteLen;

	sub.InvalidateCache();
	sub._gcLength = UInt32::MaxValue();
	sub.ReleaseNormalizationCache();

	return sub;
}