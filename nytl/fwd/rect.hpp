// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_FWD_RECT
#define NYTL_INCLUDE_FWD_RECT

#include <cstdlib> // std::size_t
#include <cstdint> // std::uint8_t

namespace nytl {

template<std::size_t D, typename T> class Rect;

template<typename T> using Rect2 = Rect<2, T>;
template<typename T> using Rect3 = Rect<3, T>;
template<typename T> using Rect4 = Rect<4, T>;

using Rect2i = Rect<2, int>;
using Rect2ui = Rect<2, unsigned int>;
using Rect2f = Rect<2, float>;
using Rect2d = Rect<2, double>;

using Rect3i = Rect<3, int>;
using Rect3ui = Rect<3, unsigned int>;
using Rect3d = Rect<3, double>;
using Rect3f = Rect<3, float>;

using Rect4i = Rect<4, int>;
using Rect4ui = Rect<4, unsigned int>;
using Rect4d = Rect<4, double>;
using Rect4f = Rect<4, float>;

}

#endif // header guard
