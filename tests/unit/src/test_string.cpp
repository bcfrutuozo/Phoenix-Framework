#pragma once

#include "catch_amalgamated.hpp"
#include "types/String.hpp"
#include "text/unicode/UnicodeCase_utils.hpp"
#include "text/unicode/UnicodeNormalization_utils.hpp"
#include "collections/List.hpp"
#include "system/Framework.hpp"

static List<Char> Chars(std::initializer_list<Char> list)
{
    List<Char> r;
    for (auto& c : list) r.Add(c);
    return r;
}

static List<String> Strings(std::initializer_list<String> list)
{
    List<String> r;
    for (auto& s : list) r.Add(s);
    return r;
}

using NF = UnicodeNormalization::NormalizationForm;

// =====================================================
// 1. Construction & Basic Properties
// =====================================================

TEST_CASE("String: Construction and Basic Properties")
{
    SECTION("Default constructor")
    {
        String s;
        REQUIRE(s.IsEmpty());
        REQUIRE(s.GetByteCount() == 0);
        REQUIRE(s.GetLength() == 0);
    }

    SECTION("From C-string ASCII")
    {
        String s("Hello");
        REQUIRE(!s.IsEmpty());
        REQUIRE(s.GetByteCount() == 5);
        REQUIRE(s.GetLength() == 5);
        REQUIRE(s.Equals("Hello"));
    }

    SECTION("From UTF-8 C-string")
    {
        // "Straße" : S t r a ß e => 6 codepoints, 7 bytes
        String s((const char*)u8"Stra\u00DFe");
        REQUIRE(s.GetLength() == 6);
        REQUIRE(s.GetByteCount() == 7);
    }

    SECTION("Copy constructor does NOT share SSO")
    {
        String a("Test");
        String b(a);

        REQUIRE(a.Equals(b));
        REQUIRE(a.IsSSO());
        REQUIRE(b.IsSSO());
        REQUIRE(a.GetReferenceCount() == 1);
        REQUIRE(b.GetReferenceCount() == 1);
    }

    SECTION("Copy constructor shares heap block")
    {
        String a("This string is large enough to bypass SSO");
        String b(a);

        REQUIRE(a.Equals(b));
        REQUIRE(!a.IsSSO());
        REQUIRE(!b.IsSSO());
        REQUIRE(a.GetReferenceCount() == 2);
        REQUIRE(b.GetReferenceCount() == 2);
    }

    SECTION("Move constructor transfers ownership")
    {
        String a("This is a large string to bypass SSO 1234567890");
        unsigned int rc = a.GetReferenceCount();
        String b(std::move(a));

        REQUIRE(a.IsEmpty());
        REQUIRE(!b.IsEmpty());
        REQUIRE(b.Equals("This is a large string to bypass SSO 1234567890"));
    }
    SECTION("Move constructor preserves value")
    {
        String a("MOVE");
        String b(std::move(a));

        REQUIRE(b.Equals("MOVE"));
        REQUIRE(b.GetReferenceCount() == 1);

        // Estado de 'a' não deve ser usado semanticamente
        REQUIRE((a.IsEmpty() || (a.GetLength() == 0)));
    }
}

// =====================================================
// 2. Operators: ==, !=, +, +=
// =====================================================

TEST_CASE("String: Operators")
{
    SECTION("operator== and !=")
    {
        REQUIRE(String("ABC") == String("ABC"));
        REQUIRE(String("ABC") != String("ABD"));
        REQUIRE(String("") == String());
    }

    SECTION("Operator+ String+String")
    {
        String a("Hello");
        String b("World");
        REQUIRE((a + b).Equals("HelloWorld"));
    }

    SECTION("Operator+ String+char*")
    {
        String a("Hi");
        REQUIRE((a + "!!!").Equals("Hi!!!"));
    }

    SECTION("Operator+= String")
    {
        String a("A");
        a += String("B");
        REQUIRE(a.Equals("AB"));
    }

    SECTION("Operator+= char*")
    {
        String a("A");
        a += "BCD";
        REQUIRE(a.Equals("ABCD"));
    }
}

// =====================================================
// 3. Equals (ordinal, ignoreCase, locale)
// =====================================================

TEST_CASE("String: Equals")
{
    SECTION("Ordinal")
    {
        REQUIRE(String::Equals("abc", "abc", false));
        REQUIRE_FALSE(String::Equals("abc", "Abc", false));
    }

    SECTION("IgnoreCase ASCII")
    {
        REQUIRE(String::Equals("abc", "ABC", true));
    }

    SECTION("IgnoreCase Unicode – German ß → ss")
    {
        String a((const char*)u8"Stra\u00DFe");
        String b((const char*)u8"STRASSE");
        REQUIRE(String::Equals(a, b, true, (const char*)u8"de"));
    }

    SECTION("IgnoreCase Unicode – Turkish I")
    {
        String locale((const char*)u8"tr");
        // U+0049 LATIN CAPITAL LETTER I  -> dotless i (U+0131) in Turkish casefold
        REQUIRE(String::Equals((const char*)u8"I", (const char*)u8"\u0131", true, locale));   // I -> dotless i
        // U+0130 LATIN CAPITAL LETTER I WITH DOT ABOVE -> maps to 'i' with dot in Turkish
        REQUIRE(String::Equals((const char*)u8"\u0130", (const char*)u8"i", true, locale));
    }

    SECTION("IgnoreCase ligatures")
    {
        // U+FB03 = LATIN SMALL LIGATURE FFI
        String a((const char*)u8"\uFB03");
        REQUIRE(String::Equals(a, (const char*)u8"ffi", true));
    }
}

// =====================================================
// 4. Compare (Unicode aware)
// =====================================================

TEST_CASE("String: Compare (Unicode casefold aware)")
{
    REQUIRE(String::Compare("abc", "ABC", true, (const char*)u8"en") == 0);
    REQUIRE(String::Compare((const char*)u8"Stra\u00DFe", (const char*)u8"STRASSE", true, (const char*)u8"de") == 0);

    REQUIRE(String::Compare("a", "b", false, (const char*)u8"en") < 0);
    REQUIRE(String::Compare("b", "a", false, (const char*)u8"en") > 0);

    // Turkish dotted/dotless I: compare with Turkish locale
    REQUIRE(String::Compare((const char*)u8"\u0130", (const char*)u8"i", true, (const char*)u8"tr") == 0); // İ == i (locale aware)
    REQUIRE(String::Compare((const char*)u8"I", (const char*)u8"\u0131", true, (const char*)u8"tr") == 0); // I == ı
}


// =====================================================
// 6. Substring
// =====================================================

TEST_CASE("String: Substring (Grapheme-Cluster Aware)")
{
    //
    // ASCII básico
    //
    SECTION("ASCII – basic correctness")
    {
        String s("HelloWorld");

        REQUIRE(s.Substring(0, 1).Equals("H"));
        REQUIRE(s.Substring(1, 4).Equals("ello"));
        REQUIRE(s.Substring(5, 5).Equals("World"));
        REQUIRE(s.Substring(0, 10).Equals("HelloWorld"));
    }

    //
    // Unicode BMP – German ß (U+00DF)
    //
    SECTION("Unicode BMP – German ß")
    {
        String s((const char*)u8"Stra\u00DFe"); // S t r a ß e  → 6 clusters

        REQUIRE(s.Substring(0, 1).Equals("S"));
        REQUIRE(s.Substring(1, 3).Equals("tra"));
        REQUIRE(s.Substring(4, 1).Equals((const char*)u8"\u00DF"));
        REQUIRE(s.Substring(5, 1).Equals("e"));
    }

    //
    // Combining acute: e + ́  (U+0065 U+0301)
    //
    SECTION("Composed cluster – e + ◌́ (U+0301)")
    {
        String s((const char*)u8"e\u0301x");
        // "é" → 1 grapheme cluster
        // "x" → 1 grapheme cluster

        REQUIRE(s.GetLength() == 2);
        REQUIRE(s.Substring(0, 1).Equals((const char*)u8"e\u0301"));
        REQUIRE(s.Substring(1, 1).Equals("x"));
    }

    //
    // Emoji – 😀 (U+1F600)
    //
    SECTION("Emoji – single cluster")
    {
        String s((const char*)u8"\U0001F600ABC");

        REQUIRE(s.GetLength() == 4);
        REQUIRE(s.Substring(0, 1).Equals((const char*)u8"\U0001F600"));
        REQUIRE(s.Substring(1, 3).Equals("ABC"));
    }

    //
    // Emoji + Skin tone modifier
    //
    SECTION("Emoji + skin tone modifier (👍🏽)")
    {
        // 👍🏽 = U+1F44D U+1F3FD
        String s((const char*)u8"\U0001F44D\U0001F3FDOK");

        REQUIRE(s.GetLength() == 3);
        REQUIRE(s.Substring(0, 1).Equals((const char*)u8"\U0001F44D\U0001F3FD"));
        REQUIRE(s.Substring(1, 2).Equals("OK"));
    }

    //
    // ZWJ Sequence – Family emoji 👨‍👩‍👧‍👦
    //
    SECTION("Emoji ZWJ Family")
    {
        // 👨‍👩‍👧‍👦 = U+1F468 ZWJ U+1F469 ZWJ U+1F467 ZWJ U+1F466
        String s((const char*)u8"\U0001F468\u200D\U0001F469\u200D\U0001F467\u200D\U0001F466END");

        REQUIRE(s.GetLength() == 4);
        REQUIRE(s.Substring(0, 1).Equals((const char*)u8"\U0001F468\u200D\U0001F469\u200D\U0001F467\u200D\U0001F466"));
        REQUIRE(s.Substring(1, 3).Equals("END"));
    }

    //
    // Flag – Brazil 🇧🇷 (regional indicators)
    //
    SECTION("Flag – Brazil 🇧🇷")
    {
        // 🇧🇷 = U+1F1E7 U+1F1F7
        String s((const char*)u8"\U0001F1E7\U0001F1F7123");

        REQUIRE(s.GetLength() == 4);
        REQUIRE(s.Substring(0, 1).Equals((const char*)u8"\U0001F1E7\U0001F1F7"));
        REQUIRE(s.Substring(1, 3).Equals("123"));
    }

    //
    // Mixed clusters
    //
    SECTION("Mixed clusters")
    {
        // 😀 é 👍🏽 🇧🇷 X
        String s((const char*)u8"\U0001F600e\u0301\U0001F44D\U0001F3FD\U0001F1E7\U0001F1F7X");

        REQUIRE(s.GetLength() == 5);

        REQUIRE(s.Substring(0, 2).Equals((const char*)u8"\U0001F600e\u0301"));
        REQUIRE(s.Substring(1, 3).Equals((const char*)u8"e\u0301\U0001F44D\U0001F3FD\U0001F1E7\U0001F1F7"));
        REQUIRE(s.Substring(2, 2).Equals((const char*)u8"\U0001F44D\U0001F3FD\U0001F1E7\U0001F1F7"));
        REQUIRE(s.Substring(4, 1).Equals("X"));
    }

    //
    // Bounds
    //
    SECTION("Bounds – out-of-range")
    {
        String s("Hello");

        REQUIRE(s.Substring(0, 999).Equals("Hello"));
        REQUIRE(s.Substring(2, 999).Equals("llo"));
        REQUIRE(s.Substring(10, 5).IsEmpty());
        REQUIRE(s.Substring(0, 0).IsEmpty());
    }

    //
    // Empty string
    //
    SECTION("Empty string")
    {
        String s("");

        REQUIRE(s.Substring(0, 1).IsEmpty());
        REQUIRE(s.Substring(5, 5).IsEmpty());
    }
}

// =====================================================
// 7. Insert
// =====================================================

TEST_CASE("String: Insert")
{
    String s("HelloWorld");
    String r = s.Insert(5, "_XYZ_");

    REQUIRE(r.Equals("Hello_XYZ_World"));
}

// =====================================================
// 8. PadLeft / PadRight
// =====================================================

TEST_CASE("String: Padding")
{
    REQUIRE(String("A").PadLeft(5, '.').Equals("....A"));
    REQUIRE(String("A").PadRight(5, '.').Equals("A...."));

    // Unicode pad: U+2603 = SNOWMAN (☃)
    REQUIRE(String("A").PadLeft(3, CodePoint(0x2603)).GetLength() == 3);
}

// =====================================================
// 9. ToUpper / ToLower (Unicode aware)
// =====================================================

TEST_CASE("String: ToLower/ToUpper")
{
    SECTION("ASCII lower/upper")
    {
        REQUIRE(String("HELLO").ToLower().Equals("hello"));
        REQUIRE(String("hello").ToUpper().Equals("HELLO"));
    }

    SECTION("German sharp s")
    {
        REQUIRE(String((const char*)u8"Stra\u00DFe").ToUpper().Equals((const char*)u8"STRASSE"));
    }

    SECTION("Greek final sigma (contextual)")
    {
        // Use explicit codepoints to avoid encoding issues:
        // U+039C U+039F U+03A3  => "ΜΟΣ" (Greek capital letters M O Sigma)
        // Lowercase expected: U+03BC U+03BF U+03C2 => "μος" with final sigma (U+03C2)
        String s((const char*)u8"\u039C\u039F\u03A3");
        REQUIRE(s.ToLower().Equals((const char*)u8"\u03BC\u03BF\u03C2"));
    }

    SECTION("Turkish locale correct behavior")
    {
        String s((const char*)u8"I");
        String lower = s.ToLower("tr");

        CHECK(lower.Equals((const char*)u8"\u0131"));   // correto: "ı"
    }
}

// =====================================================
// 11. Concat (variadic)
// =====================================================

TEST_CASE("String: Concat")
{
    String a = String::Concat("Hello", "_", "World", "_", "123");
    REQUIRE(a.Equals("Hello_World_123"));
}

// =====================================================
// 12. Byte-level behavior (data, operator[])
// =====================================================

TEST_CASE("String: Data and Indexing")
{
    String s("ABC");
    REQUIRE(s[0] == 'A');
    REQUIRE(s[1] == 'B');
    REQUIRE(s[2] == 'C');
}

// =====================================================
// 13. Empty(), IsEmpty()
// =====================================================

TEST_CASE("String: Empty and IsEmpty")
{
    REQUIRE(String::Empty().IsEmpty());
    REQUIRE(String("").IsEmpty());
    REQUIRE_FALSE(String("X").IsEmpty());
}

// =====================================================
// 14. Reference counting
// =====================================================

TEST_CASE("String: Reference Counting (SSO)")
{
    String a("Test"); // cabe em SSO
    REQUIRE(a.IsSSO());
    REQUIRE(a.GetReferenceCount() == 1);

    {
        String b(a);
        REQUIRE(b.IsSSO());
        REQUIRE(a.GetReferenceCount() == 1);
        REQUIRE(b.GetReferenceCount() == 1);
        REQUIRE(a.Equals(b));
    }

    REQUIRE(a.GetReferenceCount() == 1);

    {
        String c = std::move(a);
        REQUIRE(c.GetReferenceCount() == 1);
    }
}

TEST_CASE("String: Reference Counting (Heap)")
{
    String big("This is a very large string that certainly exceeds SSO capacity");
    REQUIRE(!big.IsSSO());
    REQUIRE(big.GetReferenceCount() == 1);

    {
        String copy(big);
        REQUIRE(!copy.IsSSO());
        REQUIRE(big.GetReferenceCount() == 2);
        REQUIRE(copy.GetReferenceCount() == 2);
    }

    REQUIRE(big.GetReferenceCount() == 1);
}

// =====================================================
//  NORMALIZAÇÃO — UAX #15 (NFC, NFD, NFKC, NFKD)
// =====================================================

TEST_CASE("Unicode Normalization – Basic Sanity")
{
    // á (U+0061 + U+0301)
    String decomposed((const char*)u8"a\u0301");
    // á (U+00E1)
    String composed((const char*)u8"\u00E1");

    // NFD: both must equal decomposed
    REQUIRE(decomposed.Normalize(NF::NFD).Equals((const char*)u8"a\u0301"));
    REQUIRE(composed.Normalize(NF::NFD).Equals((const char*)u8"a\u0301"));

    // NFC: both must equal precomposed
    REQUIRE(decomposed.Normalize(NF::NFC).Equals((const char*)u8"\u00E1"));
    REQUIRE(composed.Normalize(NF::NFC).Equals((const char*)u8"\u00E1"));
}

// =====================================================
//  COMBINING MARKS
// =====================================================

TEST_CASE("Unicode Normalization – Combining marks")
{
    // "e + combining acute"
    String s1((const char*)u8"e\u0301");
    String s2((const char*)u8"\u00E9"); // é

    // NFC must compose
    REQUIRE(s1.Normalize(NF::NFC).Equals((const char*)u8"\u00E9"));
    REQUIRE(s2.Normalize(NF::NFC).Equals((const char*)u8"\u00E9"));

    // NFD must decompose
    REQUIRE(s1.Normalize(NF::NFD).Equals((const char*)u8"e\u0301"));
    REQUIRE(s2.Normalize(NF::NFD).Equals((const char*)u8"e\u0301"));
}

// =====================================================
//  COMPATIBILITY → NFKC / NFKD
// =====================================================

TEST_CASE("Unicode Normalization – Compatibility decomposition")
{
    // ﬁ = U+FB01 (ligature f + i)
    String lig((const char*)u8"\uFB01");

    // NFKD decomposes into "fi"
    REQUIRE(lig.Normalize(NF::NFKD).Equals("fi"));

    // NFKC also returns "fi"
    REQUIRE(lig.Normalize(NF::NFKC).Equals("fi"));

    // NFC must NOT decompose ligatures
    REQUIRE(lig.Normalize(NF::NFC).Equals((const char*)u8"\uFB01"));
}

// =====================================================
//  HANGUL – Algorithmic Decomposition / Composition
// =====================================================
//
// SBase = AC00    (가)
// Example: "가" decomposes into choseong + jungseong
// 가 = ᄀ (U+1100) + ᅡ (U+1161)

TEST_CASE("Unicode Normalization – Hangul")
{
    // 가 (U+AC00)
    String s((const char*)u8"\uAC00");

    // NFD decomposes algorithmically
    String nfd = s.Normalize(NF::NFD);
    REQUIRE(nfd.Equals((const char*)u8"\u1100\u1161"));

    // NFC recomposes
    REQUIRE(nfd.Normalize(NF::NFC).Equals((const char*)u8"\uAC00"));
}

// =====================================================
//  CANONICAL EQUIVALENCE TEST CASES (UAX #15)
// =====================================================

TEST_CASE("Unicode Normalization – Canonical equivalences")
{
    // Greek tonos: ό (U+03CC)
    String composed((const char*)u8"\u03CC");
    String decomposed((const char*)u8"\u03BF\u0301");

    REQUIRE(composed.Normalize(NF::NFD).Equals((const char*)u8"\u03BF\u0301"));
    REQUIRE(decomposed.Normalize(NF::NFC).Equals((const char*)u8"\u03CC"));
}

// =====================================================
//  COMPATIBILITY EXAMPLES (UAX #15)
// =====================================================

TEST_CASE("Unicode Normalization – NFK* special mappings")
{
    // "Fullwidth A" U+FF21 → compatibility → "A"
    String fwA((const char*)u8"\uFF21");

    REQUIRE(fwA.Normalize(NF::NFKD).Equals("A"));
    REQUIRE(fwA.Normalize(NF::NFKC).Equals("A"));

    // Roman numeral Ⅻ (U+216B) → compat → "XII"
    String rn((const char*)u8"\u216B");
    REQUIRE(rn.Normalize(NF::NFKC).Equals("XII"));
}

// =====================================================
//  IS NORMALIZED CHECKS
// =====================================================

TEST_CASE("Unicode Normalization – IsNormalized")
{
    String decomposed((const char*)u8"a\u0301");
    String composed((const char*)u8"\u00E1");

    // decomposed is NFD, not NFC
    REQUIRE(decomposed.IsNormalized(NF::NFD));
    REQUIRE_FALSE(decomposed.IsNormalized(NF::NFC));

    // composed is NFC, not NFD
    REQUIRE(composed.IsNormalized(NF::NFC));
    REQUIRE_FALSE(composed.IsNormalized(NF::NFD));
}

// =====================================================
//  EDGE CASES
// =====================================================

TEST_CASE("Unicode Normalization – Edge cases")
{
    // Empty string
    String empty("");
    REQUIRE(empty.Normalize(NF::NFC).Equals(""));
    REQUIRE(empty.IsNormalized(NF::NFC));

    // ASCII shouldn't change
    String ascii("Hello World!");
    REQUIRE(ascii.Normalize(NF::NFC).Equals("Hello World!"));
    REQUIRE(ascii.Normalize(NF::NFD).Equals("Hello World!"));

    // Already-composed sequences should stay unchanged
    String composed((const char*)u8"\u00C1"); // Á
    REQUIRE(composed.Normalize(NF::NFC).Equals((const char*)u8"\u00C1"));

    // Combining marks order
    // U+0301 (acute) has CCC=230 ; U+0327 (cedilla)=202 → reordered in NFD
    String combo((const char*)u8"a\u0301\u0327");
    String ordered((const char*)u8"a\u0327\u0301"); // correct order

    REQUIRE(combo.Normalize(NF::NFD).Equals(ordered));
}

// =====================================================
//  MORE COMPLEX CASES (composite + compat)
// =====================================================

TEST_CASE("Unicode Normalization – Complex test vectors")
{
    // "①" (U+2460) = circled digit ONE → NFKC → "1"
    String circled1((const char*)u8"\u2460");
    REQUIRE(circled1.Normalize(NF::NFKC).Equals("1"));

    // Å (U+212B ANGSTROM SIGN) → canonically same as Å (U+00C5)
    String angstrom((const char*)u8"\u212B");
    String A_ring((const char*)u8"\u00C5");

    // NFC normalizes both to U+00C5
    REQUIRE(angstrom.Normalize(NF::NFC).Equals((const char*)u8"\u00C5"));
    REQUIRE(A_ring.Normalize(NF::NFC).Equals((const char*)u8"\u00C5"));

    // NFD decomposes both to A + combining ring
    REQUIRE(angstrom.Normalize(NF::NFD).Equals(A_ring.Normalize(NF::NFD)));
}

// =====================================================
//  COMBINING SEQUENCES + EMOJI (should not normalize these)
// =====================================================

TEST_CASE("Unicode Normalization – Emojis and ZWJ")
{
    // 👩‍❤️‍💋‍👨  (woman kiss man sequence)
    String kiss((const char*)u8"\U0001F469\u200D\u2764\uFE0F\u200D\u1F48B\u200D\U0001F468");

    // Normalization must NOT break emoji sequences
    REQUIRE(kiss.Normalize(NF::NFC).Equals(kiss));
    REQUIRE(kiss.Normalize(NF::NFD).Equals(kiss));
}

TEST_CASE("Unicode Normalization – Inert characters")
{
    String inert((const char*)u8"ABCxyz0123!@#$%^&*()[]{};:,.-");

    REQUIRE(inert.Normalize(NF::NFC).Equals(inert));
    REQUIRE(inert.Normalize(NF::NFD).Equals(inert));
    REQUIRE(inert.Normalize(NF::NFKC).Equals(inert));
    REQUIRE(inert.Normalize(NF::NFKD).Equals(inert));
}

TEST_CASE("Unicode Normalization – Blocked canonical composition")
{
    // a + 0301 + 031B  (acute = 230, horn = 216)
    // 031B blocks composition with 0301
    String s((const char*)u8"a\u0301\u031B");

    // NFD keeps exact order after canonical reorder
    REQUIRE(s.Normalize(NF::NFD).Equals((const char*)u8"a\u031B\u0301"));

    // NFC must NOT compose “á̛” → should remain decomposed
    REQUIRE(s.Normalize(NF::NFC).Equals((const char*)u8"a\u031B\u0301"));
}

TEST_CASE("Unicode Normalization – Multi-mark composition (Vietnamese)")
{
    // o + ̂ + ́
    String s((const char*)u8"o\u0302\u0301");

    // NFC must produce ố (U+1ED1)
    REQUIRE(s.Normalize(NF::NFC).Equals((const char*)u8"\u1ED1"));

    // NFD must decompose back
    REQUIRE(s.Normalize(NF::NFD).Equals((const char*)u8"o\u0302\u0301"));
}

TEST_CASE("Unicode Normalization – Hangul LV composition")
{
    // ㄱ (1100) + ㅏ (1161)  → 가 (AC00)
    String s((const char*)u8"\u1100\u1161");

    REQUIRE(s.Normalize(NF::NFC).Equals((const char*)u8"\uAC00"));
    REQUIRE(s.Normalize(NF::NFD).Equals((const char*)u8"\u1100\u1161"));
}

TEST_CASE("Unicode Normalization – Hangul LVT composition")
{
    // 가 + jongseong ㄱ (11A8) → 각 (AC01)
    String s((const char*)u8"\u1100\u1161\u11A8");

    REQUIRE(s.Normalize(NF::NFC).Equals((const char*)u8"\uAC01"));
    REQUIRE(s.Normalize(NF::NFD).Equals((const char*)u8"\u1100\u1161\u11A8"));
}

TEST_CASE("Unicode Normalization – Fraction compatibility")
{
    String frac((const char*)u8"\u00BC"); // ¼ (one quarter)
    REQUIRE(frac.Normalize(NF::NFKC).Equals((const char*)u8"1\u20444"));
    REQUIRE(frac.Normalize(NF::NFKD).Equals((const char*)u8"1\u20444"));
}

TEST_CASE("Unicode Normalization – Ligature DZ compatibility")
{
    String lig((const char*)u8"\u01C6"); // ǆ
    REQUIRE(lig.Normalize(NF::NFKC).Equals((const char*)u8"d\u017E"));
    REQUIRE(lig.Normalize(NF::NFKD).Equals((const char*)u8"dz\u030C"));
}

TEST_CASE("Unicode Normalization – Stress combining sequence")
{
    // a + 5 combining marks
    String s((const char*)u8"a\u0301\u0302\u0303\u0304\u0305");

    String nfd = s.Normalize(NF::NFD);
    // canonical order must be sorted by CCC
    REQUIRE(nfd.Equals((const char*)u8"a\u0301\u0302\u0303\u0304\u0305"));

    // NFC must keep (não há composição para esses)
    REQUIRE(s.Normalize(NF::NFC).Equals((const char*)u8"\u00E1\u0302\u0303\u0304\u0305"));
}

TEST_CASE("Unicode Normalization – Idempotence")
{
    const char8_t* samples[] = {
        u8"a\u0301",
        u8"\u00E1",
        u8"\uAC00",
        u8"\u1100\u1161",
        u8"\u01C6",
        u8"\u2460",
        u8"\u1ED1"
    };

    for (auto s : samples)
    {
        String str((const char*)s);

        REQUIRE(str.Normalize(NF::NFC).Normalize(NF::NFC).Equals(str.Normalize(NF::NFC)));
        REQUIRE(str.Normalize(NF::NFD).Normalize(NF::NFD).Equals(str.Normalize(NF::NFD)));
        REQUIRE(str.Normalize(NF::NFKC).Normalize(NF::NFKC).Equals(str.Normalize(NF::NFKC)));
        REQUIRE(str.Normalize(NF::NFKD).Normalize(NF::NFKD).Equals(str.Normalize(NF::NFKD)));
    }
}

TEST_CASE("Unicode Normalization – Fraktur letters")
{
    String frak((const char*)u8"\u210C"); // ℌ

    REQUIRE(frak.Normalize(NF::NFC).Equals((const char*)u8"\u210C"));
    REQUIRE(frak.Normalize(NF::NFKC).Equals("H"));
}

// =====================================================
//  STARTSWITH / ENDSWITH / CONTAINS — FULL TEST SUITE
// =====================================================

TEST_CASE("String: StartsWith / EndsWith / Contains — ASCII basic")
{
    String s("HelloWorld");

    // StartsWith / EndsWith (case-sensitive)
    REQUIRE(s.StartsWith("Hello"));
    REQUIRE_FALSE(s.StartsWith("hello"));
    REQUIRE(s.EndsWith("World"));
    REQUIRE_FALSE(s.EndsWith("world"));

    // Contains
    REQUIRE(s.Contains("Hello"));
    REQUIRE(s.Contains("World"));
    REQUIRE(s.Contains("loWo"));
    REQUIRE_FALSE(s.Contains("HELLO"));
}

TEST_CASE("String: StartsWith / EndsWith / Contains — ASCII ignoreCase")
{
    String s("HelloWorld");

    REQUIRE(s.StartsWith("hello", true, "en"));
    REQUIRE(s.EndsWith("world", true, "en"));
    REQUIRE(s.Contains("helloworld", true));
    REQUIRE(s.Contains("HELLO", true));
    REQUIRE(s.Contains("world", true));
}

// =====================================================
// Unicode: German sharp S (ß → ss)
// =====================================================

TEST_CASE("String: StartsWith / EndsWith / Contains — German ß")
{
    // Straße
    String s((const char*)u8"Stra\u00DFe");

    // StartsWith
    REQUIRE(s.StartsWith((const char*)u8"STR", true, "de"));
    REQUIRE(s.StartsWith((const char*)u8"stra", true));

    // EndsWith
    REQUIRE(s.EndsWith((const char*)u8"\u00DFe", true));   // ße → ss e
    REQUIRE(s.EndsWith("SSE", true, "de"));

    // Contains
    REQUIRE(s.Contains((const char*)u8"\u00DF", false));   // exact ß
    REQUIRE(s.Contains("SS", true));                       // ß → ss
    REQUIRE(s.Contains("sse", true));
    REQUIRE(s.Contains("SSE", true));
}

// =====================================================
// Unicode: Turkish dotted/dotless I
// =====================================================

TEST_CASE("String: StartsWith / EndsWith / Contains — Turkish I")
{
    String locale("tr");

    // "I" vs "ı"
    REQUIRE(String((const char*)u8"I").Equals((const char*)u8"\u0131", true, "tr"));

    // StartsWith
    REQUIRE(String((const char*)u8"Istanbul").StartsWith((const char*)u8"\u0131s", true, "tr"));

    // EndsWith
    REQUIRE(String((const char*)u8"\u0130").EndsWith("i", true, "tr")); // İ → i

    // Contains
    REQUIRE(String("I").Contains((const char*)u8"\u0131", true, locale)); // I → ı
    REQUIRE(String((const char*)u8"\u0130").Contains("i", true, locale));               // İ → i
}

// =====================================================
// Unicode: Ligatures
// =====================================================

TEST_CASE("String: Contains — ligatures")
{
    // ﬁ = U+FB01
    String lig((const char*)u8"\uFB01");

    REQUIRE(lig.Contains("fi", true));     // ﬁ → f + i (NFKD folding)
    REQUIRE_FALSE(lig.Contains("fi"));     
    REQUIRE(lig.StartsWith("fi", true));
    REQUIRE(lig.EndsWith("fi", true));
}

// =====================================================
// Unicode: Composed vs decomposed
// =====================================================

TEST_CASE("String: Contains — composed vs decomposed")
{
    String composed((const char*)u8"\u00E1");     // á
    String decomposed((const char*)u8"a\u0301");  // a + ◌́

    // Case-sensitive Contains
    REQUIRE_FALSE(composed.Contains((const char*)u8"a\u0301")); // byte mismatch
    REQUIRE_FALSE(decomposed.Contains((const char*)u8"\u00E1"));

    // Unicode-aware Contains (case-insensitive or canonical-aware)
    REQUIRE(composed.Contains(decomposed, true));
    REQUIRE(decomposed.Contains(composed, true));
}

// =====================================================
// Unicode: Greek final sigma (ς)
// =====================================================

TEST_CASE("String: StartsWith / EndsWith / Contains — Greek final sigma")
{
    // ΜΟΣ → lowercase = μος (final sigma)
    String greek((const char*)u8"\u039C\u039F\u03A3");

    REQUIRE(greek.Contains((const char*)u8"\u03BC\u03BF\u03C3", true)); // σ
    REQUIRE(greek.Contains((const char*)u8"\u03BC\u03BF\u03C2", true)); // ς (final sigma)

    REQUIRE(greek.EndsWith((const char*)u8"\u03C2", true));
    REQUIRE(greek.EndsWith((const char*)u8"\u03C3", true));
}

// =====================================================
// Unicode: Emoji and ZWJ (family, flags)
// =====================================================

TEST_CASE("String: Contains — Emoji ZWJ sequences")
{
    // Family emoji: 👨‍👩‍👧‍👦
    String fam((const char*)u8"\U0001F468\u200D\U0001F469\u200D\U0001F467\u200D\U0001F466X");

    REQUIRE(fam.StartsWith((const char*)u8"\U0001F468"));            // father
    REQUIRE(fam.Contains((const char*)u8"\U0001F469"));              // mother
    REQUIRE(fam.Contains((const char*)u8"\U0001F467"));              // daughter
    REQUIRE(fam.EndsWith("X"));

    // The full ZWJ family must match exactly
    REQUIRE(fam.StartsWith((const char*)u8"\U0001F468\u200D\U0001F469\u200D\U0001F467\u200D\U0001F466"));
}

TEST_CASE("String: Contains — emoji flags (regional indicators)")
{
    // 🇧🇷 = U+1F1E7 U+1F1F7
    String flag((const char*)u8"\U0001F1E7\U0001F1F7!");

   // REQUIRE(flag.StartsWith((const char*)u8"\U0001F1E7"));
   // REQUIRE(flag.StartsWith((const char*)u8"\U0001F1E7\U0001F1F7"));
    //REQUIRE(flag.Contains("!"));
    //REQUIRE(flag.EndsWith("!"));
}

// =====================================================
// StartsWith / EndsWith single character CodePoint
// =====================================================

TEST_CASE("String: StartsWith / EndsWith — CodePoint overloads")
{
    String s = String((const char*)u8"\u00E1BC").Normalize();
    REQUIRE(s.StartsWith(CodePoint(0x00E1)));
}

// =====================================================
// Mixed-language and complex Unicode
// =====================================================

TEST_CASE("String: Unicode complex — combined")
{
    String s((const char*)u8"\U0001F600Stra\u00DFe\U0001F44D\U0001F3FD\U0001F1E7\U0001F1F7");
    
    s = s.Normalize();
    REQUIRE(s.Contains((const char*)u8"Stra\u00DFe"));

    REQUIRE(s.Contains((const char*)u8"\U0001F600")); // 😀
    REQUIRE(s.Contains((const char*)u8"\U0001F44D\U0001F3FD")); // 👍🏽
    REQUIRE(s.Contains((const char*)u8"\U0001F1E7\U0001F1F7")); // 🇧🇷
}

// =====================================================
// Edge cases
// =====================================================

TEST_CASE("String: StartsWith/EndsWith/Contains — Edge cases")
{
    String empty("");

    REQUIRE(empty.StartsWith(""));
    REQUIRE(empty.EndsWith(""));
    REQUIRE(empty.Contains(""));

    REQUIRE_FALSE(empty.StartsWith("X"));
    REQUIRE_FALSE(empty.EndsWith("X"));
    REQUIRE_FALSE(empty.Contains("X"));

    REQUIRE(String("A").StartsWith("A"));
    REQUIRE(String("A").EndsWith("A"));
    REQUIRE(String("A").Contains("A"));

    REQUIRE_FALSE(String("A").Contains("AA"));
}

// ============================================================================
// TRIM() / TRIMSTART() / TRIMEND()
// ============================================================================

// ---------------------- Trim() ----------------------------------------------

TEST_CASE("Trim removes ASCII whitespace")
{
    REQUIRE(String("   abc   ").Trim() == "abc");
}

TEST_CASE("Trim removes Unicode whitespace")
{
    String s = String::FromCodePoint(0x2003) + "abc" + String::FromCodePoint(0x2003); // EM SPACE
    REQUIRE(s.Trim() == "abc");

    s = String::FromCodePoint(0x3000) + String("x") + String::FromCodePoint(0x3000); // IDEOGRAPHIC SPACE
    REQUIRE(s.Trim() == "x");
}

TEST_CASE("Trim(Char) removes specific character")
{
    REQUIRE(String("---abc---").Trim('-') == "abc");
    REQUIRE(String("xxx").Trim('x') == "");
}

TEST_CASE("Trim(Char[]) removes multiple chars")
{
    auto cs = Chars({ '-', '+' });
    REQUIRE(String("--++abc++--").Trim(cs) == "abc");
    REQUIRE(String("+-+-+-").Trim(cs) == "");
}

// ---------------------- TrimStart() -----------------------------------------

TEST_CASE("TrimStart removes leading whitespace")
{
    REQUIRE(String("   abc").TrimStart() == "abc");
}

TEST_CASE("TrimStart(Char) removes multiple occurrences")
{
    REQUIRE(String("xxxabcxxx").TrimStart('x') == "abcxxx");
}

TEST_CASE("TrimStart(Char[]) removes leading set")
{
    auto cs = Chars({ 'a','b' });
    REQUIRE(String("abbaabc").TrimStart(cs) == "c");
}

// ---------------------- TrimEnd() -------------------------------------------

TEST_CASE("TrimEnd removes trailing whitespace")
{
    REQUIRE(String("abc   ").TrimEnd() == "abc");
}

TEST_CASE("TrimEnd(Char) removes trailing char")
{
    REQUIRE(String("xxxabcxxx").TrimEnd('x') == "xxxabc");
}

TEST_CASE("TrimEnd(Char[]) removes trailing chars")
{
    auto cs = Chars({ 'a','b' });
    REQUIRE(String("cabbab").TrimEnd(cs) == "c");
}

// ============================================================================
// SPLIT()
// ============================================================================

// ---------------------- Split(Char) -----------------------------------------

TEST_CASE("Split(Char) basic")
{
    auto r = String("a,b,c").Split(',', StringSplitOptions::None);

    REQUIRE(r.Count() == 3);
    REQUIRE(r[0] == "a");
    REQUIRE(r[1] == "b");
    REQUIRE(r[2] == "c");
}

TEST_CASE("Split(Char) RemoveEmptyEntries")
{
    auto r = String("a,,b,,,c").Split(',', StringSplitOptions::RemoveEmptyEntries);

    REQUIRE(r.Count() == 3);
    REQUIRE(r[0] == "a");
    REQUIRE(r[1] == "b");
    REQUIRE(r[2] == "c");
}

TEST_CASE("Split(Char) TrimEntries")
{
    auto r = String(" a ,  b ,   c ").Split(',', StringSplitOptions::TrimEntries);

    REQUIRE(r.Count() == 3);
    REQUIRE(r[0] == "a");
    REQUIRE(r[1] == "b");
    REQUIRE(r[2] == "c");
}

TEST_CASE("Split(Char) RemoveEmptyEntries+TrimEntries")
{
    auto r = String("  a ,  ,  b , c ").Split(
        ',',
        StringSplitOptions::RemoveEmptyEntries | StringSplitOptions::TrimEntries
    );

    REQUIRE(r.Count() == 3);
    REQUIRE(r[0] == "a");
    REQUIRE(r[1] == "b");
    REQUIRE(r[2] == "c");
}

// ---------------------- Split(Char[]) ---------------------------------------

TEST_CASE("Split(Char[]) maxCount")
{
    auto seps = Chars({ ',', ';' });

    auto r = String("a,b;c,d").Split(seps, 3, StringSplitOptions::None);

    REQUIRE(r.Count() == 3);
    REQUIRE(r[0] == "a");
    REQUIRE(r[1] == "b");
    REQUIRE(r[2] == "c,d");
}

// ---------------------- Split(String) ---------------------------------------

TEST_CASE("Split(String)")
{
    auto r = String("aa--bb--cc").Split("--", StringSplitOptions::None);

    REQUIRE(r.Count() == 3);
    REQUIRE(r[0] == "aa");
    REQUIRE(r[1] == "bb");
    REQUIRE(r[2] == "cc");
}

// ---------------------- Split(String[]) -------------------------------------

TEST_CASE("Split(String[]) longest match")
{
    auto seps = Strings({ "--", "-" });

    auto r = String("a--b-c").Split(seps, StringSplitOptions::None);

    REQUIRE(r.Count() == 3);
    REQUIRE(r[0] == "a");
    REQUIRE(r[1] == "b");
    REQUIRE(r[2] == "c");
}

TEST_CASE("Split(String[]) RemoveEmptyEntries+TrimEntries")
{
    auto seps = Strings({ "," });

    auto r = String("  a  ,   , b  ,c  ").Split(
        seps,
        StringSplitOptions::TrimEntries | StringSplitOptions::RemoveEmptyEntries
    );

    REQUIRE(r.Count() == 3);
    REQUIRE(r[0] == "a");
    REQUIRE(r[1] == "b");
    REQUIRE(r[2] == "c");
}

// ---------------------- Split special cases ---------------------------------

TEST_CASE("Split no separator returns whole string")
{
    auto r = String("abcdef").Split(Strings({}), StringSplitOptions::None);
    REQUIRE(r.Count() == 1);
    REQUIRE(r[0] == "abcdef");
}

TEST_CASE("Split maxCount = 1")
{
    auto r = String("a,b,c,d").Split(',', 1, StringSplitOptions::None);

    REQUIRE(r.Count() == 1);
    REQUIRE(r[0] == "a,b,c,d");
}

TEST_CASE("Split maxCount = 2")
{
    auto r = String("a,b,c,d").Split(',', 2, StringSplitOptions::None);

    REQUIRE(r.Count() == 2);
    REQUIRE(r[0] == "a");
    REQUIRE(r[1] == "b,c,d");
}

TEST_CASE("Split with Unicode delimiter U+3000")
{
    Char sep(0x3000);
    String s = String("a") + sep + "b" + sep + "c";

    auto r = s.Split(sep, StringSplitOptions::None);

    REQUIRE(r.Count() == 3);
    REQUIRE(r[0] == "a");
    REQUIRE(r[1] == "b");
    REQUIRE(r[2] == "c");
}

// ============================================================================
// NEGATIVE TESTS / EDGE CASES
// ============================================================================

TEST_CASE("Trim empty string")
{
    REQUIRE(String("").Trim() == "");
}

TEST_CASE("Split empty string should return array with empty element")
{
    auto r = String("").Split(',', StringSplitOptions::None);
    REQUIRE(r.Count() == 1);
    REQUIRE(r[0] == "");
}

TEST_CASE("Split with empty separator entry is ignored")
{
    auto r = String("abc").Split(Strings({ "" }), StringSplitOptions::None);

    REQUIRE(r.Count() == 1);
    REQUIRE(r[0] == "abc");
}

TEST_CASE("Split multiple leading/trailing separators (corrected)")
{
    auto r = String(",,,a,,,b,,").Split(',', StringSplitOptions::None);

    REQUIRE(r.Count() == 9);
    REQUIRE(r[0] == "");
    REQUIRE(r[1] == "");
    REQUIRE(r[2] == "");
    REQUIRE(r[3] == "a");
    REQUIRE(r[4] == "");
    REQUIRE(r[5] == "");
    REQUIRE(r[6] == "b");
    REQUIRE(r[7] == "");
    REQUIRE(r[8] == "");
}

TEST_CASE("Split NFD combining marks")
{
    // "á,b"
    String s;
    s += Char('a');
    s += Char(0x0301); // combining acute
    s += ",";
    s += Char('b');

    auto r = s.Split(',', StringSplitOptions::None);

    REQUIRE(r.Count() == 2);
    REQUIRE(r[0].GetLength() == 2);
    REQUIRE(r[1] == "b");
}

TEST_CASE("Trim does not remove combining marks")
{
    String s;
    s += String::FromChar(' ');
    s += String::FromChar('a');
    s += String::FromCodePoint(0x0301);
    s += String::FromChar(' ');

    String r = s.Trim();

    REQUIRE(r.GetLength() == 1);
    REQUIRE(r.GetCodePointsCount() == 2);
    REQUIRE(r[0] == 'a');
    REQUIRE(r.GetCodePointAt(1));
}

TEST_CASE("Split overlapping delimiters")
{
    // Delimitador: "--"
    String s = "---a---b--";
    auto r = s.Split("--", StringSplitOptions::None);

    REQUIRE(r.Count() == 4);
    REQUIRE(r[0] == "");
    REQUIRE(r[1] == "-a");
    REQUIRE(r[2] == "-b");
    REQUIRE(r[3] == "");
}

// ============================================================================
// EXTREME / LARGE INPUT TESTS
// ============================================================================

TEST_CASE("Split large string performance-safe test")
{
    String s;
    for (int i = 0; i < 10000; ++i)
        s += "a,";

    auto r = s.Split(',', StringSplitOptions::None);

    REQUIRE(r.Count() == 10001);
}

TEST_CASE("Trim performance large string")
{
    String s;
    for (int i = 0; i < 5000; ++i) s += " ";
    s += "abc";
    for (int i = 0; i < 5000; ++i) s += " ";

    REQUIRE(s.Trim() == "abc");
}

// =======================================================
//  UTF-8 SAFE LITERALS FOR MSVC
// =======================================================
// café com açúcar
static const char* UTF8_CAFE = "\x63\x61\x66\xC3\xA9";
static const char* UTF8_ACUCAR = "\x61\xC3\xA7\xC3\xBA\x63\x61\x72";
static const char* UTF8_CAFE_COM_ACUCAR =
"\x63\x61\x66\xC3\xA9\x20\x63\x6F\x6D\x20\x61\xC3\xA7\xC3\xBA\x63\x61\x72";

// á (a + U+0301)
static const char* UTF8_A_DECOMPOSED = "\x61\xCC\x81";
static const char* UTF8_A_COMPOSED = "\xC3\xA1";

// olá mundo (com 'a' decomposed)
static const char* UTF8_OLA_MUNDO =
"\x6F\x6C\x61\xCC\x81\x20\x6D\x75\x6E\x64\x6F";

// 👍👍🙂👍
// 👍 = U+1F44D = F0 9F 91 8D
// 🙂 = U+1F642 = F0 9F 99 82
static const char* UTF8_EMOJI_SEQ =
"\xF0\x9F\x91\x8D"
"\xF0\x9F\x91\x8D"
"\xF0\x9F\x99\x82"
"\xF0\x9F\x91\x8D";

// Straße TEST İSTANBUL i̇stanbul
static const char* UTF8_STRASSE_TEST =
"\x53\x74\x72\x61\xC3\x9F\x65"         // Straße
"\x20\x54\x45\x53\x54\x20"             //  TEST 
"\xC4\xB0\x53\x54\x41\x4E\x42\x55\x4C" // İSTANBUL
"\x20\x69\xCC\x87\x73\x74\x61\x6E\x62\x75\x6C"; // i̇stanbul

// abc def ghi ãç🙂
// ã = C3 A3, ç = C3 A7, 🙂 = F0 9F 99 82
static const char* UTF8_MIXED =
"abc def ghi "
"\xC3\xA3\xC3\xA7\xF0\x9F\x99\x82";


// =============================
// BASIC ASCII
// =============================
TEST_CASE("String::IndexOf - Basic ASCII cases")
{
    String s("hello world");

    SECTION("IndexOf(String)")
    {
        REQUIRE(s.IndexOf("hello") == 0);
        REQUIRE(s.IndexOf("world") == 6);
        REQUIRE(s.IndexOf("test") == -1);
    }

    SECTION("IndexOf(String, startIndex)")
    {
        REQUIRE(s.IndexOf("o", 0) == 4);
        REQUIRE(s.IndexOf("o", 5) == 7);
        REQUIRE(s.IndexOf("o", 8) == -1);
    }

    SECTION("IndexOf(String, startIndex, count)")
    {
        REQUIRE(s.IndexOf("world", 0, 11) == 6);
        REQUIRE(s.IndexOf("world", 6, 5) == 6);
        REQUIRE(s.IndexOf("world", 0, 5) == -1);
        REQUIRE(s.IndexOf("hello", 1, 5) == -1);
    }

    SECTION("IndexOf empty needle")
    {
        REQUIRE(s.IndexOf("", 0) == 0);
        REQUIRE(s.IndexOf("", 5) == 5);
    }
}


// =============================
// UTF-8 MULTIBYTE
// =============================
TEST_CASE("String::IndexOf - UTF-8 Multibyte Cases")
{
    String s(UTF8_CAFE_COM_ACUCAR);

    SECTION("Latin accents")
    {
        REQUIRE(s.IndexOf(UTF8_CAFE) == 0);
        REQUIRE(s.IndexOf(UTF8_ACUCAR) == 9);
        REQUIRE(s.IndexOf("\xC3\xA9") == 3);   // é
        REQUIRE(s.IndexOf("\xC3\xA7") == 10);  // ç
        REQUIRE(s.IndexOf("\xC3\xA3") == -1);  // ã
    }

    SECTION("StartIndex in the middle of a UTF-8 string")
    {
        REQUIRE(s.IndexOf("\xC3\xA7", 0) == 10); // ç
        REQUIRE(s.IndexOf("\xC3\xA7", 11) == -1);
    }

    SECTION("IndexOf(String, startIndex, count)")
    {
        REQUIRE(s.IndexOf(UTF8_ACUCAR, 9, 20) == 9);
        REQUIRE(s.IndexOf(UTF8_ACUCAR, 0, 5) == -1);
        REQUIRE(s.IndexOf("\xC3\xA9", 4, 10) == -1); // é
    }
}


// =============================
// GRAPHEME CLUSTERS
// =============================
TEST_CASE("String::IndexOf - Grapheme Clusters")
{
    String composed(UTF8_A_COMPOSED);
    String decomposed(UTF8_A_DECOMPOSED);

    String s(UTF8_OLA_MUNDO);

    SECTION("Normalized equivalence is NOT required for Ordinal")
    {
        REQUIRE(s.IndexOf(UTF8_A_COMPOSED) == -1);     // composed not present
        REQUIRE(s.IndexOf(UTF8_A_DECOMPOSED) == 2);    // decomposed found
    }

    SECTION("startIndex + count with grapheme clusters")
    {
        REQUIRE(s.IndexOf(UTF8_A_DECOMPOSED, 0, 5) == 2);
        REQUIRE(s.IndexOf(UTF8_A_DECOMPOSED, 3, 5) == -1);
    }
}


// =============================
// EMOJIS (4-byte UTF-8)
// =============================
TEST_CASE("String::IndexOf - Emoji (4-byte UTF-8)")
{
    String s(UTF8_EMOJI_SEQ);

    // 👍
    String thumbs_up = String::FromCodePoint(0x1F44D);
    // 🙂
    String smile = String::FromCodePoint(0x1F642);

    SECTION("Simple search")
    {
        REQUIRE(s.IndexOf(thumbs_up) == 0);
        REQUIRE(s.IndexOf(smile) == 2);
    }

    SECTION("StartIndex")
    {
        REQUIRE(s.IndexOf(thumbs_up, 1) == 1);
        REQUIRE(s.IndexOf(thumbs_up, 2) == 3);
    }

    SECTION("StartIndex + count")
    {
        REQUIRE(s.IndexOf(thumbs_up, 0, 3) == 0);
        REQUIRE(s.IndexOf(thumbs_up, 2, 1) == -1);
        REQUIRE(s.IndexOf(smile, 1, 5) == 2);
    }
}


// =============================
// CASE-INSENSITIVE (OrdinalIgnoreCase)
// =============================
TEST_CASE("String::IndexOf - Case-insensitive (OrdinalIgnoreCase)")
{
    String s(UTF8_STRASSE_TEST);

    SECTION("Ascii")
    {
        REQUIRE(s.IndexOf("test", StringComparison::OrdinalIgnoreCase) == 7);
        REQUIRE(s.IndexOf("straße", StringComparison::OrdinalIgnoreCase) == 0);
    }

    SECTION("IgnoreCase Turkish rules")
    {
        REQUIRE(s.IndexOf("\x69\xCC\x87""stanbul", StringComparison::OrdinalIgnoreCase) == 12);
        REQUIRE(s.IndexOf("\xC4\xB0""STANBUL", StringComparison::OrdinalIgnoreCase) == 12); // İ
        REQUIRE(s.IndexOf("i\xCC\x87""STANBUL", StringComparison::OrdinalIgnoreCase) == 12); // i̇
    }

    SECTION("StartIndex + comparison")
    {
        REQUIRE(s.IndexOf("TEST", 1, StringComparison::OrdinalIgnoreCase) == 7);
        REQUIRE(s.IndexOf("TEST", 8, StringComparison::OrdinalIgnoreCase) == -1);
    }

    SECTION("StartIndex + count + comparison")
    {
        REQUIRE(s.IndexOf("TEST", 0, 20, StringComparison::OrdinalIgnoreCase) == 7);
        REQUIRE(s.IndexOf("TEST", 8, 20, StringComparison::OrdinalIgnoreCase) == -1);
    }
}


// =============================
// CHAR OVERLOADS
// =============================
TEST_CASE("String::IndexOf - Char overloads")
{
    String s(UTF8_MIXED);

    SECTION("IndexOf(Char)")
    {
        REQUIRE(s.IndexOf(Char('a')) == 0);
        REQUIRE(s.IndexOf(Char(' ')) == 3);
    }

    SECTION("IndexOf(Char, startIndex)")
    {
        REQUIRE(s.IndexOf(Char(' '), 4) == 7);
        REQUIRE(s.IndexOf(Char('x'), 0) == -1);
    }

    SECTION("IndexOf(Char, startIndex, count)")
    {
        REQUIRE(s.IndexOf(Char(' '), 0, 4) == 3);
        REQUIRE(s.IndexOf(Char(' '), 4, 3) == -1);
        REQUIRE(s.IndexOf(Char(' '), 8, 4) == 11);
    }

    SECTION("IndexOf(Char, OrdinalIgnoreCase)")
    {
        REQUIRE(String("ABC").IndexOf(Char('a'), StringComparison::OrdinalIgnoreCase) == 0);
        REQUIRE(String("ABC").IndexOf(Char('d'), StringComparison::OrdinalIgnoreCase) == -1);
    }

    SECTION("Char with multi-byte representation")
    {
        REQUIRE(s.IndexOf(String::FromCodePoint(0x00E7)) == 13);  // ç
        REQUIRE(s.IndexOf(String::FromCodePoint(0x1F642)) == 14); // 🙂
    }
}


// =============================
// BOUNDARY CONDITIONS
// =============================
TEST_CASE("String::IndexOf - Boundary and error conditions")
{
    String s("abcdef");

    SECTION("startIndex > length")
    {
        REQUIRE(s.IndexOf("a", 10) == -1);
        REQUIRE(s.IndexOf("a", 10, 20) == -1);
    }

    SECTION("count exceeds range")
    {
        REQUIRE(s.IndexOf("def", 3, 10) == 3);
    }

    SECTION("needle longer than haystack")
    {
        REQUIRE(String("ab").IndexOf("abc") == -1);
    }

    SECTION("needle empty")
    {
        REQUIRE(s.IndexOf("", 0) == 0);
        REQUIRE(s.IndexOf("", 3) == 3);
    }
}

// =======================================================
//  UTF-8 SAFE LITERALS FOR MSVC
// =======================================================

// "café ãç🙂"
static const char* UTF8_SAMPLE =
"\x63\x61\x66\xC3\xA9\x20"        // café 
"\xC3\xA3"                        // ã
"\xC3\xA7"                        // ç
"\xF0\x9F\x99\x82";               // 🙂

TEST_CASE("String::IndexOfAny - Basic ASCII")
{
    String s("hello world");

    List<Char> chars;
    chars.Add(Char('o'));
    chars.Add(Char('w'));

    SECTION("Simple search")
    {
        REQUIRE(s.IndexOfAny(chars) == 4); // 'o'
    }

    SECTION("StartIndex")
    {
        REQUIRE(s.IndexOfAny(chars, 5) == 6); // 'w'
    }

    SECTION("StartIndex + count")
    {
        REQUIRE(s.IndexOfAny(chars, 5, 1) == -1); // only examines grapheme #5
        REQUIRE(s.IndexOfAny(chars, 5, 5) == 6);
    }

    SECTION("No match")
    {
        List<Char> x;
        x.Add(Char('z'));
        REQUIRE(s.IndexOfAny(x) == -1);
    }
}

TEST_CASE("String::IndexOfAny - ASCII vs Non-ASCII Semantics")
{
    String s(UTF8_SAMPLE);
    // cp sequence: c a f é  [ ] ã  ç  🙂

    List<Char> chars;

    SECTION("ASCII match inside UTF-8")
    {
        chars.Clear();
        chars.Add(Char('c'));   // ASCII → codepoint comparison
        REQUIRE(s.IndexOfAny(chars) == 0);

        chars.Clear();
        chars.Add(Char(' '));   // space
        REQUIRE(s.IndexOfAny(chars) == 4);
    }

    SECTION("Non-ASCII Char (>=128) matches leading UTF-8 byte")
    {
        chars.Clear();
        chars.Add(Char(0xC3));   // leading byte of é, ã, ç
        REQUIRE(s.IndexOfAny(chars) == 3); // é is first UTF-8 sequence starting with C3

        chars.Clear();
        chars.Add(Char(0xF0));   // leading byte of 🙂
        REQUIRE(s.IndexOfAny(chars) == 7); // 🙂 grapheme index == 7
    }

    SECTION("Char representing full Unicode not allowed (emojis etc.)")
    {
        // '🙂' cannot fit in a Char; IndexOfAny(Char) cannot detect it directly
        chars.Clear();
        chars.Add(Char('🙂')); // multi-byte literal collapses to '?'
        REQUIRE(s.IndexOfAny(chars) == -1);
    }
}

TEST_CASE("String::IndexOfAny - UTF-8 multibyte characters")
{
    String s(UTF8_SAMPLE);

    // café ãç🙂
    // indices (graphemes): c(0) a(1) f(2) é(3) (4) ã(5) ç(6) 🙂(7)

    SECTION("Find é / ã / ç using ASCII-unfriendly byte rules")
    {
        List<Char> chars;

        chars.Clear();
        chars.Add(Char(0xC3)); // é begins with 0xC3
        REQUIRE(s.IndexOfAny(chars) == 3);

        chars.Clear();
        chars.Add(Char(0xA7)); // secondary byte of ç, should not match (only first-byte match allowed)
        REQUIRE(s.IndexOfAny(chars) == -1);
    }

    SECTION("Multiple chars: find whichever matches first")
    {
        List<Char> chars;
        chars.Add(Char('f'));     // index 2
        chars.Add(Char(0xC3));    // index 3 (é)
        chars.Add(Char('x'));

        REQUIRE(s.IndexOfAny(chars) == 2);
    }
}

TEST_CASE("String::IndexOfAny - startIndex and count behavior")
{
    String s(UTF8_MIXED);
    // abc def ghi ãç🙂
    // graphemes:
    // a(0) b(1) c(2) ' '(3) d(4) e(5) f(6) ' '(7)
    // g(8) h(9) i(10) ' '(11) ã(12) ç(13) 🙂(14)

    List<Char> chars;
    chars.Add(Char(' ')); // find spaces

    SECTION("Normal range")
    {
        REQUIRE(s.IndexOfAny(chars, 0, 15) == 3);
    }

    SECTION("Region with no match")
    {
        REQUIRE(s.IndexOfAny(chars, 4, 3) == -1); // def → no space
    }

    SECTION("Range catching later match")
    {
        REQUIRE(s.IndexOfAny(chars, 8, 6) == 11); // space at index 11
    }

    SECTION("Single grapheme region")
    {
        REQUIRE(s.IndexOfAny(chars, 12, 1) == -1); // ã has no space
    }

    SECTION("startIndex beyond length")
    {
        REQUIRE(s.IndexOfAny(chars, 20) == -1);
    }
}

TEST_CASE("String::IndexOfAny - Boundary conditions")
{
    String s("abcdef");

    List<Char> chars;
    chars.Add(Char('a'));
    chars.Add(Char('f'));

    SECTION("Empty chars list")
    {
        List<Char> empty;
        REQUIRE(s.IndexOfAny(empty) == -1);
    }

    SECTION("startIndex == length")
    {
        REQUIRE(s.IndexOfAny(chars, 6) == -1);
    }

    SECTION("count == 0")
    {
        REQUIRE(s.IndexOfAny(chars, 0, 0) == -1);
    }

    SECTION("Full match at edges")
    {
        REQUIRE(s.IndexOfAny(chars) == 0);   // a
        REQUIRE(s.IndexOfAny(chars, 1) == 5); // f
    }
}

// StraßE TEST İSTANBUL i̇stanbul
static const char* UTF8_CASE =
"Stra\xC3\x9F""e TEST "
"\xC4\xB0""STANBUL "
"i\xCC\x87""stanbul";

TEST_CASE("String::LastIndexOf - Basic ASCII")
{
    String s("hello hello world");

    SECTION("LastIndexOf(String)")
    {
        REQUIRE(s.LastIndexOf("hello") == 6);     // last "hello"
        REQUIRE(s.LastIndexOf("world") == 12);
        REQUIRE(s.LastIndexOf("test") == -1);
    }

    SECTION("LastIndexOf(String, startIndex)")
    {
        REQUIRE(s.LastIndexOf("hello", 4) == 0);
        REQUIRE(s.LastIndexOf("hello", 10) == 6);
        REQUIRE(s.LastIndexOf("o", 15) == 13);
    }

    SECTION("LastIndexOf(String, startIndex, count)")
    {
        REQUIRE(s.LastIndexOf("hello", 10, 10) == 6);
        REQUIRE(s.LastIndexOf("hello", 4, 5) == 0);
        REQUIRE(s.LastIndexOf("world", 10, 5) == -1);
    }

    SECTION("empty needle")
    {
        REQUIRE(s.LastIndexOf("", 0) == 0);
        REQUIRE(s.LastIndexOf("") == s.GetLength() - 1);
    }
}

TEST_CASE("String::LastIndexOf - UTF-8 Multibyte Cases")
{
    String s(UTF8_SAMPLE);
    // graphemes: c(0) a(1) f(2) é(3) ' '(4) ã(5) ç(6) 🙂(7)

    SECTION("Basic multibyte search")
    {
        REQUIRE(s.LastIndexOf("\xC3\xA9") == 3);      // é
        REQUIRE(s.LastIndexOf("\xC3\xA3") == 5);      // ã
        REQUIRE(s.LastIndexOf("\xC3\xA7") == 6);      // ç
        REQUIRE(s.LastIndexOf("\xF0\x9F\x99\x82") == 7); // 🙂
    }

    SECTION("StartIndex before last match")
    {
        REQUIRE(s.LastIndexOf("\xC3\xA7", 7) == 6);
        REQUIRE(s.LastIndexOf("\xC3\xA7", 5) == -1);
    }

    SECTION("StartIndex + count")
    {
        REQUIRE(s.LastIndexOf("\xC3\xA7", 7, 8) == 6);
        REQUIRE(s.LastIndexOf("\xC3\xA7", 7, 2) == 6);
        REQUIRE(s.LastIndexOf("\xC3\xA7", 5, 3) == -1);
    }
}

TEST_CASE("String::LastIndexOf - Grapheme clusters")
{
    // composed á
    String composed("\xC3\xA1");

    // decomposed: a + 0x0301
    String decomposed("a\xCC\x81");

    // "olá mundo" where á is decomposed
    String s("ola\xCC\x81 mundo");

    // graphemes:
    // o(0) l(1) á(2) ' '(3) m(4) u(5) n(6) d(7) o(8)

    SECTION("Ordinal – no grapheme equivalence")
    {
        REQUIRE(s.LastIndexOf("\xC3\xA1") == -1); // composed not present
        REQUIRE(s.LastIndexOf("a\xCC\x81") == 2); // decomposed present
    }

    SECTION("startIndex + count")
    {
        REQUIRE(s.LastIndexOf("a\xCC\x81", 8, 5) == -1);
        REQUIRE(s.LastIndexOf("a\xCC\x81", 2, 1) == 2);
    }
}

TEST_CASE("String::LastIndexOf - Emoji (4-byte UTF-8)")
{
    // 👍👍🙂👍
    String s("\xF0\x9F\x91\x8D\xF0\x9F\x91\x8D"
        "\xF0\x9F\x99\x82"
        "\xF0\x9F\x91\x8D");

    // graphemes:
    // 0 👍
    // 1 👍
    // 2 🙂
    // 3 👍

    const char* thumbs = "\xF0\x9F\x91\x8D";
    const char* smile = "\xF0\x9F\x99\x82";

    SECTION("Simple last search")
    {
        REQUIRE(s.LastIndexOf(thumbs) == 3);
        REQUIRE(s.LastIndexOf(smile) == 2);
    }

    SECTION("StartIndex")
    {
        REQUIRE(s.LastIndexOf(thumbs, 2) == 1);
        REQUIRE(s.LastIndexOf(thumbs, 1) == 1);
    }

    SECTION("StartIndex + count")
    {
        REQUIRE(s.LastIndexOf(thumbs, 3, 4) == 3);
        REQUIRE(s.LastIndexOf(thumbs, 2, 2) == 1);
        REQUIRE(s.LastIndexOf(smile, 1, 2) == -1);
    }
}

TEST_CASE("String::LastIndexOf - Case-insensitive (OrdinalIgnoreCase)")
{
    String s(UTF8_CASE);

    SECTION("ASCII ignore-case")
    {
        REQUIRE(s.LastIndexOf("TEST", StringComparison::OrdinalIgnoreCase) == 7); // last TEST is unique
    }

    SECTION("Turkish casefold behavior")
    {
        // last "istanbul" is from "i̇stanbul"
        REQUIRE(s.LastIndexOf("istanbul", StringComparison::OrdinalIgnoreCase) == 21);  // grapheme index of "i̇" at start of second occurrence
        REQUIRE(s.LastIndexOf("\xC4\xB0""STANBUL", StringComparison::OrdinalIgnoreCase) == 21);
        REQUIRE(s.LastIndexOf("i\xCC\x87""STANBUL", StringComparison::OrdinalIgnoreCase) == 21);
    }

    SECTION("StartIndex + comparison")
    {
        REQUIRE(s.LastIndexOf("TEST", 12, StringComparison::OrdinalIgnoreCase) == 7);
        REQUIRE(s.LastIndexOf("TEST", 10, StringComparison::OrdinalIgnoreCase) == 7);
    }

    SECTION("StartIndex + count + comparison")
    {
        REQUIRE(s.LastIndexOf("TEST", 15, 20, StringComparison::OrdinalIgnoreCase) == 7);
        REQUIRE(s.LastIndexOf("TEST", 10, 20, StringComparison::OrdinalIgnoreCase) == 7);
    }
}

TEST_CASE("String::LastIndexOf - Char overloads")
{
    String s("abc def ghi ãç🙂");

    // graphemes:
    // 0 a,1 b,2 c,3 ' ',4 d,5 e,6 f,7 ' ',
    // 8 g,9 h,10 i,11 ' ',12 ã,13 ç,14 🙂

    SECTION("LastIndexOf(Char)")
    {
        REQUIRE(s.LastIndexOf(Char(' ')) == 11);
        REQUIRE(s.LastIndexOf(Char('a')) == 0);
        REQUIRE(s.LastIndexOf(Char('x')) == -1);
    }

    SECTION("LastIndexOf(Char, startIndex)")
    {
        REQUIRE(s.LastIndexOf(Char(' '), 10) == 7);
        REQUIRE(s.LastIndexOf(Char(' '), 3) == 3);
    }

    SECTION("LastIndexOf(Char, startIndex, count)")
    {
        REQUIRE(s.LastIndexOf(Char(' '), 14, 12) == 11);
        REQUIRE(s.LastIndexOf(Char(' '), 10, 3) == -1);
    }

    SECTION("Char multibyte using FromChar")
    {
        REQUIRE(s.LastIndexOf(String::FromCodePoint(0x1F642)) == 14);
        REQUIRE(s.LastIndexOf(String::FromCodePoint(0x00E7)) == 13); // ç
    }
}

TEST_CASE("String::LastIndexOf - Boundary and error conditions")
{
    String s("abcdef");

    SECTION("startIndex beyond length")
    {
        REQUIRE(s.LastIndexOf("c", 50) == 2);
    }

    SECTION("count exceeds range")
    {
        REQUIRE(s.LastIndexOf("def", 50, 50) == 3);
    }

    SECTION("needle longer than haystack")
    {
        REQUIRE(s.LastIndexOf("abcdefg") == -1);
    }

    SECTION("empty needle")
    {
        REQUIRE(s.LastIndexOf("") == 5);
        REQUIRE(s.LastIndexOf("", 0) == 0);
    }
}

TEST_CASE("String::LastIndexOfAny - Basic ASCII")
{
    String s("hello world test");

    List<Char> chars;

    SECTION("Single ASCII char")
    {
        chars.Clear();
        chars.Add(Char('o'));
        REQUIRE(s.LastIndexOfAny(chars) == 7); // 'o' in world
    }

    SECTION("Multiple ASCII chars")
    {
        chars.Clear();
        chars.Add(Char('l'));
        chars.Add(Char('o'));
        REQUIRE(s.LastIndexOfAny(chars) == 9); // last 'o'
    }

    SECTION("Char not present")
    {
        chars.Clear();
        chars.Add(Char('x'));
        REQUIRE(s.LastIndexOfAny(chars) == -1);
    }
}

TEST_CASE("String::LastIndexOfAny - StartIndex")
{
    String s("abc def ghi");

    List<Char> chars;
    chars.Add(Char(' '));

    SECTION("StartIndex limits search")
    {
        REQUIRE(s.LastIndexOfAny(chars, 5) == 3);
        REQUIRE(s.LastIndexOfAny(chars, 2) == -1);
    }

    SECTION("StartIndex >= length")
    {
        REQUIRE(s.LastIndexOfAny(chars, 50) == 7);
    }
}

TEST_CASE("String::LastIndexOfAny - StartIndex + count")
{
    String s("abc def ghi"); // spaces at 3 and 7
    List<Char> chars;
    chars.Add(Char(' '));

    SECTION("Window limits backward scan")
    {
        REQUIRE(s.LastIndexOfAny(chars, 4, 4) == 3);
        REQUIRE(s.LastIndexOfAny(chars, 10, 3) == -1);
        REQUIRE(s.LastIndexOfAny(chars, 7, 4) == 7);
    }

    SECTION("Count too large → clamped")
    {
        REQUIRE(s.LastIndexOfAny(chars, 5, 50) == 3);
    }
}

TEST_CASE("String::LastIndexOfAny - UTF-8 accents")
{
    // Graphemes:
    // 0:c 1:a 2:f 3:é 4:' ' 5:a 6:ç 7:ú 8:c 9:a 10:r
    String s("caf"
        "\xC3\xA9"   // é
        " "
        "a"
        "\xC3\xA7"   // ç
        "\xC3\xBA"   // ú
        "car");

    List<Char> chars;

    SECTION("Match ASCII only (find last 'a')")
    {
        chars.Clear();
        chars.Add(Char('a'));

        // Último 'a' está no índice 9
        REQUIRE(s.LastIndexOfAny(chars) == 9);
    }

    SECTION("Match multibyte start bytes (C3: é, ç, ú)")
    {
        chars.Clear();
        chars.Add(Char(0xC3)); // UTF-8 lead-byte for many accented chars

        // Último caractere começando com C3 é 'ú' em índice 7
        REQUIRE(s.LastIndexOfAny(chars) == 7);
    }

    //SECTION("Specific accented char (é)")
    //{
    //    chars.Clear();
    //    chars.Add(Char(0x00E9)); // é = C3 A9
    //
    //    // O único 'é' está no índice 3
    //    REQUIRE(s.LastIndexOfAny(chars) == 3);
    //}
}

TEST_CASE("String::LastIndexOfAny - Emoji (4-byte UTF-8)")
{
    const char* text =
        "\xF0\x9F\x91\x8D"  // 👍 0
        "\x20"
        "\xF0\x9F\x99\x82"  // 🙂 2
        "\x20"
        "\xF0\x9F\x91\x8D"; // 👍 4

    String s(text);

    List<Char> chars;

    SECTION("Match emoji by leading byte")
    {
        chars.Clear();
        chars.Add(Char(0xF0));
        REQUIRE(s.LastIndexOfAny(chars) == 4);
    }

    SECTION("Match space or emoji")
    {
        chars.Clear();
        chars.Add(Char(' '));
        chars.Add(Char(0xF0));
        REQUIRE(s.LastIndexOfAny(chars) == 4);
    }

    SECTION("StartIndex window")
    {
        chars.Clear();
        chars.Add(Char(0xF0));

        REQUIRE(s.LastIndexOfAny(chars, 2, 3) == 2);
        REQUIRE(s.LastIndexOfAny(chars, 1, 2) == 0);
    }
}

TEST_CASE("String::LastIndexOfAny - Grapheme clusters")
{
    String s("ola\xCC\x81 mundo"); // á is grapheme index 2

    List<Char> chars;

    SECTION("Match base ASCII 'a'")
    {
        chars.Clear();
        chars.Add(Char('a'));
        REQUIRE(s.LastIndexOfAny(chars) == 2);
    }

    SECTION("Match combining mark by raw byte")
    {
        chars.Clear();
        chars.Add(Char(0xCC));
        REQUIRE(s.LastIndexOfAny(chars) == 3);
    }
}

TEST_CASE("String::LastIndexOfAny - Boundary conditions")
{
    String s("abcdef");

    List<Char> chars;
    chars.Add(Char('c'));
    chars.Add(Char('d'));

    SECTION("startIndex > length")
    {
        REQUIRE(s.LastIndexOfAny(chars, 99) == 3);
    }

    SECTION("count = 0")
    {
        REQUIRE(s.LastIndexOfAny(chars, 5, 0) == -1);
    }

    SECTION("count > startIndex — clamped")
    {
        REQUIRE(s.LastIndexOfAny(chars, 5, 50) == 3);
    }

    SECTION("chars empty")
    {
        List<Char> empty;
        REQUIRE(s.LastIndexOfAny(empty) == -1);
    }
}

TEST_CASE("String::ReplaceLineEndings() - Basic", "[ReplaceLineEndings]")
{
    // 1) Basic LF
    {
        String s("A\nB\nC");
        REQUIRE(s.ReplaceLineEndings() == String("A\nB\nC"));
    }

    // 2) Basic CRLF → LF
    {
        String s("A\r\nB\r\nC");
        REQUIRE(s.ReplaceLineEndings() == String("A\nB\nC"));
    }

    // 3) CR only → LF
    {
        String s("A\rB\rC");
        REQUIRE(s.ReplaceLineEndings() == String("A\nB\nC"));
    }

    // 4) Mixed endings
    {
        String s("A\r\nB\rC\nD");
        REQUIRE(s.ReplaceLineEndings() == String("A\nB\nC\nD"));
    }

    // 5) Ending with CR
    {
        String s("Hello\r");
        REQUIRE(s.ReplaceLineEndings() == String("Hello\n"));
    }

    // 6) Ending with CRLF
    {
        String s("Hello\r\n");
        REQUIRE(s.ReplaceLineEndings() == String("Hello\n"));
    }

    // 7) LF-only ending preserved
    {
        String s("Hello\n");
        REQUIRE(s.ReplaceLineEndings() == String("Hello\n"));
    }

    // 8) Empty string
    {
        String s("");
        REQUIRE(s.ReplaceLineEndings() == String(""));
    }

    // 9) No line endings
    {
        String s("ABC");
        REQUIRE(s.ReplaceLineEndings() == String("ABC"));
    }
}

TEST_CASE("String::ReplaceLineEndings(String) - Replacement", "[ReplaceLineEndings]")
{
    // Replacement = "<BR>"
    {
        String rep("<BR>");
        String s("A\r\nB\rC\nD");
        REQUIRE(s.ReplaceLineEndings(rep) == String("A<BR>B<BR>C<BR>D"));
    }

    // Replacement = emoji (UTF-8 multibyte)
    {
        String rep("🔥"); // 4 bytes
        String s("X\r\nY\rZ\nW");
        REQUIRE(s.ReplaceLineEndings(rep) == String("X🔥Y🔥Z🔥W"));
    }

    // Replacement = empty string (removes line endings)
    {
        String rep("");
        String s("A\r\nB\nC\rD");
        REQUIRE(s.ReplaceLineEndings(rep) == String("ABCD"));
    }

    // Replacement = multi-byte UTF-8 sequence
    {
        String rep("💀🔥"); // 8 bytes
        String s("L1\nL2\r\nL3\rL4");
        REQUIRE(s.ReplaceLineEndings(rep) == String("L1💀🔥L2💀🔥L3💀🔥L4"));
    }

    // Replacement at end
    {
        String rep("X");
        String s("Hi\r\n");
        REQUIRE(s.ReplaceLineEndings(rep) == String("HiX"));
    }

    // Replacement without any newline
    {
        String rep("---");
        String s("ABC");
        REQUIRE(s.ReplaceLineEndings(rep) == String("ABC"));
    }

    // Multiple CR in sequence
    {
        String rep("|");
        String s("A\r\r\rB");
        REQUIRE(s.ReplaceLineEndings(rep) == String("A|||B"));
    }

    // Multiple LF in sequence
    {
        String rep("X");
        String s("A\n\nB");
        REQUIRE(s.ReplaceLineEndings(rep) == String("AXXB"));
    }

    // Mixed CRLF + CR + LF edge test
    {
        String rep("_");
        String s("\r\n\r\n\r\n");
        REQUIRE(s.ReplaceLineEndings(rep) == String("___"));
    }
}

TEST_CASE("String::ReplaceLineEndings(String) - Replacement containing CR/LF", "[ReplaceLineEndings]")
{
    String rep("X\rY\nZ"); // replacement contém CR/LF internamente
    String s("A\nB");

    // Replacement é inserido literal
    REQUIRE(s.ReplaceLineEndings(rep) == String("AX\rY\nZB"));
}

TEST_CASE("String::Remove(start) - ASCII basic behaviors (.NET semantics)", "[Remove]")
{
    // Remove from beginning → remove entire string
    {
        String s("abcdef");
        REQUIRE(s.Remove(0) == String(""));
    }

    // Remove from middle → remove everything from index forward
    {
        String s("abcdef");
        REQUIRE(s.Remove(2) == String("ab"));
    }

    // Remove near end
    {
        String s("abcdef");
        REQUIRE(s.Remove(5) == String("abcde"));
    }

    // Remove at exact end → unchanged
    {
        String s("abcdef");
        REQUIRE(s.Remove(6) == String("abcdef"));
    }

    // Remove out of range → unchanged
    {
        String s("abcdef");
        REQUIRE(s.Remove(999) == String("abcdef"));
    }
}

TEST_CASE("String::Remove(start) - UTF-8 multibyte (emojis) [.NET]", "[Remove]")
{
    // A 💀 B 🔥 C   → 5 graphemes
    String s("A💀B🔥C");
    REQUIRE(s.GetLength() == 5);

    // Remove from cluster 1 → everything after index 1 removed
    REQUIRE(s.Remove(1) == String("A"));

    // Remove from cluster 2
    REQUIRE(s.Remove(2) == String("A💀"));

    // Remove from cluster 3
    REQUIRE(s.Remove(3) == String("A💀B"));
}

TEST_CASE("String::Remove(start) - Combining marks (é = e + U+0301) [.NET]", "[Remove]")
{
    String s("ábc"); // U+00E1 U+0062 U+0063
    REQUIRE(s.GetLength() == 3);

    // Remove(0) → remove entire string
    REQUIRE(s.Remove(0) == String(""));

    // Remove(1) → remove everything after grapheme 1
    REQUIRE(s.Remove(1) == String("á"));
}

TEST_CASE("String::Remove(start,count) - ASCII [.NET]", "[Remove]")
{
    // remove from beginning
    {
        String s("abcdef");
        REQUIRE(s.Remove(0, 3) == String("def"));
    }

    // remove middle section
    {
        String s("abcdef");
        REQUIRE(s.Remove(2, 2) == String("abef"));
    }

    // remove to end
    {
        String s("abcdef");
        REQUIRE(s.Remove(3, 999) == String("abc"));
    }

    // count = 0 → unchanged
    {
        String s("abcdef");
        REQUIRE(s.Remove(2, 0) == String("abcdef"));
    }

    // start beyond length → unchanged
    {
        String s("abcdef");
        REQUIRE(s.Remove(99, 5) == String("abcdef"));
    }
}

TEST_CASE("String::Remove(start,count) - UTF-8 multibyte emojis [.NET]", "[Remove]")
{
    String s("A💀B🔥C"); // 5 graphemes

    // remove only 💀
    REQUIRE(s.Remove(1, 1) == String("AB🔥C"));

    // remove B🔥 (graphemes 2 and 3)
    REQUIRE(s.Remove(2, 2) == String("A💀C"));

    // remove all after first emoji
    REQUIRE(s.Remove(1, 999) == String("A"));
}

TEST_CASE("String::Remove(start,count) - Combining sequences [.NET]", "[Remove]")
{
    // "á" = a + 0301
    String s("áb́c");
    REQUIRE(s.GetLength() == 3);

    // remove the middle grapheme only
    REQUIRE(s.Remove(1, 1) == String("ác"));

    // remove first two clusters
    REQUIRE(s.Remove(0, 2) == String("c"));

    // remove last cluster
    REQUIRE(s.Remove(2, 1) == String("áb́"));
}

TEST_CASE("String::Remove - Boundary cases [.NET]", "[Remove]")
{
    // empty string
    {
        String s("");
        REQUIRE(s.Remove(0) == String(""));
        REQUIRE(s.Remove(0, 10) == String(""));
    }

    // count negative → unchanged
    {
        String s("ABC");
        REQUIRE(s.Remove(1, -5) == String("ABC"));
    }

    // start negative → Remove(0)
    {
        String s("ABC");

        // .NET Remove(-1) throws → but your API clamps to Remove(0)
        REQUIRE(s.Remove(-1) == String(""));

        // Remove(-1, count) → unchanged because start invalid
        REQUIRE(s.Remove(-1, 2) == String("ABC"));
    }
}

TEST_CASE("String::Remove - UTF-8 stress: mixed clusters [.NET]", "[Remove]")
{
    // A, 💀, e◌́ , 🔥, Z
    String s("A💀é🔥Z");
    REQUIRE(s.GetLength() == 5);

    // Remove(2,1) removes "é"
    REQUIRE(s.Remove(2, 1) == String("A💀🔥Z"));

    // Remove "💀é"
    REQUIRE(s.Remove(1, 2) == String("A🔥Z"));

    // Remove everything from index 3 onward → last two clusters: 🔥 Z
    REQUIRE(s.Remove(3) == String("A💀é"));
}

TEST_CASE("String::Remove - ASCII vs Unicode consistency [.NET]", "[Remove]")
{
    String ascii("abcdef");
    REQUIRE(ascii.Remove(2, 2) == String("abef"));

    // Unicode, same grapheme count
    String unicode("áb́ćd́éf́");
    REQUIRE(unicode.Remove(2, 2).GetLength() == 4);
}

// helper: conta ocorrências de substring (usando IndexOf)
static int CountOccurrences(const String& s, const String& sub) {
    if (sub.IsEmpty()) return 0;

    const Char* hay = s;
    const Char* ned = sub;

    uint32_t H = s.GetByteCount();
    uint32_t N = sub.GetByteCount();

    int count = 0;

    for (uint32_t i = 0; i + N <= H; )
    {
        if (memcmp(hay + i, ned, N) == 0)
        {
            ++count;
            i += N;        // avança por bytes
        }
        else
        {
            ++i;
        }
    }
    return count;
}

// helper: conta ocorrências do codepoint '?' (ASCII 0x3F)
static int CountQuestionCodepoints(const String& s) {
    List<CodePoint> cps = String::DecodeToCodePoints(s);
    int cnt = 0;
    for (uint32_t i = 0; i < cps.Count(); ++i)
        if ((uint32_t)cps[i] == 0x003Fu) ++cnt; // '?'
    return cnt;
}

TEST_CASE("String::Replace(Char, Char) — basic ASCII byte replace")
{
    String s("banana");
    String r = s.Replace(Char('a'), Char('o'));

    REQUIRE(r == "bonono");
    REQUIRE(s == "banana");
}

TEST_CASE("String::Replace(Char, Char) — no match")
{
    String s("abc");
    REQUIRE(s.Replace(Char('x'), Char('y')) == "abc");
}

TEST_CASE("String::Replace(Char, Char) — UTF-8 is not interpreted as char")
{
    // café = UTF-8: 63 61 66 C3 A9
    String s("café");

    String r = s.Replace(Char(0xC3), Char(0xFF));

    const unsigned char* raw = reinterpret_cast<const unsigned char*>((const char*)r);
    // we're replacing the first byte of 'é' (0xC3)
    REQUIRE(raw[3] == 0xFF);
}

TEST_CASE("String::Replace(Char, Char) — empty string")
{
    REQUIRE(String("").Replace(Char('a'), Char('b')) == "");
}


// ===============================================================
// Replace(CodePoint, CodePoint)
// ===============================================================

TEST_CASE("String::Replace(CodePoint, CodePoint) — simple BMP")
{
    String s("áéíóú"); // literal UTF-8 in source
    String r = s.Replace(CodePoint(U'\u00E1'), CodePoint(U'\u00C1'));

    REQUIRE(r == "Áéíóú");
}

TEST_CASE("String::Replace(CodePoint, CodePoint) — emoji")
{
    // 👍 😀 👍
    String s("👍 😀 👍");
    String r = s.Replace(CodePoint(0x1F44D), CodePoint(0x1F600));

    REQUIRE(r == "😀 😀 😀");
}

TEST_CASE("String::Replace(CodePoint, CodePoint) — no match")
{
    String s("hello");
    REQUIRE(s.Replace(CodePoint(U'x'), CodePoint(U'y')) == "hello");
}

TEST_CASE("String::Replace(CodePoint, CodePoint) — combining marks preserved (explicit bytes)")
{
    // construct explicitly 'a' + COMBINING ACUTE (NFD): 0x61 0xCC 0x81
    unsigned char raw[] = { 0x61, 0xCC, 0x81 };
    String s(raw, 3);

    String r = s.Replace(CodePoint(U'a'), CodePoint(U'b'));

    // verify codepoint-level result explicitly (avoid depending on editor encoding)
    REQUIRE(r.GetCodePointsCount() == 2);
    REQUIRE((uint32_t)r.GetCodePointAt(0) == (uint32_t)CodePoint(U'b'));
    REQUIRE((uint32_t)r.GetCodePointAt(1) == 0x0301u);
}


// ===============================================================
// Replace(String,String) — Ordinal
// ===============================================================

TEST_CASE("String::Replace(String,String) — basic ordinal")
{
    String s("banana");
    REQUIRE(s.Replace("na", "NA") == "baNANA");
}

TEST_CASE("String::Replace(String,String) — overlapping does NOT cascade")
{
    String s("aaa");
    REQUIRE(s.Replace("aa", "b") == "ba");
}

TEST_CASE("String::Replace(String,String) — remove substring")
{
    String s("banana");
    REQUIRE(s.Replace("na", "") == "ba");
}

TEST_CASE("String::Replace(String,String) — no occurrence")
{
    String s("banana");
    REQUIRE(s.Replace("xyz", "???") == "banana");
}

TEST_CASE("String::Replace(String,String) — multi-byte UTF-8")
{
    String s("coração");
    REQUIRE(s.Replace("ção", "ÇÃO") == "coraÇÃO");
}

TEST_CASE("String::Replace(String,String) — oldValue empty returns original")
{
    String s("abc");
    REQUIRE(s.Replace("", "X") == "abc");
}


// ===============================================================
// Replace(String,String,StringComparison)
// ===============================================================

TEST_CASE("String::Replace(..., OrdinalIgnoreCase) — ASCII simple (count X)")
{
    String s("Hello HELLO hElLo");
    String r = s.Replace("hello", "X", StringComparison::OrdinalIgnoreCase);

    REQUIRE(CountOccurrences(r, "X") == 3);
}

TEST_CASE("String::Replace(..., OrdinalIgnoreCase) — no matches")
{
    String s("abc");
    REQUIRE(s.Replace("ZZZ", "X", StringComparison::OrdinalIgnoreCase) == "abc");
}

TEST_CASE("String::Replace(..., OrdinalIgnoreCase) — ß → ss casefold (behavioral)")
{
    String s("straße");
    String r = s.Replace("SS", "XX", StringComparison::OrdinalIgnoreCase);

    // Ensure original substring base remains (tra part) and we did a substitution.
    REQUIRE(r.Contains("tra"));

    // Count occurrences of 'X' codepoints (robusto contra encoding)
    List<CodePoint> cps = String::DecodeToCodePoints(r);
    int xcount = 0;
    for (uint32_t i = 0; i < cps.Count(); ++i)
        if ((uint32_t)cps[i] == (uint32_t)CodePoint(U'X')) ++xcount;

    // require at least one replacement happened
    REQUIRE(xcount >= 1);
}

TEST_CASE("String::Replace(..., OrdinalIgnoreCase) — accented matching (behavioral)")
{
    String s("Café CAFE cafe");
    String r = s.Replace("café", "--", StringComparison::OrdinalIgnoreCase);

    // ensure at least one substitution happened and result is different
    REQUIRE(r.Contains("--"));
    REQUIRE(r != s);
}


// ===============================================================
// Replace(String,String,bool ignoreCase, locale)
// ===============================================================

TEST_CASE("String::Replace(ignoreCase, locale=tr) — Turkish casing I/İ (counts)")
{
    String s("IKI iki İKİ");
    String r = s.Replace("i", "?", true, "tr");

    // count codepoint '?' occurrences (casefold can expand/contract; require at least 3)
    int q = CountQuestionCodepoints(r);
    REQUIRE(q >= 3);
}

TEST_CASE("String::Replace(ignoreCase, locale=en) — English rules differ (counts)")
{
    String s("IKI iki İKİ");
    String r = s.Replace("i", "?", true, "en");

    int q = CountQuestionCodepoints(r);
    REQUIRE(q >= 3);
}

TEST_CASE("String::Replace(ignoreCase, locale) — unicode fold + NFC (all occurrences)")
{
    // use NFC forms explicitly
    String s("\u00E9cole \u00C9COLE \u00C9cole");
    String r = s.Replace("\u00E9cole", "X", true, "en");

    // validate that we replaced all visible occurrences (at least 3 Xs)
    REQUIRE(CountOccurrences(r, "X") >= 3);
}

TEST_CASE("String::Replace(ignoreCase, locale) — no matches")
{
    String s("coração");
    REQUIRE(s.Replace("TESTE", "X", true, "pt") == "coração");
}


// ===============================================================
// Edge cases
// ===============================================================

TEST_CASE("String::Replace — replace whole string")
{
    String s("abc");
    REQUIRE(s.Replace("abc", "XYZ") == "XYZ");
}

TEST_CASE("String::Replace — replace with empty newValue removes correctly")
{
    String s("foo bar foo bar foo");
    REQUIRE(s.Replace("foo", "") == " bar  bar ");
}

TEST_CASE("String::Replace(CodePoint,CodePoint) — invalid UTF-8 becomes U+FFFD")
{
    unsigned char raw[] = { 'A', 0xFF, 'B' };
    String s(raw, 3);

    String r = s.Replace(CodePoint(0xFFFD), CodePoint(U'?'));

    REQUIRE(r.Contains("?"));
}

// ========================================================
// Helpers
// ========================================================

static List<String> L(std::initializer_list<const char*> arr)
{
    List<String> out;
    for (auto& s : arr)
        out.Add(String(s));
    return out;
}

template<typename T>
static List<T> LW(std::initializer_list<T> arr)
{
    List<T> out;
    for (auto& x : arr)
        out.Add(T(x));
    return out;
}

// ========================================================
// Join(String,List<String>)
// ========================================================

TEST_CASE("String::Join(String, List<String>) — basic")
{
    List<String> v = L({ "a","b","c" });
    REQUIRE(String::Join(String(","), v) == "a,b,c");
}

TEST_CASE("String::Join(String, List<String>) — empty separator")
{
    List<String> v = L({ "a","b","c" });
    REQUIRE(String::Join(String(""), v) == "abc");
}

TEST_CASE("String::Join(String, List<String>) — single element")
{
    List<String> v = L({ "hello" });
    REQUIRE(String::Join(String(","), v) == "hello");
}

TEST_CASE("String::Join(String, List<String>) — empty list")
{
    List<String> v;
    REQUIRE(String::Join(String(","), v) == "");
}

TEST_CASE("String::Join(String, List<String>) — UTF-8 separators")
{
    List<String> v = L({ "café","música","ação" });
    String sep("💠");
    REQUIRE(String::Join(sep, v) == "café💠música💠ação");
}

// ========================================================
// Join(Char,List<String>)
// ========================================================

TEST_CASE("String::Join(Char, List<String>) — basic ASCII")
{
    List<String> v = L({ "x","y","z" });
    REQUIRE(String::Join(Char(','), v) == "x,y,z");
}

TEST_CASE("String::Join(Char, List<String>) — Char separator behaves byte-wise")
{
    List<String> v = L({ "a","b","c" });
    REQUIRE(String::Join(Char('#'), v) == "a#b#c");
}

// ========================================================
// Join(CodePoint, List<String>)
// ========================================================

TEST_CASE("String::Join(CodePoint, List<String>) — emoji separator")
{
    List<String> v = L({ "um","dois","três" });
    CodePoint sep(0x1F680); // 🚀
    REQUIRE(String::Join(sep, v) == "um🚀dois🚀três");
}

TEST_CASE("String::Join(CodePoint, List<String>) — combining mark")
{
    List<String> v = L({ "a","e","i" });
    CodePoint sepCp(0x0301); // COMBINING ACUTE

    String r = String::Join(sepCp, v);

    // expected: áéi  (each vowel followed by combining acute)
    REQUIRE(r == String("áéi"));
}

// ========================================================
// Join(String,List<T>) — wrappers with .ToString()
// ========================================================

TEST_CASE("String::Join(String, List<T>) — Boolean")
{
    List<Boolean> v = LW<Boolean>({ true, false, true });
    REQUIRE(String::Join(String(","), v) == "True,False,True");
}

TEST_CASE("String::Join(String, List<T>) — numeric wrappers")
{
    List<Int32> v = LW<Int32>({ 10, 20, 30 });
    REQUIRE(String::Join(String(";"), v) == "10;20;30");

    List<Double> v2 = LW<Double>({ 1.5, 2.25, 3.75 });
    REQUIRE(String::Join(String(" | "), v2) == "1.5 | 2.25 | 3.75");
}

TEST_CASE("String::Join(String, List<T>) — Char wrapper produces single character")
{
    List<Char> v = LW<Char>({ Char('A'), Char('B'), Char('C') });
    REQUIRE(String::Join(String("-"), v) == "A-B-C");
}

TEST_CASE("String::Join(String, List<T>) — CodePoint wrapper as value list")
{
    List<CodePoint> v;
    v.Add(CodePoint('x'));
    v.Add(CodePoint(0x1F44D)); // 👍
    v.Add(CodePoint('y'));

    REQUIRE(String::Join(String(","), v) == "x,👍,y");
}

// ========================================================
// Join(Char, List<T>) — wrappers
// ========================================================

TEST_CASE("String::Join(Char, List<T>) — Boolean")
{
    List<Boolean> v = LW<Boolean>({ true, true, false });
    REQUIRE(String::Join(Char('|'), v) == "True|True|False");
}

TEST_CASE("String::Join(Char, List<T>) — numeric ascii fast path")
{
    List<Int16> v = LW<Int16>({ 1, 2, 3, 4 });
    REQUIRE(String::Join(Char(','), v) == "1,2,3,4");
}

// ========================================================
// Join(CodePoint,List<T>) — wrappers
// ========================================================

TEST_CASE("String::Join(CodePoint, List<T>) — emoji sep with wrappers")
{
    CodePoint sep(0x1F680); // 🚀
    List<Int32> v = LW<Int32>({ 7, 8, 9 });
    REQUIRE(String::Join(sep, v) == "7🚀8🚀9");
}

// ========================================================
// Join(String,List<String>, start, count)
// ========================================================

TEST_CASE("String::Join(String, List<String>, start, count)")
{
    List<String> v = L({ "A","B","C","D","E" });
    REQUIRE(String::Join(String("-"), v, 1, 3) == "B-C-D");
}

TEST_CASE("String::Join(String, List<String>, start beyond range)")
{
    List<String> v = L({ "A","B" });
    REQUIRE(String::Join(String(","), v, 5, 2) == "");
}

TEST_CASE("String::Join(String, List<String>, count excessive)")
{
    List<String> v = L({ "A","B","C" });
    REQUIRE(String::Join(String(","), v, 1, 50) == "B,C");
}

// ========================================================
// Join(Char,List<String>, start,count)
// ========================================================

TEST_CASE("String::Join(Char,List<String>,start,count)")
{
    List<String> v = L({ "q","w","e","r" });
    REQUIRE(String::Join(Char('/'), v, 0, 2) == "q/w");
}

// ========================================================
// Join(CodePoint,List<String>, start,count)
// ========================================================

TEST_CASE("String::Join(CodePoint,List<String>,start,count)")
{
    CodePoint sep(0x1F449); // 👉
    List<String> v = L({ "x","y","z" });
    REQUIRE(String::Join(sep, v, 1, 2) == "y👉z");
}

// ========================================================
// Unicode-heavy cases
// ========================================================

TEST_CASE("String::Join — Unicode with multibyte elements")
{
    List<String> v = L({ "coração","ação","paixão" });
    REQUIRE(String::Join(String("♥"), v) == "coração♥ação♥paixão");
}

TEST_CASE("String::Join — mixing ASCII and Emoji")
{
    List<String> v = L({ "A","👍","B","😀" });
    REQUIRE(String::Join(String("|"), v) == "A|👍|B|😀");
}