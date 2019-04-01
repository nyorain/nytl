// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_FWD_MAT
#define NYTL_INCLUDE_FWD_MAT

#include <cstdlib> // std::size_t

namespace nytl {

using std::size_t;

template<size_t R, size_t C, typename T> struct Mat;
template<size_t D, typename T> using SquareMat = Mat<D, D, T>;

template<class T> using Mat2 = SquareMat<2,T>;
template<class T> using Mat3 = SquareMat<3,T>;
template<class T> using Mat4 = SquareMat<4,T>;

template<class T> using Mat23 = Mat<2,3,T>;
template<class T> using Mat24 = Mat<2,4,T>;

template<class T> using Mat32 = Mat<3,2,T>;
template<class T> using Mat34 = Mat<3,4,T>;

template<class T> using Mat42 = Mat<4,2,T>;
template<class T> using Mat43 = Mat<4,3,T>;

using Mat2f = Mat2<float>;
using Mat2d = Mat2<double>;
using Mat2i = Mat2<int>;
using Mat2ui = Mat2<unsigned int>;

using Mat3f = Mat3<float>;
using Mat3d = Mat3<double>;
using Mat3i = Mat3<int>;
using Mat3ui = Mat3<unsigned int>;

using Mat4f = Mat4<float>;
using Mat4d = Mat4<double>;
using Mat4i = Mat4<int>;
using Mat4ui = Mat4<unsigned int>;

} // namespace nytl

#endif //header guard
