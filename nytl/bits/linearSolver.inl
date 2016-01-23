/*
 * The MIT License (MIT)
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
namespace nytl {
#endif

///\relates mat solutionSet
///Interprets the given matrix as a linearEquotationSystem and solves it.
template<std::size_t R, std::size_t C, typename P>
solutionSet<C - 1> solve(const mat<R, C, P>& m)
{
	constexpr static auto V = C - 1; //number Variables
	constexpr static auto E = R; //number Equotations

	auto res = rrefMatCopy(m);
	vec<V, typename solutionSet<V>::expression> solution = {};

	vec<V, int> varNumbers; //Variable ID of the column
	varNumbers.fill(-1);

	std::size_t varCount = 0;
	int currVar = 0;

	for(std::size_t r(0); r < E; ++r)
	{
		currVar = -1;
		for(std::size_t c(0); c < V; ++c)
		{
			if(res[r][c] != 0)
			{
				if(currVar == -1)
				{
					currVar = c;
				}
				else
				{
					if(varNumbers[c] == -1) //not registered yet
					{
						varNumbers[c] = varCount++;
						solution[c].variablePart[varNumbers[c]] = 1;
					}

					solution[currVar].variablePart[varNumbers[c]] = -res[r][c];
				}			
			}
		}

		if(currVar == -1) //all vars == 0
		{
			if(res[r].back() != 0) //result != 0 -> not solvable
				return solutionSet<V>();
			else //just an empty row
				continue; 
		}

		solution[currVar].constPart = res[r].back();
	}

	return solutionSet<V>(varCount, solution);
}

//solutionSet
template<std::size_t N>
template<std::size_t D, typename P>
vec<N, double> solutionSet<N>::solution(const vec<D, P>& vars) const
{
	vec<N, double> ret;
	if(!solvable())
	{
		ret.fill(std::numeric_limits<double>::quiet_NaN());
	}
	else if(unambigouoslySolvable())
	{
		for(std::size_t i(0); i < N; ++i)
			ret[i] = solution_[i].constPart;		
	}
	else
	{
		for(std::size_t i(0); i < N; ++i)
			ret[i] = solution_[i].constPart + sum(solution_[i].variablePart * vars);		
	}

	return ret;
}

template<std::size_t N>
vec<N, double> solutionSet<N>::solution() const
{
	vec<N, double> ret;
	if(!solvable())
	{
		ret.fill(std::numeric_limits<double>::quiet_NaN());
	}
	else if(unambigouoslySolvable())
	{
		for(std::size_t i(0); i < N; ++i)
			ret[i] = solution_[i].constPart;		
	}
	else
	{
		//will convert to vec of needed size, full of 0's
		return solution(vec2i(0, 0));
	}
	return ret;
}

//domainedSolutionSet
template<std::size_t N>
domainedSolutionSet<N>::domainedSolutionSet(const solutionSet<N>& sset, 
		const vec<N, linearDomain>& domains) : solutionSet_(sset), domains_(domains)
{
	try
	{
		bake();
	}
	catch(const std::exception& err)
	{
		///\todo warning and/or rethrow here
	}
}

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
}

template<std::size_t N> dynVecd 
domainedSolutionSet<N>::solution(const dynVecui& seq, const dynVecb& minmax, bool pbake)
{
	if(pbake) bake();

	const auto* cpy = this;
	return cpy->solution(seq, minmax);
}

template<std::size_t N> dynVecd 
domainedSolutionSet<N>::solution(const dynVecui& seq, const dynVecb& minmax) const
{
	auto vars = dynVecd(solutionSet_.numberVariables());
	for(std::size_t i(0); i < solutionSet_.numberVariables(); ++i)
	{
		auto s = seq[i];
		auto m = minmax[s];
		vars[s] = double(m);

		for(auto& d : dependentDomains_[s][m])
		{
			//ignore(skip) dependentDomains_ with unresolved variables
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
			}
			else
			{
				auto n = d.constPart + sum(d.variablePart * vars);
				vars[s] = std::max(vars[s], n);
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

//linearEquotationSystem
template<std::size_t E, std::size_t V, typename P>
solutionSet<V> linearEquotationSystem<E, V, P>::solve() const
{
	return nytl::solve(asMat());
}

#ifdef DOXYGEN
}
#endif

