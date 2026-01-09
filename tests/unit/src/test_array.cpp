#pragma once

#include "catch_amalgamated.hpp"

#include "System/Collections/Array.hpp"
#include "System/Types/UInt64.hpp"
#include "System/Types/String.hpp"

TEST_CASE("Array<int> basics", "[Array][int]") {

    SECTION("Construction & basic access") {
        Array<int> a(5);
        REQUIRE(a.GetLength() == 5);

        for (size_t i = 0; i < 5; ++i)
            a[i] = (int)(i * 10);

        REQUIRE(a[0] == 0);
        REQUIRE(a[1] == 10);
        REQUIRE(a[2] == 20);
        REQUIRE(a[3] == 30);
        REQUIRE(a[4] == 40);

        REQUIRE(a.GetFront() == 0);
        REQUIRE(a.GetBack() == 40);
    }

    SECTION("Copy constructor") {
        Array<int> a(3);
        a[0] = 1; a[1] = 2; a[2] = 3;

        Array<int> b(a);
        REQUIRE(b.GetLength() == 3);
        REQUIRE(b[0] == 1);
        REQUIRE(b[1] == 2);
        REQUIRE(b[2] == 3);

        REQUIRE(b == a);
    }

    SECTION("Move constructor") {
        Array<int> a(3);
        a[0] = 10; a[1] = 20; a[2] = 30;

        Array<int> b(std::move(a));
        REQUIRE(b.GetLength() == 3);
        REQUIRE(b[0] == 10);
        REQUIRE(b[1] == 20);
        REQUIRE(b[2] == 30);

        REQUIRE(a.GetLength() == 0);
    }

    SECTION("Contains, Count, IndexOf, LastIndexOf") {
        Array<int> a(5);
        a[0] = 10;
        a[1] = 20;
        a[2] = 20;
        a[3] = 50;
        a[4] = 20;

        REQUIRE(a.Contains(10));
        REQUIRE(a.Contains(50));
        REQUIRE(!a.Contains(123));

        REQUIRE(a.Count(20) == 3);

        REQUIRE(a.IndexOf(20) == 1);
        REQUIRE(a.LastIndexOf(20) == 4);
    }

    SECTION("IndicesOf") {
        Array<int> a(5);
        a[0] = 7;
        a[1] = 2;
        a[2] = 7;
        a[3] = 7;
        a[4] = 1;

        Array<UInt64> idx = a.IndicesOf(7);

        REQUIRE(idx.GetLength() == 3);
        REQUIRE(idx[0] == 0);
        REQUIRE(idx[1] == 2);
        REQUIRE(idx[2] == 3);
    }

    SECTION("Fill and Clear") {
        Array<int> a(4);
        a.Fill(99);

        REQUIRE(a[0] == 99);
        REQUIRE(a[1] == 99);
        REQUIRE(a[2] == 99);
        REQUIRE(a[3] == 99);

        a.Clear();
        REQUIRE(a[0] == 0);
        REQUIRE(a[1] == 0);
        REQUIRE(a[2] == 0);
        REQUIRE(a[3] == 0);
    }

    SECTION("Reverse") {
        Array<int> a(5);
        for (int i = 0; i < 5; i++)
            a[i] = i + 1;

        a.Reverse();

        REQUIRE(a[0] == 5);
        REQUIRE(a[1] == 4);
        REQUIRE(a[2] == 3);
        REQUIRE(a[3] == 2);
        REQUIRE(a[4] == 1);
    }

    SECTION("Static Copy") {
        Array<int> src(3);
        src[0] = 11;
        src[1] = 22;
        src[2] = 33;

        Array<int> dest(3);
        dest.Fill(0);

        Array<int>::Copy(src, dest);
        REQUIRE(dest[0] == 11);
        REQUIRE(dest[1] == 22);
        REQUIRE(dest[2] == 33);
    }

    SECTION("Static Copy with indices") {
        Array<int> src(4);
        src[0] = 10; src[1] = 20; src[2] = 30; src[3] = 40;

        Array<int> dest(2);
        dest.Fill(1);

        Array<int>::Copy(src, 1, dest, 1);

        // dest.resize happened automatically
        REQUIRE(dest.GetLength() == 4);
        REQUIRE(dest[0] == 1);
        REQUIRE(dest[1] == 20);
        REQUIRE(dest[2] == 30);
        REQUIRE(dest[3] == 40);
    }

    SECTION("Static Resize") {
        Array<int> a(3);
        a[0] = 1; a[1] = 2; a[2] = 3;

        Array<int> b = Array<int>::Resize(a, 5);

        REQUIRE(b.GetLength() == 5);
        REQUIRE(b[0] == 1);
        REQUIRE(b[1] == 2);
        REQUIRE(b[2] == 3);
        REQUIRE(b[3] == 0);
        REQUIRE(b[4] == 0);

        Array<int> c = Array<int>::Resize(a, 1);
        REQUIRE(c.GetLength() == 1);
        REQUIRE(c[0] == 1);
    }
}

TEST_CASE("Array<char>", "[Array][char]") {
    Array<char> a(3);
    a[0] = 'A'; a[1] = 'B'; a[2] = 'C';

    REQUIRE(a.GetFront() == 'A');
    REQUIRE(a.GetBack() == 'C');

    REQUIRE(a.Contains('B'));
}

TEST_CASE("Array<bool>", "[Array][bool]") {
    Array<bool> a(4);
    a[0] = true;
    a[1] = false;
    a[2] = true;
    a[3] = true;

    REQUIRE(a.Count(true) == 3);
    REQUIRE(a.Count(false) == 1);
}

TEST_CASE("Array<UInt64> wrapper", "[Array][UInt64]") {
    Array<UInt64> a(3);
    a[0] = UInt64(10);
    a[1] = UInt64(20);
    a[2] = UInt64(30);

    REQUIRE(a.Contains(UInt64(20)));
    REQUIRE(a.IndexOf(UInt64(30)) == 2);

    UInt64 idx(1);
    REQUIRE(a[idx] == UInt64(20)); // implicit conversion works
}

TEST_CASE("Array<String>", "[Array][String]") {
    Array<String> a(3);

    a[0] = String("Hello");
    a[1] = String("World");
    a[2] = String("!");

    REQUIRE(a.GetLength() == 3);
    REQUIRE(a[0] == String("Hello"));
    REQUIRE(a[1] == String("World"));
    REQUIRE(a[2] == String("!"));

    REQUIRE(a.Contains(String("World")));
    REQUIRE(a.IndexOf(String("!")) == 2);

    // Reverse
    a.Reverse();
    REQUIRE(a[0] == String("!"));
    REQUIRE(a[2] == String("Hello"));

    // Resize
    Array<String> b = Array<String>::Resize(a, 5);
    REQUIRE(b.GetLength() == 5);
    REQUIRE(b[0] == String("!"));
    REQUIRE(b[1] == String("World"));
    REQUIRE(b[2] == String("Hello"));
    REQUIRE(b[3] == String(""));
    REQUIRE(b[4] == String(""));
}