// Auto-generated: UnicodeCase_utils.hpp (depends on generated headers)
#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include "UnicodeCase_generated.hpp"
#include "UnicodeCase_special.hpp"
#include "UnicodeCase_props.hpp"

namespace UnicodeCase {

inline bool locale_is_turkic(const std::string& locale) noexcept {
    if (locale.size() >= 2) {
        if (locale.rfind("tr", 0) == 0) return true;
        if (locale.rfind("az", 0) == 0) return true;
    }
    return false;
}

inline bool is_letter(uint32_t cp) noexcept {
    return small_binsearch_has(LETTERS, sizeof(LETTERS)/sizeof(LETTERS[0]), cp);
}

inline bool is_cased(uint32_t cp) noexcept {
    return small_binsearch_has(CASED, sizeof(CASED)/sizeof(CASED[0]), cp);
}

inline bool is_soft_dotted(uint32_t cp) noexcept {
    return small_binsearch_has(SOFT_DOTTED, sizeof(SOFT_DOTTED)/sizeof(SOFT_DOTTED[0]), cp);
}

// Final sigma context: return true if we should use final-sigma mapping (i.e., produce SMALL FINAL SIGMA)
inline bool apply_final_sigma_context(const std::vector<uint32_t>& cps, size_t i) noexcept {
    // Look backwards for a cased letter; look forwards for a letter
    bool foundBefore = false;
    if (i > 0) {
        for (size_t j = i; j-- > 0;) {
            if (is_cased(cps[j])) { foundBefore = true; break; }
        }
    }
    bool foundAfter = false;
    for (size_t j = i + 1; j < cps.size(); ++j) {
        if (is_letter(cps[j])) { foundAfter = true; break; }
    }
    // If there is a cased letter before AND a letter after => medial sigma -> NOT final
    // final sigma when NOT (foundBefore && foundAfter)
    return !(foundBefore && foundAfter);
}

// Map cp to upper sequence (returns vector of codepoints)
inline std::vector<uint32_t> map_to_upper_sequence(uint32_t cp, const std::string& locale, const std::vector<uint32_t>& context) {
    // Check special entries first (linear scan; small fraction)
    for (const auto& e : SPECIAL_ENTRIES) {
        if (e.cp != cp) continue;
        // condition empty -> unconditional
        if (!e.condition || e.condition[0] == '\0') {
            std::vector<uint32_t> out(e.upper, e.upper + e.upper_len);
            if (!out.empty()) return out;
            break; // fallback
        }
        std::string cond(e.condition);
        if (cond.find("Final_Sigma") != std::string::npos) {
            // need context index; caller must pass context with current codepoint placed at position i ???
            // We cannot evaluate here without index; caller should instead detect 'σ' (0x03C3) special-case.
            // Fallback: ignore conditional here and use simple mapping; the String-level code will call apply_final_sigma_context.
        }
        if (cond.find("Turkic") != std::string::npos || cond.find("AZ") != std::string::npos) {
            if (locale_is_turkic(locale)) {
                std::vector<uint32_t> out(e.upper, e.upper + e.upper_len);
                return out;
            }
            // else skip
        }
        // other conditions ignored here
    }
    // fallback to simple mapping
    uint32_t mapped = to_upper_simple(cp);
    return std::vector<uint32_t>(1, mapped);
}

inline std::vector<uint32_t> map_to_lower_sequence(uint32_t cp, const std::string& locale, const std::vector<uint32_t>& context) {
    for (const auto& e : SPECIAL_ENTRIES) {
        if (e.cp != cp) continue;
        if (!e.condition || e.condition[0] == '\0') {
            std::vector<uint32_t> out(e.lower, e.lower + e.lower_len);
            if (!out.empty()) return out;
            break;
        }
        std::string cond(e.condition);
        if (cond.find("Final_Sigma") != std::string::npos) {
            // same caveat as above
        }
        if (cond.find("Turkic") != std::string::npos) {
            if (locale_is_turkic(locale)) {
                std::vector<uint32_t> out(e.lower, e.lower + e.lower_len);
                return out;
            }
            // else skip
        }
    }
    uint32_t mapped = to_lower_simple(cp);
    return std::vector<uint32_t>(1, mapped);
}

} // namespace UnicodeCase
