#pragma once

#include "System/Types.hpp"

class Point : public Object<Point>
{
public:

	constexpr Point()
		:
		_x(0),
		_y(0)
	{

	}

	constexpr Point(i32 x, i32 y)
		:
		_x(x),
		_y(y)
	{

	}

	constexpr Point(Point const&) = default;
	constexpr Point(Point&&) = default;
	constexpr Point& operator=(Point const&) = default;
	constexpr Point& operator=(Point&&) = default;

	inline constexpr i32 GetX() const noexcept { return _x; }
	inline constexpr void SetX(i32 x) noexcept { _x = x; }
	inline constexpr i32 GetY() const noexcept { return _y; }
	inline constexpr void SetY(i32 y) noexcept { _y = y; }

	inline constexpr Point Add(const Point& other) const noexcept { return Point(_x + other._x, _y + other._y); }
	inline constexpr Point Add(i32 x, i32 y) const noexcept { return Point(_x + x, _y + y); }
	inline constexpr Point Subtract(const Point& other) const noexcept { return Point(_x - other._x, _y - other._y); }
	inline constexpr Point Subtract(i32 x, i32 y) const noexcept { return Point(_x - x, _y - y); }
	inline constexpr Point Multiply(const Point& other) const noexcept { return Point(_x * other._x, _y * other._y); }
	inline constexpr Point Multiply(i32 x, i32 y) const noexcept { return Point(_x * x, _y * y); }
	inline constexpr Point Divide(const Point& other) const noexcept { return Point(_x / other._x, _y / other._y); }
	inline constexpr Point Divide(i32 x, i32 y) const noexcept { return Point(_x / x, _y / y); }

	inline constexpr Point operator+(const Point& other) const noexcept { return Add(other); }
	inline constexpr Point operator-(const Point& other) const noexcept { return Subtract(other); }
	inline constexpr Point operator*(const Point& other) const noexcept { return Multiply(other); }
	inline constexpr Point operator/(const Point& other) const noexcept { return Divide(other); }

	inline constexpr Point& operator+=(const Point& other) noexcept { *this = Add(other); return *this; }
	inline constexpr Point& operator-=(const Point& other) noexcept { *this = Subtract(other); return *this;}
	inline constexpr Point& operator*=(const Point& other) noexcept { *this = Multiply(other); return *this;}
	inline constexpr Point& operator/=(const Point& other) noexcept { *this = Divide(other); return *this;}

private:

	i32 _x;
	i32 _y;
};