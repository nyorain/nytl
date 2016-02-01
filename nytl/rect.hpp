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
///\brief Includes the Rect template class as well as several operators for it.

#pragma once

#include <nytl/vec.hpp>
#include <nytl/simplex.hpp>

#include <vector>
#include <ostream>
#include <algorithm>

namespace nytl
{
//Rect
template<size_t D, class P> class Rect;

template<class P> using Rect2 = Rect<2, P>;
template<class P> using Rect3 = Rect<3, P>;
template<class P> using Rect4 = Rect<4, P>;

typedef Rect<2, int> Rect2i;
typedef Rect<2, unsigned int> Rect2ui;
typedef Rect<2, double> Rect2d;
typedef Rect<2, float> Rect2f;
typedef Rect<2, char> Rect2c;
typedef Rect<2, unsigned char> Rect2uc;
typedef Rect<2, long> Rect2l;
typedef Rect<2, unsigned long> Rect2ul;

typedef Rect<3, int> Rect3i;
typedef Rect<3, unsigned int> Rect3ui;
typedef Rect<3, double> Rect3d;
typedef Rect<3, float> Rect3f;
typedef Rect<3, char> Rect3c;
typedef Rect<3, unsigned char> Rect3uc;
typedef Rect<3, long> Rect3l;
typedef Rect<3, unsigned long> Rect3ul;

typedef Rect<4, int> Rect4i;
typedef Rect<4, unsigned int> Rect4ui;
typedef Rect<4, double> Rect4d;
typedef Rect<4, float> Rect4f;
typedef Rect<4, char> Rect4c;
typedef Rect<4, unsigned char> Rect4uc;
typedef Rect<4, long> Rect4l;
typedef Rect<4, unsigned long> Rect4ul;

///\ingroup math
///\brief Templated class that represents the matheMatic hyperRect (n-box) concept.
///\tparam D The dimension of the hyperRectangle
///\tparam P The precision of the hyperRectangle.
///\details The hyperRectangle is the generalization of a Rectangle for higher dimensions.
///It represents an area that is aligned with the spaces dimensions at a given position with
///a given size. There exist various operators for the Rect template class e.g. to check for
///intersection, compute unions or differences.
///There exist specialization for a 2-dimensional hyperRect (just a Rectangle), a 3-dimensional
///hyperRect (also called box) with additional features. 
template<size_t D, class P> 
class Rect
{
public:
    using value_type = P;
    using VecType = Vec<D, value_type>;

public:
	VecType position;
	VecType size;

public:
	Rect(VecType pposition = VecType(), VecType psize = VecType()) noexcept 
		: position(pposition), size(psize) {}
    ~Rect() noexcept = default;

    Rect(const Rect<D, P>& other) noexcept = default;
	Rect& operator=(const Rect<D, P>& other) noexcept = default;

    Rect(Rect<D, P>&& other) noexcept = default;
	Rect& operator=(Rect<D, P>&& other) noexcept = default;

	Vec<D, double> center() const { return (double)(position + (size / 2.)); };
	bool empty() const { return anyvalue_typeGreater(size, 0); }

	template<size_t oD, class oP> 
	operator Rect<oD, oP>() const { return Rect<oD, oP>(position, size); }
};

//Rect2 specialization
template<class P> class Rect<2, P>
{
public:
    static constexpr std::size_t D = 2;
    using value_type = P;
    using VecType = Vec<D, value_type>;

public:
	VecType position;
	VecType size;

public:
	Rect(const VecType& pposition, const VecType& psize = VecType()) noexcept 
		: position(pposition), size(psize) {}
	Rect(P x = P(), P y = P(), P width = P(), P height = P()) noexcept 
		: position(x,y), size(width,height) {}

	~Rect() noexcept = default;

	Rect(const Rect<D, P>& other) noexcept = default;
	Rect& operator=(const Rect<D, P>& other) noexcept = default;

    Rect(Rect<D, P>&& other) noexcept = default;
	Rect& operator=(Rect<D, P>&& other) noexcept = default;

	Vec<D, double> center() const { return (double)(position + (size / 2.)); };

    Vec<2, P> topLeft() const { return position; }
	Vec<2, P> topRight() const { return position + Vec<2, P>(size.x, 0); }
	Vec<2, P> bottomLeft() const { return position + Vec<2, P>(0, size.y);}
	Vec<2, P> bottomRight() const { return position + size; }

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
	operator Rect<oD, oP>() const { return Rect<oD, oP>(position, size); }
};

//Rect3 specialization
template<class P> class Rect<3, P>
{
public:
    static constexpr std::size_t D = 3;
    using value_type = P;
    using VecType = Vec<D, value_type>;

public:
	VecType position;
	VecType size;

public:
	Rect(const VecType& pposition, const VecType& psize = VecType()) noexcept 
		: position(pposition), size(psize) {}
	Rect(P x = P(), P y = P(), P z = P(), P width = P(), P height = P(), P depth = P()) noexcept
        : position(x,y,z), size(width,height,depth) {}

	~Rect() noexcept = default;

	Rect(const Rect<D, P>& other) noexcept = default;
	Rect& operator=(const Rect<D, P>& other) noexcept = default;

    Rect(Rect<D, P>&& other) noexcept = default;
	Rect& operator=(Rect<D, P>&& other) noexcept = default;

	Vec<D, double> center() const { return (double)(position + (size / 2.)); };

    Vec<2, P> topLeft() const { return position.xy(); }
	Vec<2, P> topRight() const { return position.xy() + Vec<2, P>(size.x, 0); }
	Vec<2, P> bottomLeft() const { return position.xy() + Vec<2, P>(0, size.y);}
	Vec<2, P> bottomRight() const { return position.xy() + size.xy(); }

    Vec<3, P> frontTopLeft() const { return position; }
	Vec<3, P> frontTopRight() const { return position + Vec<3, P>(size.x, 0, 0); }
	Vec<3, P> frontBottomLeft() const { return position + Vec<3, P>(0, size.y, 0);}
	Vec<3, P> frontBottomRight() const { return position + Vec<3, P>(size.x, size.y, 0); }
    Vec<3, P> backTopLeft() const { return position + Vec<3, P>(0, 0, size.z); }
	Vec<3, P> backTopRight() const { return position + Vec<3, P>(size.x, 0, size.z); }
	Vec<3, P> backBottomLeft() const { return position + Vec<3, P>(0, size.y, size.z);}
	Vec<3, P> backBottomRight() const { return position + size; }

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
	operator Rect<oD, oP>() const { return Rect<oD, oP>(position, size); }
};

//operators
#include <nytl/bits/rect.inl>

}
