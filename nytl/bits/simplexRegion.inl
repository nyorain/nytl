/* The MIT License (MIT)
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

#pragma once

#ifdef DOXYGEN
namespace nytl{
#endif

//utility
///\relates SimplexRegion DomainedSolutionSet
///Creates a convex shape (SimplexRegion) as a geometrical representation of a DomainedSolutionSet.
template<std::size_t D>
SimplexRegion<D, double, D> createConvex(const DomainedSolutionSet<D>& solution)
{
	auto points = outLinePoints(solution);
	return createConvex<D, double>(points);
}

///\relates SimplexRegion
///Creates a convex shape(SimplexRegion) from its outLine points.
template<std::size_t D, typename P, std::size_t A = D>
SimplexRegion<D, P, A> createConvex(const std::vector<Vec<D, P>>& points)
{
	std::vector<Line<D, P>> Lines;
	for(auto it = points.cbegin(); it != points.cend(); ++it)
	{
		auto& p = *it;
		for(auto it2 = it; it2 != points.cend(); ++it2)
		{
			auto& p2 = *it2;

			if(&p == &p2) continue;
			Lines.push_back(Line<D, P>{p, p2});
		}
	}

	auto Lines2 = Lines;
	for(auto it = Lines.cbegin(); it != Lines.cend(); ++it)
	{
		auto& l1 = *it;
		for(auto l2 : Lines2)
		{
			if(all(l1.a == l2.a) || all(l1.b == l2.b) || all(l1.b == l2.a) || all(l1.a == l2.b)) 
			{
				continue;
			}

			if(intersects(l1, l2))
			{
				it = Lines.erase(it);
				--it;
				break;
			}
		}
	}

	return createConvex(Lines);
}

///\relates SimplexRegion
///Creates a convex shape(SimplexRegion) from its outlining Lines.
template<std::size_t D, typename P, std::size_t A = D>
SimplexRegion<D, P, A> createConvex(std::vector<Line<D, P>> Lines)
{
	SimplexRegion<D, P, A> ret;

	//newly created Lines, inserted AFTER Simplex is completly found
	for(std::size_t i(0); i < Lines.size() && Lines.size() > A + 1;)
	{
		std::vector<Line<D, P>> newLines {};
		auto Line = Lines[i];
		Lines.erase(Lines.cbegin() + i);

		Simplex<D, P, A> simp;
		simp.points()[0] = Line.a;
		simp.points()[1] = Line.b;
		
		std::size_t idx = 1;

		//Lines at Line.a (center point)
		for(std::size_t i2(0); i2 < Lines.size(); ++i2)
		{
			auto& Line2 = Lines[i2];

			if(all(Line2.a == Line.a))
			{
				simp.points()[++idx] = Line2.b;
				Lines.erase(Lines.cbegin() + i2);
				newLines.push_back({simp.points()[idx - 1], simp.points()[idx]});
			}
			else if(all(Line2.b == Line.a))
			{
				simp.points()[++idx] = Line2.a;
				Lines.erase(Lines.cbegin() + i2);
				newLines.push_back({simp.points()[idx - 1], simp.points()[idx]});
			}
		
			if(idx == A) break; //all found
		}

		assert(idx == A); //error here! could not construct Simplex

		//find Lines on the opposite site of Line.a
		//they belong to the Simplex, but do not have point Line.a
		for(std::size_t i2(0); i2 < Lines.size(); ++i2)
		{
			auto& Line2 = Lines[i2];
			bool foundA = 0, foundB = 0;

			for(auto& p : simp.points())
			{
				if(all(p == Line2.a)) foundA = 1;
				if(all(p == Line2.b)) foundB = 1;

				if(foundA && foundB) break;
			}

			if(foundA && foundB)
			{
				Lines.erase(Lines.cbegin() + i2);
			}
		}

		std::cout << simp << "\n";
		ret.addNoCheck(simp);
		Lines.insert(Lines.cend(), newLines.cbegin(), newLines.cend());
	}


	//code dupilcation here -- argh -> search for better loop above
	if(Lines.size() == A + 1) //should be always true?
	{
		Simplex<D, P, A> simp;
		std::size_t idx = 0;

		for(std::size_t i(0); i < Lines.size(); ++i)
		{
			auto& Line2 = Lines[i];
			bool foundA = 0, foundB = 0;

			for(std::size_t i2(0); i2 < idx; ++i2)
			{
				auto& p = simp.points()[i2];
				if(all(p == Line2.a)) foundA = 1;
				if(all(p == Line2.b)) foundB = 1;

				if(foundA && foundB) break;
			}

			if(!foundA)
			{
				simp.points()[idx++] = Line2.a;
			}
			if(!foundB)
			{
				simp.points()[idx++] = Line2.b;
			}
		}
	
		ret.addNoCheck(simp);
	}

	return ret;
}


///tests
///\relates SimplexRegion
///Tests if a SimplexRegion intersects with a Simplex. Symmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	intersects(const SimplexRegion<D, P, A>& r, const Simplex<D, P, A>& s)
{
	for(auto& rs : r)
		if(intersects(rs, s)) return 1;

	return 0;
}

///\relates SimplexRegion
///Tests two SimplexRegions for intersection. Symmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	intersects(const SimplexRegion<D, P, A>& r1, const SimplexRegion<D, P, A>& r2)
{
	for(auto& rs : r2)
		if(intersects(r1, rs)) return 1;

	return 0;
}

///\relates SimplexRegion
///Tests if the given SimplexRegion fully contains the given Simplex. Asymmetrical operator.
template<std::size_t D, typename P, std::size_t A>bool 
	contains(const SimplexRegion<D, P, A>& r, const Simplex<D, P, A>& s)
{
	for(auto& rs : r)
		if(!contains(rs, s)) return 0;

	return 1;
}

///\relates SimplexRegion
///Tests if the first SimplexRegion fully contains the second one. Asymmetrical operator.
template<std::size_t D, typename P, std::size_t A>bool 
	contains(const SimplexRegion<D, P, A>& r1, const SimplexRegion<D, P, A>& r2)
{
	for(auto& s : r2)
		if(!contains(r1, s)) return 0;

	return 1;
}

///\relates SimplexRegion
///Tests if the given SimplexRegion contains the given point.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const SimplexRegion<D, P, A>& r, const Vec<D, P>& v)
{
	for(auto& s : r)
		if(contains(s, v)) return 1;

	return 0;
}

///\relates SimplexRegion
template<std::size_t D, typename P, std::size_t A> SimplexRegion<D, P, A> 
	intersection(const SimplexRegion<D, P, A>&, const SimplexRegion<D, P, A>&)
{
}

///\relates SimplexRegion
template<std::size_t D, typename P, std::size_t A> SimplexRegion<D, P, A> 
	combination(const SimplexRegion<D, P, A>&, const SimplexRegion<D, P, A>&)
{
}

///\relates SimplexRegion
template<std::size_t D, typename P, std::size_t A> SimplexRegion<D, P, A> 
	symmetricDifference(const SimplexRegion<D, P, A>&, const SimplexRegion<D, P, A>&)
{
}

///\relates SimplexRegion
template<std::size_t D, typename P, std::size_t A> SimplexRegion<D, P, A> 
	difference(const SimplexRegion<D, P, A>&, const SimplexRegion<D, P, A>&)
{
}

#ifdef DOXYGEN
}
#endif
