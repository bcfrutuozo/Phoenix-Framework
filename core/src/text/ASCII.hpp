#pragma once

#include <cstdint>
#include <cstring>

#include "types/Char.hpp"

namespace ASCII
{
	static inline bool IsAllASCII(const Char* p, uint32_t len) noexcept
	{
		for (uint32_t i = 0; i < len; ++i)
		{
			if (p[i] & 0x80)
				return false;
		}
		return true;
	}

	static inline constexpr Char ToLower(Char c) noexcept
	{
		if (c >= 'A' && c <= 'Z')
			return (c - 'A' + 'a');
		return c;
	}

	static inline constexpr Char ToUpper(Char c) noexcept
	{
		if (c >= 'a' && c <= 'z')
			return (c - 'a' + 'A');
		return c;
	}

	static inline bool Equals(const Char* a, const Char* b, uint32_t len) noexcept
	{
		if (len == 0) return true;
		// memcmp on Char works because sizeof(Char) == 1
		return memcmp(a, b, static_cast<size_t>(len) * sizeof(Char)) == 0;
	}

	static inline bool EqualsIgnoreCase(const Char* a, const Char* b, uint32_t len) noexcept
	{
		for (uint32_t i = 0; i < len; ++i)
		{
			if (ToLower(a[i]) != ToLower(b[i])) return false;
		}
		return true;
	}

	static inline int Compare(const Char* a, uint32_t lenA, const Char* b, uint32_t lenB) noexcept
	{
		uint32_t m = (lenA < lenB ? lenA : lenB);

		for (uint32_t i = 0; i < m; ++i)
		{
			Char ca = a[i];
			Char cb = b[i];

			if (ca < cb) return -1;
			if (ca > cb) return 1;
		}

		if (lenA < lenB) return -1;
		if (lenA > lenB) return 1;
		return 0;
	}

	static inline int CompareIgnoreCase(const Char* a, uint32_t lenA, const Char* b, uint32_t lenB) noexcept
	{
		uint32_t m = (lenA < lenB ? lenA : lenB);

		for (uint32_t i = 0; i < m; ++i)
		{
			Char va = ToLower(a[i]);
			Char vb = ToLower(b[i]);

			if (va < vb) return -1;
			if (va > vb) return 1;
		}

		if (lenA < lenB) return -1;
		if (lenA > lenB) return 1;
		return 0;
	}

	static inline uint32_t CountOccurrences(const Char* hay, uint32_t H,
		const Char* ned, uint32_t N) noexcept
	{
		if (N == 0 || N > H) return 0;

		uint32_t count = 0;

		for (uint32_t i = 0; i + N <= H; )
		{
			bool match = true;

			for (uint32_t j = 0; j < N; ++j)
			{
				Char a = hay[i + j];
				Char b = ned[j];
				if (a != b) { match = false; break; }
			}

			if (match) { ++count; i += N; }
			else { ++i; }
		}

		return count;
	}

	static inline uint32_t CountOccurrencesIgnoreCase(const Char* hay, uint32_t H, const Char* ned, uint32_t N) noexcept
	{
		if (N == 0 || N > H) return 0;

		uint32_t count = 0;

		for (uint32_t i = 0; i + N <= H; )
		{
			bool match = true;

			for (uint32_t j = 0; j < N; ++j)
			{
				Char a = ToLower(hay[i + j]);
				Char b = ToLower(ned[j]);

				if (a != b) { match = false; break; }
			}

			if (match) { ++count; i += N; }
			else { ++i; }
		}

		return count;
	}

	static inline bool EndsWith(const Char* hay, uint32_t H, const Char* nee, uint32_t N) noexcept
	{
		if (N == 0) return true;
		if (N > H) return false;
		uint32_t start = H - N;
		return Equals(hay + start, nee, N);
	}

	static inline bool EndsWithIgnoreCase(const Char* hay, uint32_t H, const Char* nee, uint32_t N) noexcept
	{
		if (N == 0) return true;
		if (N > H) return false;
		uint32_t start = H - N;
		return EqualsIgnoreCase(hay + start, nee, N);
	}

	static inline const Char* Find(const Char* hay, uint32_t H, const Char* ned, uint32_t N) noexcept
	{
		if (N == 0) return hay;
		if (N > H) return nullptr;

		for (uint32_t i = 0; i + N <= H; ++i)
		{
			bool ok = true;

			for (uint32_t j = 0; j < N; ++j)
			{
				Char a = hay[i + j];
				Char b = ned[j];

				if (a != b) { ok = false; break; }
			}

			if (ok) return hay + i;
		}

		return nullptr;
	}

	static inline const Char* FindIgnoreCase(const Char* hay, uint32_t H, const Char* ned, uint32_t N) noexcept
	{
		if (N == 0) return hay;
		if (N > H) return nullptr;

		for (uint32_t i = 0; i + N <= H; ++i)
		{
			bool ok = true;

			for (uint32_t j = 0; j < N; ++j)
			{
				Char a = ToLower(hay[i + j]);
				Char b = ToLower(ned[j]);

				if (a != b) { ok = false; break; }
			}

			if (ok) return hay + i;
		}

		return nullptr;
	}

	static inline int64_t IndexOf(const Char* hay, uint32_t H, const Char* ned, uint32_t N, uint32_t startIndex) noexcept
	{
		if (N == 0) return static_cast<int64_t>(startIndex);
		if (N > H) return -1;
		if (startIndex >= H) return -1;

		// Clamp loop bound
		uint32_t upper = H - N;
		for (uint32_t i = startIndex; i <= upper; ++i)
		{
			if (Equals(hay + i, ned, N)) return static_cast<int64_t>(i);
		}
		return -1;
	}

	static inline int64_t LastIndexOf(const Char* hay, uint32_t H, const Char* ned, uint32_t N, uint32_t startIndex) noexcept
	{
		if (N == 0)
		{
			// Similar to .NET behavior: return min(startIndex, length-1) but if H==0 return 0
			if (H == 0) return 0;
			uint32_t s = (startIndex < H ? startIndex : (H - 1));
			return static_cast<int64_t>(s);
		}

		if (N > H) return -1;
		if (H == 0) return -1;

		uint32_t s = (startIndex < H ? startIndex : (H - 1));

		for (int64_t i = static_cast<int64_t>(s); i >= 0; --i)
		{
			uint32_t ui = static_cast<uint32_t>(i);
			if (ui + N > H) continue;
			if (Equals(hay + ui, ned, N)) return static_cast<int64_t>(ui);
		}
		return -1;
	}

	static inline void ReplaceOrdinal(const Char* src, uint32_t H, const Char* oldv, uint32_t O, const Char* newv, uint32_t N, Char* dst) noexcept
	{
		uint32_t i = 0, w = 0;

		while (i < H)
		{
			bool match = true;

			if (i + O <= H)
			{
				for (uint32_t j = 0; j < O; ++j)
				{
					if (src[i + j] != oldv[j])
					{
						match = false; break;
					}
				}
			}
			else match = false;

			if (match)
			{
				for (uint32_t k = 0; k < N; ++k)
					dst[w++] = newv[k];

				i += O;
			}
			else
			{
				dst[w++] = src[i++];
			}
		}
	}

	static inline void ReplaceIgnoreCase(const Char* src, uint32_t H, const Char* oldv, uint32_t O, const Char* newv, uint32_t N, Char* dst) noexcept
	{
		uint32_t i = 0, w = 0;

		while (i < H)
		{
			bool match = true;

			if (i + O <= H)
			{
				for (uint32_t j = 0; j < O; ++j)
				{
					Char a = ToLower(src[i + j]);
					Char b = ToLower(oldv[j]);

					if (a != b) { match = false; break; }
				}
			}
			else match = false;

			if (match)
			{
				for (uint32_t k = 0; k < N; ++k)
					dst[w++] = newv[k];

				i += O;
			}
			else
			{
				dst[w++] = src[i++];
			}
		}
	}

	static inline bool StartsWith(const Char* hay, uint32_t H, const Char* nee, uint32_t N) noexcept
	{
		if (N == 0) return true;
		if (N > H) return false;
		return Equals(hay, nee, N);
	}

	static inline bool StartsWithIgnoreCase(const Char* hay, uint32_t H, const Char* nee, uint32_t N) noexcept
	{
		if (N == 0) return true;
		if (N > H) return false;
		return EqualsIgnoreCase(hay, nee, N);
	}
}