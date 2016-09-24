// The MIT License (MIT)
//
// Copyright (c) 2016 nyorain
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
