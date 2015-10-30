/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Jan Kelling
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <nytl/vec.hpp>
#include <nytl/line.hpp>

namespace nytl
{

//typedefs
template<size_t dim, typename prec> class triangle;
template<typename prec> using triangle2 = triangle<2, prec>;
template<typename prec> using triangle3 = triangle<3, prec>;
template<typename prec> using triangle4 = triangle<4, prec>;

using triangle2f = triangle<2, float>;
using triangle3f = triangle<3, float>;
using triangle4f = triangle<4, float>;

using triangle2d = triangle<2, double>;
using triangle3d = triangle<3, double>;
using triangle4d = triangle<4, double>;

using triangle2i = triangle<2, int>;
using triangle3i = triangle<3, int>;
using triangle4i = triangle<4, int>;

using triangle2ui = triangle<2, unsigned int>;
using triangle3ui = triangle<3, unsigned int>;
using triangle4ui = triangle<4, unsigned int>;


//should rlly be declared here? use helper member functions?
//tests
template<std::size_t dim, typename prec> constexpr
bool intersects(const triangle<dim, prec>&, const triangle<dim, prec>&);
template<std::size_t dim, typename prec> constexpr
bool intersects(const triangle<dim, prec>&, const line<dim, prec>&);

template<std::size_t dim, typename prec> constexpr 
bool contains(const triangle<dim, prec>&, const triangle<dim, prec>&);
template<std::size_t dim, typename prec> constexpr 
bool contains(const triangle<dim, prec>&, const line<dim, prec>&);
template<std::size_t dim, typename prec> NYUTIL_CPP14_CONSTEXPR  
bool contains(const triangle<dim, prec>&, const vec<dim, prec>&);


//class
template<size_t dim, typename prec>
class triangle
{
public:
    using value_type = prec;
    using vec_type = vec<dim, prec>;
    using triangle_type = triangle<dim, prec>;
    using line_type = line<dim, prec>;

public:
    vec_type a;
    vec_type b;
    vec_type c;

public:
    constexpr triangle() = default;
    constexpr triangle(const vec_type& xa, const vec_type& xb, const vec_type& xc) 
		: a(xa), b(xb), c(xc) {}

    ~triangle() noexcept = default;

    constexpr triangle(const triangle_type& other) noexcept = default;
    constexpr triangle& operator=(const triangle_type& other) noexcept = default;

    constexpr triangle(triangle_type&& other) noexcept = default;
    constexpr triangle& operator=(triangle_type&& other) noexcept = default;

    //
    constexpr double size() const { return length(b - a) * length(c - a) * 0.5; }

    //todo
    constexpr float angleA() const { return angle(AB().difference(), AC().difference()); }
    constexpr float angleB() const { return angle(BA().difference(), BC().difference()); }
    constexpr float angleC() const { return angle(CB().difference(), CA().difference()); }

    constexpr line_type AB() const { return line_type(a, b); }
    constexpr line_type AC() const { return line_type(a, c); }
    constexpr line_type BC() const { return line_type(b, c); }
    constexpr line_type BA() const { return line_type(b, a); }
    constexpr line_type CA() const { return line_type(c, a); }
    constexpr line_type CB() const { return line_type(c, b); }

    //conversion
    template<size_t odim, typename oprec> constexpr
    operator triangle<odim, oprec>() const { return triangle<odim, oprec>(a, b, c); }
};

//utility and operators/test
#include <nytl/bits/triangle.inl>

}
