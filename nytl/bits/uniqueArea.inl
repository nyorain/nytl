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

//tests
///\relates uniqueArea
///Tests two uniqueAreas for intersection. Symmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	intersects(const uniqueArea<D, P, A>&, const uniqueArea<D, P, A>&)
{
}

///\relates uniqueArea
///Tests if the first uniqueArea fully contains the second one. Asymmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const uniqueArea<D, P, A>&, const uniqueArea<D, P, A>&)
{
}

///\relates uniqueArea
///Tests if the given uniqueArea contains the given point.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const uniqueArea<D, P, A>&, const vec<D, P>&)
{
}

///\relates uniqueRegion
///Tests two uniqueRegions for intersection. Symmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	intersects(const uniqueRegion<D, P, A>&, const uniqueRegion<D, P, A>&)
{
}

///\relates uniqueRegion
///Tests if the first uniqueRegion fully contains the second one. Asymmetrical operator.
template<std::size_t D, typename P, std::size_t A>bool 
	contains(const uniqueRegion<D, P, A>&, const uniqueRegion<D, P, A>&)
{
}

///\relates uniqueRegion
///Tests if the given uniqueRegion contains the given point.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const uniqueRegion<D, P, A>&, const vec<D, P>&)
{
}


//operators
///\relates uniqueArea
///Returns the region of intersection between the two given uniqueAreas. Result is not guaranteed
///to be representable by a uniqueArea and therefore a uniqueRegion (i.e. the intersection of two
///triangles is not guaranteed to be a triangle). Symmetrical operator. [AND]
template<std::size_t D, typename P, std::size_t A> uniqueRegion<D, P, A> 
	intersection(const uniqueArea<D, P, A>&, const uniqueArea<D, P, A>&)
{
}

///\relates uniqueArea
///Combines the first and the second area (Offically called union but this name is not available 
///in c++). Result is not guaranteed to be representable by one
///single uniqueArea and is therefore a uniqueRegion. Symmetrical operator. [OR]
template<std::size_t D, typename P, std::size_t A> uniqueRegion<D, P, A> 
	combination(const uniqueArea<D, P, A>&, const uniqueArea<D, P, A>&)
{
}

///\relates uniqueArea
///Returns the symmetric difference of the two given uniqueAreas, so basically the region
///in the given space where exactly one of the two given areas are located.
///Symmetrical operator. [XOR]
template<std::size_t D, typename P, std::size_t A> uniqueRegion<D, P, A> 
	symmetricDifference(const uniqueArea<D, P, A>&, const uniqueArea<D, P, A>&)
{
}

///\relates uniqueArea
///Subtracts the second area from the first one and returns the "rest" of the first area.
///Return type is a uniqueRegion since the result is not guaranteed to be representable by one
///single uniqueArea. Asymmetrical operator. [AND NOT]
template<std::size_t D, typename P, std::size_t A> uniqueRegion<D, P, A> 
	difference(const uniqueArea<D, P, A>&, const uniqueArea<D, P, A>&)
{
}


///\relates uniqueRegion
template<std::size_t D, typename P, std::size_t A> uniqueRegion<D, P, A> 
	intersection(const uniqueRegion<D, P, A>&, const uniqueRegion<D, P, A>&)
{
}

///\relates uniqueRegion
template<std::size_t D, typename P, std::size_t A> uniqueRegion<D, P, A> 
	combination(const uniqueRegion<D, P, A>&, const uniqueRegion<D, P, A>&)
{
}

///\relates uniqueRegion
template<std::size_t D, typename P, std::size_t A> uniqueRegion<D, P, A> 
	symmetricDifference(const uniqueRegion<D, P, A>&, const uniqueRegion<D, P, A>&)
{
}

///\relates uniqueRegion
template<std::size_t D, typename P, std::size_t A> uniqueRegion<D, P, A> 
	difference(const uniqueRegion<D, P, A>&, const uniqueRegion<D, P, A>&)
{
}

}
