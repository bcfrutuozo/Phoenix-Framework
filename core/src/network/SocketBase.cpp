#include "SocketBase.hpp"
#include "network/os/SocketOS.hpp"

SocketBase::SocketBase() noexcept
{
    _handle.Reset();
}

SocketBase::~SocketBase() noexcept
{
    Close();
}

SocketBase::SocketBase(SocketBase&& other) noexcept
{
    _handle = other._handle;
    other._handle.Reset();
}

SocketBase& SocketBase::operator=(SocketBase&& other) noexcept
{
    if (this != &other)
    {
        Close();
        _handle = other._handle;
        other._handle.Reset();
    }
    return *this;
}

Boolean SocketBase::IsValid() const noexcept
{
    return _handle.IsValid();
}

SocketHandle SocketBase::Handle() const noexcept
{
    return _handle;
}

void SocketBase::Close() noexcept
{
    if (_handle.IsValid())
    {
        SocketOS::Close(_handle);
        _handle.Reset();
    }
}

SocketError SocketBase::SetBlocking(BlockingMode mode) noexcept
{
    if (!_handle.IsValid())
        return SocketError::InvalidHandle;

    return SocketOS::SetBlocking(_handle, mode);
}

SocketError SocketBase::SetRecvTimeout(UInt32 ms) noexcept
{
    return SocketOS::SetRecvTimeout(_handle, ms);
}

SocketError SocketBase::SetSendTimeout(UInt32 ms) noexcept
{
    return SocketOS::SetSendTimeout(_handle, ms);
}