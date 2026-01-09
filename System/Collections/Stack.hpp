#pragma once

#include <stddef.h>

#include "Array.hpp"
#include "System/Types.hpp"

// ================================================================
//  Stack<T> - Dynamic Array Based
// ================================================================

template<typename T>
class Stack {

public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = u64;
    using pointer = T*;
    using const_pointer = const T*;

private:

    pointer Data;
    size_type Size;
    size_type Capacity;

    static constexpr size_type k_default_capacity = 4;

    // ------------------------------------------------------------
    // Allocador interno (value-init)
    // ------------------------------------------------------------
    static pointer allocate(size_type cap) {
        if (cap == 0) return nullptr;
        return new value_type[cap](); // value-init
    }

    // ------------------------------------------------------------
    // Crescimento automático (dobrando a capacidade)
    // ------------------------------------------------------------
    void grow() {
        size_type newCap = (Capacity == 0) ? k_default_capacity : Capacity * 2;
        pointer newBuf = allocate(newCap);

        // Copiar elementos atuais
        for (size_type i = 0; i < Size; ++i)
            newBuf[i] = Data[i];

        delete[] Data;
        Data = newBuf;
        Capacity = newCap;
    }

public:

    // ------------------------------------------------------------
    // Construtores
    // ------------------------------------------------------------
    Stack() noexcept :
        Data(nullptr),
        Size(0),
        Capacity(0)
    {
    }

    explicit Stack(size_type initialCapacity) :
        Data(nullptr),
        Size(0),
        Capacity(initialCapacity)
    {
        if (initialCapacity > 0)
            Data = allocate(initialCapacity);
    }

    Stack(const Stack& other) :
        Data(nullptr),
        Size(other.Size),
        Capacity(other.Capacity)
    {
        if (Capacity > 0) {
            Data = allocate(Capacity);
            for (size_type i = 0; i < Size; ++i)
                Data[i] = other.Data[i];
        }
    }

    Stack(Stack&& other) noexcept :
        Data(other.Data),
        Size(other.Size),
        Capacity(other.Capacity)
    {
        other.Data = nullptr;
        other.Size = 0;
        other.Capacity = 0;
    }

    // ------------------------------------------------------------
    // Destrutor
    // ------------------------------------------------------------
    ~Stack() {
        if (Data) delete[] Data;
    }

    // ------------------------------------------------------------
    // Operadores
    // ------------------------------------------------------------
    Stack& operator=(const Stack& other) {
        if (&other == this) return *this;

        Stack temp(other);
        Swap(temp);
        return *this;
    }

    Stack& operator=(Stack&& other) noexcept {
        if (&other == this) return *this;

        if (Data) delete[] Data;

        Data = other.Data;
        Size = other.Size;
        Capacity = other.Capacity;

        other.Data = nullptr;
        other.Size = 0;
        other.Capacity = 0;

        return *this;
    }

    Boolean operator==(const Stack& other) const noexcept {
        if (Size != other.Size) return false;
        for (size_type i = 0; i < Size; ++i)
            if (!(Data[i] == other.Data[i]))
                return false;
        return true;
    }

    Boolean operator!=(const Stack& other) const noexcept {
        return !(*this == other);
    }

    // ------------------------------------------------------------
    // Métodos estáticos Create (igual ao Array, Queue)
    // ------------------------------------------------------------
    static Stack Create(size_type capacity) {
        return Stack(capacity);
    }

    static Stack From(const_pointer src, size_type length) {
        Stack s(length);
        s.Size = length;
        for (size_type i = 0; i < length; ++i)
            s.Data[i] = src[i];
        return s;
    }

    static Stack From(const Array<T>& array) {
        return From(array.begin(), array.GetLength());
    }

    static Stack From(std::initializer_list<T> list) {
        Stack s(list.size());
        s.Size = list.size();
        size_type i = 0;
        for (auto& v : list)
            s.Data[i++] = v;
        return s;
    }

    // ------------------------------------------------------------
    // API principal
    // ------------------------------------------------------------
    inline constexpr size_type GetLength() const noexcept { return Size; }
    inline constexpr Boolean IsEmpty() const noexcept { return Size == 0; }

    void EnsureCapacity(size_type min) {
        if (Capacity >= min) return;
        while (Capacity < min)
            grow();
    }

    void Push(const_reference value) {
        if (Size == Capacity)
            grow();
        Data[Size++] = value;
    }

    value_type Pop() {
        if (IsEmpty())
            throw "Stack is empty";

        --Size;
        return Data[Size];
    }

    value_type Peek() const {
        if (IsEmpty())
            throw "Stack is empty";
        return Data[Size - 1];
    }

    Boolean Contains(const_reference v) const noexcept {
        for (size_type i = 0; i < Size; ++i)
            if (Data[i] == v)
                return true;
        return false;
    }

    void Clear() noexcept {
        Size = 0;
        // opcional:
        // for (size_type i = 0; i < Capacity; ++i) Data[i] = value_type();
    }

    Array<value_type> ToArray() const {
        Array<value_type> a(Size);
        for (size_type i = 0; i < Size; ++i)
            a[i] = Data[i];
        return a;
    }

    void Swap(Stack& other) noexcept {
        auto tmpData = Data;
        auto tmpSize = Size;
        auto tmpCap = Capacity;

        Data = other.Data;
        Size = other.Size;
        Capacity = other.Capacity;

        other.Data = tmpData;
        other.Size = tmpSize;
        other.Capacity = tmpCap;
    }
};