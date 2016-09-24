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
///\brief Includes the Rect template class as well as several operators for it.

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
