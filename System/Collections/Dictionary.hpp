#pragma once

#include "System/Types.hpp"
#include "System/Memory.hpp"
#include "System/Types/Hash.hpp"

static constexpr u64 MAP_INITIAL_CAPACITY = 16;
static constexpr Single MAP_MAX_LOAD = 0.70f;

template<typename K, typename V>
class Map
{
    Map()
        : _capacity(MAP_INITIAL_CAPACITY)
        , _count(0)
    {
        _entries = (Entry*)Memory::Alloc(sizeof(Entry) * _capacity);
        for (uint64_t i = 0; i < _capacity; ++i)
            _entries[i].occupied = false;
    }

    ~Map()
    {
        Clear();
        Memory::Free(_entries);
    }

    void Clear()
    {
        for (uint64_t i = 0; i < _capacity; ++i)
            _entries[i].occupied = false;
        _count = 0;
    }

    Boolean Insert(const K& key, const V& value)
    {
        if ((_count + 1) > (u64)(_capacity * MAP_MAX_LOAD))
            Rehash(_capacity * 2);

        u32 hash = Hash(key);

        for (uint64_t i = 0; i < _capacity; ++i)
        {
            uint64_t idx = Probe(hash, i);
            Entry& e = _entries[idx];

            if (!e.occupied || e.tombstone)
            {
                e.key = key;
                e.value = value;
                e.hash = hash;
                e.occupied = true;
                e.tombstone = false;
                ++_count;
                return true;
            }

            if (e.hash == hash && e.key == key)
            {
                e.value = value; // overwrite
                return true;
            }
        }

        return false;
    }

    V* Find(const K& key)
    {
        u32 hash = Hash(key);

        for (uint64_t i = 0; i < _capacity; ++i)
        {
            uint64_t idx = Probe(hash, i);
            Entry& e = _entries[idx];

            if (!e.occupied)
                return nullptr;

            if (!e.tombstone && e.hash == hash && e.key == key)
                return &e.value;
        }

        return nullptr;
    }

    const V* Find(const K& key) const
    {
        return const_cast<Map*>(this)->Find(key);
    }


    Boolean Contains(const K& key) const
    {
        return Find(key) != nullptr;
    }

    Boolean Remove(const K& key)
    {
        UInt32 hash = Hash(key);

        for (uint64_t i = 0; i < _capacity; ++i)
        {
            uint64_t idx = Probe(hash, i);
            Entry& e = _entries[idx];

            if (!e.occupied)
                return false;

            if (!e.tombstone && e.hash == hash && e.key == key)
            {
                e.tombstone = true;
                --_count;
                return true;
            }
        }

        return false;
    }

    u64 Count() const noexcept
    {
        return _count;
    }

private:

    void Rehash(u64 newCapacity)
    {
        Entry* oldEntries = _entries;
        Size oldCap = _capacity;

        _entries = (Entry*)System::Alloc(sizeof(Entry) * newCapacity);
        _capacity = newCapacity;
        _count = 0;

        for (uint64_t i = 0; i < _capacity; ++i)
            _entries[i].occupied = false;

        for (uint64_t i = 0; i < oldCap; ++i)
        {
            if (oldEntries[i].occupied && !oldEntries[i].tombstone)
                Insert(oldEntries[i].key, oldEntries[i].value);
        }

        System::Free(oldEntries);
    }

    u64 Probe(u32 hash, u64 i) const
    {
        return (hash + i) % _capacity;
    }

    struct Entry
    {
        K key;
        V value;
        UInt32 hash;
        Boolean occupied;
        Boolean tombstone;
    };

    Entry* _entries;
    u64   _capacity;
    u64   _count;
};