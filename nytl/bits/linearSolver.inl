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
	else if(unambigouosSolvable())
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

//linearEquotationSystem
template<std::size_t E, std::size_t V, typename P>
solutionSet<V> linearEquotationSystem<E, V, P>::solve() const
{
	auto res = rrefMatCopy(asMat());

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

