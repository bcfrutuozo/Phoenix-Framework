#pragma once

#include <stddef.h>

#include "types/Boolean.hpp"
#include "types/UInt64.hpp"
#include "Array.hpp"

template<typename T>
class Queue {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = UInt64;
    using pointer = T*;
    using const_pointer = const T*;

private:
    pointer Data;
    size_type Size;
    size_type Capacity;
    size_type Head;
    size_type Tail;

    static constexpr size_type k_default_capacity = 4;

    // Value-initializing allocator
    static pointer allocate(size_type cap) {
        if (cap == 0) return nullptr;
        return new value_type[cap](); // value-init = zero for primitives
    }

    // Expande a capacidade usando regra de crescimento tipo List<T> / .NET Queue
    void grow() {
        size_type newCap = (Capacity == 0) ? k_default_capacity : Capacity * 2;
        pointer newBuf = allocate(newCap);

        // copiar dados na ordem lógica atual
        for (size_type i = 0; i < Size; ++i)
            newBuf[i] = Data[(Head + i) % Capacity];

        delete[] Data;

        Data = newBuf;
        Capacity = newCap;
        Head = 0;
        Tail = Size;
    }

public:

    // ------------------------------------------------------------
    // Constructors / Destructor
    // ------------------------------------------------------------
    Queue() noexcept :
        Data(nullptr),
        Size(0),
        Capacity(0),
        Head(0),
        Tail(0) {
    }

    explicit Queue(size_type initialCapacity) :
        Data(nullptr),
        Size(0),
        Capacity(initialCapacity),
        Head(0),
        Tail(0)
    {
        if (initialCapacity > 0)
            Data = allocate(initialCapacity);
    }

    Queue(const Queue& other) :
        Data(nullptr),
        Size(other.Size),
        Capacity(other.Capacity),
        Head(0),
        Tail(other.Size)
    {
        if (Capacity > 0) {
            Data = allocate(Capacity);
            for (size_type i = 0; i < Size; ++i)
                Data[i] = other.Data[(other.Head + i) % other.Capacity];
        }
    }

    Queue(Queue&& other) noexcept :
        Data(other.Data),
        Size(other.Size),
        Capacity(other.Capacity),
        Head(other.Head),
        Tail(other.Tail)
    {
        other.Data = nullptr;
        other.Size = 0;
        other.Capacity = 0;
        other.Head = 0;
        other.Tail = 0;
    }

    ~Queue() {
        if (Data) delete[] Data;
    }

    Queue& operator=(const Queue& other) {
        if (&other == this) return *this;
        Queue temp(other);
        Swap(temp);
        return *this;
    }

    Queue& operator=(Queue&& other) noexcept {
        if (&other == this) return *this;

        if (Data) delete[] Data;

        Data = other.Data;
        Size = other.Size;
        Capacity = other.Capacity;
        Head = other.Head;
        Tail = other.Tail;

        other.Data = nullptr;
        other.Size = 0;
        other.Capacity = 0;
        other.Head = 0;
        other.Tail = 0;

        return *this;
    }

    // ------------------------------------------------------------
    // Core Queue API
    // ------------------------------------------------------------
    inline constexpr size_type GetLength() const noexcept { return Size; }
    inline constexpr Boolean IsEmpty() const noexcept { return Size == 0; }

    void EnsureCapacity(size_type min) {
        if (Capacity >= min) return;
        while (Capacity < min) grow();
    }

    void Enqueue(const_reference value) {
        if (Size == Capacity) grow();
        if (Capacity == 0) grow();

        Data[Tail] = value;
        Tail = (Tail + 1) % Capacity;
        ++Size;
    }

    value_type Dequeue() {
        if (IsEmpty()) throw "Queue is empty";

        value_type out = Data[Head];
        Head = (Head + 1) % Capacity;
        --Size;
        return out;
    }

    value_type Peek() const {
        if (IsEmpty()) throw "Queue is empty";
        return Data[Head];
    }

    Boolean Contains(const_reference v) const noexcept {
        for (size_type i = 0; i < Size; ++i)
            if (Data[(Head + i) % Capacity] == v)
                return true;
        return false;
    }

    void Clear() noexcept {
        Size = 0;
        Head = 0;
        Tail = 0;
    }

    // ------------------------------------------------------------
    // Array conversion
    // ------------------------------------------------------------
    Array<value_type> ToArray() const {
        Array<value_type> arr(Size);
        for (size_type i = 0; i < Size; ++i)
            arr[i] = Data[(Head + i) % Capacity];
        return arr;
    }

    // ------------------------------------------------------------
    // Swapping
    // ------------------------------------------------------------
    void Swap(Queue& other) noexcept {
        using std::swap;

        swap(Data, other.Data);
        swap(Size, other.Size);
        swap(Capacity, other.Capacity);
        swap(Head, other.Head);
        swap(Tail, other.Tail);
    }

    // ------------------------------------------------------------
    // Operators
    // ------------------------------------------------------------
    Boolean operator==(const Queue& other) const noexcept {
        if (Size != other.Size) return false;
        for (size_type i = 0; i < Size; ++i) {
            if (Data[(Head + i) % Capacity] != other.Data[(other.Head + i) % other.Capacity])
                return false;
        }
        return true;
    }

    inline constexpr Boolean operator!=(const Queue& other) const noexcept {
        return !(*this == other);
    }

    template<typename... Args>
    static Queue<T> Create(Args&&... args) {
        constexpr size_type N = sizeof...(Args);
        Queue<T> r(N);
        T values[] = { T(static_cast<Args&&>(args))... };
        for (size_type i = 0; i < N; ++i) r.Enqueue(values[i])
            ;
        return r;
    }
};
