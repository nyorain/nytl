#include <nytl/nytl.hpp>
#include <nytl/triangle.hpp>
#include <nytl/line.hpp>
#include <iostream>
#include <fstream>
#include <cassert>
#include <set>
using namespace nytl;

/*
using vec1d = vec<1, double>;

template<std::size_t D, typename P, std::size_t A = D>
simplexRegion<D, P, A> convexFromPoints(const std::vector<vec<D, P>>& points)
{
}

template<std::size_t D, typename P, std::size_t A>
simplexRegion<D, double, A> intersection2(const simplex<D, P, A>& sa, const simplex<D, P, A>& sb)
{
	//general equotation system that is fulfilled by all points in the intersectio area	
	mat<D + 2, (A + 1) * 2 + 1, P> eqs;
	for(std::size_t i(0); i < A + 1; ++i)
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

	std::cout << eqs << std::endl;

	//les
	auto les = linearEquotationSystem<D + 2, (A + 1) * 2, P>(eqs);
	auto solution = les.solve();

	//analzye solutionSet
	if(!solution.solvable()) //no intersection
	{
		return {};
	}
	else if(solution.unambigouoslySolvable()) //baiscally just a point
	{
		//return cartesian(sa, static_cast<vec<D, P>>(solution.solution()));
		return {};
	}
	else //area
	{
		//assert solution.numberVariables() == A?
		
		domainedSolutionSet<(A + 1) * 2> dss;
		dss.solutionSet_ = solution;
		dss.domains_.fill(linearDomain{0, 1});
		dss.bake();
	

		//only allows ~32(maybe 64 or more) dimensions cause of this

		std::vector<vec<D, double>> cartesianPoints;
		for(auto& comb : sequences)
		{
			auto sol = dss.solution(dynVeci{0, 1}, comb, 0);
			auto cart = cartesian(sa, vec<A + 1, double>(solution.solution(sol)));
			cartesianPoints.push_back(cart);
			//std::cout << "{0, 1}, " << comb << " --> " << sol << " --> " << cart << "\n";

			sol = dss.solution(dynVeci{1, 0}, comb, 0);
			cart = cartesian(sa, vec<A + 1, double>(solution.solution(sol)));
			cartesianPoints.push_back(cart);
			//std::cout << "{1, 0}, " << comb << " --> " << sol << " --> " << cart << "\n";
		}

		//erase doubled points
		std::sort(cartesianPoints.begin(), cartesianPoints.end(), 
				[](const vec<D, double>& a, const vec<D, double>& b){ return all(a < b); });
		auto newEnd = std::unique(cartesianPoints.begin(), cartesianPoints.end(), 
				[](vec<D, double> a, vec<D, double> b) -> bool { return all(a == b); });
		cartesianPoints.erase(newEnd, cartesianPoints.cend());


		return convexFromPoints<D, double, A>(cartesianPoints);
	}
}
*/
int main()
{
	//tetrahedron<3, float> tr1{{50.f, 0.f, 0.f}, {0.f, 100.f, 0.f}, {100.f, 100.f, 0.f}, {50.f, 50.f, 50.f}};
	
	triangle2f tr1{{50.f, 0.f}, {0.f, 100.f}, {100.f, 100.f}};
	triangle2f tr2{{0.f, 50.f}, {100.f, 50.f}, {50.f, 150.f}};

	std::cout << "--: " << "hello" << std::endl;
	std::cout << "sa: " << &tr1 << std::endl;
	std::cout << "sb: " << &tr2 << "\n";

	auto intsec = nytl::intersection(tr1, tr2);
	std::cout << "\n\n" << dumpContainer(intsec.areas());

	//auto r = intersection(tr1, tr2);
	/*
	for(auto& s : r.areas())
	{
		std::cout << s.points() << "\n";
	}
	*/
}
