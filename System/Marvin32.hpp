#pragma once

#include <cstdint>

static inline uint32_t rotl32(uint32_t x, int r) noexcept
{
    return (x << r) | (x >> (32 - r));
}

struct Marvin32
{
    uint32_t p0;
    uint32_t p1;

    explicit Marvin32(uint64_t seed) noexcept
    {
        p0 = (uint32_t)seed;
        p1 = (uint32_t)(seed >> 32);
    }

    inline void Mix(uint32_t data) noexcept
    {
        p0 += data;
        p1 ^= p0;
        p0 = rotl32(p0, 20) + p1;
        p1 = rotl32(p1, 9) ^ p0;
    }

    inline uint32_t Finish() noexcept
    {
        p0 += 0x80;
        p1 ^= p0;
        p0 = rotl32(p0, 20) + p1;
        p1 = rotl32(p1, 9) ^ p0;
        return p0 ^ p1;
    }

    static uint32_t Compute(const uint8_t* data, size_t len, uint64_t seed) noexcept
    {
        Marvin32 m(seed);

        size_t i = 0;
        while (i + 4 <= len)
        {
            uint32_t block =
                (uint32_t)data[i] |
                ((uint32_t)data[i + 1] << 8) |
                ((uint32_t)data[i + 2] << 16) |
                ((uint32_t)data[i + 3] << 24);

            m.Mix(block);
            i += 4;
        }

        // tail (0–3 bytes)
        uint32_t tail = 0;
        size_t rem = len - i;
        for (size_t k = 0; k < rem; ++k)
            tail |= ((uint32_t)data[i + k] << (8 * k));

        m.Mix(tail);
        return m.Finish();
    }
};