// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_FWD_MAT
#define NYTL_INCLUDE_FWD_MAT

#include <cstdlib> // std::size_t

namespace nytl {

template<std::size_t R, std::size_t C, typename T> struct Mat;
template<std::size_t D, typename T> using SquareMat = Mat<D, D, T>;

template<class T> using Mat2 = SquareMat<2,T>;
template<class T> using Mat3 = SquareMat<3,T>;
template<class T> using Mat4 = SquareMat<4,T>;

template<class T> using Mat23 = Mat<2,3,T>;
template<class T> using Mat24 = Mat<2,4,T>;

template<class T> using Mat32 = Mat<3,2,T>;
template<class T> using Mat34 = Mat<3,4,T>;

template<class T> using Mat42 = Mat<4,2,T>;
template<class T> using Mat43 = Mat<4,3,T>;

}

#endif //header guard
