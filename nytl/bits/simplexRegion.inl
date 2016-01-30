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

//utility
///\relates simplexRegion domainedSolutionSet
///Creates a convex shape (simplexRegion) as a geometrical representation of a domainedSolutionSet.
template<std::size_t D>
simplexRegion<D, double, D> createConvex(const domainedSolutionSet<D>& solution)
{
	auto points = outlinePoints(solution);
	return createConvex<D, double>(points);
}

///\relates simplexRegion
///Creates a convex shape(simplexRegion) from its outline points.
template<std::size_t D, typename P, std::size_t A = D>
simplexRegion<D, P, A> createConvex(const std::vector<Vec<D, P>>& points)
{
	std::vector<line<D, P>> lines;
	for(auto it = points.cbegin(); it != points.cend(); ++it)
	{
		auto& p = *it;
		for(auto it2 = it; it2 != points.cend(); ++it2)
		{
			auto& p2 = *it2;

			if(&p == &p2) continue;
			lines.push_back(line<D, P>{p, p2});
		}
	}

	auto lines2 = lines;
	for(auto it = lines.cbegin(); it != lines.cend(); ++it)
	{
		auto& l1 = *it;
		for(auto l2 : lines2)
		{
			if(all(l1.a == l2.a) || all(l1.b == l2.b) || all(l1.b == l2.a) || all(l1.a == l2.b)) 
			{
				continue;
			}

			if(intersects(l1, l2))
			{
				it = lines.erase(it);
				--it;
				break;
			}
		}
	}

	return createConvex(lines);
}

///\relates simplexRegion
///Creates a convex shape(simplexRegion) from its outlining lines.
template<std::size_t D, typename P, std::size_t A = D>
simplexRegion<D, P, A> createConvex(std::vector<line<D, P>> lines)
{
	simplexRegion<D, P, A> ret;

	//newly created lines, inserted AFTER simplex is completly found
	for(std::size_t i(0); i < lines.size() && lines.size() > A + 1;)
	{
		std::vector<line<D, P>> newLines {};
		auto line = lines[i];
		lines.erase(lines.cbegin() + i);

		simplex<D, P, A> simp;
		simp.points()[0] = line.a;
		simp.points()[1] = line.b;
		
		std::size_t idx = 1;

		//lines at line.a (center point)
		for(std::size_t i2(0); i2 < lines.size(); ++i2)
		{
			auto& line2 = lines[i2];

			if(all(line2.a == line.a))
			{
				simp.points()[++idx] = line2.b;
				lines.erase(lines.cbegin() + i2);
				newLines.push_back({simp.points()[idx - 1], simp.points()[idx]});
			}
			else if(all(line2.b == line.a))
			{
				simp.points()[++idx] = line2.a;
				lines.erase(lines.cbegin() + i2);
				newLines.push_back({simp.points()[idx - 1], simp.points()[idx]});
			}
		
			if(idx == A) break; //all found
		}

		assert(idx == A); //error here! could not construct simplex

		//find lines on the opposite site of line.a
		//they belong to the simplex, but do not have point line.a
		for(std::size_t i2(0); i2 < lines.size(); ++i2)
		{
			auto& line2 = lines[i2];
			bool foundA = 0, foundB = 0;

			for(auto& p : simp.points())
			{
				if(all(p == line2.a)) foundA = 1;
				if(all(p == line2.b)) foundB = 1;

				if(foundA && foundB) break;
			}

			if(foundA && foundB)
			{
				lines.erase(lines.cbegin() + i2);
			}
		}

		std::cout << simp << "\n";
		ret.addNoCheck(simp);
		lines.insert(lines.cend(), newLines.cbegin(), newLines.cend());
	}


	//code dupilcation here -- argh -> search for better loop above
	if(lines.size() == A + 1) //should be always true?
	{
		simplex<D, P, A> simp;
		std::size_t idx = 0;

		for(std::size_t i(0); i < lines.size(); ++i)
		{
			auto& line2 = lines[i];
			bool foundA = 0, foundB = 0;

			for(std::size_t i2(0); i2 < idx; ++i2)
			{
				auto& p = simp.points()[i2];
				if(all(p == line2.a)) foundA = 1;
				if(all(p == line2.b)) foundB = 1;

				if(foundA && foundB) break;
			}

			if(!foundA)
			{
				simp.points()[idx++] = line2.a;
			}
			if(!foundB)
			{
				simp.points()[idx++] = line2.b;
			}
		}
	
		ret.addNoCheck(simp);
	}

	return ret;
}


///tests
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
	contains(const simplexRegion<D, P, A>& r, const Vec<D, P>& v)
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
