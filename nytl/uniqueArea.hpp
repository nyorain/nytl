/* The MIT License (MIT)
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

#include <vector>
#include <type_traits>

namespace nytl
{

///Short enable-if typedef
template<std::size_t D, std::size_t A> using DimMatch = typename std::enable_if<D >= A>::type;

///The uniqueArea<D, P, A> template class defines an unique area with \cA dimensions
///of \cP precision in an \cD dimensional space.
///So e.g. uniqueArea<3, float, 2> describes a triangle in a 3-dimensional space.
///This template class does only works if D >= A, since the dimension of the area
///can not be higher than the dimension of the space that contains this area.
///The area is called unique, since it does have a variable number of points defining it; always
///enough to describe exactly one, unambigous area with the given dimension and precision in
///the given space.
template<std::size_t D, typename P = float, std::size_t A = D, typename = DimMatch<D, A>>
class uniqueArea
{
public:
	using vec_type = vec<D, P>;

public:
	///Holds the points that define the area.
	vec<A + 1, vec_type> area;

public:
	///Returns the size of the area (e.g. for a 3-dimensional area this would be the volume)
	double size() const;

	///Returns the center point of the area.
	vec_type center() const;

	///Converts the object to a uniqueArea with a different dimension or precision.
	///Note that the area dimension A cannot be changed, only the space dimension D.
	///Works only if the new D is still greater equal A.
	template<std::size_t ND, typename NP, typename = DimMatch<D, A>> 
		operator uniqueArea<ND, NP, A>() const;
};

///Describes a region of multiple unique areas.
///Look at uniqueArea for more information.
template<std::size_t D, typename P = float, std::size_t A = D, typename = DimMatch<D, A>>
class uniqueRegion
{
public:
	using area_type = uniqueArea<D, P, A>;
	using region_type = uniqueRegion<D, P, A>;
	using vector_type = std::vector<area_type>;
	using size_type = typename vector_type::size_type;

public:
	///Vector of uniqueAreas that holds the areas that define this region.
	///None of the uniqueAreas in this vector should intersect with each other.
	vector_type region;

public:
	///Adds a uniqueArea to this region. Effectively only adds the part of the uniqueArea that
	///is not already part of the region.
	void add(const area_type& area);

	///Makes this region object the union of itself and the argument-given region.
	void add(const region_type& region);

	///Subtracts a uniqueArea from this region. Effectively checks every uniqueArea of this region
	///for intersection and resizes it if needed.
	void subtract(const area_type& area);

	///Subtracts the given region from this object.
	void subtract(const region_type& area);

	///Returns the size of the area.
	double size() const;

	///Returns the number of uniqueAreas this region contains.
	size_type count() const;

	///Converts the region to a region object of different precision and space dimension.
	template<std::size_t ND, typename NP, typename = DimMatch<D, A>> 
		operator uniqueRegion<ND, NP, A>() const;
};

///Typedef the specializations.
///Include <nytl/line.hpp> | <nytl/triangle.hpp> | <nytl/tetrahedron.hpp> to get their
///extra features.
//TODO XXX

//template<std::size_t D, typename P = float> using line = uniqueArea<D, P, 1>;
//template<std::size_t D, typename P = float> using triangle = uniqueArea<D, P, 2>;
//template<std::size_t D, typename P = float> using tetrahedron = uniqueArea<D, P, 3>;

}

//operators/utility
#include <nytl/bits/uniqueArea.inl>
