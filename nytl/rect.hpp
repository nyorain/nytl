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
#include <nytl/triangle.hpp>
#include <nytl/line.hpp>

#include <vector>
#include <ostream>
#include <algorithm>

namespace nytl
{
//rect
template<size_t dim, class prec> class rect;

template<class prec> using rect2 = rect<2, prec>;
template<class prec> using rect3 = rect<3, prec>;
template<class prec> using rect4 = rect<4, prec>;

typedef rect<2, int> rect2i;
typedef rect<2, unsigned int> rect2ui;
typedef rect<2, double> rect2d;
typedef rect<2, float> rect2f;
typedef rect<2, char> rect2c;
typedef rect<2, unsigned char> rect2uc;
typedef rect<2, long> rect2l;
typedef rect<2, unsigned long> rect2ul;

typedef rect<3, int> rect3i;
typedef rect<3, unsigned int> rect3ui;
typedef rect<3, double> rect3d;
typedef rect<3, float> rect3f;
typedef rect<3, char> rect3c;
typedef rect<3, unsigned char> rect3uc;
typedef rect<3, long> rect3l;
typedef rect<3, unsigned long> rect3ul;

typedef rect<4, int> rect4i;
typedef rect<4, unsigned int> rect4ui;
typedef rect<4, double> rect4d;
typedef rect<4, float> rect4f;
typedef rect<4, char> rect4c;
typedef rect<4, unsigned char> rect4uc;
typedef rect<4, long> rect4l;
typedef rect<4, unsigned long> rect4ul;

//should rlly be declared here? use helper member functions?
//todo: intersection overload for point of intersection for types
//tests
template<std::size_t dim, typename prec> bool intersects(const rect<dim, prec>&, const rect<dim, prec>&);
template<std::size_t dim, typename prec> bool intersects(const rect<dim, prec>&, const line<dim, prec>&);
template<std::size_t dim, typename prec> bool intersects(const rect<dim, prec>&, const triangle<dim, prec>&);

template<std::size_t dim, typename prec> bool contains(const rect<dim, prec>&, const rect<dim, prec>&);
template<std::size_t dim, typename prec> bool contains(const rect<dim, prec>&, const line<dim, prec>&);
template<std::size_t dim, typename prec> bool contains(const rect<dim, prec>&, const triangle<dim, prec>&);
template<std::size_t dim, typename prec> bool contains(const rect<dim, prec>&, const vec<dim, prec>&);

//operations
template<std::size_t dim, typename prec> rect<dim, prec> intersection(const rect<dim, prec>&, const rect<dim, prec>&);
template<std::size_t dim, typename prec> std::vector<rect<dim, prec>> combination(const rect<dim, prec>&, const rect<dim, prec>&);
template<std::size_t dim, typename prec> std::vector<rect<dim, prec>> difference(const rect<dim, prec>&, const rect<dim, prec>&);
template<std::size_t dim, typename prec> std::vector<rect<dim, prec>> symmetricDifference(const rect<dim, prec>&, const rect<dim, prec>&);


//rect
template<size_t dim, class prec> class rect
{
public:
    using value_type = prec;
    using vec_type = vec<dim, value_type>;

public:
	vec_type position;
	vec_type size;

public:
	rect(vec_type pposition = vec_type(), vec_type psize = vec_type()) noexcept : position(pposition), size(psize) {}
    ~rect() noexcept = default;

    rect(const rect<dim, prec>& other) noexcept = default;
	rect& operator=(const rect<dim, prec>& other) noexcept = default;

    rect(rect<dim, prec>&& other) noexcept = default;
	rect& operator=(rect<dim, prec>&& other) noexcept = default;

	vec<dim, double> center() const { return (double)(position + (size / 2.d)); };
	bool empty() const { return anyValueGreater(size, 0); }

	//conversion
	template<size_t odim, class oprec> operator rect<odim, oprec>() const { return rect<odim, oprec>(position, size); }
};

//rect2
template<class prec> class rect<2, prec>
{
public:
    static constexpr std::size_t dim = 2;
    using value_type = prec;
    using vec_type = vec<dim, value_type>;

public:
	vec_type position;
	vec_type size;

public:
	rect(const vec_type& pposition, const vec_type& psize = vec_type()) noexcept : position(pposition), size(psize) {}
	rect(prec x = prec(), prec y = prec(), prec width = prec(), prec height = prec()) noexcept : position(x,y), size(width,height) {}

	~rect() noexcept = default;

	rect(const rect<dim, prec>& other) noexcept = default;
	rect& operator=(const rect<dim, prec>& other) noexcept = default;

    rect(rect<dim, prec>&& other) noexcept = default;
	rect& operator=(rect<dim, prec>&& other) noexcept = default;

	vec<dim, double> center() const { return (double)(position + (size / 2.d)); };

    vec<2, prec> topLeft() const { return position; }
	vec<2, prec> topRight() const { return position + vec<2, prec>(size.x, 0); }
	vec<2, prec> bottomLeft() const { return position + vec<2, prec>(0, size.y);}
	vec<2, prec> bottomRight() const { return position + size; }

	const prec& left() const { return position.x; }
	const prec right() const { return position.x + size.x; }
	const prec& top() const { return position.y; }
	const prec bottom() const { return position.y + size.y; }

    prec& left() { return position.x; }
	prec& top() { return position.y; }

	const prec& width() const { return size.x; }
	const prec& height() const { return size.y; }

    prec& width() { return size.x; }
	prec& height() { return size.y; }

    bool empty() const { return anyValueGreater(size, 0); }

    //conversion
	template<size_t odim, class oprec>
	operator rect<odim, oprec>() const { return rect<odim, oprec>(position, size); }
};

//rect3
template<class prec> class rect<3, prec>
{
public:
    static constexpr std::size_t dim = 3;
    using value_type = prec;
    using vec_type = vec<dim, value_type>;

public:
	vec_type position;
	vec_type size;

public:
	rect(const vec_type& pposition, const vec_type& psize = vec_type()) noexcept : position(pposition), size(psize) {}
	rect(prec x = prec(), prec y = prec(), prec z = prec(), prec width = prec(), prec height = prec(), prec depth = prec()) noexcept
        : position(x,y,z), size(width,height,depth) {}

	~rect() noexcept = default;

	rect(const rect<dim, prec>& other) noexcept = default;
	rect& operator=(const rect<dim, prec>& other) noexcept = default;

    rect(rect<dim, prec>&& other) noexcept = default;
	rect& operator=(rect<dim, prec>&& other) noexcept = default;

	vec<dim, double> center() const { return (double)(position + (size / 2.d)); };

    vec<2, prec> topLeft() const { return position.xy(); }
	vec<2, prec> topRight() const { return position.xy() + vec<2, prec>(size.x, 0); }
	vec<2, prec> bottomLeft() const { return position.xy() + vec<2, prec>(0, size.y);}
	vec<2, prec> bottomRight() const { return position.xy() + size.xy(); }

    vec<3, prec> frontTopLeft() const { return position; }
	vec<3, prec> frontTopRight() const { return position + vec<3, prec>(size.x, 0, 0); }
	vec<3, prec> frontBottomLeft() const { return position + vec<3, prec>(0, size.y, 0);}
	vec<3, prec> frontBottomRight() const { return position + vec<3, prec>(size.x, size.y, 0); }
    vec<3, prec> backTopLeft() const { return position + vec<3, prec>(0, 0, size.z); }
	vec<3, prec> backTopRight() const { return position + vec<3, prec>(size.x, 0, size.z); }
	vec<3, prec> backBottomLeft() const { return position + vec<3, prec>(0, size.y, size.z);}
	vec<3, prec> backBottomRight() const { return position + size; }

	const prec& left() const { return position.x; }
	prec right() const { return position.x + size.x; }
	const prec& top() const { return position.y; }
	prec bottom() const { return position.y + size.y; }
	const prec& front() const { return position.z + size.z; }
	prec back() const { return position.z + size.z; }

    prec& left() { return position.x; }
	prec& top() { return position.y; }
	prec& front() { return position.z; }

	const prec& width() const { return size.x; }
	const prec& height() const { return size.y; }
	const prec& depth() const { return size.z; }

    prec& width() { return size.x; }
	prec& height() { return size.y; }
	prec& depth() { return size.z; }

    bool empty() const { return anyValueGreater(size, 0); }

    //conversion
	template<size_t odim, class oprec>
	operator rect<odim, oprec>() const { return rect<odim, oprec>(position, size); }
};

//operators
#include <nytl/bits/rect.inl>

}
