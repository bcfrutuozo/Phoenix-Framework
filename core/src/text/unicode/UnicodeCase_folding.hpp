#pragma once
#include <cstdint>

namespace UnicodeCase
{
    struct CFEntry {
        uint32_t cp;
        const uint32_t* seq;
        uint32_t len;
    };

    // ---- FULL CASE FOLDING (F) from CaseFolding.txt ----
    // Somente entradas realmente necessárias (Unicode >= 12),
    // reduzido ao essencial para otimizar performance.
    // Cada entrada seq[] nunca tem mais que 3 codepoints.

    static constexpr uint32_t CF_00DF[] = { 0x0073, 0x0073 };           // ß → ss
    static constexpr uint32_t CF_0130[] = { 0x0069, 0x0307 };           // İ → i + dot above (turkish)
    static constexpr uint32_t CF_FB00[] = { 0x0066, 0x0066 };           // ﬀ → ff
    static constexpr uint32_t CF_FB01[] = { 0x0066, 0x0069 };           // ﬁ → fi
    static constexpr uint32_t CF_FB02[] = { 0x0066, 0x006C };           // ﬂ → fl
    static constexpr uint32_t CF_FB03[] = { 0x0066, 0x0066, 0x0069 };   // ﬃ → ffi
    static constexpr uint32_t CF_FB04[] = { 0x0066, 0x0066, 0x006C };   // ﬄ → ffl
    static constexpr uint32_t CF_FB05[] = { 0x0073, 0x0074 };           // ﬅ → st
    static constexpr uint32_t CF_FB06[] = { 0x0073, 0x0074 };           // ﬆ → st

    static constexpr uint32_t CF_03C2[] = { 0x03C3 };                   // σ (final sigma) → σ normal
    static constexpr uint32_t CF_03C3[] = { 0x03C3 };                   // σ → σ
    static constexpr uint32_t CF_03F2[] = { 0x03C3 };                   // ϲ → σ (lunate sigma)

    // Tabela principal: (ordenada por cp para busca binária rápida)
    static constexpr CFEntry CASEFOLD_TABLE[] =
    {
        { 0x00DF, CF_00DF, 2 },
        { 0x0130, CF_0130, 2 },
        { 0x03C2, CF_03C2, 1 },
        { 0x03C3, CF_03C3, 1 },
        { 0x03F2, CF_03F2, 1 },

        { 0xFB00, CF_FB00, 2 },
        { 0xFB01, CF_FB01, 2 },
        { 0xFB02, CF_FB02, 2 },
        { 0xFB03, CF_FB03, 3 },
        { 0xFB04, CF_FB04, 3 },
        { 0xFB05, CF_FB05, 2 },
        { 0xFB06, CF_FB06, 2 },
    };

    static constexpr uint32_t CASEFOLD_TABLE_COUNT =
        sizeof(CASEFOLD_TABLE) / sizeof(CASEFOLD_TABLE[0]);
}