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
#include <nytl/constants.hpp>
#include <nytl/simplex.hpp>
#include <nytl/log.hpp>

namespace nytl
{

//typedefs
template<size_t D, typename P = float> using line = simplex<D, P, 1>;
template<typename P = float> using line2 = line<2, P>;
template<typename P = float> using line3 = line<3, P>;
template<typename P = float> using line4 = line<4, P>;

using line2f = line<2, float>;
using line3f = line<3, float>;
using line4f = line<4, float>;

using line2i = line<2, int>;
using line3i = line<3, int>;
using line4i = line<4, int>;

using line2ui = line<2, unsigned int>;
using line3ui = line<3, unsigned int>;
using line4ui = line<4, unsigned int>;


///Simplex specialization for a 2-dimensional area (line).
///Look at simplex for more information.
template<size_t D, typename P>
class simplex<D, P, 1>
{
public:
    using value_type = P;
    using vec_type = vec<D, value_type>;

public:
    vec_type a;
    vec_type b;

public:
    simplex(const vec_type& xa, const vec_type& xb) noexcept : a(xa), b(xb) {}
    simplex() noexcept = default;

	//simplex
    double size() const { return distance(a, b); }
	vec_type center() const { return (a + b) / 2; }
	vec<2, double> barycentric(const vec_type& v) const;
	bool sameSpace(const vec_type& v) const;

	vec<3, vec_type>& asVec(){ return *reinterpret_cast<vec_type*>(this); }
	const vec<3, vec_type>& asVec() const { return *reinterpret_cast<const vec_type*>(this); }

    template<size_t OD, typename OP>
	operator line<OD, OP>() const { return line<OD, OP>(a, b); }

	//line-specific
	///Alias for size(). Returns the length of the line.
	double length() const { return size(); }

	///Returns the vector that lays between the two points
    vec_type difference() const { return b - a; }

	///Returns a normalized (length = 1) gradient vector.
    vec_type gradient() const { return normalize(difference()); }

	///Returns the gradient vector in relation to the given dimension parameter.
	///If e.g. dim is 0, the x component of the returned gradient vector will be 1 and all
	///other components will be set in relation.
    vec_type gradient(std::size_t dim) const { return difference() / difference()[dim]; }

	///Returns wheter the line is defined for the given value in the given dimension.
    bool definedAt(const value_type& value, std::size_t dimension = 0) const;

	///Returns the point of the line at the given value in the given dimension.
	///One should check with definedAt(value, dimension) if the line is defined for the given
	///value before using this. If it is not, this function will produce a warning and return an
	///empty vec.
    vec_type valueAt(const value_type& value, std::size_t dimension = 0) const;

	///Returns the smallest value the line is defined for in the given dimension.
    value_type smallestValue(std::size_t dim) const { return min(a[dim], b[dim]); }

	///Returns the greatest value the line is defined for in the given dimension.
    value_type greatestValue(std::size_t dim) const { return max(a[dim], b[dim]); }
};

//implementation, utility and operators
#include <nytl/bits/line.inl>

}
