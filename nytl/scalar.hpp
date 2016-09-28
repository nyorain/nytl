// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Defines several useful constants and conversion/utility functions for scalar numbers.

#pragma once

#ifndef NYTL_INCLUDE_SCALAR_HPP
#define NYTL_INCLUDE_SCALAR_HPP

#include <functional>
#include <algorithm>

namespace nytl
{

constexpr const double cPi = 3.14159265359;
constexpr const double cDeg = cPi / 180.0;
constexpr const double cE = 2.71828182845;

//degrees/radians
template<typename P>
constexpr auto degrees(const P& rad) -> decltype(rad / cDeg)
{
    return rad / cDeg;
}

template<typename P>
constexpr auto radians(const P& deg) -> decltype(deg * cDeg)
{
    return deg * cDeg;
}

//clamp
///TODO: use std version instead
constexpr double clamp(double val, double minVal, double maxVal)
{
    return
        val <= minVal ? minVal :
        val >= maxVal ? maxVal:
        val;
}

//mix
constexpr double mix(double x, double y, double a)
{
    return (x * (1 - a)) + (y * a);
}

//TODO: fix that. change everywhere to std::min/max
using std::min;
using std::max;

constexpr unsigned long fac(unsigned long n)
{
	return (n <= 1ul) ? 1ul : n * fac(n - 1);
}

}

#endif //header guard
