#pragma once

// Minimal C headers only
#include <new>      // placement new if needed
#include <stddef.h> // size_t
#include <stdint.h> // uint*_t

#include "Array.hpp"
#include "types/UInt64.hpp"
#include "types/Int32.hpp"

// This List<T> is a dynamic-array (vector-like) container implemented without std::.

template<typename T>
class List {
public:
    using value_type = T;
    using size_type = UInt64;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = value_type*;
    using const_iterator = const value_type*;

private:
    pointer m_data;
    size_type m_size;
    size_type m_capacity;

    static constexpr size_type k_default_capacity = 4;

    // allocate n objects, value-initialized
    static pointer allocate(size_type n) {
        if (n == 0) return nullptr;
        // value-initialize so primitives become zero
        return new value_type[n]();
    }

    // allocate and copy existing elements via assignment
    static pointer allocate_and_copy(const_pointer src, size_type n) {
        if (n == 0) return nullptr;
        pointer p = new value_type[n]();
        for (size_type i = 0; i < n; ++i) p[i] = src[i];
        return p;
    }

    // grow capacity to at least min_capacity
    void grow_to_at_least(size_type min_capacity) {
        if (m_capacity >= min_capacity) return;
        size_type newCap = (m_capacity == 0) ? k_default_capacity : m_capacity * 2;
        while (newCap < min_capacity) {
            // avoid overflow
            newCap = (newCap > (size_type(0) - newCap)) ? min_capacity : newCap * 2;
            if (newCap < m_capacity) { newCap = min_capacity; break; }
        }
        reserve_internal(newCap);
    }

    // internal reserve: allocate new buffer of newCap and copy existing elements
    void reserve_internal(size_type newCap) {
        if (newCap == m_capacity) return;

        pointer newBuf = nullptr;
        if (newCap > 0) {
            newBuf = new value_type[newCap](); // default-constructed (not value-initialized)
            // copy existing elements
            for (size_type i = 0; i < m_size; ++i) newBuf[i] = m_data[i];
            // if new slots should be zeroed, user can call ClearRange or rely on default constructor
        }

        // release old buffer
        if (m_data) delete[] m_data;
        m_data = newBuf;
        m_capacity = newCap;
    }

public:
    // --- constructors / destructor ---
    List() noexcept
        : m_data(nullptr), m_size(0), m_capacity(0) {
    }

    explicit List(size_type capacity)
        : m_data(nullptr), m_size(0), m_capacity(0) {
        if (capacity > 0) {
            m_data = allocate(capacity);
            m_capacity = capacity;
        }
    }

    // construct with count copies of value
    List(size_type count, const_reference value)
        : m_data(nullptr), m_size(0), m_capacity(0) {
        if (count == 0) return;
        m_data = allocate(count);
        m_capacity = count;
        for (size_type i = 0; i < count; ++i) m_data[i] = value;
        m_size = count;
    }

    // construct from raw buffer (copy)
    List(const_pointer src, size_type length)
        : m_data(nullptr), m_size(0), m_capacity(0) {
        if (length == 0) return;
        m_data = allocate_and_copy(src, length);
        m_capacity = length;
        m_size = length;
    }

    // copy constructor
    List(const List& other)
        : m_data(nullptr), m_size(0), m_capacity(0) {
        if (other.m_size == 0) return;
        m_data = allocate_and_copy(other.m_data, other.m_size);
        m_size = other.m_size;
        m_capacity = other.m_size;
    }

    // move constructor
    List(List&& other) noexcept
        : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    // destructor
    ~List() noexcept {
        if (m_data) {
            delete[] m_data;
            m_data = nullptr;
        }
        m_size = 0;
        m_capacity = 0;
    }

    // copy assignment (copy-and-swap)
    List& operator=(const List& other) {
        if (&other == this) return *this;
        List tmp(other);
        Swap(tmp);
        return *this;
    }

    // move assignment
    List& operator=(List&& other) noexcept {
        if (&other == this) return *this;
        if (m_data) delete[] m_data;
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
        return *this;
    }

    // swap
    void Swap(List& other) noexcept {
        pointer td = m_data; m_data = other.m_data; other.m_data = td;
        size_type ts = m_size; m_size = other.m_size; other.m_size = ts;
        size_type tc = m_capacity; m_capacity = other.m_capacity; other.m_capacity = tc;
    }

    // --- capacity / size ---
    inline constexpr size_type Count() const noexcept { return m_size; }
    inline constexpr size_type Capacity() const noexcept { return m_capacity; }
    inline constexpr Boolean IsEmpty() const noexcept { return m_size == 0; }

    // Ensure there is room for at least min_capacity elements
    void EnsureCapacity(size_type min_capacity) {
        if (min_capacity <= m_capacity) return;
        grow_to_at_least(min_capacity);
    }

    // Reserve exact capacity (no shrink-to-fit if smaller)
    void Reserve(size_type newCapacity) {
        if (newCapacity <= m_capacity) return;
        reserve_internal(newCapacity);
    }

    void Resize(size_type newSize)
    {
        if (newSize > m_capacity)
            grow_to_at_least(newSize);

        // value-initialize novos elementos
        for (size_type i = m_size; i < newSize; ++i)
            m_data[i] = value_type();

        m_size = newSize;
    }

    // Reduce capacity to size
    void ShrinkToFit() {
        if (m_capacity == m_size) return;
        if (m_size == 0) {
            if (m_data) { delete[] m_data; m_data = nullptr; m_capacity = 0; }
            return;
        }
        reserve_internal(m_size);
    }

    // --- element access ---
    reference operator[](size_type idx) {
        if (idx >= m_size) throw "index out of range";
        return m_data[idx];
    }

    const_reference operator[](size_type idx) const {
        if (idx >= m_size) throw "index out of range";
        return m_data[idx];
    }

    reference GetAt(size_type idx) { return (*this)[idx]; }
    const_reference GetAt(size_type idx) const { return (*this)[idx]; }

    reference GetFront() {
        if (m_size == 0) throw "list is empty";
        return m_data[0];
    }

    const_reference GetFront() const {
        if (m_size == 0) throw "list is empty";
        return m_data[0];
    }

    reference GetBack() {
        if (m_size == 0) throw "list is empty";
        return m_data[m_size - 1];
    }

    const_reference GetBack() const {
        if (m_size == 0) throw "list is empty";
        return m_data[m_size - 1];
    }

    // --- modifiers ---

    // append
    void Add(const_reference value) {
        if (m_size + 1 > m_capacity) grow_to_at_least(m_size + 1);
        m_data[m_size++] = value;
    }

    // append (move)
    void Add(value_type&& value) {
        if (m_size + 1 > m_capacity) grow_to_at_least(m_size + 1);
        m_data[m_size++] = static_cast<value_type&&>(value);
    }

    // append range from another List
    void AddRange(const List& other) {
        if (other.m_size == 0) return;
        size_type needed = m_size + other.m_size;
        if (needed > m_capacity) grow_to_at_least(needed);
        for (size_type i = 0; i < other.m_size; ++i) m_data[m_size + i] = other.m_data[i];
        m_size = needed;
    }

    // append range from Array<T>-like (pointer + length)
    void AddRange(const_pointer src, size_type length) {
        if (length == 0) return;
        size_type needed = m_size + length;
        if (needed > m_capacity) grow_to_at_least(needed);
        for (size_type i = 0; i < length; ++i) m_data[m_size + i] = src[i];
        m_size = needed;
    }

    template<typename... Args>
    static List<T> Create(Args&&... args) {
        constexpr size_type N = sizeof...(Args);
        List<T> r(N);
        T values[] = { T(static_cast<Args&&>(args))... };
        for (size_type i = 0; i < N; ++i) r[i] = values[i];
        return r;
    }

    // insert at index (0..m_size). Insert at m_size == push back
    void Insert(size_type index, const_reference value) {
        if (index > m_size) throw "index out of range";
        if (m_size + 1 > m_capacity) grow_to_at_least(m_size + 1);
        // shift right
        for (size_type i = m_size; i > index; --i) m_data[i] = m_data[i - 1];
        m_data[index] = value;
        ++m_size;
    }

    // insert range from another List at index
    void InsertRange(size_type index, const List& other) {
        if (index > m_size) throw "index out of range";
        if (other.m_size == 0) return;
        size_type needed = m_size + other.m_size;
        if (needed > m_capacity) grow_to_at_least(needed);
        // move tail
        for (size_type i = m_size; i > index; --i) m_data[i + other.m_size - 1] = m_data[i - 1];
        // copy new
        for (size_type j = 0; j < other.m_size; ++j) m_data[index + j] = other.m_data[j];
        m_size = needed;
    }

    // remove the first occurrence; returns true if removed
    Boolean Remove(const_reference value) {
        size_type idx = IndexOfOrReturnMax(value);
        if (idx == (size_type)-1) return false;
        RemoveAt(idx);
        return true;
    }

    // remove at index and return removed value
    value_type RemoveAt(size_type index) {
        if (index >= m_size) throw "index out of range";
        value_type removed = m_data[index];
        for (size_type i = index + 1; i < m_size; ++i) m_data[i - 1] = m_data[i];
        --m_size;
        return removed;
    }

    // remove a range starting at index count elements (index must be valid)
    void RemoveRange(size_type index, size_type count) {
        if (count == 0) return;
        if (index >= m_size) throw "index out of range";
        if (index + count > m_size) throw "range out of bounds";
        for (size_type i = index + count; i < m_size; ++i) m_data[i - count] = m_data[i];
        m_size -= count;
    }

    // clear (does not release capacity)
    void Clear() noexcept {
        for (size_type i = 0; i < m_size; ++i) m_data[i] = value_type();
        m_size = 0;
    }

    // fast clear (does not initialize elements)
    void FastClear() noexcept {
        m_size = 0;
    }

    // --- search / queries ---

    bool Contains(const_reference v) const noexcept {
        return IndexOfNoThrow(v) != (size_type)-1;
    }

    // return first index or -1 (as size_type max)
    size_type IndexOf(const_reference v) const {
        size_type idx = IndexOfNoThrow(v);
        return idx;
    }

    size_type LastIndexOf(const_reference v) const {
        if (m_size == 0) return (size_type)-1;
        for (size_type i = m_size; i > 0; --i) {
            if (m_data[i - 1] == v) return i - 1;
        }
        return (size_type)-1;
    }

    size_type Count(const_reference v) const noexcept {
        size_type c = 0;
        for (size_type i = 0; i < m_size; ++i) if (m_data[i] == v) ++c;
        return c;
    }

    // convert to an Array-like dynamic buffer
    // returns a new List-managed buffer copy
    List ToListCopy() const {
        List r;
        if (m_size == 0) return r;
        r.m_data = allocate_and_copy(m_data, m_size);
        r.m_size = m_size;
        r.m_capacity = m_size;
        return r;
    }

    // Convert this List<T> into Array<T> (deep copy)
    Array<value_type> ToArray() const {
        if (m_size == 0) {
            return Array<value_type>(0);
        }
        return Array<value_type>(m_data, m_size);
    }

    // convert to plain buffer (Array style)
    // If you have Array<T> in your project, you can create Array<T> from m_data/m_size easily.
    pointer Data() noexcept { return m_data; }
    const_pointer Data() const noexcept { return m_data; }

    // iterators
    iterator begin() noexcept { return m_data; }
    iterator end() noexcept { return m_data + m_size; }

    const_iterator begin() const noexcept { return m_data; }
    const_iterator end() const noexcept { return m_data + m_size; }

    const_iterator cbegin() const noexcept { return m_data; }
    const_iterator cend() const noexcept { return m_data + m_size; }

private:
    // helper: indexOf that returns max-size if not found
    size_type IndexOfNoThrow(const_reference v) const noexcept {
        for (size_type i = 0; i < m_size; ++i) if (m_data[i] == v) return i;
        return (size_type)-1;
    }

    // wrapper to keep signature consistent with older IndexOf returning -1
    size_type IndexOfOrReturnMax(const_reference v) const noexcept {
        return IndexOfNoThrow(v);
    }
};