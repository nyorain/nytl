#include <nytl/nytl.hpp>
#include <nytl/triangle.hpp>
#include <nytl/line.hpp>
#include <iostream>
#include <fstream>
#include <cassert>
#include <set>
using namespace nytl;

using vec1d = vec<1, double>;

//
class linearDomain
{
public:
	double minimum;
	double maximum;

	void newMin(double nmin){ minimum = std::max(nmin, minimum); }
	void newMax(double nmax){ maximum = std::min(nmax, maximum); }
};

template<std::size_t N>
class domainedSolutionSet
{
public:
	using expressionType = typename solutionSet<N>::expression;

public:
	solutionSet<N> solutionSet_;
	vec<N, linearDomain> domains_;
	std::vector<vec2<std::vector<expressionType>>> dependentDomains_;

public:
	void bake();
	dynVecd solution(const dynVecui& seq, const dynVecb& minmax) const;
};

template<std::size_t N>
void domainedSolutionSet<N>::bake()
{
	dependentDomains_.resize(solutionSet_.numberVariables());

	for(std::size_t i(0); i < N; ++i)
	{
		auto& expr = solutionSet_.solution_[i];
		if(all(expr.variablePart == 0)) //const
		{
			if(domains_[i].minimum > expr.constPart || domains_[i].maximum < expr.constPart)
			{
				//does not match domain (no variable part -> NEVER matches domain), throw
				throw std::invalid_argument("domainedSolutionSet::solution");
			}
		}

		//variablePart
		for(std::size_t v(0); v < solutionSet_.numberVariables(); ++v)
		{
			if(expr.variablePart[v] == 0) continue;

			auto exp = expressionType{};

			exp.constPart = (domains_[i].minimum - expr.constPart) / expr.variablePart[v];
			exp.variablePart = -expr.variablePart / expr.variablePart[v];
			exp.variablePart[v] = 0;
			dependentDomains_[v][(expr.variablePart[v] < 0)].push_back(exp); 

			exp.constPart = (domains_[i].maximum - expr.constPart) / expr.variablePart[v];
			exp.variablePart = -expr.variablePart / expr.variablePart[v];
			exp.variablePart[v] = 0;
			dependentDomains_[v][(expr.variablePart[v] > 0)].push_back(exp); 
		}
	}

	/*
	std::size_t imm(0);
	std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";

	for(auto& d : dependentDomains_)
	{
		for(auto& d2 : d[0])
		{
			std::cout << imm << " >= " << d2.constPart << " + " << d2.variablePart << "\n";
		}

		for(auto& d2 : d[1])
		{
			std::cout << imm << " <= " << d2.constPart << " + " << d2.variablePart << "\n";
		}
		imm++;
	}

	std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
	*/

	return;
}

template<std::size_t N>
dynVecd domainedSolutionSet<N>::solution(const dynVecui& seq, const dynVecb& minmax) const
{
	auto vars = dynVecd(solutionSet_.numberVariables());
	for(std::size_t i(0); i < solutionSet_.numberVariables(); ++i)
	{
		auto s = seq[i];
		auto m = minmax[s];
		vars[s] = double(m);

		for(auto& d : dependentDomains_[s][m])
		{
			//ignore(skip) dependentcomains with unresolved variables
			bool skip = 0;
			for(std::size_t vp(i); vp < solutionSet_.numberVariables(); ++vp)
			{
				if(d.variablePart[seq[vp]] != 0) skip = 1;
			}
			if(skip) continue;

			if(m)
			{
				auto n = d.constPart + sum(d.variablePart * vars);
				vars[s] = std::min(vars[s], n);
				std::cout << "min: " << s << " " << vars[s] << " " << n << "\n"; 
			}
			else
			{
				auto n = d.constPart + sum(d.variablePart * vars);
				vars[s] = std::max(vars[s], n);
				std::cout << "max: " << s << " " << vars[s] << " " << n << "\n"; 
			}
		}

		//check
		for(auto& d : dependentDomains_[s][!m])
		{
			//ignore(skip) dependentcomains with unresolved variables
			bool skip = 0;
			for(std::size_t vp(i); vp < solutionSet_.numberVariables(); ++vp)
			{
				if(d.variablePart[seq[vp]] != 0) skip = 1;
			}
			if(skip) continue;

			if(m)
			{
				auto n = d.constPart + sum(d.variablePart * vars);
				if(vars[s] < n)
				{
					throw std::invalid_argument("dependentSolutionSet::solution :not solveable");
				}
			}
			else
			{
				auto n = d.constPart + sum(d.variablePart * vars);
				if(vars[s] > n)
				{
					throw std::invalid_argument("dependentSolutionSet::solution :not solveable");
				}
			}
		}
	}

	return vars;
}

//
template<std::size_t D, typename P, std::size_t A>
vec<D, double> cartesian(const simplex<D, P, A>& sa, const vec<A + 1, double>& barycentric)
{
	auto ret = vec<D, double>{};
	for(std::size_t i(0); i < A + 1; ++i)
	{
		ret += barycentric[i] * sa.points()[i];
	}
	return ret;
}

template<std::size_t D, typename P, std::size_t A = D>
simplexRegion<D, P, A> convexFromPoints(const std::vector<vec<D, P>>& points)
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

	std::cout << lines.size() << "lines remaining\n";

	auto lines2 = lines;
	for(auto it = lines.cbegin(); it != lines.cend(); ++it)
	{
		auto& l1 = *it;
		for(auto l2 : lines2)
		{
			if(all(l1.a == l2.a) || all(l1.b == l2.b) || all(l1.b == l2.a) || all(l1.a == l2.b)) 
			{
				std::cout << "continue\n";
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

	std::cout << lines.size() << "lines remaining\n";

	simplexRegion<D, P, A> ret;
	for(std::size_t i(0); i < lines.size(); ++i)
	{
		auto line = lines[i];
		lines.erase(lines.cbegin() + i);

		simplex<D, P, A> simp;
		simp.points()[0] = line.a;
		simp.points()[1] = line.b;
		
		std::size_t idx = 1;
		for(auto i2(0); i2 < lines.size(); ++i2)
		{
			auto& line2 = lines[i2];

			if(all(line2.a == line.a))
			{
				simp.points()[++idx] = line2.b;
				lines.erase(lines.cbegin() + i2);
				lines.push_back({simp.points()[idx - 1], simp.points()[idx]});
			}
			else if(all(line2.b == line.a))
			{
				simp.points()[++idx] = line2.a;
				lines.erase(lines.cbegin() + i2);
				lines.push_back({simp.points()[idx - 1], simp.points()[idx]});
			}
		
			if(idx == A) break; //all found
		}

		assert(idx == A); //error here! could not construct simplex
		ret.addNoCheck(simp);
	}


	//code dupilcation here -- argh -> search for better loop above
	if(lines.size() == 3) //should be always true?
	{
		auto& line = lines[0];

		simplex<D, P, A> simp;
		simp.points()[0] = line.a;
		simp.points()[1] = line.b;

		std::size_t idx = 1;
		for(std::size_t i(1); i < lines.size(); ++i)
		{
			auto& line2 = lines[i];
			if(all(line2.a == line.a))
			{
				simp.points()[++idx] = line2.b;
				lines.erase(lines.cbegin() + i);
				lines.push_back({simp.points()[idx - 1], simp.points()[idx]});
			}
			else if(all(line2.b == line.a))
			{
				simp.points()[++idx] = line2.a;
				lines.erase(lines.cbegin() + i);
				lines.push_back({simp.points()[idx - 1], simp.points()[idx]});
			}
		}
	
		assert(idx == A); //error here! could not construct simplex
		ret.addNoCheck(simp);
	}

	return ret;
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
	
		std::vector<dynVecb> sequences(std::pow(2, solution.numberVariables()), 
				dynVecb(solution.numberVariables()));

		//only allows ~32(maybe 64 or more) dimensions cause of this
		for(std::size_t i(0); i < (1 << solution.numberVariables()); ++i)
		{
			for(std::size_t o(0); o < solution.numberVariables(); ++o)
			{
				sequences[i][o] = (i & (1 << o));
			}

			//std::cout << "seq: " << sequences[i] << "\n";
		}

		std::vector<vec<D, double>> cartesianPoints;
		for(auto& comb : sequences)
		{
			auto sol = dss.solution(dynVeci{0, 1}, comb);
			auto cart = cartesian(sa, solution.solution(sol));
			cartesianPoints.push_back(cart);
			//std::cout << "{0, 1}, " << comb << " --> " << sol << " --> " << cart << "\n";

			sol = dss.solution(dynVeci{1, 0}, comb);
			cart = cartesian(sa, solution.solution(sol));
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

int main()
{
	triangle2f tr1{{50.f, 0.f}, {0.f, 100.f}, {100.f, 100.f}};
	triangle2f tr2{{0.f, 50.f}, {100.f, 50.f}, {50.f, 150.f}};

	auto r = intersection2(tr1, tr2);

	for(auto& s : r.areas())
	{
		std::cout << s.points() << "\n";
	}
}
