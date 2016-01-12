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

//exception////////////////////////////////////////////////////////////////////////////////
///\relates simplex
///\brief This exception is thrown when two given arguments do not lay in the same spaces.
///\details For some computations (i.e. barycentric coordinates) it is required that two
///geometric areas (e.g. simplex and point) lay in the same space.
class invalid_space : public std::invalid_argument
{
public:
	invalid_space() : std::invalid_argument("Invalid argument given: different space") {}
	invalid_space(const std::string& func) 
		: std::invalid_argument("Invalid argument given: different space at function " + func) {}  
};

///\relates simplex
///\brief This exception is thrown if a given simplex object argument is not valid.
///\details Needed because some computations (e.g. barycentric coordinates or sameSpace-check)
///can not be done swith an invalid simplex object.
class invalid_simplex : public std::invalid_argument
{
public:
	invalid_simplex() : std::invalid_argument("Invalid simplex object parameter") {}
	invalid_simplex(const std::string& func) 
		: std::invalid_argument("Invalid simplex object parameter given at function " + func) {}  
};


//detail implementation///////////////////////////////////////////////
namespace detail
{

template<std::size_t D, typename P, std::size_t A>
double simplexSize(const simplex<D, P, A>& s)
{
	squareMat<D, P> m;
	for(std::size_t i(1); i <= A; ++i)
	{
		m.col(i) = s.points[i] - s.points[0];
	}

	return det(m) / fac(D);
}

template<std::size_t D, typename P, std::size_t A>
vec<D, P> simplexCenter(const simplex<D, P, A>& s)
{
	return (sum(s.points) / s.points.size());
}

//throws invalid_space if given point does not lay in space(line, plane, area ...) of simplex
template<std::size_t D, typename P, std::size_t A>
vec<A + 1, double> simplexBarycentric(const simplex<D, P, A>& s, const vec<D, P>& v)
{
	if(!s.valid())
	{
		throw invalid_simplex("simplexBarycentric");
	}

	mat<D, A + 1, double> m;
	for(std::size_t c(0); c < A; ++c)
	{
		m.col(c) = s.points()[c] - s.points()[A];
	}

	m.col(A) = v - s.points()[A];

	auto& les = reinterpret_cast<linearEquotationSystem<D, A, double>&>(m);
	auto sol = les.solve();

	if(!sol.solvable())
	{
		throw invalid_space("simplexBarycentric");
	}
	else if(!sol.unambigouosSolvable())
	{
		//should never happen since simplex is previously checked for validity.
		throw invalid_simplex("simplexBarycentric, :2");
	}

	return sol.solution();
}

///TODO: should this function really throw? not just return false?
template<std::size_t D, typename P, std::size_t A>
bool simplexSameSpace(const simplex<D, P, A>& s, const vec<D, P>& v)
{
	if(!s.valid())
	{
		throw invalid_simplex("simplexSameSpace");
	}

	mat<D, A + 1, double> m;
	for(std::size_t c(0); c < A; ++c)
	{
		m.col(c) = s.points()[c] - s.points()[A];
	}

	m.col(A) = v - s.points()[A];

	auto& les = reinterpret_cast<linearEquotationSystem<D, A, double>&>(m);
	auto sol = les.solve();

	if(!sol.solvable())
	{
		return false;
	}
	else if(!sol.unambigouosSolvable())
	{
		//should never happen since simplex is previously checked for validity.
		throw invalid_simplex("simplexSameSpace, :2");
	}

	return true;
}

template<std::size_t OD, typename OP, std::size_t D, typename P, std::size_t A>
simplex<OD, OP, A> simplexConversion(const simplex<D, P, A>& s)
{
	return simplex<OD, OP, A>{s.points()};
}

template<std::size_t D, typename P, std::size_t A>
bool simplexValid(const simplex<D, P, A>& s)
{
	return (s.size() >= 0);
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
		catch(const invalid_space& err)
		{
			return false;
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



//member///////////////////////////////////////////////
template<std::size_t D, typename P, std::size_t A>
double simplex<D, P, A>::size() const
{
	return detail::simplexSize(*this);
}


template<std::size_t D, typename P, std::size_t A>
vec<D, P> simplex<D, P, A>::center() const
{
	return detail::simplexCenter(*this);
}

template<std::size_t D, typename P, std::size_t A>
vec<A + 1, double> simplex<D, P, A>::barycentric(const vec<D, P>& v) const
{
	return detail::simplexBarycentric(*this, v);
}

template<std::size_t D, typename P, std::size_t A>
template<std::size_t OD, typename OP>
simplex<D, P, A>::operator simplex<OD, OP, A>() const
{
	return detail::simplexConversion<OD, OP>(*this);
}

template<std::size_t D, typename P, std::size_t A>
bool simplex<D, P, A>::valid() const
{
	return detail::simplexValid(*this);
}


//tests/////////////////////////////////////
///\relates simplex
///Returns whether the simplex lays in the same space as the given point.
///If e.g. the simplexes D == 2 it checks whether they lay on the same plane.
///If D == A for the simplex object, this function will always return true.
///\warning Will throw nytl::invalid_simplex if the simplex object is not valid (i.e. size
///<= 0).
template<std::size_t D, typename P, std::size_t A> bool
	sameSpace(const simplex<D, P, A>& s, const vec<D, P>& v)
{
	return detail::simplexSameSpace(s, v);
}

///\TODO: intersection does not check for containment atm. Probably unexpected.
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

#ifdef DOXYGEN
} //nytl
#endif
