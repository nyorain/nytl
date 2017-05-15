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

} // namespace nytl

#endif // header guard
