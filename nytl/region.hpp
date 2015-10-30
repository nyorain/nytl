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
#include <nytl/rect.hpp>
#include <nytl/triangle.hpp>
#include <nytl/line.hpp>

#include <vector>

namespace nytl
{

//typedef
template<std::size_t dim, typename prec> class region;

template<class prec> using region2 = region<2, prec>;
template<class prec> using region3 = region<3, prec>;
template<class prec> using region4 = region<4, prec>;

typedef region<2, int> region2i;
typedef region<2, unsigned int> region2ui;
typedef region<2, double> region2d;
typedef region<2, float> region2f;
typedef region<2, char> region2c;
typedef region<2, unsigned char> region2uc;
typedef region<2, long> region2l;
typedef region<2, unsigned long> region2ul;

typedef region<3, int> region3i;
typedef region<3, unsigned int> region3ui;
typedef region<3, double> region3d;
typedef region<3, float> region3f;
typedef region<3, char> region3c;
typedef region<3, unsigned char> region3uc;
typedef region<3, long> region3l;
typedef region<3, unsigned long> region3ul;

typedef region<4, int> region4i;
typedef region<4, unsigned int> region4ui;
typedef region<4, double> region4d;
typedef region<4, float> region4f;
typedef region<4, char> region4c;
typedef region<4, unsigned char> region4uc;
typedef region<4, long> region4l;
typedef region<4, unsigned long> region4ul;

//should rlly be declared here? use helper member functions?
//tests
template<std::size_t dim, typename prec> NYUTIL_CPP14_CONSTEXPR bool intersects(const region<dim, prec>&, const region<dim, prec>&);
template<std::size_t dim, typename prec> NYUTIL_CPP14_CONSTEXPR bool intersects(const region<dim, prec>&, const rect<dim, prec>&);
template<std::size_t dim, typename prec> NYUTIL_CPP14_CONSTEXPR bool intersects(const region<dim, prec>&, const line<dim, prec>&);
template<std::size_t dim, typename prec> NYUTIL_CPP14_CONSTEXPR bool intersects(const region<dim, prec>&, const triangle<dim, prec>&);

template<std::size_t dim, typename prec> NYUTIL_CPP14_CONSTEXPR bool contains(const region<dim, prec>&, const region<dim, prec>&);
template<std::size_t dim, typename prec> NYUTIL_CPP14_CONSTEXPR bool contains(const region<dim, prec>&, const rect<dim, prec>&);
template<std::size_t dim, typename prec> NYUTIL_CPP14_CONSTEXPR bool contains(const region<dim, prec>&, const line<dim, prec>&);
template<std::size_t dim, typename prec> NYUTIL_CPP14_CONSTEXPR bool contains(const region<dim, prec>&, const triangle<dim, prec>&);
template<std::size_t dim, typename prec> NYUTIL_CPP14_CONSTEXPR bool contains(const region<dim, prec>&, const vec<dim, prec>&);

//operations
template<std::size_t dim, typename prec> region<dim, prec> intersection(const region<dim, prec>&, const region<dim, prec>&);
template<std::size_t dim, typename prec> region<dim, prec> intersection(const region<dim, prec>&, const rect<dim, prec>&);

template<std::size_t dim, typename prec> region<dim, prec> combination(const region<dim, prec>&, const region<dim, prec>&);
template<std::size_t dim, typename prec> region<dim, prec> combination(const region<dim, prec>&, const rect<dim, prec>&);

template<std::size_t dim, typename prec> region<dim, prec> difference(const region<dim, prec>&, const region<dim, prec>&);
template<std::size_t dim, typename prec> region<dim, prec> difference(const region<dim, prec>&, const rect<dim, prec>&);

template<std::size_t dim, typename prec> region<dim, prec> symmetricDifference(const region<dim, prec>&, const region<dim, prec>&);
template<std::size_t dim, typename prec> region<dim, prec> symmetricDifference(const region<dim, prec>&, const rect<dim, prec>&);


//todo: cache complex compute results?
//region
template<std::size_t dim, typename prec>
class region
{
template<std::size_t, typename T> friend class region;

public:
    using value_type = prec;
    using vec_type = vec<dim, prec>;
    using rect_type = rect<dim, prec>;
    using triangle_type = triangle<dim, prec>;
    using region_type = region<dim, prec>;
    using line_type = line<dim, prec>;

protected:
    std::vector<rect_type> rects_ {};

public:
    region(const std::vector<rect_type>& rects) noexcept : rects_(rects) {};
    region() noexcept = default;
    ~region() noexcept = default;

    region(const region_type& other) = default;
    region& operator=(const region_type& other) = default;

    region(region_type&& other) noexcept = default;
    region& operator=(region_type&& other) noexcept = default;

    //change the region
	void add(const rect_type& r);
	void add(const region_type& r);

	void subtract(const rect_type& r);
	void subtract(const region_type& r);

	//operator
	region& operator|=(const rect_type& r);
	region& operator|=(const region_type& r);

    region& operator&=(const rect_type& r);
	region& operator&=(const region_type& r);

    region& operator^=(const rect_type& r);
	region& operator^=(const region_type& r);

	//
	rect_type extents() const;
	const std::vector<rect_type>& getRects() const { return rects_; }

	template<std::size_t odim, typename oprec>
	operator region<odim, oprec>() const { region<odim, oprec> ret; ret.rects_.insert(ret.rects_.cend(), rects_.cbegin(), rects_.cend()); return ret; }
};

//operators
#include <nytl/bits/region.inl>

}
