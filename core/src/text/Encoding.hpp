#pragma once

#include "types/Char.hpp"
#include "types/CodePoint.hpp"
#include "types/UInt32.hpp"
#include "collections/List.hpp"
#include "ASCII.hpp"
#include "UTF8.hpp"

static constexpr char HEX[17] = "0123456789ABCDEF";

enum class Encoding
{
	ASCII,
	UTF8,
	UTF16,
	UTF32
};

namespace ASCIIEncoding
{
	static inline bool IsAllASCII(const Char* bytes, uint32_t len) noexcept
	{
		return ::ASCII::IsAllASCII(bytes, len);
	}

	static inline int Compare(const Char* a, uint32_t lenA, const Char* b, uint32_t lenB) noexcept
	{
		return ::ASCII::Compare(a, lenA, b, lenB);
	}

	static inline int CompareIgnoreCase(const Char* a, uint32_t lenA, const Char* b, uint32_t lenB) noexcept
	{
		return ::ASCII::CompareIgnoreCase(a, lenA, b, lenB);
	}

	static inline const Char* Find(const Char* haystack, uint32_t hlen, const Char* needle, uint32_t nlen) noexcept
	{
		return ::ASCII::Find(haystack, hlen, needle, nlen);
	}

	static inline uint32_t CountOccurrences(const Char* h, uint32_t hl, const Char* n, uint32_t nl) noexcept
	{
		return ::ASCII::CountOccurrences(h, hl, n, nl);
	}

	static inline uint32_t CountOccurrencesIgnoreCase(const Char* hay, uint32_t H, const Char* ned, uint32_t N) noexcept
	{
		return ::ASCII::CountOccurrencesIgnoreCase(hay, H, ned, N);
	}

	static inline void ReplaceOrdinal(const Char* src, uint32_t srcLen, const Char* oldv, uint32_t oldLen, const Char* newv, uint32_t newLen, Char* dst) noexcept
	{
		::ASCII::ReplaceOrdinal(src, srcLen, oldv, oldLen, newv, newLen, dst);
	}

	static inline void ReplaceIgnoreCase(const Char* src, uint32_t H, const Char* oldv, uint32_t O, const Char* newv, uint32_t N, Char* dst) noexcept
	{
		::ASCII::ReplaceIgnoreCase(src, H, oldv, O, newv, N, dst);
	}

	static inline Char ToLower(Char c) noexcept
	{
		return ::ASCII::ToLower(c);
	}

	static inline Char ToUpper(Char c) noexcept
	{
		return ::ASCII::ToUpper(c);
	}

	static inline bool Equals(const Char* a, const Char* b, uint32_t len) noexcept
	{
		return ::ASCII::Equals(a, b, len);
	}

	static inline bool EqualsIgnoreCase(const Char* a, const Char* b, uint32_t len) noexcept
	{
		return ::ASCII::EqualsIgnoreCase(a, b, len);
	}

	static inline const Char* FindIgnoreCase(const Char* hay, uint32_t H, const Char* ned, uint32_t N) noexcept
	{
		return ::ASCII::FindIgnoreCase(hay, H, ned, N);
	}

	static inline int64_t IndexOf(const Char* hay, uint32_t H, const Char* ned, uint32_t N, uint32_t startIndex) noexcept
	{
		return ::ASCII::IndexOf(hay, H, ned, N, startIndex);
	}

	static inline int64_t LastIndexOf(const Char* hay, uint32_t H, const Char* ned, uint32_t N, uint32_t startIndex) noexcept
	{
		return ::ASCII::LastIndexOf(hay, H, ned, N, startIndex);
	}

	static inline bool StartsWith(const Char* hay, uint32_t H, const Char* nee, uint32_t N) noexcept
	{
		return ::ASCII::StartsWith(hay, H, nee, N);
	}

	static inline bool StartsWithIgnoreCase(const Char* hay, uint32_t H, const Char* nee, uint32_t N) noexcept
	{
		return ::ASCII::StartsWithIgnoreCase(hay, H, nee, N);
	}

	static inline bool EndsWith(const Char* hay, uint32_t H, const Char* nee, uint32_t N) noexcept
	{
		return ::ASCII::EndsWith(hay, H, nee, N);
	}

	static inline bool EndsWithIgnoreCase(const Char* hay, uint32_t H, const Char* nee, uint32_t N) noexcept
	{
		return ::ASCII::EndsWithIgnoreCase(hay, H, nee, N);
	}
};

struct UTF8Encoding
{
	// bytes → codepoints
	static void Decode(const Char* bytes, uint32_t byteCount, List<CodePoint>& outCodePoints) noexcept
	{
		using namespace UTF8;

		outCodePoints.Clear();

		if (!bytes || byteCount == 0)
			return;

		::UTF8::Decode(bytes, byteCount, outCodePoints);
	}

	// codepoints → bytes
	static uint32_t Encode(const List<CodePoint>& codePoints, Char* outBytes) noexcept
	{
		using namespace UTF8;

		if (codePoints.Count() == 0)
			return 0;

		Char* dst = reinterpret_cast<Char*>(outBytes);
		return ::UTF8::Encode(codePoints, dst);
	}

	static List<Char> Encode(const List<CodePoint>& codePoints)
	{
		using namespace UTF8;

		List<Char> out;

		if (codePoints.Count() == 0)
			return out;

		uint32_t totalBytes = 0;
		for (CodePoint cp : codePoints)
			totalBytes += cp.ByteCount();

		out.Reserve(totalBytes);

		for (CodePoint cp : codePoints)
		{
			auto enc = ::UTF8::encode_utf8(cp);
			for (uint32_t i = 0; i < enc.Length; ++i)
				out.Add(enc.Bytes[i]);
		}

		return out;
	}

	static uint32_t Encode(CodePoint cp, Char* outBytes) noexcept
	{
		auto enc = ::UTF8::encode_utf8(cp);

		for (uint32_t i = 0; i < enc.Length; ++i)
			outBytes[i] = enc.Bytes[i];

		return enc.Length;
	}

	static List<Char> Encode(CodePoint cp)
	{
		List<Char> out;
		out.Reserve(4);

		auto enc = ::UTF8::encode_utf8(cp);

		for (uint32_t i = 0; i < enc.Length; ++i)
			out.Add(enc.Bytes[i]);

		return out;
	}

	static inline List<Byte> GetBytes(const Char* bytes, uint32_t byteCount)
	{
		List<Byte> out;
		out.EnsureCapacity(byteCount);

		for (uint32_t i = 0; i < byteCount; ++i)
			out.Add(bytes[i]);

		return out;
	}

	// Quantos chars hex serão gerados
	static uint32_t ToHexLength(uint32_t byteCount) noexcept
	{
		return byteCount == 0 ? 0 : (byteCount * 3 - 1);
	}

	// Escreve HEX ASCII no buffer fornecido
	static void ToHex(const unsigned char* bytes, uint32_t byteCount, Char* outChars) noexcept
	{
		uint32_t w = 0;
		for (uint32_t i = 0; i < byteCount; ++i)
		{
			unsigned char b = bytes[i];

			outChars[w++] = Char(HEX[b >> 4]);
			outChars[w++] = Char(HEX[b & 0x0F]);

			if (i + 1 < byteCount)
				outChars[w++] = Char(' ');
		}
	}
};

struct UTF16Encoding
{
	static constexpr bool IsHighSurrogate(char16_t c) noexcept {
		return c >= 0xD800 && c <= 0xDBFF;
	}

	static constexpr bool IsLowSurrogate(char16_t c) noexcept {
		return c >= 0xDC00 && c <= 0xDFFF;
	}

	static constexpr bool IsSurrogate(char16_t c) noexcept {
		return c >= 0xD800 && c <= 0xDFFF;
	}

	static void Decode(const char16_t* units, uint32_t unitCount, List<CodePoint>& outCodePoints) noexcept
	{
		outCodePoints.Clear();

		if (!units || unitCount == 0)
			return;

		for (uint32_t i = 0; i < unitCount; ++i)
		{
			uint32_t u = units[i];

			if (IsHighSurrogate(u) && i + 1 < unitCount)
			{
				uint32_t lo = units[i + 1];
				if (IsLowSurrogate(u))
				{
					uint32_t cp =
						0x10000 +
						((u - 0xD800) << 10) +
						(lo - 0xDC00);

					outCodePoints.Add(CodePoint(cp));
					++i;
					continue;
				}
			}

			outCodePoints.Add(CodePoint(u));
		}
	}

	static uint32_t Encode(const List<CodePoint>& codePoints, char16_t* outUnits) noexcept
	{
		uint32_t w = 0;

		for (CodePoint cp : codePoints)
		{
			uint32_t v = (uint32_t)cp;

			if (v <= 0xFFFF)
			{
				outUnits[w++] = (char16_t)v;
			}
			else
			{
				v -= 0x10000;
				outUnits[w++] = (char16_t)(0xD800 + (v >> 10));
				outUnits[w++] = (char16_t)(0xDC00 + (v & 0x3FF));
			}
		}

		return w;
	}

	static List<char16_t> Encode(const List<CodePoint>& codePoints)
	{
		List<char16_t> out;
		out.Reserve(codePoints.Count() * 2);

		for (CodePoint cp : codePoints)
		{
			uint32_t v = (uint32_t)cp;

			if (v <= 0xFFFF)
			{
				out.Add((char16_t)v);
			}
			else
			{
				v -= 0x10000;
				out.Add((char16_t)(0xD800 + (v >> 10)));
				out.Add((char16_t)(0xDC00 + (v & 0x3FF)));
			}
		}

		return out;
	}

	static inline uint32_t ToHexLength(uint32_t unitCount) noexcept
	{
		// "XXXX " → 5 chars por unidade, menos o último espaço
		return unitCount == 0 ? 0 : (unitCount * 5 - 1);
	}

	static void ToHex(const char16_t* units, uint32_t unitCount, Char* outChars) noexcept
	{
		uint32_t w = 0;

		for (uint32_t i = 0; i < unitCount; ++i)
		{
			uint16_t v = static_cast<uint16_t>(units[i]);

			outChars[w++] = Char(HEX[(v >> 12) & 0xF]);
			outChars[w++] = Char(HEX[(v >> 8) & 0xF]);
			outChars[w++] = Char(HEX[(v >> 4) & 0xF]);
			outChars[w++] = Char(HEX[v & 0xF]);

			if (i + 1 < unitCount)
				outChars[w++] = Char(' ');
		}
	}
};

struct UTF32Encoding
{
	static void Decode(const char32_t* units, uint32_t unitCount, List<CodePoint>& outCodePoints) noexcept
	{
		outCodePoints.Clear();

		if (!units || unitCount == 0)
			return;

		for (uint32_t i = 0; i < unitCount; ++i)
			outCodePoints.Add(CodePoint(units[i]));
	}

	static uint32_t Encode(const List<CodePoint>& codePoints, char32_t* outUnits) noexcept
	{
		uint32_t w = 0;

		for (CodePoint cp : codePoints)
			outUnits[w++] = (char32_t)(uint32_t)cp;

		return w;
	}

	static uint32_t ToHexLength(uint32_t unitCount) noexcept
	{
		return unitCount == 0 ? 0 : (unitCount * 9 - 1); // "XXXXXXXX "
	}

	static void ToHex(const char32_t* units, uint32_t unitCount, Char* outChars) noexcept
	{
		uint32_t w = 0;
		for (uint32_t i = 0; i < unitCount; ++i)
		{
			uint32_t v = units[i];

			for (int s = 28; s >= 0; s -= 4)
				outChars[w++] = Char(HEX[(v >> s) & 0xF]);

			if (i + 1 < unitCount)
				outChars[w++] = Char(' ');
		}
	}
};