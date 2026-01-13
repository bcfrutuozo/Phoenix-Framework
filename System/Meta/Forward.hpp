// ---------------------------------------------
//  remove_reference
// ---------------------------------------------
template<typename T>
struct remove_reference { using type = T; };

template<typename T>
struct remove_reference<T&> { using type = T; };

template<typename T>
struct remove_reference<T&&> { using type = T; };

template<typename T>
using remove_reference_t = typename remove_reference<T>::type;

// ---------------------------------------------
//  is_lvalue_reference
// ---------------------------------------------
template<typename T>
struct is_lvalue_reference { static constexpr bool value = false; };

template<typename T>
struct is_lvalue_reference<T&> { static constexpr bool value = true; };

// ---------------------------------------------
//  forward
// ---------------------------------------------
template<typename T>
constexpr T&& Forward(remove_reference_t<T>& t) noexcept
{
    return static_cast<T&&>(t);
}

template<typename T>
constexpr T&& Forward(remove_reference_t<T>&& t) noexcept
{
    static_assert(!is_lvalue_reference<T>::value,
        "Bad forward: cannot forward an rvalue as an lvalue.");
    return static_cast<T&&>(t);
}