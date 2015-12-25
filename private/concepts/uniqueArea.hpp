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
///This template class does only make sense if D >= A, since the dimension of the area
///can not be higher than the dimension of the space that contains this area.
///The area is called unique, since it does have a variable number of points defining it; always
///enough to describe exactly one, unambigous area with the given dimension and precision in
///the given space.
template<std::size_t D, typename P = float, std::size_t A = D, typename = DimMatch<D, A>>
class uniqueArea
{
public:
	using VecType = vec<D, P>;

public:
	///Holds the points that define the area.
	vec<A + 1, VecType> area;

public:
	///Returns the size of the area (e.g. for a 3-dimensional area this would be the volume)
	double size() const;

	///Returns the center point of the area.
	VecType center() const;

	///
	template<std::size_t ND, typename NP, typename = DimMatch<D, A>> 
		operator uniqueArea<ND, NP, A>() const;
};

///Describes a region of multiple unique areas.
///Look at uniqueArea for more information.
template<std::size_t D, typename P = float, std::size_t A = D, typename = DimMatch<D, A>>
class uniqueRegion
{
public:
	using AreaType = uniqueArea<D, P, A>;
	using VectorType = std::vector<AreaType>;
	using SizeType = typename VectorType::size_type;
	using Iterator = typename VectorType::iterator;
	using ConstIterator = typename VectorType::const_iterator;
	using ReverseIterator = typename VectorType::reverse_iterator;
	using ReverseConstIterator = typename VectorType::reverse_const_iterator;

public:
	///Vector of uniqueAreas that holds the areas that define this region.
	///None of the uniqueAreas in this vector should intersect with each other.
	VectorType region;

public:
	///Adds a uniqueArea to this region. Effectively only adds the part of the uniqueArea that
	///is not already part of the region.
	void add(const AreaType& area);

	///Subtracts a uniqueArea from this region. Effectively checks every uniqueArea of this region
	///for intersection and resizes it if needed.
	void subtract(const AreaType& area);

	///Returns the size of the area.
	double size() const;

	///Returns the number of uniqueAreas this region contains.
	SizeType count() const;
};

//typedefs
template<std::size_t D, typename P = float> using line = uniqueArea<D, P, 1>;
template<std::size_t D, typename P = float> using triangle = uniqueArea<D, P, 2>;
template<std::size_t D, typename P = float> using tetraeder = uniqueArea<D, P, 3>;


//tests//////////////////////////////////////////////
///\relates uniqueArea
///Tests two uniqueAreas for intersection. Symmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	intersects(const uniqueArea<D, P, A>&, const uniqueArea<D, P, A>&);

///\relates uniqueArea
///Tests if the first uniqueArea fully contains the second one. Asymmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const uniqueArea<D, P, A>&, const uniqueArea<D, P, A>&);

///\relates uniqueArea
///Tests if the given uniqueArea contains the given point.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const uniqueArea<D, P, A>&, const vec<D, P>&);

///\relates uniqueRegion
///Tests two uniqueRegions for intersection. Symmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	intersects(const uniqueRegion<D, P, A>&, const uniqueRegion<D, P, A>&);

///\relates uniqueRegion
///Tests if the first uniqueRegion fully contains the second one. Asymmetrical operator.
template<std::size_t D, typename P, std::size_t A>bool 
	contains(const uniqueRegion<D, P, A>&, const uniqueRegion<D, P, A>&);

///\relates uniqueRegion
///Tests if the given uniqueRegion contains the given point.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const uniqueRegion<D, P, A>&, const vec<D, P>&);


//operators////////////////////////////////////////
///\relates uniqueArea
///Returns the region of intersection between the two given uniqueAreas. Result is not guaranteed
///to be representable by a uniqueArea and therefore a uniqueRegion (i.e. the intersection of two
///triangles is not guaranteed to be a triangle). Symmetrical operator. [AND]
template<std::size_t D, typename P, std::size_t A> uniqueRegion<D, P, A> 
	intersection(const uniqueArea<D, P, A>&, const uniqueArea<D, P, A>&);

///\relates uniqueArea
///Combines the first and the second area (Offically called union but this name is not available 
///in c++). Result is not guaranteed to be representable by one
///single uniqueArea and is therefore a uniqueRegion. Symmetrical operator. [OR]
template<std::size_t D, typename P, std::size_t A> uniqueRegion<D, P, A> 
	combination(const uniqueArea<D, P, A>&, const uniqueArea<D, P, A>&);

///\relates uniqueArea
///Returns the symmetric difference of the two given uniqueAreas, so basically the region
///in the given space where exactly one of the two given areas are located.
///Symmetrical operator. [XOR]
template<std::size_t D, typename P, std::size_t A> uniqueRegion<D, P, A> 
	symmetricDifference(const uniqueArea<D, P, A>&, const uniqueArea<D, P, A>&);

///\relates uniqueArea
///Subtracts the second area from the first one and returns the "rest" of the first area.
///Return type is a uniqueRegion since the result is not guaranteed to be representable by one
///single uniqueArea. Asymmetrical operator. [AND NOT]
template<std::size_t D, typename P, std::size_t A> uniqueRegion<D, P, A> 
	difference(const uniqueArea<D, P, A>&, const uniqueArea<D, P, A>&);

}
