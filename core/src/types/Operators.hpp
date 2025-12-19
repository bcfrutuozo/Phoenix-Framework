#pragma once

#include "meta/ValueType.hpp"

/*
 * Operator+ (Addition)
 */
template<typename L, typename R>
constexpr auto operator+(L const& lhs, R const& rhs) noexcept requires(is_promotion_wrapper<L>::value && is_promotion_wrapper<R>::value)
{
	using Result = promote_t<L, R>;
	auto token = Wrapper<L>::Access();
	return Result(WrapperAccess<L>::get(lhs, token) +
		WrapperAccess<R>::get(rhs, token));
}

/*
 * Operator- (Subtraction)
 */
template<typename L, typename R>
constexpr auto operator-(L const& lhs, R const& rhs) noexcept requires(is_promotion_wrapper<L>::value && is_promotion_wrapper<R>::value)
{
	using Result = promote_t<L, R>;
	auto token = Wrapper<L>::Access();
	return Result(WrapperAccess<L>::get(lhs, token) -
		WrapperAccess<R>::get(rhs, token));
}

/*
* Operator* (Multiplication)
*/
template<typename L, typename R>
constexpr auto operator*(L const& lhs, R const& rhs) noexcept requires(is_promotion_wrapper<L>::value && is_promotion_wrapper<R>::value)
{
	using Result = promote_t<L, R>;
	auto token = Wrapper<L>::Access();
	return Result(WrapperAccess<L>::get(lhs, token) *
		WrapperAccess<R>::get(rhs, token));
}

/*
 * Operator/ (Division)
 */
template<typename L, typename R>
constexpr auto operator/(L const& lhs, R const& rhs) noexcept requires(is_promotion_wrapper<L>::value && is_promotion_wrapper<R>::value)
{
	using Result = promote_t<L, R>;
	auto token = Wrapper<L>::Access();
	return Result(WrapperAccess<L>::get(lhs, token) /
		WrapperAccess<R>::get(rhs, token));
}

/*
 * Operator% (Modulo)
 */
template<typename L, typename R>
constexpr auto operator%(L const& lhs, R const& rhs) noexcept requires(is_wrapper_integral<L>::value&& is_wrapper_integral<R>::value)
{
	using Result = promote_t<L, R>;
	auto token = Wrapper<L>::Access();
	return Result(WrapperAccess<L>::get(lhs, token) %
		WrapperAccess<R>::get(rhs, token));
}

/*
 * Operator& (Bitwise AND)
 */
template<typename L, typename R>
constexpr auto operator&(L const& lhs, R const& rhs) noexcept requires(is_wrapper_integral<L>::value&& is_wrapper_integral<R>::value)
{
	using Result = promote_t<L, R>;
	auto token = Wrapper<L>::Access();
	return Result(WrapperAccess<L>::get(lhs, token) &
		WrapperAccess<R>::get(rhs, token));
}

/*
 * Operator| (Bitwise OR)
 */
template<typename L, typename R>
constexpr auto operator|(L const& lhs, R const& rhs) noexcept requires(is_wrapper_integral<L>::value&& is_wrapper_integral<R>::value)
{
	using Result = promote_t<L, R>;
	auto token = Wrapper<L>::Access();
	return Result(WrapperAccess<L>::get(lhs, token) |
		WrapperAccess<R>::get(rhs, token));
}

/*
 * Operator^ (Bitwise XOR)
 */
template<typename L, typename R>
constexpr auto operator^(L const& lhs, R const& rhs) noexcept requires(is_wrapper_integral<L>::value&& is_wrapper_integral<R>::value)
{
	using Result = promote_t<L, R>;
	auto token = Wrapper<L>::Access();
	return Result(WrapperAccess<L>::get(lhs, token) ^
		WrapperAccess<R>::get(rhs, token));
}

/*
 * Operator<< (Shift-Left)
 */
template<typename L, typename R>
constexpr auto operator<<(L const& lhs, R const& rhs) noexcept requires(is_wrapper_integral<L>::value&& is_wrapper_integral<R>::value)
{
	using Result = promote_t<L, R>;
	auto token = Wrapper<L>::Access();
	return Result(WrapperAccess<L>::get(lhs, token) <<
		WrapperAccess<R>::get(rhs, token));
}


/*
 * Operator>> (Shift-Right)
 */
template<typename L, typename R>
constexpr auto operator>>(L const& lhs, R const& rhs) noexcept requires(is_wrapper_integral<L>::value&& is_wrapper_integral<R>::value)
{
	using Result = promote_t<L, R>;
	auto token = Wrapper<L>::Access();
	return Result(WrapperAccess<L>::get(lhs, token) >>
		WrapperAccess<R>::get(rhs, token));
}


/*
 * Operator== (Equality)
 */
template<typename L, typename R>
constexpr Boolean operator==(L const& lhs, R const& rhs) noexcept requires(is_promotion_wrapper<L>::value && is_promotion_wrapper<R>::value)
{
	auto token = Wrapper<L>::Access();
	return Boolean(WrapperAccess<L>::get(lhs, token) ==
		WrapperAccess<R>::get(rhs, token));
}

/*
 * Operator!= (Inequality)
 */
template<typename L, typename R>
constexpr Boolean operator!=(L const& lhs, R const& rhs) noexcept requires(is_promotion_wrapper<L>::value && is_promotion_wrapper<R>::value)
{
	auto token = Wrapper<L>::Access();
	return Boolean(WrapperAccess<L>::get(lhs, token) !=
		WrapperAccess<R>::get(rhs, token));
}

/*
 * Operator< (Less Than)
 */
template<typename L, typename R>
constexpr Boolean operator<(L const& lhs, R const& rhs) noexcept requires(is_promotion_wrapper<L>::value && is_promotion_wrapper<R>::value)
{
	auto token = Wrapper<L>::Access();
	return Boolean(WrapperAccess<L>::get(lhs, token) <
		WrapperAccess<R>::get(rhs, token));
}

/*
 * Operator<= (Less Than or Equal to )
 */
template<typename L, typename R>
constexpr Boolean operator<=(L const& lhs, R const& rhs) noexcept requires(is_promotion_wrapper<L>::value && is_promotion_wrapper<R>::value)
{
	auto token = Wrapper<L>::Access();
	return Boolean(WrapperAccess<L>::get(lhs, token) <=
		WrapperAccess<R>::get(rhs, token));
}

/*
 * Operator> (Greater than)
 */
template<typename L, typename R>
constexpr Boolean operator>(L const& lhs, R const& rhs) noexcept requires(is_promotion_wrapper<L>::value&& is_promotion_wrapper<R>::value)
{
	auto token = Wrapper<L>::Access();
	return Boolean(WrapperAccess<L>::get(lhs, token) >
		WrapperAccess<R>::get(rhs, token));
}

/*
 * Operator>= (Greater Than or Equal to )
 */
template<typename L, typename R>
constexpr Boolean operator>=(L const& lhs, R const& rhs) noexcept requires(is_promotion_wrapper<L>::value&& is_promotion_wrapper<R>::value)
{
	auto token = Wrapper<L>::Access();
	return Boolean(WrapperAccess<L>::get(lhs, token) >=
		WrapperAccess<R>::get(rhs, token));
}