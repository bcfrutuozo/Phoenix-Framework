#pragma once

#include "catch_amalgamated.hpp"

#include "System/Collections/Stack.hpp"
#include "System/Collections/Array.hpp"
#include "System/Types/Text/String.hpp"
#include "System/Types/Primitives/UInt64.hpp"

// ------------------------------------------------------------
// Struct custom para testar compatibilidade
// ------------------------------------------------------------
struct Custom {
    int a, b;
    Custom() : a(0), b(0) {}
    Custom(int x, int y) : a(x), b(y) {}

    bool operator==(const Custom& o) const { return a == o.a && b == o.b; }
    bool operator!=(const Custom& o) const { return !(*this == o); }
};

// ------------------------------------------------------------
// Stack<int>
// ------------------------------------------------------------
TEST_CASE("Stack<int> - basic push/pop", "[Stack][int]") {
    Stack<int> s;

    REQUIRE(s.IsEmpty());
    REQUIRE(s.GetLength() == 0);

    s.Push(10);
    s.Push(20);
    s.Push(30);

    REQUIRE(s.GetLength() == 3);
    REQUIRE(s.Peek() == 30);

    REQUIRE(s.Pop() == 30);
    REQUIRE(s.Pop() == 20);
    REQUIRE(s.Pop() == 10);

    REQUIRE(s.IsEmpty());
}

TEST_CASE("Stack<int> - growth test", "[Stack][growth]") {
    Stack<int> s;

    for (int i = 0; i < 100; i++)
        s.Push(i);

    REQUIRE(s.GetLength() == 100);

    // LIFO check
    for (int i = 99; i >= 0; i--)
        REQUIRE(s.Pop() == i);

    REQUIRE(s.IsEmpty());
}

TEST_CASE("Stack<int> - Contains / Clear", "[Stack][ops]") {
    Stack<int> s;

    s.Push(1);
    s.Push(2);
    s.Push(3);

    REQUIRE(s.Contains(2));
    REQUIRE(!s.Contains(999));

    s.Clear();
    REQUIRE(s.IsEmpty());
    REQUIRE(s.GetLength() == 0);
}

TEST_CASE("Stack<int> - ToArray", "[Stack][array]") {
    Stack<int> s;

    s.Push(10);
    s.Push(20);
    s.Push(30);
    s.Push(40);

    Array<int> arr = s.ToArray();

    REQUIRE(arr.GetLength() == 4);
    REQUIRE(arr[0] == 10);
    REQUIRE(arr[1] == 20);
    REQUIRE(arr[2] == 30);
    REQUIRE(arr[3] == 40);
}

// ------------------------------------------------------------
// Copy and Move
// ------------------------------------------------------------
TEST_CASE("Stack<int> - copy constructor", "[Stack][copy]") {
    Stack<int> s;
    s.Push(10);
    s.Push(20);
    s.Push(30);

    Stack<int> c(s);

    REQUIRE(c.GetLength() == 3);

    REQUIRE(c.Pop() == 30);
    REQUIRE(c.Pop() == 20);
    REQUIRE(c.Pop() == 10);
}

TEST_CASE("Stack<int> - move constructor", "[Stack][move]") {
    Stack<int> s;
    s.Push(5);
    s.Push(7);

    Stack<int> m(std::move(s));

    REQUIRE(m.GetLength() == 2);
    REQUIRE(m.Pop() == 7);
    REQUIRE(m.Pop() == 5);

    REQUIRE(s.GetLength() == 0); // moved-from
}

// ------------------------------------------------------------
// Equality / Inequality
// ------------------------------------------------------------
TEST_CASE("Stack<int> - equality", "[Stack][eq]") {
    Stack<int> a;
    Stack<int> b;

    for (int i = 0; i < 5; i++) {
        a.Push(i);
        b.Push(i);
    }

    REQUIRE(a == b);

    a.Push(99);

    REQUIRE(a != b);
}

// ------------------------------------------------------------
// Stack<char>
// ------------------------------------------------------------
TEST_CASE("Stack<char>", "[Stack][char]") {
    Stack<char> s;
    s.Push('A');
    s.Push('B');
    s.Push('C');

    REQUIRE(s.GetLength() == 3);

    REQUIRE(s.Pop() == 'C');
    REQUIRE(s.Pop() == 'B');
    REQUIRE(s.Pop() == 'A');
}

// ------------------------------------------------------------
// Stack<bool>
// ------------------------------------------------------------
TEST_CASE("Stack<bool>", "[Stack][bool]") {
    Stack<bool> s;
    s.Push(true);
    s.Push(false);
    s.Push(true);

    REQUIRE(s.GetLength() == 3);

    REQUIRE(s.Pop() == true);
    REQUIRE(s.Pop() == false);
    REQUIRE(s.Pop() == true);
}

// ------------------------------------------------------------
// Stack<UInt64>
// ------------------------------------------------------------
TEST_CASE("Stack<UInt64>", "[Stack][UInt64]") {
    Stack<UInt64> s;

    s.Push(UInt64(10));
    s.Push(UInt64(20));
    s.Push(UInt64(30));

    REQUIRE(s.Contains(UInt64(20)));
    REQUIRE(s.GetLength() == 3);

    Array<UInt64> arr = s.ToArray();
    REQUIRE(arr.GetLength() == 3);
    REQUIRE(arr[1] == UInt64(20));

    REQUIRE(s.Pop() == UInt64(30));
    REQUIRE(s.Pop() == UInt64(20));
    REQUIRE(s.Pop() == UInt64(10));
}

// ------------------------------------------------------------
// Stack<String>
// ------------------------------------------------------------
TEST_CASE("Stack<String>", "[Stack][String]") {
    Stack<String> s;

    s.Push(String("Apple"));
    s.Push(String("Banana"));
    s.Push(String("Cherry"));

    REQUIRE(s.Peek() == String("Cherry"));
    REQUIRE(s.Pop() == String("Cherry"));
    REQUIRE(s.Pop() == String("Banana"));
    REQUIRE(s.Pop() == String("Apple"));
}

// ------------------------------------------------------------
// Stack<Custom>
// ------------------------------------------------------------
TEST_CASE("Stack<Custom>", "[Stack][Custom]") {
    Stack<Custom> s;

    s.Push(Custom(1, 2));
    s.Push(Custom(3, 4));
    s.Push(Custom(5, 6));

    REQUIRE(s.GetLength() == 3);
    REQUIRE(s.Peek() == Custom(5, 6));

    REQUIRE(s.Pop() == Custom(5, 6));
    REQUIRE(s.Pop() == Custom(3, 4));
    REQUIRE(s.Pop() == Custom(1, 2));
}