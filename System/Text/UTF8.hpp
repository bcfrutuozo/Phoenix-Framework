#pragma once

#include <cstdlib>
#include <cstring>

#include "System/Types.hpp"
#include "System/Collections/List.hpp"
#include "unicode/UnicodeNormalization_utils.hpp"
#include "unicode/UnicodeCase_utils.hpp"

namespace UTF8
{
	struct UTF8EncodeResult
	{
		Char Bytes[4];
		UInt64 Length;
	};

	struct UTF8DecodeResult
	{
		CodePoint cp;
		UInt64 Length;
		Boolean IsValid;
	};

	// CodePoint -> UTF-8
	constexpr UTF8EncodeResult encode_utf8(CodePoint cp) noexcept {
		UTF8EncodeResult result{ {Char(), Char(), Char(), Char()}, 0 };

		if (!cp.IsValid()) {
			// invalid codepoint -> zero length, caller must check
			result.Length = 0;
			return result;
		}

		uint32_t v = static_cast<uint32_t>(cp);

		if (v <= 0x7F) {
			result.Bytes[0] = Char(static_cast<uint8_t>(v));
			result.Length = 1;
		}
		else if (v <= 0x7FF) {
			result.Bytes[0] = Char(static_cast<uint8_t>(0xC0 | (v >> 6)));
			result.Bytes[1] = Char(static_cast<uint8_t>(0x80 | (v & 0x3F)));
			result.Length = 2;
		}
		else if (v <= 0xFFFF) {
			result.Bytes[0] = Char(static_cast<uint8_t>(0xE0 | (v >> 12)));
			result.Bytes[1] = Char(static_cast<uint8_t>(0x80 | ((v >> 6) & 0x3F)));
			result.Bytes[2] = Char(static_cast<uint8_t>(0x80 | (v & 0x3F)));
			result.Length = 3;
		}
		else {
			// até 0x10FFFF
			result.Bytes[0] = Char(static_cast<uint8_t>(0xF0 | (v >> 18)));
			result.Bytes[1] = Char(static_cast<uint8_t>(0x80 | ((v >> 12) & 0x3F)));
			result.Bytes[2] = Char(static_cast<uint8_t>(0x80 | ((v >> 6) & 0x3F)));
			result.Bytes[3] = Char(static_cast<uint8_t>(0x80 | (v & 0x3F)));
			result.Length = 4;
		}

		return result;
	}

	// UTF-8 bytes -> CodePoint
	constexpr UTF8DecodeResult decode_utf8(const Char* bytes, size_t maxLen) noexcept
	{
		UTF8DecodeResult r{ CodePoint(), 0, false };

		if (maxLen == 0) {
			return r;
		}

		uint8_t b0 = static_cast<uint8_t>(bytes[0]);

		// ASCII
		if ((b0 & 0x80) == 0x00) {
			r.cp = CodePoint(static_cast<char32_t>(b0));
			r.Length = 1;
			r.IsValid = true;
			return r;
		}

		// determinando o tamanho esperado
		size_t expected = 0;
		uint32_t cp = 0;

		if ((b0 & 0xE0) == 0xC0) {
			expected = 2;
			cp = (b0 & 0x1F);
		}
		else if ((b0 & 0xF0) == 0xE0) {
			expected = 3;
			cp = (b0 & 0x0F);
		}
		else if ((b0 & 0xF8) == 0xF0) {
			expected = 4;
			cp = (b0 & 0x07);
		}
		else {
			return r; // inválido
		}

		if (maxLen < expected) {
			return r; // não há bytes suficientes
		}

		// lê continuations
		for (size_t i = 1; i < expected; ++i) {
			uint8_t bx = static_cast<uint8_t>(bytes[i]);
			if ((bx & 0xC0) != 0x80) {
				return r; // byte de continuação inválido
			}
			cp = (cp << 6) | (bx & 0x3F);
		}

		CodePoint tmp(static_cast<char32_t>(cp));
		if (!tmp.IsValid()) {
			return r; // codepoint inválido como scalar Unicode
		}

		r.cp = tmp;
		r.Length = expected;
		r.IsValid = true;
		return r;
	}

	// ---------------------------------------------
	// 1) Decode UTF-8 → List<CodePoint>
	// ---------------------------------------------
	inline void Decode(const Char* bytes, uint32_t len, List<CodePoint>& out)
	{
		out.Clear();
		if (!bytes || len == 0)
			return;

		out.EnsureCapacity(len); // worst-case: 1 byte → 1 CP

		uint32_t pos = 0;
		while (pos < len)
		{
			auto r = UTF8::decode_utf8(bytes + pos, len - pos);

			if (!r.IsValid || r.Length == 0)
			{
				out.Add(CodePoint(0xFFFD)); // replacement char
				pos += 1;
				continue;
			}

			out.Add(r.cp);
			pos += (uint32_t)r.Length;
		}
	}

	// ---------------------------------------------
	// 2) Encode List<CodePoint> → UTF-8 bytes
	// returns: total byte length
	// ---------------------------------------------
	inline uint32_t Encode(const List<CodePoint>& cps, Char* out)
	{
		uint32_t w = 0;

		for (uint32_t i = 0; i < cps.Count(); ++i)
		{
			UTF8::UTF8EncodeResult enc = UTF8::encode_utf8(cps[i]);
			for (uint32_t k = 0; k < enc.Length; ++k)
				out[w++] = enc.Bytes[k];
		}

		return w;
	}

	// ---------------------------------------------
	// 3) Casefold — wrapper around your UnicodeCase
	// ---------------------------------------------
	inline void CaseFold(const List<CodePoint>& in, const char* locale, uint32_t localeLen, List<CodePoint>& out)
	{
		out.Clear();
		out.EnsureCapacity(in.Count() * 4);

		uint32_t cpCount = in.Count();

		for (uint32_t i = 0; i < cpCount; ++i)
		{
			List<CodePoint> seq;
			seq.EnsureCapacity(4);

			UnicodeCase::map_to_casefold_sequence_nostd(
				in[i],
				locale,
				localeLen,
				in,
				i,
				seq
			);

			for (uint32_t k = 0; k < seq.Count(); ++k)
				out.Add(seq[k]);
		}

		// Turkish rule:
		bool isTurkic = UnicodeCase::locale_is_turkic(locale, localeLen);
		if (isTurkic)
		{
			for (int i = (int)out.Count() - 1; i >= 0; --i)
				if ((uint32_t)out[i] == 0x0307)
					out.RemoveAt(i);
		}
	}

	// ---------------------------------------------
	// 4) Normalize (NFC/NFD/NFKC/NFKD)
	// ---------------------------------------------
	inline void Normalize(const List<CodePoint>& in, ::NormalizationForm form, List<CodePoint>& out)
	{
		out.Clear();

		if (in.Count() == 0)
			return;

		// 1) Decompose
		bool compat = (form == ::NormalizationForm::NFKC || form == ::NormalizationForm::NFKD);

		List<CodePoint> temp;
		temp.EnsureCapacity(in.Count() * 3);

		for (uint32_t i = 0; i < in.Count(); ++i)
		{
			CodePoint local[32];
			uint32_t localLen = 0;

			UnicodeNormalization::DecomposeCodePoint(in[i], compat, local, localLen);

			for (uint32_t k = 0; k < localLen; ++k)
				temp.Add(local[k]);
		}

		// 2) Reorder by CCC
		UnicodeNormalization::ReorderByCCC(temp);

		// 3) Compose if NFC/NFKC
		if (form == ::NormalizationForm::NFC || form == ::NormalizationForm::NFKC)
		{
			UnicodeNormalization::Compose(temp);
		}

		// final result
		out = temp;
	}

	// ---------------------------------------------
	// 5) FoldAndNormalize(String s, locale)
	// Combines: Decode → CaseFold → Normalize → Decode again
	// ---------------------------------------------
	inline void FoldAndNormalize(const Char* bytes, uint32_t len, const char* locale,
		List<CodePoint>& out)
	{
		List<CodePoint> cps;
		List<CodePoint> folded;

		// decode
		Decode(bytes, len, cps);

		// casefold
		uint32_t localeLen = (uint32_t)strlen(locale);
		CaseFold(cps, locale, localeLen, folded);

		// normalize NFC
		Normalize(folded, NormalizationForm::NFC, out);
	}

	// ---------------------------------------------
	// 6) Compute grapheme boundaries (wrapper)
	// ---------------------------------------------
	inline void ComputeGraphemeBoundaries(const List<CodePoint>& cps,
		List<CodePoint>& boundaries)
	{
		boundaries.Clear();
		uint32_t gc = 0;
		UnicodeCase::ComputeGraphemeBoundaries(
			cps,
			cps.Count(),
			boundaries,
			gc
		);
	}

	// ---------------------------------------------
	// 7) Compare Unicode (ordinal)
	// ---------------------------------------------
	inline int Compare(const List<CodePoint>& a, const List<CodePoint>& b)
	{
		uint32_t ma = a.Count();
		uint32_t mb = b.Count();
		uint32_t m = (ma < mb ? ma : mb);

		for (uint32_t i = 0; i < m; ++i)
		{
			if (a[i] < b[i]) return -1;
			if (a[i] > b[i]) return 1;
		}

		if (ma < mb) return -1;
		if (ma > mb) return 1;

		return 0;
	}

	// ---------------------------------------------
	// 7b) Compare Unicode-ignore-case
	// ---------------------------------------------
	inline int CompareIgnoreCase(const List<CodePoint>& a, const List<CodePoint>& b, const char* locale)
	{
		List<CodePoint> fa, fb;
		CaseFold(a, locale, (uint32_t)strlen(locale), fa);
		CaseFold(b, locale, (uint32_t)strlen(locale), fb);

		Normalize(fa, ::NormalizationForm::NFC, fa);
		Normalize(fb, ::NormalizationForm::NFC, fb);

		return Compare(fa, fb);
	}

	inline bool Contains(const List<CodePoint>& hay, const List<CodePoint>& ned)
	{
		uint32_t H = hay.Count();
		uint32_t N = ned.Count();

		if (N == 0)
			return true;
		if (N > H)
			return false;

		uint32_t limit = H - N;
		for (uint32_t i = 0; i <= limit; ++i)
		{
			bool match = true;
			for (uint32_t j = 0; j < N; ++j)
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

	// ---------------------------------------------
	// 8) StartsWith / EndsWith
	// ---------------------------------------------
	inline bool StartsWith(const List<CodePoint>& hay, const List<CodePoint>& nee)
	{
		if (nee.Count() > hay.Count())
			return false;

		for (uint32_t i = 0; i < nee.Count(); ++i)
			if (hay[i] != nee[i])
				return false;

		return true;
	}

	inline bool EndsWith(const List<CodePoint>& hay, const List<CodePoint>& nee)
	{
		uint32_t H = hay.Count();
		uint32_t N = nee.Count();

		if (H < N) return false;

		uint32_t start = H - N;
		for (uint32_t i = 0; i < N; ++i)
			if (hay[start + i] != nee[i])
				return false;

		return true;
	}

	inline bool StartsWithIgnoreCase(const List<CodePoint>& hay, const List<CodePoint>& nee, const char* locale)
	{
		List<CodePoint> fh, fn;

		CaseFold(hay, locale, (uint32_t)strlen(locale), fh);
		CaseFold(nee, locale, (uint32_t)strlen(locale), fn);

		Normalize(fh, ::NormalizationForm::NFC, fh);
		Normalize(fn, ::NormalizationForm::NFC, fn);

		return StartsWith(fh, fn);
	}

	inline bool EndsWithIgnoreCase(const List<CodePoint>& hay, const List<CodePoint>& nee, const char* locale)
	{
		List<CodePoint> fh, fn;

		CaseFold(hay, locale, (uint32_t)strlen(locale), fh);
		CaseFold(nee, locale, (uint32_t)strlen(locale), fn);

		Normalize(fh, ::NormalizationForm::NFC, fh);
		Normalize(fn, ::NormalizationForm::NFC, fn);

		return EndsWith(fh, fn);
	}

	// ---------------------------------------------
	// 9) Unicode IndexOf / LastIndexOf
	// ---------------------------------------------
	inline int64_t IndexOf(const List<CodePoint>& hay, const List<CodePoint>& ned, uint32_t start)
	{
		uint32_t H = hay.Count();
		uint32_t N = ned.Count();

		if (N == 0) return start;
		if (start >= H) return -1;
		if (N > H) return -1;

		uint32_t limit = H - N;
		for (uint32_t i = start; i <= limit; ++i)
		{
			bool match = true;
			for (uint32_t j = 0; j < N; ++j)
			{
				if (hay[i + j] != ned[j])
				{
					match = false;
					break;
				}
			}
			if (match) return (int64_t)i;
		}

		return -1;
	}

	inline int64_t LastIndexOf(const List<CodePoint>& hay, const List<CodePoint>& ned, uint32_t start)
	{
		uint32_t H = hay.Count();
		uint32_t N = ned.Count();

		if (N == 0)
			return (start < H ? start : (H == 0 ? 0 : H - 1));

		if (H == 0 || N > H)
			return -1;

		if (start >= H)
			start = H - 1;

		for (int64_t i = (int64_t)start; i >= 0; --i)
		{
			if ((uint64_t)i + N > H)
				continue;

			bool match = true;
			for (uint32_t j = 0; j < N; ++j)
			{
				if (hay[i + j] != ned[j])
				{
					match = false;
					break;
				}
			}

			if (match)
				return i;
		}

		return -1;
	}

	// ---------------------------------------------
	// 10) Replace (Unicode-only)
	// ---------------------------------------------
	inline void Replace(const List<CodePoint>& hay, const List<CodePoint>& oldv, const List<CodePoint>& newv, List<CodePoint>& out)
	{
		out.Clear();

		uint32_t H = hay.Count();
		uint32_t O = oldv.Count();
		uint32_t N = newv.Count();

		if (O == 0)
		{
			out = hay;
			return;
		}

		for (uint32_t i = 0; i < H; )
		{
			bool match = true;

			if (i + O <= H)
			{
				for (uint32_t j = 0; j < O; ++j)
				{
					if (hay[i + j] != oldv[j])
					{
						match = false;
						break;
					}
				}
			}
			else match = false;

			if (match)
			{
				for (uint32_t k = 0; k < N; ++k)
					out.Add(newv[k]);

				i += O;
			}
			else
			{
				out.Add(hay[i]);
				i++;
			}
		}
	}
}