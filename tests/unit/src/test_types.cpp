#pragma once

// =====================================================
//  Numeric Wrappers – Full Catch2 Test Suite
// =====================================================

#include "catch_amalgamated.hpp"

#include "types/Types.hpp"

// =====================================================
// 1. Construction from primitives
// =====================================================

TEST_CASE("Wrappers: construction from primitives")
{
    Int16 i16 = 10;
    Int32 i32 = 20;
    Int64 i64 = 30;

    UInt16 u16 = 10;
    UInt32 u32 = 20;
    UInt64 u64 = 30;

    REQUIRE((int)i16 == 10);
    REQUIRE((int)i32 == 20);
    REQUIRE((long long)i64 == 30);

    REQUIRE((int)u16 == 10);
    REQUIRE((unsigned int)u32 == 20u);
    REQUIRE((unsigned long long)u64 == 30ull);
}

// =====================================================
// 2. Wrapper → Wrapper promotion
// =====================================================

TEST_CASE("Wrappers: cross-type promotion")
{
    Int16 a = 42;

    Int32 b = a;
    Int64 c = b;
    Double d = c;
    Single f = d;

    REQUIRE((int)b == 42);
    REQUIRE((long long)c == 42);
    REQUIRE((double)d == (42.0));
    REQUIRE((float)f == (42.0f));
}

// =====================================================
// 3. Wrapper → primitive
// =====================================================

TEST_CASE("Wrappers: wrapper to primitive")
{
    Int32 a = 123;
    UInt32 b = 456;

    int i = a;
    unsigned int u = b;
    double d = a;

    REQUIRE(i == 123);
    REQUIRE(u == 456u);
    REQUIRE(d == (123.0));
}

// =====================================================
// 4. Primitive assignment
// =====================================================

TEST_CASE("Wrappers: primitive assignment")
{
    Int16 a;
    Int32 b;
    UInt64 c;

    a = 10;
    b = 20;
    c = 30;

    REQUIRE((int)a == 10);
    REQUIRE((int)b == 20);
    REQUIRE((unsigned long long)c == 30ull);
}

// =====================================================
// 5. Arithmetic cross-type
// =====================================================

TEST_CASE("Wrappers: arithmetic cross-type")
{
    Int16 a = 5;
    Int32 b = 10;
    Int64 c = 20;

    REQUIRE((int)(a + b) == 15);
    REQUIRE((int)(b - a) == 5);
    REQUIRE((long long)(c * a) == 100);
    REQUIRE((int)(c / b) == 2);
}

// =====================================================
// 6. Compound assignment
// =====================================================

TEST_CASE("Wrappers: compound assignment")
{
    Int32 a = 10;

    a += 5;
    REQUIRE((int)a == 15);

    a -= 3;
    REQUIRE((int)a == 12);

    a *= 2;
    REQUIRE((int)a == 24);

    a /= 4;
    REQUIRE((int)a == 6);
}

// =====================================================
// 7. Comparisons
// =====================================================

TEST_CASE("Wrappers: comparisons")
{
    Int16 a = 10;
    Int32 b = 10;
    Int64 c = 20;

    REQUIRE(a == b);
    REQUIRE(a != c);
    REQUIRE(a < c);
    REQUIRE(c > b);
    REQUIRE(a <= b);
    REQUIRE(c >= b);

    REQUIRE(a == 10);
    REQUIRE(10 == a);
}

// =====================================================
// 8. Bitwise operations
// =====================================================

TEST_CASE("Wrappers: bitwise operations")
{
    UInt32 a = 0b10101010;
    UInt32 b = 0b11001100;

    REQUIRE((unsigned int)(a & b) == 0b10001000);
    REQUIRE((unsigned int)(a | b) == 0b11101110);
    REQUIRE((unsigned int)(a ^ b) == 0b01100110);
    REQUIRE((unsigned int)(~a) == ~0b10101010u);
}

// =====================================================
// 9. Increment / Decrement
// =====================================================

TEST_CASE("Wrappers: increment and decrement")
{
    Int32 a = 10;

    REQUIRE((int)++a == 11);
    REQUIRE((int)a++ == 11);
    REQUIRE((int)a == 12);

    REQUIRE((int)--a == 11);
    REQUIRE((int)a-- == 11);
    REQUIRE((int)a == 10);
}

// =====================================================
// 10. Overflow (unchecked semantics)
// =====================================================

TEST_CASE("Wrappers: overflow behavior")
{
    UInt16 u16 = 0xFFFF;
    u16 += 1;
    REQUIRE((int)u16 == 0);

    Int16 i16 = Int16::MaxValue();
    i16 += 1;
    REQUIRE((int)i16 == Int16::MinValue());
}

// =====================================================
// 11. Signed / Unsigned interaction
// =====================================================

TEST_CASE("Wrappers: signed unsigned interaction")
{
    Int32 s = -1;
    UInt32 u = 1;

    //auto r = s + u;
    //REQUIRE((int)r == 0);
}

// =====================================================
// 12. Floating point interop
// =====================================================

TEST_CASE("Wrappers: floating point interop")
{
    Int32 a = 10;
    Double d = 2.5;

    Double r = a + d;
    REQUIRE((double)r == (12.5));

    Int32 b = static_cast<Int32>(d);
    REQUIRE((int)b == 2);
}

// =====================================================
// 14. Byte access (endianness-aware)
// =====================================================

TEST_CASE("Wrappers: byte access")
{
    UInt16 u16 = 0x1234;
    REQUIRE(u16[0] == 0x34);
    REQUIRE(u16[1] == 0x12);

    UInt32 u32 = 0x11223344;
    REQUIRE(u32[0] == 0x44);
    REQUIRE(u32[1] == 0x33);
    REQUIRE(u32[2] == 0x22);
    REQUIRE(u32[3] == 0x11);

    UInt64 u64 = 0x0102030405060708ULL;
    REQUIRE(u64[0] == 0x08);
    REQUIRE(u64[7] == 0x01);
}

// =====================================================
// 15. Constexpr guarantees
// =====================================================

constexpr Int16 cx16 = 10;
constexpr Int32 cx32 = cx16;
constexpr int cxi = cx32;

static_assert(cxi == 10);

constexpr UInt32 cxu32 = 0xAABBCCDD;
static_assert(cxu32[0] == 0xDD);
static_assert(cxu32[3] == 0xAA);

// =====================================================
// 16. HashCode consistency
// =====================================================

TEST_CASE("Wrappers: GetHashCode consistency")
{
    Int32 a = 42;
    Int32 b = 42;
    Int32 c = 43;

    REQUIRE(a.GetHashCode() == b.GetHashCode());
    REQUIRE(a.GetHashCode() != c.GetHashCode());
}

// =====================================================
//  Numeric Wrappers – Advanced Conversion & Promotion
// =====================================================

// =====================================================
// 18. Boolean -> Integer
// =====================================================

TEST_CASE("Wrappers: bool to integer conversion")
{
    Boolean t = true;
    Boolean f = false;

    Int32 a = t;
    Int32 b = f;

    REQUIRE((int)a == 1);
    REQUIRE((int)b == 0);
}

// =====================================================
// 19. Byte / Char -> Integer promotion
// =====================================================

TEST_CASE("Wrappers: byte and char promotion")
{
    Byte b = 0xFF;
    Char c = 'A';

    Int32 x = b;
    Int32 y = c;

    REQUIRE((int)x == 255);
    REQUIRE((int)y == 65);
}

// =====================================================
// 20. Floating point -> Integer (truncation)
// =====================================================

TEST_CASE("Wrappers: float to int truncation")
{
    Double d = 3.99;
    Single f = -2.75f;

    Int32 a = static_cast<Int32>(d);
    Int32 b = static_cast<Int32>(f);

    REQUIRE((int)a == 3);
    REQUIRE((int)b == -2);
}

// =====================================================
// 21. Signed <-> Unsigned explicit casts
// =====================================================

TEST_CASE("Wrappers: signed unsigned cast behavior")
{
    Int32 s = -1;
    UInt32 u = static_cast<UInt32>(s);

    REQUIRE((unsigned int)u == 0xFFFFFFFFu);

    Int32 back = static_cast<Int32>(u);
    REQUIRE((int)back == -1);
}

// =====================================================
// 22. Explicit narrowing conversion
// =====================================================

TEST_CASE("Wrappers: explicit narrowing conversion")
{
    Int32 big = 300;

    SByte small = static_cast<SByte>(big);
    REQUIRE((int)small == 44); // 300 % 256
}

// =====================================================
// 23. Mixed expression tree (promotion order)
// =====================================================

TEST_CASE("Wrappers: complex mixed expression")
{
    Int16 a = 10;
    UInt32 b = 20;
    Double c = 1.5;

    auto r = a + b * c;

    REQUIRE((double)r == 40.0);
}

// =====================================================
// 24. Division promotion (int -> float)
// =====================================================

TEST_CASE("Wrappers: promotion order")
{
    Int32 a = 5;
    Single b = 2.0f;

    auto r = a / b;

    REQUIRE((float)r == 2.5f);
}

// =====================================================
// 25. Cross-type comparisons
// =====================================================

TEST_CASE("Wrappers: cross-type comparisons")
{
    Int32 a = 10;
    Double b = 10.0;
    UInt64 c = 10;

    REQUIRE(a == b);
    REQUIRE(b == c);
    REQUIRE(a <= c);
    REQUIRE(!(a < b));
}

// =====================================================
// 26. Signed vs Unsigned comparison (C++ trap)
// =====================================================

TEST_CASE("Wrappers: signed vs unsigned comparison")
{
    Int32 s = -1;
    UInt32 u = 1;

    // C++ standard behavior
    REQUIRE(!(s < u));
    REQUIRE(s > u);
}

// =====================================================
// 27. constexpr cast chain
// =====================================================

constexpr Int32 cx_i32 = 10;
constexpr Double cx_d = cx_i32;
constexpr Int32 cx_back = static_cast<Int32>(cx_d);

static_assert((int)cx_back == 10);

// =====================================================
// 29. Ternary operator promotion
// =====================================================

TEST_CASE("Wrappers: ternary operator promotion")
{
    Int32 a = 10;
    Double b = 2.5;

    auto r = (a > 5) ? (Double)a : b; // Quality-issue :D - This means we are forced to push conversion

    REQUIRE((double)r == 10.0);
}

// =====================================================
// 30. Assignment from floating point
// =====================================================

TEST_CASE("Wrappers: floating point assignment")
{
    Double d = 3.14;

    Int32 a = d;
    REQUIRE((int)a == 3);
}