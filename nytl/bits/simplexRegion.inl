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

#ifdef DOXYGEN
namespace nytl{
#endif

///\relates simplexRegion
///Tests if a simplexRegion intersects with a simplex. Symmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	intersects(const simplexRegion<D, P, A>& r, const simplex<D, P, A>& s)
{
	for(auto& rs : r)
		if(intersects(rs, s)) return 1;

	return 0;
}

///\relates simplexRegion
///Tests two simplexRegions for intersection. Symmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	intersects(const simplexRegion<D, P, A>& r1, const simplexRegion<D, P, A>& r2)
{
	for(auto& rs : r2)
		if(intersects(r1, rs)) return 1;

	return 0;
}

///\relates simplexRegion
///Tests if the given simplexRegion fully contains the given simplex. Asymmetrical operator.
template<std::size_t D, typename P, std::size_t A>bool 
	contains(const simplexRegion<D, P, A>& r, const simplex<D, P, A>& s)
{
	for(auto& rs : r)
		if(!contains(rs, s)) return 0;

	return 1;
}

///\relates simplexRegion
///Tests if the first simplexRegion fully contains the second one. Asymmetrical operator.
template<std::size_t D, typename P, std::size_t A>bool 
	contains(const simplexRegion<D, P, A>& r1, const simplexRegion<D, P, A>& r2)
{
	for(auto& s : r2)
		if(!contains(r1, s)) return 0;

	return 1;
}

///\relates simplexRegion
///Tests if the given simplexRegion contains the given point.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const simplexRegion<D, P, A>& r, const vec<D, P>& v)
{
	for(auto& s : r)
		if(contains(s, v)) return 1;

	return 0;
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

#ifdef DOXYGEN
}
#endif
