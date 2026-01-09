#include "UnicodeCase_utils.hpp"

namespace UnicodeCase
{
	bool locale_is_turkic(const char* locale, uint32_t len) noexcept
	{
		if (!locale || len < 2)
			return false;

		// Verifica prefixo "tr"
		if (locale[0] == 't' && locale[1] == 'r')
			return true;

		// Verifica prefixo "az"
		if (locale[0] == 'a' && locale[1] == 'z')
			return true;

		return false;
	}

	bool apply_final_sigma_context(const List<CodePoint>& cps, uint32_t index) noexcept
	{
		if (cps.Count() == 0)
			return false;

		// =======================
		// 1. BACKWARD: existe uma letra "cased" antes?
		// =======================
		bool foundBefore = false;

		if (index > 0) {
			for (uint32_t j = index; j-- > 0; ) {
				CodePoint cp = cps[j];

				if (is_cased(cp)) {
					foundBefore = true;
					break;
				}
			}
		}

		// =======================
		// 2. FORWARD: existe uma letra depois?
		// =======================
		bool foundAfter = false;

		for (uint32_t j = index + 1; j < cps.Count(); ++j) {
			CodePoint cp = cps[j];

			if (is_letter(cp)) {
				foundAfter = true;
				break;
			}
		}

		// =======================
		// 3. Regra do Unicode:
		//
		// Final Sigma é usado quando:
		//      NÃO (foundBefore && foundAfter)
		//
		// Ou seja:
		//  - há letra cased antes
		//  - NÃO há letra depois
		//
		// =======================
		return !(foundBefore && foundAfter);
	}

	void map_to_upper_sequence_nostd(
		CodePoint cp,
		const char* localeBytes,
		uint32_t localeLen,
		List<CodePoint>& output
	) noexcept
	{
		output.Clear();

		bool isTurkic = UnicodeCase::locale_is_turkic(localeBytes, localeLen);

		for (const auto& e : SPECIAL_ENTRIES)
		{
			if (e.cp != (uint32_t)cp) continue;

			bool use = false;

			if (e.condition[0] == '\0')
				use = true;
			else if ((e.condition[0] == 't' && e.condition[1] == 'r') ||
				(e.condition[0] == 'a' && e.condition[1] == 'z'))
				use = isTurkic;

			if (use)
			{
				output.EnsureCapacity(e.upper_len);
				for (uint32_t k = 0; k < e.upper_len; ++k)
					if (e.upper[k] != 0xFFFFFFFF)
						output.Add(CodePoint(e.upper[k]));
				return;
			}
		}

		output.Add(CodePoint(to_upper_simple((uint32_t)cp)));
	}

	void map_to_lower_sequence_nostd(
		CodePoint cp,
		const char* localeBytes,
		uint32_t localeLen,
		const List<CodePoint>& context,
		uint32_t index,
		List<CodePoint>& output) noexcept
	{
		output.Clear();

		bool isTurkic = UnicodeCase::locale_is_turkic(localeBytes, localeLen);

		// SPECIAL ENTRIES CHECK
		for (const auto& e : SPECIAL_ENTRIES)
		{
			if (e.cp != (uint32_t)cp) continue;

			bool use = false;

			if (e.condition[0] == '\0')
				use = true;
			else if ((e.condition[0] == 't' && e.condition[1] == 'r') ||
				(e.condition[0] == 'a' && e.condition[1] == 'z'))
				use = isTurkic;
			else if (e.condition[0] == 'F')
				use = apply_final_sigma_context(context, index);

			if (use)
			{
				output.EnsureCapacity(e.lower_len);
				for (uint32_t k = 0; k < e.lower_len; ++k)
					if (e.lower[k] != 0xFFFFFFFF)
						output.Add(CodePoint(e.lower[k]));
				return;
			}
		}

		// fallback lower
		output.Add(CodePoint(to_lower_simple((uint32_t)cp)));
	}

	void map_to_casefold_sequence_nostd(
		CodePoint cp,
		const char* localeBytes,
		uint32_t localeLen,
		const List<CodePoint>& context,
		uint32_t index,
		List<CodePoint>& output
	) noexcept
	{
		output.Clear();

		// ---------- TURKIC CHECK (case-insensitive for "tr" and "az") ----------
		bool isTurkic = false;
		if (localeLen >= 2 && localeBytes != nullptr) {
			char c0 = localeBytes[0];
			char c1 = localeBytes[1];
			// normalize simple ASCII letters to lower-case without locale calls
			if (c0 >= 'A' && c0 <= 'Z') c0 = (char)(c0 - 'A' + 'a');
			if (c1 >= 'A' && c1 <= 'Z') c1 = (char)(c1 - 'A' + 'a');

			if ((c0 == 't' && c1 == 'r') || (c0 == 'a' && c1 == 'z'))
				isTurkic = true;
		}

		// Special Turkish/Turkic rules:
		if (isTurkic)
		{
			if ((uint32_t)cp == 0x0049u)   // 'I'
			{
				output.EnsureCapacity(1);
				output.Add(CodePoint(0x0131u)); // ı
				return;
			}
			else if ((uint32_t)cp == 0x0130u) // İ
			{
				output.EnsureCapacity(2);
				output.Add(CodePoint(0x0069u)); // 'i'
				output.Add(CodePoint(0x0307u)); // combining dot above
				return;
			}
		}

		// ---------- FINAL SIGMA ----------
		if ((uint32_t)cp == 0x03A3u || (uint32_t)cp == 0x03C3u || (uint32_t)cp == 0x03C2u) // Sigma family
		{
			bool finalSigma = apply_final_sigma_context(context, index);

			if (finalSigma)
			{
				output.EnsureCapacity(1);
				output.Add(CodePoint(0x03C2u)); // final sigma
				return;
			}
			else
			{
				output.EnsureCapacity(1);
				output.Add(CodePoint(0x03C3u)); // normal sigma
				return;
			}
		}

		// ---------- CASE FOLD TABLE ----------
		// small binary search over CASEFOLD_TABLE (entries use uint32_t)
		uint32_t low = 0, high = CASEFOLD_TABLE_COUNT;
		uint32_t key = (uint32_t)cp;

		while (low < high)
		{
			uint32_t mid = (low + high) >> 1;
			uint32_t entryCp = CASEFOLD_TABLE[mid].cp;
			if (entryCp == key)
			{
				const CFEntry& e = CASEFOLD_TABLE[mid];
				output.EnsureCapacity(e.len);
				for (uint32_t i = 0; i < e.len; ++i)
				{
					// skip sentinel values if any (your table used 0xFFFFFFFF)
					if (e.seq[i] != 0xFFFFFFFFu)
						output.Add(CodePoint(e.seq[i]));
				}
				return;
			}
			if (entryCp < key)
				low = mid + 1;
			else
				high = mid;
		}

		// ---------- FALLBACK: simple lower-case ----------
		output.EnsureCapacity(1);
		output.Add(CodePoint(to_lower_simple((uint32_t)cp)));
	}

	bool IsGraphemeBreak(CodePoint prev, CodePoint next,
		const List<CodePoint>& cps, uint32_t len, uint32_t index) noexcept
	{
		GBP a = GetGBP(prev);
		GBP b = GetGBP(next);

		// GB3: CR × LF
		if (a == GBP::CR && b == GBP::LF)
			return false;

		// GB4 / GB5: break before or after Control / CR / LF
		if (a == GBP::Control || a == GBP::CR || a == GBP::LF) return true;
		if (b == GBP::Control || b == GBP::CR || b == GBP::LF) return true;

		// GB9: × Extend
		if (b == GBP::Extend)
			return false;

		// GB9a: × ZWJ
		if (b == GBP::ZWJ)
			return false;

		// GB9b: Prepend ×
		if (a == GBP::Prepend)
			return false;

		// GB11: Extended pictographic sequences with ZWJ
		if (a == GBP::ZWJ && b == GBP::ExtendedPictographic)
		{
			int32_t k = (int32_t)index - 2;
			while (k >= 0 && GetGBP(cps[k]) == GBP::Extend) k--;
			if (k >= 0 && GetGBP(cps[k]) == GBP::ExtendedPictographic)
				return false;
		}

		// -------------------------------
		// GB12 / GB13: Regional Indicator pairs
		// -------------------------------
		// If prev and next are both RI, we must count how many consecutive RIs
		// immediately before 'next' (i.e. up to index-1). If that count is ODD
		// -> join (no break). If EVEN -> break.
		auto isRI = [](uint32_t cp) { return cp >= 0x1F1E6 && cp <= 0x1F1FF; };

		if (isRI(prev) && isRI(next))
		{
			// count consecutive RIs ending at position index-1 (the 'prev')
			uint32_t countRIs = 1; // prev is one RI
			int32_t k = (int32_t)index - 2;
			while (k >= 0 && isRI(cps[k]))
			{
				countRIs++;
				k--;
			}

			// If countRIs is odd -> join (no break). If even -> break.
			if ((countRIs & 1u) == 1u) return false;
			else return true;
		}

		// Default = break
		return true;
	}

	void ComputeGraphemeBoundaries(const List<CodePoint>& cps, uint32_t count, List<CodePoint>& boundaries, uint32_t& outCount) noexcept
	{
		outCount = 0;
		if (count == 0)
			return;

		boundaries.Clear();       // IMPORTANTÍSSIMO
		boundaries.EnsureCapacity(count);

		// first cluster always starts at index 0
		boundaries.Add(0);
		outCount = 1;

		for (uint32_t i = 1; i < count; i++)
		{
			if (UnicodeCase::IsGraphemeBreak(cps[i - 1], cps[i], cps, count, i))
			{
				boundaries.Add(i);
				outCount++;
			}
		}

		// nunca permita boundaries fora do range
		if (outCount == 0)
		{
			boundaries.Add(0);
			outCount = 1;
		}
	}

	bool IsEmojiExtendedPictographic(CodePoint cp)
	{
		// Extended pictographic property data (Unicode 15+)
		// Covers ❤ (2764), 💋, 👩, 👨, etc.
		//
		// For performance we only include the essential ranges:
		if (cp == 0x2764) return true;  // ❤
		if (cp == 0x1F48B) return true; // 💋
		if (cp >= 0x1F300 && cp <= 0x1FAFF) return true; // emoji blocks
		if (cp >= 0x1F1E6 && cp <= 0x1F1FF) return true; // flags (regional indicators)
		return false;
	}
}