#pragma once

#include "System/Types.hpp"

class Size : public Object<Size>
{
public:

	constexpr Size()
		:
		_width(0),
		_height(0)
	{ }

	constexpr Size(i32 width, i32 height)
		:
		_width(width),
		_height(height)
	{

	}

	constexpr Size(Size const&) = default;
	constexpr Size(Size&&) = default;
	constexpr Size& operator=(Size const&) = default;
	constexpr Size& operator=(Size&&) = default;

	inline constexpr i32 GetWidth() const noexcept { return _width; }
	inline constexpr void SetWidth(i32 width) noexcept { _width = width; }
	inline constexpr i32 GetHeight() const noexcept { return _height; }
	inline constexpr void SetHeight(i32 height) noexcept { _height = height; }

	inline constexpr Size Add(const Size& other) const noexcept { return Size(_width + other._width, _height + other._height); }
	inline constexpr Size Add(i32 width, i32 height) const noexcept { return Size(_width + width, _height + height); }
	inline constexpr Size Subtract(const Size& other) const noexcept { return Size(_width - other._width, _height - other._height); }
	inline constexpr Size Subtract(i32 width, i32 height) const noexcept { return Size(_width - width, _height - height); }
	inline constexpr Size Multiply(const Size& other) const noexcept { return Size(_width * other._width, _height * other._height); }
	inline constexpr Size Multiply(i32 width, i32 height) const noexcept { return Size(_width * width, _height * height); }
	inline constexpr Size Divide(const Size& other) const noexcept { return Size(_width / other._width, _height / other._height); }
	inline constexpr Size Divide(i32 width, i32 height) const noexcept { return Size(_width / width, _height / height); }

	inline constexpr Size operator+(const Size& other) const noexcept { return Add(other); }
	inline constexpr Size operator-(const Size& other) const noexcept { return Subtract(other); }
	inline constexpr Size operator*(const Size& other) const noexcept { return Multiply(other); }
	inline constexpr Size operator/(const Size& other) const noexcept { return Divide(other); }

	inline constexpr Size& operator+=(const Size& other) noexcept { *this = Add(other); return *this; }
	inline constexpr Size& operator-=(const Size& other) noexcept { *this = Subtract(other); return *this; }
	inline constexpr Size& operator*=(const Size& other) noexcept { *this = Multiply(other); return *this; }
	inline constexpr Size& operator/=(const Size& other) noexcept { *this = Divide(other); return *this; }

private:

	i32 _width;
	i32 _height;
};