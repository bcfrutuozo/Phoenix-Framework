#pragma once

#include "System/Meta/TypeTraits.hpp"
#include "System/Globals.hpp"

// ==============================================================
//  Object - no virtual and no overhead
//  - Polimorfismo estático via CRTP
//  - Zero bytes adicionados à classe derivada
// ==============================================================

template <typename T>
struct Object
{
	// ----------------------------------------------
	//  ReferenceEquals(a,b)
	// ----------------------------------------------
	static bool ReferenceEquals(const T& a, const T& b) noexcept { return &a == &b; }

	// ----------------------------------------------
	//  Equals
	//  T must implement: 
	//      bool Equals(const T&) const noexcept
	// ----------------------------------------------
	auto Equals(const T& other) const noexcept { return static_cast<const T*>(this)->Equals(other); }

	// ----------------------------------------------
	//  GetHashCode
	//  T must implement:
	//      uint32_t GetHashCode() const noexcept
	// ----------------------------------------------
	auto GetHashCode() const noexcept
	{
		uintptr_t addr = reinterpret_cast<uintptr_t>(this);

		// espalhar os bits do ponteiro
		uint64_t hash = addr ^ (addr >> 16);

		// aplicar seed randomica global (opcional, igual .NET Core)
		hash ^= Phoenix::GLOBAL_HASH_SEED;

		// mistura leve
		hash *= 0x85ebca6b;
		hash ^= (hash >> 13);

		return (uint32_t)hash;
	}

	// ----------------------------------------------
	//  ToString
	//  T must implement:
	//      String ToString() const noexcept
	// ----------------------------------------------
	auto ToString() const noexcept {
		return static_cast<const T*>(this)->ToString();
	}
};


// ==============================================================
//  IEquatable<T>
//  - Usado para tipos que implementam comparação por valor
// ==============================================================

template <typename T>
struct IEquatable
{
	// A classe T deve implementar:
	//   bool Equals(const T&) const noexcept
	//
	auto Equals(const T& other) const noexcept {
		return static_cast<const T*>(this)->Equals(other);
	}
};