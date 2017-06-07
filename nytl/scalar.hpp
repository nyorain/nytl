// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Various simple scalar related utility helpers.

#pragma once

#ifndef NYTL_INCLUDE_SCALAR
#define NYTL_INCLUDE_SCALAR

namespace nytl {
namespace constants {
	constexpr const auto pi = 3.14159265359;
	constexpr const auto degree = pi / 180.0; // one degree as radian
	constexpr const auto e = 2.71828182845;

	constexpr const auto dynamicSize = 0u; // make vec/span dynamic sized
} // namespace constants

/// \brief simply constexpr implementation of std::accumulate.
/// \requires Type 'InputIt' must be a InputIterator
/// \requiers T must be CopyAssignable and CopyConstructible
template<typename InputIt, typename T, typename BinaryOp>
constexpr auto accumulate(InputIt begin, InputIt last, T init, BinaryOp op)
{
	while(begin != last) init = op(init, *(begin++));
	return init;
}

/// \brief Converts the given angle in radians to degrees.
/// \requires P must represent a mathematical field.
template<typename P>
constexpr auto degrees(P rad)
{
	return rad / constants::degree;
}

/// \brief Converts the given angle in degrees to radians.
/// \requires P must represent a mathematical field.
template<typename P>
constexpr auto radians(P deg)
{
	return deg * constants::degree;
}

// TODO: C++17
/// \brief Clamps the given value between min and max.
/// \requires P must represent a mathematical field.
template<typename P>
constexpr auto clamp(P x, P min, P max)
{
	return (x > max) ? max : ((x < min) ? min : x);
}

/// \brief Returns the linear mix of x and y with factor a.
/// \requires P must represent a mathematical field.
template<typename P>
constexpr auto mix(P x, P y, P a)
{
	return x * (1 - a) + y * a;
}

/// \brief Returns a smooth interpolation between 0 and 1 depending on value
/// in the given range of [min, max]. Uses hermite interpolation.
template<typename P>
constexpr auto smoothstep(P min, P max, P x)
{
	auto t = clamp((x - min) / (max - min), P {0}, P {1});
	return t * t * (3 - 2 * t);
}

/// \brief Constexpr factorial implementation
/// Will overflow the returned value quickly (for sth. like n > 20).
/// \returns The factorial of 'n'
constexpr unsigned long factorial(unsigned int n)
{
	return (n <= 1ul) ? 1ul : n * factorial(n - 1);
}

/// Maps the given signed number onto a unique unsigned one.
/// Maps 0 to 0, 1 to 2, 2 to 4, -1 to 1, -2 to 3 and so on.
unsigned int mapUnsigned(int x)
{
	return (x < 0) ? -x * 2 - 1 : x * 2;
}

/// Reverses the mapUnsigned function.
/// Returns the signed number that would be mapped onto the given unique
/// unsigned number.
int unmapUnsigned(unsigned int x)
{
	auto ix = static_cast<int>(x);
	return (ix % 2) ? -(ix + 1) / 2 : ix / 2;
}

/// Combines the two given unsigned numbers into a single unique one
/// using the cantorsche pairing function. Combine it with calls
/// to mapUnsigned to enable it for signed x,y inputs.
unsigned int pair(unsigned int x, unsigned int y)
{
	return (x + y) * (x + y + 1) / 2 + y;
}

/// Returns clamp(value, min, max), i.e. value or min/max if value exceeds those bounds.
/// Stores in store if the value was clamped, i.e. sets store to 0 if it was not clamped,
/// to -1 if it was clamped to min or to 1 if is was clamped to max.
auto clampStore(float value, float min, float max, int& store)
{
	store = 0;
	if(value <= min) {
		store = -1;
		return min;
	} else if(value >= max) {
		store = 1;
		return max;
	}

	return value;
}

} // namespace nytl

#endif // header guard
