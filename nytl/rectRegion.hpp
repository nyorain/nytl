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
///\brief Defines the RectRegion template class for representing an area consisting of Rects.

#pragma once

#include <nytl/vec.hpp>
#include <nytl/rect.hpp>
#include <nytl/triangle.hpp>
#include <nytl/line.hpp>

#include <vector>

namespace nytl
{

//typedef
template<std::size_t dim, typename prec> class RectRegion;

template<class prec> using RectRegion2 = RectRegion<2, prec>;
template<class prec> using RectRegion3 = RectRegion<3, prec>;
template<class prec> using RectRegion4 = RectRegion<4, prec>;

typedef RectRegion<2, int> RectRegion2i;
typedef RectRegion<2, unsigned int> RectRegion2ui;
typedef RectRegion<2, double> RectRegion2d;
typedef RectRegion<2, float> RectRegion2f;
typedef RectRegion<2, char> RectRegion2c;
typedef RectRegion<2, unsigned char> RectRegion2uc;
typedef RectRegion<2, long> RectRegion2l;
typedef RectRegion<2, unsigned long> RectRegion2ul;

typedef RectRegion<3, int> RectRegion3i;
typedef RectRegion<3, unsigned int> RectRegion3ui;
typedef RectRegion<3, double> RectRegion3d;
typedef RectRegion<3, float> RectRegion3f;
typedef RectRegion<3, char> RectRegion3c;
typedef RectRegion<3, unsigned char> RectRegion3uc;
typedef RectRegion<3, long> RectRegion3l;
typedef RectRegion<3, unsigned long> RectRegion3ul;

typedef RectRegion<4, int> RectRegion4i;
typedef RectRegion<4, unsigned int> RectRegion4ui;
typedef RectRegion<4, double> RectRegion4d;
typedef RectRegion<4, float> RectRegion4f;
typedef RectRegion<4, char> RectRegion4c;
typedef RectRegion<4, unsigned char> RectRegion4uc;
typedef RectRegion<4, long> RectRegion4l;
typedef RectRegion<4, unsigned long> RectRegion4ul;

//should rlly be declared here? use helper member functions?
//tests
/*
template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR 
	bool intersects(const RectRegion<dim, prec>&, const RectRegion<dim, prec>&);
template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR 
	bool intersects(const RectRegion<dim, prec>&, const Rect<dim, prec>&);
template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR 
	bool intersects(const RectRegion<dim, prec>&, const Line<dim, prec>&);
template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR 
	bool intersects(const RectRegion<dim, prec>&, const Triangle<dim, prec>&);


template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR 
	bool contains(const RectRegion<dim, prec>&, const RectRegion<dim, prec>&);
template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR 
	bool contains(const RectRegion<dim, prec>&, const Rect<dim, prec>&);
template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR 
	bool contains(const RectRegion<dim, prec>&, const Line<dim, prec>&);
template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR 
	bool contains(const RectRegion<dim, prec>&, const Triangle<dim, prec>&);
template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR 
	bool contains(const RectRegion<dim, prec>&, const Vec<dim, prec>&);

//operations
template<std::size_t dim, typename prec> RectRegion<dim, prec>
   	intersection(const RectRegion<dim, prec>&, const RectRegion<dim, prec>&);
template<std::size_t dim, typename prec> RectRegion<dim, prec>
   	intersection(const RectRegion<dim, prec>&, const Rect<dim, prec>&);
template<std::size_t dim, typename prec> RectRegion<dim, prec>
   	intersection(const Rect<dim, prec>&, const RectRegion<dim, prec>&);

template<std::size_t dim, typename prec> RectRegion<dim, prec>
   	combination(const RectRegion<dim, prec>&, const RectRegion<dim, prec>&);
template<std::size_t dim, typename prec> RectRegion<dim, prec>
   	combination(const RectRegion<dim, prec>&, const Rect<dim, prec>&);
template<std::size_t dim, typename prec> RectRegion<dim, prec>
   	combination(const Rect<dim, prec>&, const RectRegion<dim, prec>&);

template<std::size_t dim, typename prec> RectRegion<dim, prec>
   	difference(const RectRegion<dim, prec>&, const RectRegion<dim, prec>&);
template<std::size_t dim, typename prec> RectRegion<dim, prec>
   	difference(const RectRegion<dim, prec>&, const Rect<dim, prec>&);
template<std::size_t dim, typename prec> RectRegion<dim, prec>
   	difference(const Rect<dim, prec>&, const RectRegion<dim, prec>&);

template<std::size_t dim, typename prec> RectRegion<dim, prec>
   	symmetricDifference(const RectRegion<dim, prec>&, const RectRegion<dim, prec>&);
template<std::size_t dim, typename prec> RectRegion<dim, prec>
   	symmetricDifference(const RectRegion<dim, prec>&, const Rect<dim, prec>&);
template<std::size_t dim, typename prec> RectRegion<dim, prec>
   	symmetricDifference(const Rect<dim, prec>&, const RectRegion<dim, prec>&);
*/

///\ingroup math
///Region composed of Rectangles.
template<std::size_t D, typename P>
class RectRegion
{
template<std::size_t, typename T> friend class RectRegion;

public:
	//naming conformance
	using Precision = P;
	using Size = std::size_t;
	constexpr static Size dim = D;

	using Reference = Precision&;
	using ConstReference = const Precision&;
	using Pointer = Precision*;
	using ConstPointer = const Precision*;

    using RectRegionType = RectRegion;
    using VecType = Vec<dim, Precision>;
    using RectType = Rect<dim, Precision>;
    using LineType = Line<dim, Precision>;

	//stl conformance
	using value_type = Precision;
	using size_type = Size;
	using reference = Reference;
	using const_reference = ConstReference;
	using pointer = Pointer;
	using const_pointer = ConstPointer;


protected:
    std::vector<RectType> rects_ {};

public:
    RectRegion(const std::vector<RectType>& rects) : rects_(rects) {};
    RectRegion() noexcept = default;
    ~RectRegion() noexcept = default;

    RectRegion(const RectRegionType& other) = default;
    RectRegion& operator=(const RectRegionType& other) = default;

    RectRegion(RectRegionType&& other) noexcept = default;
    RectRegion& operator=(RectRegionType&& other) noexcept = default;

    //change the RectRegion
	void add(const RectType& r);
	void add(const RectRegionType& r);

	void subtract(const RectType& r);
	void subtract(const RectRegionType& r);

	//operator
	RectRegion& operator|=(const RectType& r);
	RectRegion& operator|=(const RectRegionType& r);

    RectRegion& operator&=(const RectType& r);
	RectRegion& operator&=(const RectRegionType& r);

    RectRegion& operator^=(const RectType& r);
	RectRegion& operator^=(const RectRegionType& r);

	//
	RectType extents() const;
	const std::vector<RectType>& rects() const { return rects_; }

	template<Size OD, typename OP>
	operator RectRegion<OD, OP>() const 
	{ 
		auto ret = RectRegion<OD, OP>{}; 
		ret.rects_.insert(ret.rects_.cend(), rects_.cbegin(), rects_.cend()); 
		return ret; 
	}
};

//operators
#include <nytl/bits/rectRegion.inl>

}
