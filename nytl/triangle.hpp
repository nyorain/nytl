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

///\file
///\brief The 2-dimensional simplex specialization (triangle).

#pragma once

#include <nytl/vec.hpp>
#include <nytl/simplex.hpp>

namespace nytl
{

//typedefs
template<typename P> using triangle2 = triangle<2, P>;
template<typename P> using triangle3 = triangle<3, P>;
template<typename P> using triangle4 = triangle<4, P>;

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

template<size_t D, typename P>
class simplex<D, P, 2>
{
public:
    using value_type = P;
    using vec_type = vec<D, P>;
    using triangle_type = triangle<D, P>;
    using line_type = line<D, P>;

public:
    vec_type a;
    vec_type b;
    vec_type c;

public:
    simplex(const vec_type& xa, const vec_type& xb, const vec_type& xc) noexcept
		: a(xa), b(xb), c(xc) {}

    simplex() noexcept = default;
    ~simplex() noexcept = default;
    simplex(const triangle_type& other) noexcept = default;
    triangle_type& operator=(const triangle_type& other) noexcept = default;

	//default
    double size() const;
	vec_type center() const;
	bool valid() const;

	vec<3, vec_type>& points()
		{ return reinterpret_cast<vec<3, vec_type>&>(*this); }
	const vec<3, vec_type>& points() const 
		{ return reinterpret_cast<const vec<3, vec_type>&>(*this); }

    template<size_t OD, typename OP> constexpr
    operator triangle<OD, OP>() const { return triangle<OD, OP>(a, b, c); }

	//triangle specific
    float angleA() const { return angle(AB().difference(), AC().difference()); }
    float angleB() const { return angle(BA().difference(), BC().difference()); }
    float angleC() const { return angle(CB().difference(), CA().difference()); }

    line_type AB() const { return line_type(a, b); }
    line_type AC() const { return line_type(a, c); }
    line_type BC() const { return line_type(b, c); }
    line_type BA() const { return line_type(b, a); }
    line_type CA() const { return line_type(c, a); }
    line_type CB() const { return line_type(c, b); }
};

//utility and operators/test
#include <nytl/bits/triangle.inl>

}
