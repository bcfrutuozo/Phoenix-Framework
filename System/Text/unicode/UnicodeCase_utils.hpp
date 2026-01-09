#pragma once

#include <cstdint>

#include "UnicodeCase_generated.hpp"
#include "UnicodeCase_special.hpp"
#include "UnicodeCase_props.hpp"
#include "UnicodeCase_folding.hpp"
#include "System/Types.hpp"
#include "System/Collections/List.hpp"

namespace UnicodeCase
{
	static constexpr bool small_binsearch_has(const CodePoint* arr, size_t n, CodePoint cp) noexcept {
		size_t lo = 0, hi = n;
		while (lo < hi) {
			size_t mid = (lo + hi) >> 1;
			uint32_t v = arr[mid];
			if (v == cp) return true;
			if (v < cp) lo = mid + 1;
			else hi = mid;
		}
		return false;
	}

	inline static constexpr bool is_letter(CodePoint cp) noexcept {
		return small_binsearch_has(LETTERS, sizeof(LETTERS) / sizeof(LETTERS[0]), cp);
	}

	inline static constexpr bool is_cased(CodePoint cp) noexcept {
		return small_binsearch_has(CASED, sizeof(CASED) / sizeof(CASED[0]), cp);
	}

	inline static constexpr bool is_soft_dotted(CodePoint cp) noexcept {
		return small_binsearch_has(SOFT_DOTTED, sizeof(SOFT_DOTTED) / sizeof(SOFT_DOTTED[0]), cp);
	}

	bool locale_is_turkic(const char* locale, uint32_t len) noexcept;

	bool apply_final_sigma_context(const List<CodePoint>& cps, uint32_t index) noexcept;
	void map_to_upper_sequence_nostd(CodePoint cp, const char* localeBytes, uint32_t localeLen, List<CodePoint>& output) noexcept;
	void map_to_lower_sequence_nostd(CodePoint cp, const char* localeBytes, uint32_t localeLen, const List<CodePoint>& context, uint32_t index, List<CodePoint>& output) noexcept;
	void map_to_casefold_sequence_nostd(CodePoint cp, const char* localeBytes, uint32_t localeLen, const List<CodePoint>& context, uint32_t index, List<CodePoint>& output) noexcept;

	bool IsGraphemeBreak(CodePoint prev, CodePoint next, const List<CodePoint>& cps, uint32_t len, uint32_t index) noexcept;
	void ComputeGraphemeBoundaries(const List<CodePoint>& cps, uint32_t count, List<CodePoint>& boundaries, uint32_t& outCount) noexcept;
	bool IsEmojiExtendedPictographic(CodePoint cp);

	static constexpr GBP GetGBP(CodePoint cp) noexcept
	{
		// Control / CR / LF
		if (cp == 0x000D) return GBP::CR;   // CR
		if (cp == 0x000A) return GBP::LF;   // LF
		if (cp <= 0x001F || (cp >= 0x007F && cp <= 0x009F))
			return GBP::Control;

		// Zero Width Joiner
		if (cp == 0x200D)
			return GBP::ZWJ;

		// Combining marks (Extend)
		// Unicode range: combining diacritics, variation selectors, etc.
		if ((cp >= 0x0300 && cp <= 0x036F) ||    // combining diacritics
			(cp >= 0x1AB0 && cp <= 0x1AFF) ||    // extended combining diacritics
			(cp >= 0x1DC0 && cp <= 0x1DFF) ||
			(cp >= 0x20D0 && cp <= 0x20FF) ||
			(cp >= 0xFE20 && cp <= 0xFE2F))
		{
			return GBP::Extend;
		}

		// Emoji modifiers (skin tones) — also Extend
		if (cp >= 0x1F3FB && cp <= 0x1F3FF)
			return GBP::Extend;

		// Regional Indicator (flags)
		if (cp >= 0x1F1E6 && cp <= 0x1F1FF)
			return GBP::RegionalIndicator;

		// Mark Spacing (GB9b)
		if (cp >= 0x0903 && cp <= 0x093B)
			return GBP::SpacingMark;

		// Extended pictographic (emojis)
		// This is the minimal safe range covering all emoji blocks
		if (IsEmojiExtendedPictographic(cp))
			return GBP::ExtendedPictographic;

		return GBP::Other;
	}
}