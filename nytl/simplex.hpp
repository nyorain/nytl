/* The MIT License (MIT)
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
///\brief Includes the unspecialized Simplex template class with operations and typedefs.

#pragma once

#include <nytl/vec.hpp>
#include <nytl/mat.hpp>
#include <nytl/tmp.hpp>
#include <nytl/scalar.hpp>
#include <nytl/linearSolver.hpp>

#include <vector>
#include <type_traits>
#include <utility>
#include <cmath>
#include <stdexcept>
#include <string>
#include <cassert>

namespace nytl
{

///Short enable-if typedef
template<std::size_t D, std::size_t A> using DimMatch = typename std::enable_if<D >= A>::type;

///\ingroup math
///\brief Templated abstraction of the Simplex concept.
///\details The Simplex<D, P, A> template class defines an unique area with \c A dimensions
///of \c P precision in an \c D dimensional space.
///So e.g. Simplex<3, float, 2> describes a Triangle in a 3-dimensional space.
///This template class does only works if D >= A, since the dimension of the area
///can not be higher than the dimension of the space that contains this area.
///The area is called unique, since it does have a variable number of points defining it; always
///enough to describe exactly one, unambigous area with the given dimension and precision in
///the given space.
template<std::size_t D, typename P = float, std::size_t A = D>
class Simplex : public deriveDummy<DimMatch<D, A>>
{
public:
	using VecType = Vec<D, P>;

public:
	///Holds the points that define the area.
	Vec<A + 1, VecType> points_ {};

public:
	template<typename... Args, typename = typename
		std::enable_if<
			std::is_convertible<
				std::tuple<Args...>,
				type_tuple_t<VecType, A + 1>
			>::value
		>::type>		
	Simplex(Args&&... args) noexcept : points_{std::forward<Args>(args)...} {}
	Simplex() noexcept = default;

	///Returns the size of the area (e.g. for a 3-dimensional Simplex this would be the volume)
	double size() const;

	///Returns the center point of the area.
	VecType center() const;

	///Returns whether the defined Simplex is valid (i.e. size > 0).
	bool valid() const;

	///Converts the object into a Vec of points. 
	///Can be used to acces (read/change/manipulate) the points.
	Vec<A + 1, VecType>& points(){ return points_; }

	///Converts the object into a const Vec of poitns. 
	///Can be used to const_iterate/read the points.
	const Vec<A + 1, VecType>& points() const { return points_; }

	///Converts the object to a Simplex with a different dimension or precision.
	///Note that the area dimension A cannot be changed, only the space dimension D.
	///Works only if the new D is still greater equal A.
	template<std::size_t OD, typename OP> 
		operator Simplex<OD, OP, A>() const;
};

///\ingroup math
///Describes a RectRegion of multiple Simplexes. 
template<std::size_t D, typename P = float, std::size_t A = D>
class SimplexRegion : public deriveDummy<DimMatch<D, A>>
{
public:
	using Simplex_type = Simplex<D, P, A>;
	using RectRegion_type = SimplexRegion<D, P, A>;
	using Vector_type = std::vector<Simplex_type>;
	using size_type = typename Vector_type::size_type;

public:
	///Vector of Simplexs that holds the areas that define this RectRegion.
	///None of the Simplexs in this Vector should intersect with each other.
	Vector_type areas_;

public:
	///Adds a Simplex to this RectRegion. Effectively only adds the part of the Simplex that
	///is not already part of the RectRegion.
	void add(const Simplex_type& area);

	///Makes this RectRegion object the union of itself and the argument-given RectRegion.
	void add(const RectRegion_type& RectRegion);

	///Adds a Simplex without checking for intersection
	void addNoCheck(const Simplex_type& Simplex) { areas_.push_back(Simplex); }

	///Subtracts a Simplex from this RectRegion. Effectively checks every Simplex of this RectRegion
	///for intersection and resizes it if needed.
	void subtract(const Simplex_type& area);

	///Subtracts the given RectRegion from this object.
	void subtract(const RectRegion_type& area);

	///Returns the size of the area.
	double size() const;

	///Returns the number of Simplexs this RectRegion contains.
	size_type count() const;

	///Returns a Vector with the given Simplexes.
	const Vector_type& areas() const { return areas_; }

	///Converts the RectRegion to a RectRegion object of different precision and space dimension.
	template<std::size_t ND, typename NP, typename = DimMatch<D, A>> 
		operator SimplexRegion<ND, NP, A>() const;
};

//To get the additional features for each specialization, include the corresponding headers:
//#include <nytl/line.hpp>
template<std::size_t D, typename P = float> using Line = Simplex<D, P, 1>;
//#include <nytl/triangle.hpp>
template<std::size_t D, typename P = float> using Triangle = Simplex<D, P, 2>;
//#include <nytl/tetrahedron.hpp>
template<std::size_t D, typename P = float> using Tetrahedron = Simplex<D, P, 3>;

//operators/utility
#include <nytl/bits/simplexRegion.inl>
#include <nytl/bits/simplex.inl>

}

