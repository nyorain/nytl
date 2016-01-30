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

//exceptions
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

//utility
//TODO: use stack here? could be problematic for higher dimensions; Vec size known
///\relates simplex
///Returns all lines of a given simplex. The size of the returnsed Vector is fac(A + 1)
template<std::size_t D, typename P, std::size_t A>
std::vector<line<D, P>> lines(const simplex<D, P, A>& simp)
{
	std::vector<line<D, P>> ret(fac(A + 1));

	auto idx = std::size_t {0};
	for(std::size_t i(0); i < A + 1; ++i)
	{
		for(std::size_t o(i + 1); o < A + 1; ++o)
		{
			ret[idx].a = simp.points()[i];
			ret[idx++].b = simp.points()[o];
		}
	}

	return ret;
}

///\relates simplex
///Outputs all simplex points to an ostream.
template<std::size_t D, typename P, std::size_t A>
std::ostream& operator<<(std::ostream& os, const simplex<D, P, A>& s)
{
	for(auto& p : s.points())
		os << p;

	return os;
}


//detail implementation
namespace detail
{

//member
template<std::size_t D, typename P, std::size_t A>
double simplexSize(const simplex<D, P, A>& s)
{
	squareMat<D, P> m;
	for(std::size_t i(1); i <= A; ++i)
	{
		m.col(i) = s.points()[i] - s.points()[0];
	}

	return std::abs(det(m)) / fac(D);
}

template<std::size_t D, typename P, std::size_t A>
Vec<D, P> simplexCenter(const simplex<D, P, A>& s)
{
	return (sum(s.points) / s.points.size());
}
template<std::size_t OD, typename OP, std::size_t D, typename P, std::size_t A>
simplex<OD, OP, A> simplexConversion(const simplex<D, P, A>& s)
{
	return simplex<OD, OP, A>{s.points()};
}

template<std::size_t D, typename P, std::size_t A>
bool simplexValid(const simplex<D, P, A>& s)
{
	return (s.size() > 0);
}

//free
//throws invalid_space if given point does not lay in space(line, plane, area ...) of simplex
template<std::size_t D, typename P, std::size_t A>
struct SimplexBarycentric
{
	static Vec<A + 1, double> call(const simplex<D, P, A>& s, const Vec<D, P>& v)
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
		else if(!sol.unambigouoslySolvable())
		{
			//should never happen since simplex is previously checked for validity.
			throw invalid_simplex("simplexBarycentric, :2");
		}
	
		return sol.solution();
	}

};

template<std::size_t D, typename P, std::size_t A>
struct SimplexSameSpace
{

	static bool call(const simplex<D, P, A>& s, const Vec<D, P>& v)
	{
		if(!s.valid())
		{
			//throw invalid_simplex("simplexSameSpace");
			return false;
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
			//throw invalid_simplex("simplexSameSpace, :2");
			return false;
		}
	
		return true;
	}
};


template<std::size_t D, typename P, std::size_t A>
struct SimplexContainsPoint
{
	static bool call(const simplex<D, P, A>& s, const Vec<D, P>& v)
	{
		try
		{
			auto bv = barycentric(s, v);
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
	static bool call(const simplex<D, P, 0>& s, const Vec<D, P>& v)
	{
		return all(s[0] == v);
	}
};

//TODO
template<std::size_t D, typename P, std::size_t A1, std::size_t A2>
struct SimplexIntersection
{
	static simplexRegion<D, double, min(A1, A2)> 
	call(const simplex<D, P, A1>& sa, const simplex<D, P, A2>& sb)
	{
		//find the outlining lines
		std::vector<line<D, double>> lines;

		//sa lines
		for(auto line : nytl::lines(sa))
		{
			auto p = intersection(sb, line).areas(); //specialization needed
			if(p.empty()) 
			{
				continue;
			}
			else
			{
				if(p[0].size() == 0)
				{
					if(contains(sb, line.a)) p[0].b = line.a;
					else if(contains(sb, line.b)) p[0].b = line.b;
				}
			}

			if(p[0].valid()) lines.push_back(p[0]);
		}

		//sb lines
		for(auto line : nytl::lines(sb))
		{
			auto p = intersection(sa, line).areas(); //specialization needed
			std::cout << "p: " << dumpContainer(p) << "\n";
			if(p.empty()) 
			{
				continue;
			}
			else
			{
				std::cout << "sssize: " << p.size() << "\n";
				if(p[0].size() == 0)
				{
					if(contains(sb, line.a)) p[0].b = line.a;
					else if(contains(sb, line.b)) p[0].b = line.b;
				}
			}

			if(p[0].valid()) lines.push_back(p[0]);
		}

		std::cout << "lines<<<<<<<<<<<:\n" << dumpContainer(lines) << "\n";
		return nytl::createConvex(lines);
	};
};

template<std::size_t D, typename P, std::size_t A1>
struct SimplexIntersection<D, P, A1, 1>
{
	static simplexRegion<D, double, 1> 
	call(const simplex<D, P, A1>& sa, const simplex<D, P, 1>& sb)
	{
		mat<D + 2, (A1 + 1) + 2 + 1, double> eqs;
		for(std::size_t i(0); i < A1 + 1; ++i)
		{
			eqs.col(i) = sa.points()[i];
			eqs.row(D)[i] = 1;
			eqs.row(D + 1)[i] = 0;
		}	

		for(std::size_t i(0); i < 2; ++i)
		{
			eqs.col(A1 + 1 + i) = -sb.points()[i];
			eqs.row(D + 1)[A1 + 1 + i] = 1;
			eqs.row(D)[A1 + 1 + i] = 0;
		}

		eqs.col((A1 + 1) + 2).fill(0); //last colums

		eqs.row(D)[(A1 + 1) + 2] = 1;
		eqs.row(D + 1)[(A1 + 1) + 2] = 1;

		auto solSet = solve(eqs);
		line<D, double> ret;

		try
		{
			auto dSolSet = domainedSolutionSet<(A1 + 1) + 2>(solSet, linearDomain{0., 1.});

			ret.a = cartesian(sb, subVec<2>(dSolSet.solution({0}, {0}, 0), A1 + 1));
			ret.b = cartesian(sb, subVec<2>(dSolSet.solution({0}, {1}, 0), A1 + 1));
		}
		catch(const std::invalid_argument& err)
		{
			return {};
		}
		
		auto r = simplexRegion<D, double, 1>{};
		r.addNoCheck(ret);

		std::cout << "line intersection between " << sa << " and " << sb << ": " << ret << "\n";

		return r;
	};
};

template<std::size_t D, typename P, std::size_t A2>
struct SimplexIntersection<D, P, 1, A2>
{
	static auto ccall(const simplex<D, P, 1>& sa, const simplex<D, P, A2>& sb) 
		-> decltype(SimplexIntersection<D, P, A2, 1>::call(sb, sa))
	{
		return SimplexIntersection<D, P, A2, 1>::call(sb, sa);
	}
};

template<std::size_t D, typename P, std::size_t A>
struct SimplexIntersects
{
	static bool call(const simplex<D, P, A>& sa, const simplex<D, P, A>& sb)
	{
		mat<D + 2, (A * 2) + 1, double> eqs;
		for(std::size_t i(0); i < A * 2; ++i)
		{
			eqs.col(i) = sa.points()[i];
			eqs.col(A + 1 + i) = -sb.points()[i];

			eqs.row(D)[i] = 1;
			eqs.row(D)[A + 1 + i] = 0;

			eqs.row(D + 1)[i] = 0;
			eqs.row(D + 1)[A + 1 + i] = 1;
		}	

		eqs.col((A + 1) * 2).fill(0);
		eqs.row(D)[(A + 1) * 2] = 1;
		eqs.row(D + 1)[(A + 1) * 2] = 1;

		rrefMat(eqs);
		//TODO
		
		return 0;
	};
};

} //detail


//member
template<std::size_t D, typename P, std::size_t A>
double simplex<D, P, A>::size() const
{
	return detail::simplexSize(*this);
}

template<std::size_t D, typename P, std::size_t A>
Vec<D, P> simplex<D, P, A>::center() const
{
	return detail::simplexCenter(*this);
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

//utility
///\relates simplex
///Converts the given cartesian coordinates into barycentric coordinates for the given simplex.
///If the given point does not lay in the same space as the simplex object,
///the function will throw an exception.
///This can be checked before with sameSpace().
///Reverse function to cartesian().
///\exception nytl::invalid_simplex If the simplex object is not valid. Can be 
///checked with (simplex.valid()) or (simplex.size() >= 0).
///\exception nytl::invalid_space If the given point does not lay in the simplex's 
///space (e.g. the simplex is a triangle in a 3-dimensional room and the triangle and the given
///point do not lay on the same plane). Can be checked with sameSpace(simplex, point).
template<std::size_t D, typename P, std::size_t A>
Vec<A + 1, double> barycentric(const simplex<D, P, A>& s, const Vec<D, P>& cart)
{
	return detail::SimplexBarycentric<D, P, A>::call(s, cart);
}


///Converts the given barycentric coordinates of the given simplex to cartesian coordinates.
///Reverse function to barycentric().
template<std::size_t D, typename P, std::size_t A>
Vec<D, double> cartesian(const simplex<D, P, A>& s, const Vec<A + 1, double>& bary)
{
	auto ret = Vec<D, double>{};
	for(std::size_t i(0); i < A + 1; ++i)
		ret += bary[i] * s.points()[i];

	return ret;
}


//tests
///\relates simplex
///Returns whether the simplex lays in the same space as the given point.
///If e.g. the simplexes D == 2 it checks whether they lay on the same plane.
///If D == A for the simplex object, this function will always return true.
template<std::size_t D, typename P, std::size_t A> bool
	sameSpace(const simplex<D, P, A>& s, const Vec<D, P>& v)
{
	return detail::SimplexSameSpace<D, P, A>::call(s, v);
}

///\relates simplex
///Tests if the given simplex contains the given point.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const simplex<D, P, A>& s, const Vec<D, P>& v)
{
	return detail::SimplexContainsPoint<D, P, A>::call(s, v);
}

///\relates simplex
///Tests two simplexs for intersection. Symmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	intersects(const simplex<D, P, A>& s1, const simplex<D, P, A>& s2)
{
	return detail::SimplexIntersects<D, P, A>::call(s1, s2);
}

///\relates simplex
///Tests if the first simplex fully contains the second one. Asymmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const simplex<D, P, A>& s1, const simplex<D, P, A>& s2)
{
	for(auto& v : s2.points())
		if(!contains(s1, v)) return 0;

	return 1;
}

//operators
///\relates simplex
///Returns the region of intersection between the two given simplexs. Result is not guaranteed
///to be representable by a simplex and therefore a simplexRegion (i.e. the intersection of two
///triangles is not guaranteed to be a triangle). Symmetrical operator. [AND]
template<std::size_t D, typename P, std::size_t A1, std::size_t A2> 
auto intersection(const simplex<D, P, A1>& sa, const simplex<D, P, A2>& sb)
	-> decltype(detail::SimplexIntersection<D, P, A1, A2>::call(sa, sb))
{
	return detail::SimplexIntersection<D, P, A1, A2>::call(sa, sb);
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

#ifdef DOXYGEN
} //nytl
#endif
