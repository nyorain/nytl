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

//detail implementation
namespace detail
{

//members
template<std::size_t D, typename P, std::size_t A>
double simplexSize(const simplex<D, P, A>& s)
{
	squareMat<D, P> m({});
	for(std::size_t i(1); i <= A; ++i)
	{
		m.col(i) = s.points[i] - s.points[0];
	}

	return det(m) / fac<D>();
}

template<std::size_t D, typename P, std::size_t A>
vec<D, P> simplexCenter(const simplex<D, P, A>& s)
{
	return (sum(s.points) / s.points.size());
}

//XXX !important
template<std::size_t D, typename P, std::size_t A>
vec<A + 1, double> simplexBarycentric(const simplex<D, P, A>& s, const vec<D, P>& v)
{
	//XXX: assure valid object
	mat<D, A + 1, double> m;

	for(std::size_t c(0); c < A; ++c)
	{
		m.col(c) = s.points[c] - s.points[A];
	}

	m.col(A) = v - s.points[A];
	if(!rrefMat(m))
	{
		//XXX: throw here, catch rref throw, document it.
		return vec<D, double>({});
	}

	//XXX: analyze matrix. might be wrong! throw then (document it!)
	vec<A + 1, double> ret = m.col(A);
	ret[A] = 1 - sum(ret);

	return ret;
}

template<std::size_t D, typename P, std::size_t A>
bool simplexSameSpace(const simplex<D, P, A>& s, const vec<D, P>& v)
{
	//needs valid/all-informational rref/ref implementation.
	//pivot
}

template<std::size_t OD, typename OP, std::size_t D, typename P, std::size_t A>
simplex<OD, OP, A> simplexConversion(const simplex<D, P, A>& s)
{
	return simplex<OD, OP, A>{s.points};
}

//tests
template<std::size_t D, typename P, std::size_t A>
struct SimplexContainsPoint
{
	static bool test(const simplex<D, P, A>& s, const vec<D, P>& v)
	{
		try
		{
			auto bv = s.barycentric(v);
			return (sum(bv) == 1 && none(bv > 1) && none(bv < 0));
		}
		catch(const std::exception& err)
		{
			return 0;
		}
	}
};

template<std::size_t D, typename P>
struct SimplexContainsPoint<D, P, 0>
{
	static bool test(const simplex<D, P, 0>& s, const vec<D, P>& v)
	{
		return all(s[0] == v);
	}
};


} //detail

//member
template<std::size_t D, typename P, std::size_t A, typename Cond>
double simplex<D, P, A, Cond>::size() const
{
	return detail::simplexSize(*this);
}


template<std::size_t D, typename P, std::size_t A, typename Cond>
vec<D, P> simplex<D, P, A, Cond>::center() const
{
	return detail::simplexCenter(*this);
}

template<std::size_t D, typename P, std::size_t A, typename Cond>
vec<A + 1, double> simplex<D, P, A, Cond>::barycentric(const vec<D, P>& v) const
{
	return detail::simplexBarycentric(*this, v);
}

template<std::size_t D, typename P, std::size_t A, typename Cond>
bool simplex<D, P, A, Cond>::sameSpace(const vec<D, P>& v) const
{
	return detail::simplexSameSpace(*this, v);
}

template<std::size_t D, typename P, std::size_t A, typename Cond>
template<std::size_t OD, typename OP>
simplex<D, P, A, Cond>::operator simplex<OD, OP, A>() const
{
	return detail::simplexConversion<OD, OP>(*this);
}

//tests
//XXX: intersection does not check for containment atm. Probably unexpected 

///\relates simplex
///Tests if the given simplex contains the given point.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const simplex<D, P, A>& s, const vec<D, P>& v)
{
	return detail::SimplexContainsPoint<D, P, A>::test(s, v);
}

///\relates simplex
///Tests two simplexs for intersection. Symmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	intersects(const simplex<D, P, A>& s1, const simplex<D, P, A>& s2)
{
	for(auto& v : s2.points)
		if(contains(s1, v)) return 1;

	return 0;
}

///\relates simplex
///Tests if the first simplex fully contains the second one. Asymmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const simplex<D, P, A>& s1, const simplex<D, P, A>& s2)
{
	for(auto& v : s2.points)
		if(!contains(s1, v)) return 0;

	return 1;
}

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

