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

///\file
///\brief Template classes and functions for solving linear equotation system.

#pragma once

#include <nytl/mat.hpp>
#include <nytl/vec.hpp>

#include <climits>

namespace nytl
{

///\brief Represents the solutions a linear equotation system can have.
///\ingroup math
template<std::size_t N>
class solutionSet
{
public:
	struct expression
	{
		double constPart;
		vec<N, double> variablePart;
	};

public:
	int solvable_ = -1;
	vec<N, expression> solution_;

public:
	solutionSet() = default;
	solutionSet(std::size_t numVars, const vec<N, expression>& sol) 
		: solvable_(numVars), solution_(sol) {} 

	expression& operator[](std::size_t i){ return solution_[i]; }
	const expression& operator[](std::size_t i) const { return solution_[i]; }

	bool solvable() const { return (solvable_ != -1); }
	bool unambigouosSolvable() const { return (solvable_ == 0); }
	std::size_t numberVariables() const { return (solvable()) ? solvable_ : 0; }

	template<std::size_t D, typename P>
	vec<N, double> solution(const vec<D, P>& variables) const;
	vec<N, double> solution() const;
};

///\brief A linear equtations with variable coefficients and a result.
///\ingroup math
///\details Represents a linear equotation with V variables and a result that have a precision of P.
template<std::size_t V, typename P>
class linearEquotation
{
public:
	vec<V, P> vars;
	P result;

	linearEquotation& operator=(const vec<V + 1, P>& values)
		{ vars = values; result = values.back(); return *this; }
};

///\brief A set on linear equotations that may be solved.
///\ingroup math
///\details Represents a linear equotation system with E equtations and V variables that have P 
///precision.
template<std::size_t E, std::size_t V, typename P>
class linearEquotationSystem
{
public:
	using equotation_type = linearEquotation<V, P>;
	using solution_type = solutionSet<V>;
	using mat_type = mat<E, V + 1, P>;

public:
	vec<E, equotation_type> equotations_;

public:
	linearEquotationSystem() = default;
	linearEquotationSystem(const mat<E, V +1, P>& m) 
		: equotations_(*reinterpret_cast<const vec<E, equotation_type>*>(&m)) {}

	equotation_type& operator[](std::size_t i){ return equotations_[i]; }
	const equotation_type& operator[](std::size_t i) const { return equotations_[i]; }

	mat_type& asMat() { return *reinterpret_cast<mat_type*>(&equotations_); }
	const mat_type& asMat() const { return *reinterpret_cast<const mat_type*>(&equotations_); }

	solution_type solve() const;
};

#include <nytl/bits/linearSolver.inl>

}
