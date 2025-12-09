#include "UnicodeNormalization_utils.hpp"

namespace UnicodeNormalization
{

	// --------------------------------------------------------
	CodePoint GetCCC(CodePoint cp)
	{
		if (CCC_RANGES_COUNT == 0) return 0;

		int idx = binary_search_ranges_uint32(CCC_RANGES, CCC_RANGES_COUNT, cp);
		if (idx < 0) return 0;

		return static_cast<uint8_t>(CCC_RANGES[idx * 3 + 2]);
	}

	// --------------------------------------------------------
	// Composition exclusions lookup
	// --------------------------------------------------------
	bool IsCompositionExcluded(CodePoint cp)
	{
		// linear search; COMP_EXCL_COUNT expected to be small relative to other tables
		for (uint32_t i = 0; i < COMP_EXCL_COUNT; ++i)
			if (COMP_EXCL[i] == cp)
				return true;
		return false;
	}

	static inline bool HangulComposeSequence(CodePoint a, CodePoint b, CodePoint& out)
	{
		// L + V -> LV
		if (a >= LBase && a < (LBase + LCount) &&
			b >= VBase && b < (VBase + VCount))
		{
			uint32_t LIndex = (uint32_t)(a - LBase);
			uint32_t VIndex = (uint32_t)(b - VBase);
			out = CodePoint(SBase + (LIndex * NCount) + (VIndex * TCount));
			return true;
		}

		// LV + T -> LVT
		if (a >= SBase && a < (SBase + SCount) &&
			b >= TBase && b < (TBase + TCount))
		{
			uint32_t SIndex = (uint32_t)(a - SBase);
			// only compose if S is an LV (i.e. SIndex % TCount == 0)
			if ((SIndex % TCount) == 0)
			{
				out = CodePoint(a + (b - TBase));
				return true;
			}
		}

		return false;
	}

	// --------------------------------------------------------
	// ComposePair: checks algorithmic Hangul first, then table
	// COMPOSE_PAIRS expected as uint64_t entries where high 32 = (a<<32)|b key, low 32 = composed
	// COMPOSE_COUNT is number of entries
	// --------------------------------------------------------
	inline bool ComposePair(CodePoint a, CodePoint b, CodePoint& out)
	{
		uint32_t av = (uint32_t)a;
		uint32_t bv = (uint32_t)b;

		// Hangul algorithm first
		CodePoint composed;
		if (HangulComposeSequence(av, bv, composed))
		{
			out = CodePoint(composed);
			return true;
		}

		uint64_t key = (uint64_t(av) << 32) | uint64_t(bv);

		int lo = 0, hi = (int)COMPOSE_COUNT - 1;
		while (lo <= hi)
		{
			int mid = (lo + hi) >> 1;
			uint64_t k = COMPOSE_PAIRS[mid];

			if (k == key)
			{
				out = CodePoint(COMPOSE_RESULT[mid]);
				return true;
			}
			if (k < key) lo = mid + 1;
			else hi = mid - 1;
		}
		return false;
	}

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
	)
	{
		// Hangul algorithmic
		if (cp.IsHangulSyllable())
		{
			uint32_t s = cp;
			CodePoint SIndex = s - SBase;
			out[outLen++] = CodePoint(LBase + (SIndex / NCount));
			out[outLen++] = CodePoint(VBase + ((SIndex % NCount) / TCount));
			uint32_t t = SIndex % TCount;
			if (t != 0)
				out[outLen++] = CodePoint(TBase + t);
			return;
		}

		// binary search
		uint32_t cpv = (uint32_t)cp;
		uint32_t low = 0;
		uint32_t high = DECOMP_COUNT;

		while (low < high)
		{
			uint32_t mid = (low + high) >> 1;
			uint32_t key = DECOMP_KEYS[mid];

			if (key == cpv)
			{
				uint32_t off = DECOMP_OFFS[mid];
				uint32_t nextOff = DECOMP_OFFS[mid + 1];
				uint32_t len = nextOff - off;
				bool isCompat = (DECOMP_FLAGS[mid] != 0);

				if (!compatibility && isCompat)
				{
					out[outLen++] = cp;
					return;
				}

				for (uint32_t i = 0; i < len; ++i)
				{
					CodePoint sub(DECOMP_TARGETS[off + i]);
					DecomposeCodePoint(sub, compatibility, out, outLen);
				}
				return;
			}

			if (key < cpv) low = mid + 1;
			else high = mid;
		}

		out[outLen++] = cp;
	}

	void ReorderByCCC(List<CodePoint>& cps)
	{
		// use an unsigned wide type for counts
		using idx_t = decltype(cps.Count());
		idx_t count64 = cps.Count();
		if (count64 < 2) return;

		// convert to uint32_t when you need that type (guard overflow)
		if (count64 > (idx_t)UINT32_MAX) {
			// extremely unlikely; clamp for safety
			// but we will still iterate using size_type semantics
		}
		uint32_t count = static_cast<uint32_t>(count64);

		// 0) Never reorder Hangul Jamo
		for (uint32_t i = 0; i < count; ++i)
		{
			CodePoint cp = cps[i];

			// L Jamo
			if (cp >= LBase && cp < LBase + LCount) return;

			// V Jamo
			if (cp >= VBase && cp < VBase + VCount) return;

			// T Jamo
			if (cp > TBase && cp <= TBase + TCount) return;
		}

		// If ANY ZWJ (U+200D) exists, skip entire reorder step.
		for (uint32_t i = 0; i < count; ++i)
			if (cps[i] == 0x200D) return;

		// Main loop: find runs and sort marks by CCC
		uint32_t i = 0;
		while (i < count) {
			// recompute count defensively in case cps was modified elsewhere
			idx_t currCount64 = cps.Count();
			if (currCount64 < i) break; // defensive exit
			if (currCount64 != count) {
				// keep 'count' consistent with actual list length
				count = static_cast<uint32_t>(currCount64);
			}
			// get current CCC safely
			uint8_t ccc_i = GetCCC(cps[i]);

			uint32_t runStart = i;
			uint32_t runMarksStart;

			if (ccc_i == 0) {
				// starter at i: marks (if any) start at i+1
				runMarksStart = i + 1;
			}
			else {
				// leading marks-run
				runMarksStart = i;
			}

			// find end of run: j is first index >= runMarksStart with CCC == 0
			uint32_t j = runMarksStart;
			// IMPORTANT: check j < cps.Count() before accessing cps[j]
			while (true) {
				idx_t currLen = cps.Count();
				if (j >= currLen) break;
				if (GetCCC(cps[j]) == 0) break;
				++j;
			}

			// stable insertion sort on marks range [runMarksStart, j-1]
			if (runMarksStart < j) {
				for (uint32_t a = runMarksStart + 1; a < j; ++a) {
					uint32_t cur = cps[a];
					uint8_t cccCur = GetCCC(cur);
					int b = (int)a;
					// note: use cps.Count() inside loop only if necessary; here we compare to runMarksStart
					while (b > (int)runMarksStart && GetCCC(cps[b - 1]) > cccCur) {
						cps[b] = cps[b - 1];
						--b;
					}
					cps[b] = cur;
				}
			}

			// move to next run
			i = (j > runStart) ? j : runStart + 1;
		}
	}

	void Compose(List<CodePoint>& cps)
	{
		uint32_t count = cps.Count();
		if (count < 2) return;

		List<CodePoint> outBuf;
		outBuf.EnsureCapacity(count + 4); // small headroom

		// push first
		outBuf.Add(cps[0]);

		int lastStarterIndex = 0;
		CodePoint lastStarter = outBuf[0];

		for (uint32_t i = 1; i < count; ++i)
		{
			CodePoint ch = cps[i];
			uint8_t ccc = GetCCC(ch);

			bool composedThisChar = false;

			// Attempt composition with lastStarter (canonical only)
			if (lastStarterIndex >= 0 && GetCCC(lastStarter) == 0)
			{
				bool blocked = false;

				// block only if there exists a mid char with 0 < midCCC < ccc
				for (int k = lastStarterIndex + 1; k < (int)outBuf.Count(); ++k)
				{
					uint8_t midCCC = GetCCC(outBuf[k]);
					if (midCCC > 0 && midCCC < ccc)
					{
						blocked = true;
						break;
					}
				}

				if (!blocked)
				{
					CodePoint composed;
					if (ComposePair(lastStarter, ch, composed) && !IsCompositionExcluded(composed))
					{
						// replace last starter with composed glyph
						outBuf[lastStarterIndex] = composed;
						lastStarter = composed;
						composedThisChar = true;
					}
				}
			}

			if (!composedThisChar)
			{
				outBuf.Add(ch);

				if (ccc == 0)
				{
					lastStarterIndex = (int)outBuf.Count() - 1;
					lastStarter = ch;
				}
			}
		}

		// copy back: replace entire cps with outBuf (safe)
		cps = outBuf;
	}
}