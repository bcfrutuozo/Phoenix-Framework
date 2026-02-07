#pragma once

#include "catch_amalgamated.hpp"

#include "System/Collections/Queue.hpp"
#include "System/Collections/Array.hpp"
#include "System/Types/Text/String.hpp"
#include "System/Types/Primitives/UInt64.hpp"

// Struct customizada para testes
struct Custom {
    int a, b;
    Custom() : a(0), b(0) {}
    Custom(int x, int y) : a(x), b(y) {}

    bool operator==(const Custom& o) const { return a == o.a && b == o.b; }
    bool operator!=(const Custom& o) const { return !(*this == o); }
};

// ------------------------------------------------------------
// Tests for Queue<int>
// ------------------------------------------------------------
TEST_CASE("Queue<int> - basic enqueue/dequeue", "[Queue][int]") {
    Queue<int> q;

    REQUIRE(q.IsEmpty());
    REQUIRE(q.GetLength() == 0);

    q.Enqueue(10);
    q.Enqueue(20);
    q.Enqueue(30);

    REQUIRE(q.GetLength() == 3);
    REQUIRE(q.Peek() == 10);

    REQUIRE(q.Dequeue() == 10);
    REQUIRE(q.Dequeue() == 20);

    REQUIRE(q.GetLength() == 1);
    REQUIRE(q.Peek() == 30);
    REQUIRE(q.Dequeue() == 30);

    REQUIRE(q.IsEmpty());
}

TEST_CASE("Queue<int> - enqueue many elements", "[Queue][growth]") {
    Queue<int> q;

    for (int i = 0; i < 100; i++)
        q.Enqueue(i);

    REQUIRE(q.GetLength() == 100);
    REQUIRE(q.Peek() == 0);

    for (int i = 0; i < 100; i++)
        REQUIRE(q.Dequeue() == i);

    REQUIRE(q.IsEmpty());
}

TEST_CASE("Queue<int> - wrap-around behavior", "[Queue][wrap]") {
    Queue<int> q;

    for (int i = 0; i < 5; i++)
        q.Enqueue(i);

    REQUIRE(q.Dequeue() == 0);
    REQUIRE(q.Dequeue() == 1);

    // agora Tail deve ter dado wrap-around
    q.Enqueue(100);
    q.Enqueue(200);

    REQUIRE(q.GetLength() == 5);

    Array<int> arr = q.ToArray();

    REQUIRE(arr.GetLength() == 5);
    REQUIRE(arr[0] == 2);
    REQUIRE(arr[1] == 3);
    REQUIRE(arr[2] == 4);
    REQUIRE(arr[3] == 100);
    REQUIRE(arr[4] == 200);
}

// ------------------------------------------------------------
// Contains, Clear, Peek
// ------------------------------------------------------------
TEST_CASE("Queue<int> - Contains / Clear / Peek", "[Queue][operations]") {
    Queue<int> q;

    q.Enqueue(1);
    q.Enqueue(2);
    q.Enqueue(3);

    REQUIRE(q.Contains(2));
    REQUIRE(!q.Contains(99));

    REQUIRE(q.Peek() == 1);

    q.Clear();
    REQUIRE(q.GetLength() == 0);
    REQUIRE(q.IsEmpty());
}

// ------------------------------------------------------------
// EnsureCapacity
// ------------------------------------------------------------
TEST_CASE("Queue<int> - EnsureCapacity", "[Queue][capacity]") {
    Queue<int> q;
    q.EnsureCapacity(50);
    REQUIRE(q.GetLength() == 0);     // não altera o tamanho
    // não temos getter para capacity, mas podemos testar expandindo:
    for (int i = 0; i < 50; i++)
        q.Enqueue(i);

    REQUIRE(q.GetLength() == 50);
}

// ------------------------------------------------------------
// Copy constructor / Move constructor
// ------------------------------------------------------------
TEST_CASE("Queue<int> - copy & move", "[Queue][copy][move]") {

    SECTION("Copy constructor") {
        Queue<int> q;
        q.Enqueue(10);
        q.Enqueue(20);
        q.Enqueue(30);

        Queue<int> c(q);

        REQUIRE(c.GetLength() == 3);
        REQUIRE(c.Dequeue() == 10);
        REQUIRE(c.Dequeue() == 20);
        REQUIRE(c.Dequeue() == 30);
    }

    SECTION("Move constructor") {
        Queue<int> q;
        q.Enqueue(11);
        q.Enqueue(22);

        Queue<int> m(std::move(q));

        REQUIRE(m.GetLength() == 2);
        REQUIRE(m.Dequeue() == 11);
        REQUIRE(m.Dequeue() == 22);

        REQUIRE(q.GetLength() == 0);
    }
}

// ------------------------------------------------------------
// Queue<char>
// ------------------------------------------------------------
TEST_CASE("Queue<char>", "[Queue][char]") {
    Queue<char> q;
    q.Enqueue('A');
    q.Enqueue('B');
    q.Enqueue('C');

    REQUIRE(q.GetLength() == 3);
    REQUIRE(q.Dequeue() == 'A');
    REQUIRE(q.Dequeue() == 'B');
    REQUIRE(q.Dequeue() == 'C');
}

// ------------------------------------------------------------
// Queue<bool>
// ------------------------------------------------------------
TEST_CASE("Queue<bool>", "[Queue][bool]") {
    Queue<bool> q;
    q.Enqueue(true);
    q.Enqueue(false);
    q.Enqueue(true);

    REQUIRE(q.GetLength() == 3);

    REQUIRE(q.Dequeue() == true);
    REQUIRE(q.Dequeue() == false);
    REQUIRE(q.Dequeue() == true);
}

// ------------------------------------------------------------
// Queue<UInt64>
// ------------------------------------------------------------
TEST_CASE("Queue<UInt64>", "[Queue][UInt64]") {
    Queue<UInt64> q;

    q.Enqueue(UInt64(10));
    q.Enqueue(UInt64(20));
    q.Enqueue(UInt64(30));

    REQUIRE(q.Contains(UInt64(20)));
    REQUIRE(q.GetLength() == 3);

    // implicit conversion through operator size_t() must work
    UInt64 idx16(1);
    // no operator[] in queue, but let's ensure comparison works
    REQUIRE(q.ToArray()[1] == UInt64(20));

    REQUIRE(q.Dequeue() == UInt64(10));
    REQUIRE(q.Dequeue() == UInt64(20));
    REQUIRE(q.Dequeue() == UInt64(30));
}

// ------------------------------------------------------------
// Queue<String>
// ------------------------------------------------------------
TEST_CASE("Queue<String>", "[Queue][String]") {
    Queue<String> q;

    q.Enqueue(String("Apple"));
    q.Enqueue(String("Banana"));
    q.Enqueue(String("Cherry"));

    REQUIRE(q.GetLength() == 3);
    REQUIRE(q.Peek() == String("Apple"));

    REQUIRE(q.Dequeue() == String("Apple"));
    REQUIRE(q.Dequeue() == String("Banana"));
    REQUIRE(q.Dequeue() == String("Cherry"));
}

// ------------------------------------------------------------
// Queue<Custom>
// ------------------------------------------------------------
TEST_CASE("Queue<Custom>", "[Queue][Custom]") {
    Queue<Custom> q;

    q.Enqueue(Custom(1, 2));
    q.Enqueue(Custom(3, 4));
    q.Enqueue(Custom(5, 6));

    REQUIRE(q.GetLength() == 3);
    REQUIRE(q.Peek() == Custom(1, 2));

    REQUIRE(q.Dequeue() == Custom(1, 2));
    REQUIRE(q.Dequeue() == Custom(3, 4));
    REQUIRE(q.Dequeue() == Custom(5, 6));
}

// ------------------------------------------------------------
// Equality tests
// ------------------------------------------------------------
TEST_CASE("Queue<T> - equality and inequality", "[Queue][compare]") {
    Queue<int> a;
    Queue<int> b;

    for (int i = 0; i < 5; i++) {
        a.Enqueue(i);
        b.Enqueue(i);
    }

    REQUIRE(a == b);

    b.Dequeue();
    b.Enqueue(999);

    REQUIRE(a != b);
}

// ------------------------------------------------------------
// ToArray
// ------------------------------------------------------------
TEST_CASE("Queue<T> - ToArray", "[Queue][array]") {
    Queue<int> q;

    q.Enqueue(10);
    q.Enqueue(20);
    q.Enqueue(30);
    q.Enqueue(40);

    Array<int> arr = q.ToArray();

    REQUIRE(arr.GetLength() == 4);
    REQUIRE(arr[0] == 10);
    REQUIRE(arr[1] == 20);
    REQUIRE(arr[2] == 30);
    REQUIRE(arr[3] == 40);
}