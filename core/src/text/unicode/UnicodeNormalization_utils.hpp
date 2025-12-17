#pragma once

#include "../../types/CodePoint.hpp"
#include "../../types/UInt32.hpp"
#include "../../collections/List.hpp"
#include "UnicodeNormalization_tables.hpp"
#include <cstdint>
#include <stdio.h>

enum class NormalizationForm 
{ 
	NFC, 
	NFD, 
	NFKC, 
	NFKD
};

namespace UnicodeNormalization
{
    // Hangul constants
    static constexpr char32_t SBase = 0xAC00;
    static constexpr char32_t LBase = 0x1100;
    static constexpr char32_t VBase = 0x1161;
    static constexpr char32_t TBase = 0x11A7;
					 
	static constexpr UInt32 LCount = 19;
    static constexpr UInt32 VCount = 21;
    static constexpr UInt32 TCount = 28;
					 
    static constexpr UInt32 NCount = VCount * TCount;
    static constexpr UInt32 SCount = LCount * NCount;

	// --------------------------------------------------------
	// binary search over flat ranges: ranges is [start,end,ccc, start,end,ccc, ...]
	// each entry is 3 uint32_t values
	// --------------------------------------------------------
	static int binary_search_ranges_uint32(const CodePoint* ranges, uint32_t rangeCount, CodePoint cp)
	{
		int lo = 0;
		int hi = (int)rangeCount - 1;

		while (lo <= hi)
		{
			int mid = (lo + hi) >> 1;
			uint32_t start = ranges[mid * 3 + 0];
			uint32_t end = ranges[mid * 3 + 1];

			if (cp < start) hi = mid - 1;
			else if (cp > end) lo = mid + 1;
			else return mid;
		}

		return -1;
	}

	// --------------------------------------------------------
	// Get canonical combining class for a codepoint
	// --------------------------------------------------------
	CodePoint GetCCC(CodePoint cp);

	// --------------------------------------------------------
	// Composition exclusions lookup
	// --------------------------------------------------------
	bool IsCompositionExcluded(CodePoint cp);

	// --------------------------------------------------------
	// Hangul helpers (algorithmic)
	// --------------------------------------------------------

	static bool HangulComposeSequence(CodePoint a, CodePoint b, CodePoint& out);

	// --------------------------------------------------------
	// ComposePair: checks algorithmic Hangul first, then table
	// COMPOSE_PAIRS expected as uint64_t entries where high 32 = (a<<32)|b key, low 32 = composed
	// COMPOSE_COUNT is number of entries
	// --------------------------------------------------------
	bool ComposePair(CodePoint a, CodePoint b, CodePoint& out);

	// --------------------------------------------------------
	// DecomposeCodePoint: recursive decomposition using DECOMP_* tables
	// Assumes DECOMP_OFFS length == DECOMP_COUNT + 1 (sentinel)
	// DECOMP_FLAGS[mid] != 0 means that decomposition is "compat" (compatibility) - generator must provide it
	// If DECOMP_FLAGS is not present in your tables, set all zeros in generated header or tell me to adapt.
	// --------------------------------------------------------
	void DecomposeCodePoint(
		CodePoint cp,
		bool compatibility,
		CodePoint* out,
		uint32_t& outLen
	);

	// --------------------------------------------------------
	// Reorder by canonical combining class (stable insertion sort)
	// --------------------------------------------------------
	void ReorderByCCC(List<CodePoint>& cps);

	// --------------------------------------------------------
	// Compose: canonical composition per UAX#15
	// Uses ComposePair + IsCompositionExcluded
	// --------------------------------------------------------
	void Compose(List<CodePoint>& cps);
}