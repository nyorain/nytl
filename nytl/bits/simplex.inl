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

#include <cmath>

namespace nytl
{

//detail implementation
namespace detail
{

template<std::size_t D, typename P, std::size_t A>
struct SimplexContainsPoint
{
	static bool test(const simplex<D, P, A>& a, const vec<D, P>& v)
	{
	}
};

/*
template<std::size_t D, typename P>
struct SimplexContainsPoint<D, P, 1>
{
	static bool test(const simplex<D, P, 1>& a, const vec<D, P>& v)
	{
		vec<D, P> gradient = a.points[1] - a.points[0];
		gradient /= gradient[0];

		return all(v == (a.points[0] + ((v[0] - a.points[0][0]) * gradient)));
	}

};

template<std::size_t D, typename P>
struct SimplexContainsPoint<D, P, 0>
{
	static bool test(const simplex<D, P, 0>& a, const vec<D, P>& v)
	{
		return (a[0] == v);
	}

};
*/

} //detail

//tests
///\relates simplex
///Tests two simplexs for intersection. Symmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	intersects(const simplex<D, P, A>&, const simplex<D, P, A>&)
{
}

///\relates simplex
///Tests if the first simplex fully contains the second one. Asymmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const simplex<D, P, A>&, const simplex<D, P, A>&)
{
}

///\relates simplex
///Tests if the given simplex contains the given point.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const simplex<D, P, A>& a, const vec<D, P>& v)
{
	return detail::SimplexContainsPoint<D, P, A>::test(a, v);
}

///\relates simplexRegion
///Tests two simplexRegions for intersection. Symmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	intersects(const simplexRegion<D, P, A>&, const simplexRegion<D, P, A>&)
{
}

///\relates simplexRegion
///Tests if the first simplexRegion fully contains the second one. Asymmetrical operator.
template<std::size_t D, typename P, std::size_t A>bool 
	contains(const simplexRegion<D, P, A>&, const simplexRegion<D, P, A>&)
{
}

///\relates simplexRegion
///Tests if the given simplexRegion contains the given point.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const simplexRegion<D, P, A>&, const vec<D, P>&)
{
}


//operators
///\relates simplex
///Returns the region of intersection between the two given simplexs. Result is not guaranteed
///to be representable by a simplex and therefore a simplexRegion (i.e. the intersection of two
///triangles is not guaranteed to be a triangle). Symmetrical operator. [AND]
template<std::size_t D, typename P, std::size_t A> simplexRegion<D, P, A> 
	intersection(const simplex<D, P, A>&, const simplex<D, P, A>&)
{
}

///\relates simplex
///Combines the first and the second area (Offically called union but this name is not available 
///in c++). Result is not guaranteed to be representable by one
///single simplex and is therefore a simplexRegion. Symmetrical operator. [OR]
template<std::size_t D, typename P, std::size_t A> simplexRegion<D, P, A> 
	combination(const simplex<D, P, A>&, const simplex<D, P, A>&)
{
}

///\relates simplex
///Returns the symmetric difference of the two given simplexs, so basically the region
///in the given space where exactly one of the two given areas are located.
///Symmetrical operator. [XOR]
template<std::size_t D, typename P, std::size_t A> simplexRegion<D, P, A> 
	symmetricDifference(const simplex<D, P, A>&, const simplex<D, P, A>&)
{
}

///\relates simplex
///Subtracts the second area from the first one and returns the "rest" of the first area.
///Return type is a simplexRegion since the result is not guaranteed to be representable by one
///single simplex. Asymmetrical operator. [AND NOT]
template<std::size_t D, typename P, std::size_t A> simplexRegion<D, P, A> 
	difference(const simplex<D, P, A>&, const simplex<D, P, A>&)
{
}


///\relates simplexRegion
template<std::size_t D, typename P, std::size_t A> simplexRegion<D, P, A> 
	intersection(const simplexRegion<D, P, A>&, const simplexRegion<D, P, A>&)
{
}

///\relates simplexRegion
template<std::size_t D, typename P, std::size_t A> simplexRegion<D, P, A> 
	combination(const simplexRegion<D, P, A>&, const simplexRegion<D, P, A>&)
{
}

///\relates simplexRegion
template<std::size_t D, typename P, std::size_t A> simplexRegion<D, P, A> 
	symmetricDifference(const simplexRegion<D, P, A>&, const simplexRegion<D, P, A>&)
{
}

///\relates simplexRegion
template<std::size_t D, typename P, std::size_t A> simplexRegion<D, P, A> 
	difference(const simplexRegion<D, P, A>&, const simplexRegion<D, P, A>&)
{
}

}
