#pragma once

#include "System/Meta/TypeTraits.hpp"
#include "SocketBase.hpp"
#include "TcpSocket.hpp"
#include "TcpListener.hpp"
#include "UdpSocket.hpp"

// Move-only
static_assert(!is_copy_constructible<TcpSocket>::value);
static_assert(is_move_constructible<TcpSocket>::value);

static_assert(!is_copy_constructible<TcpListener>::value);
static_assert(is_move_constructible<TcpListener>::value);

static_assert(!is_copy_constructible<UdpSocket>::value);
static_assert(is_move_constructible<UdpSocket>::value);

// Zero-overhead (handle only)
static_assert(sizeof(SocketBase) == sizeof(SocketHandle));

// Polymorphism not required
static_assert(!is_polymorphic<SocketBase>::value);