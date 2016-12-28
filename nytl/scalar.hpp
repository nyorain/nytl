// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Various simple scalar related utility helpers.

#pragma once

#ifndef NYTL_INCLUDE_SCALAR_HPP
#define NYTL_INCLUDE_SCALAR_HPP

namespace nytl {

namespace constants {
	constexpr const auto pi = 3.14159265359;
	constexpr const auto degree = pi / 180.0; // one degree as radian
	constexpr const auto e = 2.71828182845;

	constexpr const auto dynamicSize = 0u; // make vec/span dyanmic sized
} // namespace constants


/// \brief simply constexpr implementation of std::accumulate.
/// \requires Type 'InputIt' must be a InputIterator
/// \requiers T must be CopyAssignable and CopyConstructible
/// \module vec
template<typename InputIt, typename T, typename BinaryOp>
constexpr auto accumulate(InputIt begin, InputIt last, T init, BinaryOp op)
{
	while(begin != last) init = op(init, *(begin++));
	return init;
}

/// \brief Converts the given angle in radians to degrees.
/// \requires P must be a mathematical field.
template<typename P>
constexpr auto degrees(const P& rad)
{
	return rad / constants::degree;
}

/// \brief Converts the given angle in degrees to radians.
/// \requires P must be a mathematical field.
template<typename P>
constexpr auto radians(const P& deg)
{
	return deg * constants::degree;
}

/// \brief Constexpr factorial implementation
/// \returns The factorial of 'n'
constexpr unsigned long factorial(unsigned int n)
{
	return (n <= 1ul) ? 1ul : n * factorial(n - 1);
}

} // namespace nytl

#endif // header guard
