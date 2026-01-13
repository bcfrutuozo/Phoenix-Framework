#pragma once

#include "System/Types.hpp"

// Simple Array<T> with .NET-like semantics (fixed-length array).
// - No use of std::*
// - Resize is static and returns a new Array<T>
// - Iterators are raw pointers
// - Reverse iterator implemented locally
// - No Add/Remove/Insert methods (handled by List<T> later)

template<typename T>
class Array {
public:
    using value_type = T;
    using size_type = u64;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = pointer;
    using const_iterator = const_pointer;

private:
    pointer m_data;
    size_type m_size;

    // Helper: default-construct N elements into raw storage allocated by new T[N]
    static pointer allocate_and_default_construct(size_type n) {
        if (n == 0) return nullptr;
        pointer p = new value_type[n](); // note the parentheses -> value-initialize elements
        return p;
    }

    // Helper: allocate and copy-construct from source (safe basic)
    static pointer allocate_and_copy(const_pointer src, size_type n) {
        if (n == 0) return nullptr;
        pointer p = new value_type[n];
        for (size_type i = 0; i < n; ++i) {
            p[i] = src[i]; // copy-assign into default-constructed slot
        }
        return p;
    }

public:
    // Default ctor -> empty array
    Array() noexcept
        : m_data(nullptr), m_size(0) {
    }

    // Construct with length; elements value-initialized (T())
    explicit Array(size_type length)
        : m_data(nullptr), m_size(length) {
        if (length == 0) {
            m_data = nullptr;
            return;
        }
        m_data = allocate_and_default_construct(length);
    }

    // Construct from raw buffer (copies elements)
    Array(const_pointer src, size_type length)
        : m_data(nullptr), m_size(length) {
        if (length == 0) {
            m_data = nullptr;
            return;
        }
        m_data = allocate_and_copy(src, length);
    }

    // Copy constructor (deep copy)
    Array(const Array& other)
        : m_data(nullptr), m_size(other.m_size) {
        if (other.m_size == 0) {
            m_data = nullptr;
            return;
        }
        m_data = allocate_and_copy(other.m_data, other.m_size);
    }

    // Move constructor (steal resources)
    Array(Array&& other) noexcept
        : m_data(other.m_data), m_size(other.m_size) {
        other.m_data = nullptr;
        other.m_size = 0;
    }

    // Destructor
    ~Array() noexcept {
        if (m_data) {
            delete[] m_data;
            m_data = nullptr;
            m_size = 0;
        }
    }

    // Copy assignment (copy-and-swap style)
    Array& operator=(const Array& other) {
        if (&other == this) return *this;
        Array tmp(other);
        Swap(tmp);
        return *this;
    }

    // Move assignment
    Array& operator=(Array&& other) noexcept {
        if (&other == this) return *this;
        if (m_data) delete[] m_data;
        m_data = other.m_data;
        m_size = other.m_size;
        other.m_data = nullptr;
        other.m_size = 0;
        return *this;
    }

    inline constexpr operator Pointer() const noexcept
    {
        return Pointer(m_data);
    }

    // Swap
    void Swap(Array& other) noexcept {
        pointer tdata = m_data;
        size_type tsize = m_size;
        m_data = other.m_data;
        m_size = other.m_size;
        other.m_data = tdata;
        other.m_size = tsize;
    }

    template<typename... Args>
    static Array<T> Create(Args&&... args) {
        constexpr size_type N = sizeof...(Args);
        Array<T> r(N);
        T values[] = { T(static_cast<Args&&>(args))... };
        for (size_type i = 0; i < N; ++i) r[i] = values[i];
        return r;
    }

    // Observers / Access
    inline size_type GetLength() const noexcept { return m_size; }
    inline Boolean IsEmpty() const noexcept { return m_size == 0; }

    inline constexpr pointer GetData() noexcept { return m_data; }
    inline constexpr const_pointer GetData() const noexcept { return m_data; }

    inline reference operator[](size_type idx) {
        // We throw plain C-string on error (no std::out_of_range)
        if (idx >= m_size) throw "index out of range";
        return m_data[idx];
    }

    inline const_reference operator[](size_type idx) const {
        if (idx >= m_size) throw "index out of range";
        return m_data[idx];
    }

    inline reference GetFront() {
        if (m_size == 0) throw "array is empty";
        return m_data[0];
    }

    inline const_reference GetFront() const {
        if (m_size == 0) throw "array is empty";
        return m_data[0];
    }

    inline reference GetBack() {
        if (m_size == 0) throw "array is empty";
        return m_data[m_size - 1];
    }

    inline const_reference GetBack() const {
        if (m_size == 0) throw "array is empty";
        return m_data[m_size - 1];
    }

    // Iterators (raw pointer)
    inline constexpr iterator begin() noexcept { return m_data; }
    inline constexpr iterator end() noexcept { return m_data ? m_data + m_size : nullptr; }
    inline constexpr const_iterator begin() const noexcept { return m_data; }
    inline constexpr const_iterator end() const noexcept { return m_data ? m_data + m_size : nullptr; }
    inline constexpr const_iterator cbegin() const noexcept { return m_data; }
    inline constexpr const_iterator cend() const noexcept { return m_data ? m_data + m_size : nullptr; }

    // Simple reverse iterator type (lightweight)
    class reverse_iterator {
        pointer m_ptr;
    public:
        reverse_iterator() : m_ptr(nullptr) {}
        explicit reverse_iterator(pointer p) : m_ptr(p) {}
        reverse_iterator& operator++() { --m_ptr; return *this; } // moves backward
        reverse_iterator operator++(int) { reverse_iterator tmp = *this; --m_ptr; return tmp; }
        reverse_iterator& operator--() { ++m_ptr; return *this; }
        reverse_iterator operator--(int) { reverse_iterator tmp = *this; ++m_ptr; return tmp; }
        reference operator*() const { return *m_ptr; }
        pointer operator->() const { return m_ptr; }
        bool operator==(const reverse_iterator& o) const { return m_ptr == o.m_ptr; }
        bool operator!=(const reverse_iterator& o) const { return m_ptr != o.m_ptr; }
    };

    inline reverse_iterator rbegin() noexcept {
        return reverse_iterator(m_data + (m_size == 0 ? 0 : m_size - 1));
    }
    inline reverse_iterator rend() noexcept {
        return reverse_iterator(m_data - 1); // sentinel: before begin()
    }

    // Fill (assign same value to every element)
    void Fill(const_reference value) {
        for (size_type i = 0; i < m_size; ++i) m_data[i] = value;
    }

    // Clear (set elements to default value T{})
    void Clear() {
        for (size_type i = 0; i < m_size; ++i) m_data[i] = value_type();
    }

    // Static Clear range (like Array.Clear in .NET)
    static void Clear(Array& a, size_type index, size_type length) {
        if (index >= a.m_size) throw "index out of range";
        if (index + length > a.m_size) throw "length out of range";
        for (size_type i = index; i < index + length; ++i) a.m_data[i] = value_type();
    }

    // Contains
    bool Contains(const_reference v) const noexcept {
        for (size_type i = 0; i < m_size; ++i)
            if (m_data[i] == v) return true;
        return false;
    }

    // IndexOf (first index) - throws if not found (mirrors your earlier design)
    size_type IndexOf(const_reference v) const {
        for (size_type i = 0; i < m_size; ++i)
            if (m_data[i] == v) return i;
        throw "element not found";
    }

    // LastIndexOf - throws if not found
    size_type LastIndexOf(const_reference v) const {
        if (m_size == 0) throw "array is empty";
        for (size_type i = m_size; i > 0; --i) {
            if (m_data[i - 1] == v) return i - 1;
        }
        throw "element not found";
    }

    // Count occurrences
    size_type Count(const_reference v) const noexcept {
        size_type c = 0;
        for (size_type i = 0; i < m_size; ++i) if (m_data[i] == v) ++c;
        return c;
    }

    // IndicesOf: returns an Array<size_type> with matching indices
    Array<size_type> IndicesOf(const_reference v) const {
        // first pass count
        size_type c = Count(v);
        Array<size_type> result(c);
        size_type j = 0;
        for (size_type i = 0; i < m_size; ++i) {
            if (m_data[i] == v) {
                result[j] = i;
                ++j;
            }
        }
        return result;
    }

    // Reverse in-place
    void Reverse() noexcept {
        if (m_size <= 1) return;
        size_type i = 0;
        size_type j = m_size - 1;
        while (i < j) {
            value_type tmp = m_data[i];
            m_data[i] = m_data[j];
            m_data[j] = tmp;
            ++i;
            --j;
        }
    }

    // Equality
    Boolean operator==(const Array& other) const noexcept {
        if (m_size != other.m_size) return false;
        for (size_type i = 0; i < m_size; ++i)
            if (!(m_data[i] == other.m_data[i])) return false;
        return true;
    }

    Boolean operator!=(const Array& other) const noexcept {
        return !(*this == other);
    }

    // Static copy: copy entire source into dest (dest must have at least source length)
    static void Copy(const Array& source, Array& destination) {
        if (source.m_size > destination.m_size) {
            // mimic .NET behavior: replace destination with source
            destination = source;
            return;
        }
        for (size_type i = 0; i < source.m_size; ++i) destination.m_data[i] = source.m_data[i];
    }

    // Static copy with indices (sourceIndex -> destinationIndex)
    static void Copy(const Array& source, size_type sourceIndex, Array& dest, size_type destIndex) {
        if (sourceIndex >= source.m_size) throw "source index out of range";
        if (destIndex > dest.m_size) throw "destination index out of range";
        size_type copyCount = source.m_size - sourceIndex;
        if (copyCount == 0) return;

        // if dest has not enough space, create a resized array and replace dest
        if (destIndex + copyCount > dest.m_size) {
            Array tmp = Resize(dest, destIndex + copyCount);
            for (size_type i = 0; i < copyCount; ++i) tmp.m_data[destIndex + i] = source.m_data[sourceIndex + i];
            dest = tmp;
            return;
        }

        for (size_type i = 0; i < copyCount; ++i) {
            dest.m_data[destIndex + i] = source.m_data[sourceIndex + i];
        }
    }

    // Static Resize: returns a new Array<T> with newSize
    // - If newSize > old: new slots are value-initialized (T())
    // - If newSize < old: array is truncated
    static Array Resize(const Array& a, size_type newSize) {
        if (newSize == a.m_size) {
            return a; // copy
        }
        if (newSize == 0) {
            return Array(); // empty
        }
        Array r(newSize);
        size_type toCopy = (a.m_size < newSize) ? a.m_size : newSize;
        for (size_type i = 0; i < toCopy; ++i) r.m_data[i] = a.m_data[i];
        // remaining slots already default-initialized in constructor
        return r;
    }
};