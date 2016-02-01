/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Jan Kelling
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
///\brief The 2-dimensional Simplex specialization (Triangle).

#pragma once

#include <nytl/vec.hpp>
#include <nytl/simplex.hpp>

namespace nytl
{

//typedefs
template<typename P> using Triangle2 = Triangle<2, P>;
template<typename P> using Triangle3 = Triangle<3, P>;
template<typename P> using Triangle4 = Triangle<4, P>;

using Triangle2f = Triangle<2, float>;
using Triangle3f = Triangle<3, float>;
using Triangle4f = Triangle<4, float>;

using Triangle2d = Triangle<2, double>;
using Triangle3d = Triangle<3, double>;
using Triangle4d = Triangle<4, double>;

using Triangle2i = Triangle<2, int>;
using Triangle3i = Triangle<3, int>;
using Triangle4i = Triangle<4, int>;

using Triangle2ui = Triangle<2, unsigned int>;
using Triangle3ui = Triangle<3, unsigned int>;
using Triangle4ui = Triangle<4, unsigned int>;

template<size_t D, typename P>
class Simplex<D, P, 2>
{
public:
    using value_type = P;
    using VecType = Vec<D, P>;
    using Triangle_type = Triangle<D, P>;
    using LineType = Line<D, P>;

public:
    VecType a;
    VecType b;
    VecType c;

public:
    Simplex(const VecType& xa, const VecType& xb, const VecType& xc) noexcept
		: a(xa), b(xb), c(xc) {}

    Simplex() noexcept = default;
    ~Simplex() noexcept = default;
    Simplex(const Triangle_type& other) noexcept = default;
    Triangle_type& operator=(const Triangle_type& other) noexcept = default;

	//default
    double size() const;
	VecType center() const;
	bool valid() const;

	Vec<3, VecType>& points()
		{ return reinterpret_cast<Vec<3, VecType>&>(*this); }
	const Vec<3, VecType>& points() const 
		{ return reinterpret_cast<const Vec<3, VecType>&>(*this); }

    template<size_t OD, typename OP> constexpr
    operator Triangle<OD, OP>() const { return Triangle<OD, OP>(a, b, c); }

	//Triangle specific
    float angleA() const { return angle(AB().difference(), AC().difference()); }
    float angleB() const { return angle(BA().difference(), BC().difference()); }
    float angleC() const { return angle(CB().difference(), CA().difference()); }

    LineType AB() const { return LineType(a, b); }
    LineType AC() const { return LineType(a, c); }
    LineType BC() const { return LineType(b, c); }
    LineType BA() const { return LineType(b, a); }
    LineType CA() const { return LineType(c, a); }
    LineType CB() const { return LineType(c, b); }
};

//utility and operators/test
#include <nytl/bits/triangle.inl>

}
