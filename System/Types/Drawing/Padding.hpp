#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"
#include "Size.hpp"

class Padding : public Object<Padding>
{
public:

	constexpr Padding(i32 all) noexcept
		:
		_left(all),
		_top(all),
		_right(all),
		_bottom(all)
	{
	}

	constexpr Padding(i32 left, i32 top, i32 right, i32 bottom) noexcept
		:
		_left(left),
		_top(top),
		_right(right),
		_bottom(bottom)
	{
	}

	inline friend Boolean operator==(const Padding& a, const Padding& b) noexcept { return a.Equals(b); }
	inline friend Boolean operator!=(const Padding& a, const Padding& b) noexcept { return !(a == b); }

	inline constexpr i32 GetAll() const noexcept { return IsPerfect() ? _left : (i32)-1; }
	inline constexpr i32 GetLeft() const noexcept { return _left; }
	inline constexpr i32 GetTop() const noexcept { return _top; }
	inline constexpr i32 GetBottom() const noexcept { return _bottom; }
	inline constexpr i32 GetRight() const noexcept { return _right; }

	inline constexpr void SetAll(i32 value) noexcept { _top = _left = _right = _bottom = value; }
	inline constexpr void SetLeft(i32 value) noexcept { _left = value; }
	inline constexpr void SetTop(i32 value) noexcept { _top = value; }
	inline constexpr void SetBottom(i32 value) noexcept { _bottom = value; }
	inline constexpr void SetRight(i32 value) noexcept { _right = value; }

	inline constexpr i32 Horizontal() const noexcept { return _left + _right; }
	inline constexpr i32 Vertical() const noexcept { return _top + _bottom; }

	inline constexpr Size ToSize() const noexcept { return Size(Horizontal(), Vertical()); }

	inline constexpr Padding Add(const Padding& other) const noexcept { return Padding(_left + other._left, _top + other._top, _right + other._right, _bottom + other._bottom); }
	inline constexpr Padding Subtract(const Padding& other) const noexcept { return Padding(_left - other._left, _top - other._top, _right - other._right, _bottom - other._bottom); }

	inline constexpr Padding operator+(const Padding& other) const noexcept { return Add(other); }
	inline constexpr Padding operator-(const Padding& other) const noexcept { return Subtract(other); }

	inline constexpr Padding& operator+=(const Padding& other) noexcept { *this = Add(other); return *this; }
	inline constexpr Padding& operator-=(const Padding& other) noexcept { *this = Subtract(other); return *this; }

	inline constexpr void Scale(Single dx, Single dy) noexcept
	{
		_left = (_left * dx);
		_top = (_top * dy);
		_right = (_right * dx);
		_bottom = (_bottom * dy);
	}

	inline constexpr void ResetAll() noexcept { _left = _top = _right = _bottom = 0; }
	inline constexpr void ResetLeft() noexcept { _left = 0; }
	inline constexpr void ResetTop() noexcept { _top = 0; }
	inline constexpr void ResetRight() noexcept { _right = 0; }
	inline constexpr void ResetBottom() noexcept { _bottom = 0; }

	Boolean Equals(const Padding& other) const noexcept;
	u32 GetHashCode() const noexcept;
	String ToString() const noexcept;

	static const Padding Empty;

private:

	inline constexpr bool IsPerfect() const noexcept { return (_left == _top && _left == _right && _left == _bottom); }

	i32 _left;
	i32 _top;
	i32 _right;
	i32 _bottom;
};

inline const Padding Padding::Empty = { 0 };