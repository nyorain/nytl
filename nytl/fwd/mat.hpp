// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_FWD_MAT
#define NYTL_INCLUDE_FWD_MAT

#include <cstdlib> // std::size_t

namespace nytl {

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

}

#endif //header guard
