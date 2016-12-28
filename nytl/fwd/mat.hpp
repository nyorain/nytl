// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_FWD_MAT_HPP
#define NYTL_INCLUDE_FWD_MAT_HPP

#include <cstdint>

namespace nytl
{

template<std::size_t R, std::size_t C, typename P> class Mat;
template<std::size_t D, class P> using SquareMat = Mat<D, D, P>;

template<class P> using Mat2 = SquareMat<2,P>;
template<class P> using Mat3 = SquareMat<3,P>;
template<class P> using Mat4 = SquareMat<4,P>;

template<class P> using Mat23 = Mat<2,3,P>;
template<class P> using Mat24 = Mat<2,4,P>;

template<class P> using Mat32 = Mat<3,2,P>;
template<class P> using Mat34 = Mat<3,4,P>;

template<class P> using Mat42 = Mat<4,2,P>;
template<class P> using Mat43 = Mat<4,3,P>;

typedef Mat2<float> Mat2f;
typedef Mat2<unsigned int> Mat2ui;
typedef Mat2<int> Mat2i;
typedef Mat2<double> Mat2d;
typedef Mat2<bool> Mat2b;

typedef Mat3<float> Mat3f;
typedef Mat3<unsigned int> Mat3ui;
typedef Mat3<int> Mat3i;
typedef Mat3<double> Mat3d;
typedef Mat3<bool> Mat3b;

typedef Mat4<float> Mat4f;
typedef Mat4<unsigned int> Mat4ui;
typedef Mat4<int> Mat4i;
typedef Mat4<double> Mat4d;
typedef Mat4<bool> Mat4b;

//
typedef Mat23<float> Mat23f;
typedef Mat23<unsigned int> Mat23ui;
typedef Mat23<int> Mat23i;
typedef Mat23<double> Mat23d;
typedef Mat23<bool> Mat23b;

typedef Mat24<float> Mat24f;
typedef Mat24<unsigned int> Mat24ui;
typedef Mat24<int> Mat24i;
typedef Mat24<double> Mat24d;
typedef Mat24<bool> Mat24b;

typedef Mat32<float> Mat32f;
typedef Mat32<unsigned int> Mat32ui;
typedef Mat32<int> Mat32i;
typedef Mat32<double> Mat32d;
typedef Mat32<bool> Mat32b;

typedef Mat34<float> Mat34f;
typedef Mat34<unsigned int> Mat34ui;
typedef Mat34<int> Mat34i;
typedef Mat34<double> Mat34d;
typedef Mat34<bool> Mat34b;

typedef Mat42<float> Mat42f;
typedef Mat42<unsigned int> Mat42ui;
typedef Mat42<int> Mat42i;
typedef Mat42<double> Mat42d;
typedef Mat42<bool> Mat42b;

typedef Mat43<float> Mat43f;
typedef Mat43<unsigned int> Mat43ui;
typedef Mat43<int> Mat43i;
typedef Mat43<double> Mat43d;
typedef Mat43<bool> Mat43b;

}

#endif //header guard
