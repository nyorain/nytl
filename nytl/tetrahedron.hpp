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
///\brief The 3-dimensional simplex spiecialization (tetrahedron).

#pragma once

#include <nytl/vec.hpp>
#include <nytl/simplex.hpp>

namespace nytl
{

//typedefs
///Defines the 3 dimensional simplex specialization.
template<size_t D, typename P> using tetrahedron = simplex<D, P, 3>;
template<typename P> using tetrahedron3 = tetrahedron<3, P>;
template<typename P> using tetrahedron4 = tetrahedron<4, P>;

using tetrahedron3f = tetrahedron<3, float>;
using tetrahedron4f = tetrahedron<4, float>;

using tetrahedron3d = tetrahedron<3, double>;
using tetrahedron4d = tetrahedron<4, double>;

using tetrahedron3i = tetrahedron<3, int>;
using tetrahedron4i = tetrahedron<4, int>;

using tetrahedron3ui = tetrahedron<3, unsigned int>;
using tetrahedron4ui = tetrahedron<4, unsigned int>;

template<size_t D, typename P>
class simplex<D, P, 3>
{
public:
    using value_type = P;
    using vec_type = vec<D, P>;
    using tetrahedron_type = tetrahedron<D, P>;
    using line_type = line<D, P>;
    using triangle_type = triangle<D, P>;

public:
    vec_type a;
    vec_type b;
    vec_type c;
    vec_type d;

public:
    simplex(const vec_type& xa, const vec_type& xb, const vec_type& xc, const vec_type& xd) noexcept
		: a(xa), b(xb), c(xc), d(xd) {}

    simplex() noexcept = default;
    ~simplex() noexcept = default;
    simplex(const tetrahedron_type& other) noexcept = default;
    tetrahedron_type& operator=(const tetrahedron_type& other) noexcept = default;

	//default
    double size() const;
	vec_type center() const;
	vec<4, double> barycentric(const vec_type& v) const;
	bool valid() const;

	vec<4, vec_type>& points()
		{ return reinterpret_cast<vec<4, vec_type>&>(*this); }
	const vec<4, vec_type>& points() const 
		{ return reinterpret_cast<const vec<4, vec_type>&>(*this); }

    template<size_t OD, typename OP> constexpr
    operator tetrahedron<OD, OP>() const { return tetrahedron<OD, OP>(a, b, c, d); }

	//tetrahedron specific
	//edges
    line_type AB() const { return line_type(a, b); }
    line_type AC() const { return line_type(a, c); }
    line_type AD() const { return line_type(a, d); }

    line_type BA() const { return line_type(b, a); }
    line_type BC() const { return line_type(b, c); }
    line_type BD() const { return line_type(b, d); }

    line_type CA() const { return line_type(c, a); }
    line_type CB() const { return line_type(c, b); }
    line_type CD() const { return line_type(c, d); }

    line_type DA() const { return line_type(d, a); }
    line_type DB() const { return line_type(d, b); }
    line_type DC() const { return line_type(d, c); }

	//faces
	triangle_type ABC() const { return triangle_type(a, b, c); }
	triangle_type ABD() const { return triangle_type(a, b, d); }
	triangle_type ACD() const { return triangle_type(a, c, d); }
	triangle_type BCD() const { return triangle_type(b, c, d); }

	//TODO: angle
};

//utility and operators/test
#include <nytl/bits/tetrahedron.inl>

}
