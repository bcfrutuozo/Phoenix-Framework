#pragma once

#include "catch_amalgamated.hpp"

#include "System/Collections/List.hpp"
#include "System/Collections/Array.hpp"
#include "System/Types/String.hpp"
#include "System/Types/UInt64.hpp"

struct Custom {
    int a;
    int b;

    Custom() : a(0), b(0) {}
    Custom(int x, int y) : a(x), b(y) {}

    bool operator==(const Custom& other) const {
        return a == other.a && b == other.b;
    }
    bool operator!=(const Custom& other) const {
        return !(*this == other);
    }
};

TEST_CASE("List<int> — basic operations", "[List][int]") {

    SECTION("Default constructor and Add") {
        List<int> lst;
        REQUIRE(lst.Count() == 0);

        lst.Add(10);
        lst.Add(20);
        lst.Add(30);

        REQUIRE(lst.Count() == 3);
        REQUIRE(lst[0] == 10);
        REQUIRE(lst[1] == 20);
        REQUIRE(lst[2] == 30);
    }

    SECTION("Insert at beginning, middle, end") {
        List<int> lst;

        lst.Add(1);
        lst.Add(3);

        lst.Insert(1, 2); // middle
        lst.Insert(0, 0); // beginning
        lst.Insert(lst.Count(), 4); // end

        REQUIRE(lst.Count() == 5);
        REQUIRE(lst[0] == 0);
        REQUIRE(lst[1] == 1);
        REQUIRE(lst[2] == 2);
        REQUIRE(lst[3] == 3);
        REQUIRE(lst[4] == 4);
    }

    SECTION("Remove by value") {
        List<int> lst;
        lst.Add(10);
        lst.Add(20);
        lst.Add(30);
        lst.Add(20);

        REQUIRE(lst.Remove(20) == true);
        REQUIRE(lst.Count() == 3);
        REQUIRE(lst[0] == 10);
        REQUIRE(lst[1] == 30);
        REQUIRE(lst[2] == 20);

        REQUIRE(lst.Remove(999) == false);
    }

    SECTION("RemoveAt") {
        List<int> lst;
        lst.Add(10);
        lst.Add(20);
        lst.Add(30);

        REQUIRE(lst.RemoveAt(1) == 20);
        REQUIRE(lst.Count() == 2);
        REQUIRE(lst[0] == 10);
        REQUIRE(lst[1] == 30);
    }

    SECTION("RemoveRange") {
        List<int> lst;
        for (int i = 0; i < 10; i++) lst.Add(i);

        lst.RemoveRange(2, 5); // remove [2..6]

        REQUIRE(lst.Count() == 5);
        REQUIRE(lst[0] == 0);
        REQUIRE(lst[1] == 1);
        REQUIRE(lst[2] == 7);
        REQUIRE(lst[3] == 8);
        REQUIRE(lst[4] == 9);
    }

    SECTION("Contains / IndexOf / LastIndexOf") {
        List<int> lst;
        lst.Add(5);
        lst.Add(10);
        lst.Add(20);
        lst.Add(10);

        REQUIRE(lst.Contains(10));
        REQUIRE(lst.Contains(20));
        REQUIRE(!lst.Contains(999));

        REQUIRE(lst.IndexOf(10) == 1);
        REQUIRE(lst.LastIndexOf(10) == 3);
        REQUIRE(lst.IndexOf(777) == (size_t)-1);
    }

    SECTION("ToArray") {
        List<int> lst;
        lst.Add(1);
        lst.Add(2);
        lst.Add(3);

        Array<int> arr = lst.ToArray();

        REQUIRE(arr.GetLength() == 3);
        REQUIRE(arr[0] == 1);
        REQUIRE(arr[1] == 2);
        REQUIRE(arr[2] == 3);
    }

    SECTION("Clear") {
        List<int> lst;
        lst.Add(1);
        lst.Add(2);
        lst.Add(3);

        lst.Clear();

        REQUIRE(lst.Count() == 0);
        REQUIRE(lst.Capacity() >= 3); // capacity stays
    }

    SECTION("FastClear") {
        List<int> lst;
        lst.Add(1);
        lst.Add(2);
        lst.Add(3);

        lst.FastClear();

        REQUIRE(lst.Count() == 0);
        REQUIRE(lst.Capacity() >= 3);
    }
}

TEST_CASE("List<char>", "[List][char]") {
    List<char> lst;
    lst.Add('A');
    lst.Add('B');
    lst.Add('C');

    REQUIRE(lst.Count() == 3);
    REQUIRE(lst[0] == 'A');
    REQUIRE(lst[2] == 'C');

    lst.Insert(1, 'Z');
    REQUIRE(lst[1] == 'Z');
}

TEST_CASE("List<bool>", "[List][bool]") {
    List<bool> lst;
    lst.Add(true);
    lst.Add(false);
    lst.Add(true);

    REQUIRE(lst.Count(true) == 2);
    REQUIRE(lst.Count(false) == 1);

    lst.Remove(false);
    REQUIRE(lst.Count(false) == 0);
}

TEST_CASE("List<UInt64>", "[List][UInt64]") {
    List<UInt64> lst;
    lst.Add(UInt64(10));
    lst.Add(UInt64(20));
    lst.Add(UInt64(30));

    REQUIRE(lst.Count() == 3);
    REQUIRE(lst.Contains(UInt64(20)));
    REQUIRE(lst.IndexOf(UInt64(30)) == 2);

    // Testa indexer com conversão implícita
    UInt64 idx(1);
    REQUIRE(lst[idx] == UInt64(20));
}

TEST_CASE("List<String>", "[List][String]") {
    List<String> lst;
    lst.Add(String("Apple"));
    lst.Add(String("Banana"));
    lst.Add(String("Cherry"));

    REQUIRE(lst.Count() == 3);
    REQUIRE(lst[0] == String("Apple"));
    REQUIRE(lst[2] == String("Cherry"));

    lst.Insert(1, String("Mango"));
    REQUIRE(lst[1] == String("Mango"));
    REQUIRE(lst.Count() == 4);

    lst.Remove(String("Banana"));
    REQUIRE(lst.Count() == 3);

    REQUIRE(lst.Contains(String("Mango")));
}

TEST_CASE("List<Custom> (custom struct)", "[List][Custom]") {
    List<Custom> lst;

    lst.Add(Custom(1, 2));
    lst.Add(Custom(3, 4));
    lst.Add(Custom(5, 6));

    REQUIRE(lst.Count() == 3);
    REQUIRE(lst[1] == Custom(3, 4));

    lst.Insert(1, Custom(7, 7));
    REQUIRE(lst[1] == Custom(7, 7));
    REQUIRE(lst.Count() == 4);

    lst.Remove(Custom(5, 6));
    REQUIRE(lst.Count() == 3);
}

TEST_CASE("List capacity management", "[List][capacity]") {

    SECTION("EnsureCapacity") {
        List<int> lst;
        lst.EnsureCapacity(100);
        REQUIRE(lst.Capacity() >= 100);
        REQUIRE(lst.Count() == 0);
    }

    SECTION("Reserve") {
        List<int> lst;
        lst.Reserve(50);
        REQUIRE(lst.Capacity() >= 50);

        // Reserve should not reduce capacity
        lst.Reserve(10);
        REQUIRE(lst.Capacity() >= 50);
    }

    SECTION("ShrinkToFit") {
        List<int> lst;
        for (int i = 0; i < 20; i++) lst.Add(i);

        size_t oldCap = lst.Capacity();
        REQUIRE(oldCap >= 20);

        lst.RemoveRange(5, 10); // remove 10 items

        lst.ShrinkToFit();
        REQUIRE(lst.Capacity() == lst.Count());
        REQUIRE(lst.Count() == 10);
    }
}

TEST_CASE("List iterators", "[List][iterators]") {

    List<int> lst;
    for (int i = 0; i < 5; i++) lst.Add(i);

    int sum = 0;
    for (auto it = lst.begin(); it != lst.end(); ++it)
        sum += *it;

    REQUIRE(sum == 10); // 0+1+2+3+4 = 10

    // const_iterator
    const List<int>& clst = lst;
    int sum2 = 0;
    for (auto it = clst.cbegin(); it != clst.cend(); ++it)
        sum2 += *it;

    REQUIRE(sum2 == 10);
}