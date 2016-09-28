// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_FWD_RECT_HPP
#define NYTL_INCLUDE_FWD_RECT_HPP

#include <cstdint>

namespace nytl
{

template<std::size_t D, class P> class Rect;

template<typename P> using Rect2 = Rect<2, P>;
template<typename P> using Rect3 = Rect<3, P>;
template<typename P> using Rect4 = Rect<4, P>;

using Rect2i = Rect<2, int>;
using Rect2ui = Rect<2, unsigned int>;
using Rect2d = Rect<2, double>;
using Rect2f = Rect<2, float>;

using Rect3i = Rect<3, int>;
using Rect3ui = Rect<3, unsigned int>;
using Rect3d = Rect<3, double>;
using Rect3f = Rect<3, float>;

using Rect4i = Rect<4, int>;
using Rect4ui = Rect<4, unsigned int>;
using Rect4d = Rect<4, double>;
using Rect4f = Rect<4, float>;

}

#endif //header guard
