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

///\file
///\brief Includes the unspecialized simplex template class with operations and typedefs.

#pragma once

#include <nytl/vec.hpp>
#include <nytl/mat.hpp>
#include <nytl/tmp.hpp>
#include <nytl/constants.hpp>

#include <vector>
#include <type_traits>
#include <utility>
#include <cmath>

namespace nytl
{

///Short enable-if typedef
template<std::size_t D, std::size_t A> using DimMatch = typename std::enable_if<D >= A>::type;

///\brief Templated abstraction of the simplex concept.
///\details The simplex<D, P, A> template class defines an unique area with \cA dimensions
///of \cP precision in an \cD dimensional space.
///So e.g. simplex<3, float, 2> describes a triangle in a 3-dimensional space.
///This template class does only works if D >= A, since the dimension of the area
///can not be higher than the dimension of the space that contains this area.
///The area is called unique, since it does have a variable number of points defining it; always
///enough to describe exactly one, unambigous area with the given dimension and precision in
///the given space.
template<std::size_t D, typename P = float, std::size_t A = D, typename = DimMatch<D, A>>
class simplex
{
public:
	using vec_type = vec<D, P>;

public:
	///Holds the points that define the area.
	vec<A + 1, vec_type> points;

public:
	template<typename... Args, typename = typename
		std::enable_if<
			std::is_convertible<
				std::tuple<Args...>,
				type_tuple_t<vec_type, A + 1>
			>::value
		>::type>		
	simplex(Args&&... args) noexcept : points{std::forward<Args>(args)...} {}
	simplex() noexcept = default;

	///Returns the size of the area (e.g. for a 3-dimensional area this would be the volume)
	double size() const;

	///Returns the center point of the area.
	vec_type center() const;

	///Returns whether the simplex lays in the same space as the given point.
	///If e.g. D==2 it checks whether they lay on the same plane.
	///If D==A this will always return true.
	bool sameSpace(const vec_type& val) const;

	///Converts the given normal coordinates into barycentric coordinates for the simplex
	///object. If the given point does not lay in the same space as the simplex object,
	///the function will output a warning and return a null-vector.
	///This can be checked before with sameSpace().
	vec<A + 1, double> barycentric(const vec_type& val) const;

	///Converts the object into a vec of points. Can be used to acces (read/change/manipulate) the
	///points.
	vec<A + 1, vec_type>& asVec(){ return points; }

	///Converts the object into a const vec of poitns. Can be used to const_iterate/read the 
	///points.
	const vec<A + 1, vec_type>& asVec() const { return points; }

	///Converts the object to a simplex with a different dimension or precision.
	///Note that the area dimension A cannot be changed, only the space dimension D.
	///Works only if the new D is still greater equal A.
	template<std::size_t OD, typename OP> 
		operator simplex<OD, OP, A>() const;
};

///Describes a region of multiple simplexes. 
template<std::size_t D, typename P = float, std::size_t A = D, typename = DimMatch<D, A>>
class simplexRegion
{
public:
	using simplex_type = simplex<D, P, A>;
	using region_type = simplexRegion<D, P, A>;
	using vector_type = std::vector<simplex_type>;
	using size_type = typename vector_type::size_type;

public:
	///Vector of simplexs that holds the areas that define this region.
	///None of the simplexs in this vector should intersect with each other.
	vector_type areas;

public:
	///Adds a simplex to this region. Effectively only adds the part of the simplex that
	///is not already part of the region.
	void add(const simplex_type& area);

	///Makes this region object the union of itself and the argument-given region.
	void add(const region_type& region);

	///Subtracts a simplex from this region. Effectively checks every simplex of this region
	///for intersection and resizes it if needed.
	void subtract(const simplex_type& area);

	///Subtracts the given region from this object.
	void subtract(const region_type& area);

	///Returns the size of the area.
	double size() const;

	///Returns the number of simplexs this region contains.
	size_type count() const;

	///Converts the region to a region object of different precision and space dimension.
	template<std::size_t ND, typename NP, typename = DimMatch<D, A>> 
		operator simplexRegion<ND, NP, A>() const;
};

//To get the additional features for each specialization, include the corresponding headers:
//#include <nytl/line.hpp>
template<std::size_t D, typename P = float> using line = simplex<D, P, 1>;
//#include <nytl/triangle.hpp>
template<std::size_t D, typename P = float> using triangle = simplex<D, P, 2>;
//#include <nytl/tetrahedron.hpp>
template<std::size_t D, typename P = float> using tetrahedron = simplex<D, P, 3>;

//operators/utility
#include <nytl/bits/simplex.inl>

}

