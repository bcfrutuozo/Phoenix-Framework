#pragma once

template<typename>
class Wrapper;        // forward

template<typename>
struct WrapperAccess; // forward

struct PrimitiveAccess {
private:

    PrimitiveAccess() = default;

    // Quem PODE criar o token
    template<typename>
    friend class Wrapper;

    // Quem PODE usar o token
    template<typename>
    friend struct WrapperAccess;
};

template<typename T>
struct WrapperAccess {
    static constexpr auto& get(T& v, PrimitiveAccess) noexcept {
        return v.Value;
    }

    static constexpr auto const& get(T const& v, PrimitiveAccess) noexcept {
        return v.Value;
    }
};

template<typename Derived>
class Wrapper
{
public:
    static constexpr PrimitiveAccess Access() noexcept { return PrimitiveAccess{}; }
};