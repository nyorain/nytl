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
///\brief Includes the rect template class as well as several operators for it.

#pragma once

#include <nytl/vec.hpp>
#include <nytl/simplex.hpp>

#include <vector>
#include <ostream>
#include <algorithm>

namespace nytl
{
//rect
template<size_t D, class P> class rect;

template<class P> using rect2 = rect<2, P>;
template<class P> using rect3 = rect<3, P>;
template<class P> using rect4 = rect<4, P>;

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

///\ingroup math
///\brief Templated class that represents the mathematic hyperrect (n-box) concept.
///\tparam D The dimension of the hyperrectangle
///\tparam P The precision of the hyperrectangle.
///\details The hyperrectangle is the generalization of a rectangle for higher dimensions.
///It represents an area that is aligned with the spaces dimensions at a given position with
///a given size. There exist various operators for the rect template class e.g. to check for
///intersection, compute unions or differences.
///There exist specialization for a 2-dimensional hyperrect (just a rectangle), a 3-dimensional
///hyperrect (also called box) with additional features. 
template<size_t D, class P> 
class rect
{
public:
    using value_type = P;
    using vec_type = vec<D, value_type>;

public:
	vec_type position;
	vec_type size;

public:
	rect(vec_type pposition = vec_type(), vec_type psize = vec_type()) noexcept 
		: position(pposition), size(psize) {}
    ~rect() noexcept = default;

    rect(const rect<D, P>& other) noexcept = default;
	rect& operator=(const rect<D, P>& other) noexcept = default;

    rect(rect<D, P>&& other) noexcept = default;
	rect& operator=(rect<D, P>&& other) noexcept = default;

	vec<D, double> center() const { return (double)(position + (size / 2.)); };
	bool empty() const { return anyValueGreater(size, 0); }

	template<size_t oD, class oP> 
	operator rect<oD, oP>() const { return rect<oD, oP>(position, size); }
};

//rect2 specialization
template<class P> class rect<2, P>
{
public:
    static constexpr std::size_t D = 2;
    using value_type = P;
    using vec_type = vec<D, value_type>;

public:
	vec_type position;
	vec_type size;

public:
	rect(const vec_type& pposition, const vec_type& psize = vec_type()) noexcept 
		: position(pposition), size(psize) {}
	rect(P x = P(), P y = P(), P width = P(), P height = P()) noexcept 
		: position(x,y), size(width,height) {}

	~rect() noexcept = default;

	rect(const rect<D, P>& other) noexcept = default;
	rect& operator=(const rect<D, P>& other) noexcept = default;

    rect(rect<D, P>&& other) noexcept = default;
	rect& operator=(rect<D, P>&& other) noexcept = default;

	vec<D, double> center() const { return (double)(position + (size / 2.)); };

    vec<2, P> topLeft() const { return position; }
	vec<2, P> topRight() const { return position + vec<2, P>(size.x, 0); }
	vec<2, P> bottomLeft() const { return position + vec<2, P>(0, size.y);}
	vec<2, P> bottomRight() const { return position + size; }

	const P& left() const { return position.x; }
	const P right() const { return position.x + size.x; }
	const P& top() const { return position.y; }
	const P bottom() const { return position.y + size.y; }

    P& left() { return position.x; }
	P& top() { return position.y; }

	const P& width() const { return size.x; }
	const P& height() const { return size.y; }

    P& width() { return size.x; }
	P& height() { return size.y; }

    bool empty() const { return any(size > 0); }

    //conversion
	template<size_t oD, class oP>
	operator rect<oD, oP>() const { return rect<oD, oP>(position, size); }
};

//rect3 specialization
template<class P> class rect<3, P>
{
public:
    static constexpr std::size_t D = 3;
    using value_type = P;
    using vec_type = vec<D, value_type>;

public:
	vec_type position;
	vec_type size;

public:
	rect(const vec_type& pposition, const vec_type& psize = vec_type()) noexcept 
		: position(pposition), size(psize) {}
	rect(P x = P(), P y = P(), P z = P(), P width = P(), P height = P(), P depth = P()) noexcept
        : position(x,y,z), size(width,height,depth) {}

	~rect() noexcept = default;

	rect(const rect<D, P>& other) noexcept = default;
	rect& operator=(const rect<D, P>& other) noexcept = default;

    rect(rect<D, P>&& other) noexcept = default;
	rect& operator=(rect<D, P>&& other) noexcept = default;

	vec<D, double> center() const { return (double)(position + (size / 2.)); };

    vec<2, P> topLeft() const { return position.xy(); }
	vec<2, P> topRight() const { return position.xy() + vec<2, P>(size.x, 0); }
	vec<2, P> bottomLeft() const { return position.xy() + vec<2, P>(0, size.y);}
	vec<2, P> bottomRight() const { return position.xy() + size.xy(); }

    vec<3, P> frontTopLeft() const { return position; }
	vec<3, P> frontTopRight() const { return position + vec<3, P>(size.x, 0, 0); }
	vec<3, P> frontBottomLeft() const { return position + vec<3, P>(0, size.y, 0);}
	vec<3, P> frontBottomRight() const { return position + vec<3, P>(size.x, size.y, 0); }
    vec<3, P> backTopLeft() const { return position + vec<3, P>(0, 0, size.z); }
	vec<3, P> backTopRight() const { return position + vec<3, P>(size.x, 0, size.z); }
	vec<3, P> backBottomLeft() const { return position + vec<3, P>(0, size.y, size.z);}
	vec<3, P> backBottomRight() const { return position + size; }

	const P& left() const { return position.x; }
	P right() const { return position.x + size.x; }
	const P& top() const { return position.y; }
	P bottom() const { return position.y + size.y; }
	const P& front() const { return position.z + size.z; }
	P back() const { return position.z + size.z; }

    P& left() { return position.x; }
	P& top() { return position.y; }
	P& front() { return position.z; }

	const P& width() const { return size.x; }
	const P& height() const { return size.y; }
	const P& depth() const { return size.z; }

    P& width() { return size.x; }
	P& height() { return size.y; }
	P& depth() { return size.z; }

    bool empty() const { return any(size > 0); }

    //conversion
	template<size_t oD, class oP>
	operator rect<oD, oP>() const { return rect<oD, oP>(position, size); }
};

//operators
#include <nytl/bits/rect.inl>

}
