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
#include <nytl/dynVec.hpp>

#include <climits>
#include <algorithm>

namespace nytl
{

///Represents the solutions a linear equotation system can have.
///\ingroup math
template<std::size_t N>
class solutionSet
{
public:
	struct expression
	{
		double constPart;
		Vec<N, double> variablePart;
	};

public:
	int solvable_ = -1;
	Vec<N, expression> solution_;

public:
	solutionSet() = default;
	solutionSet(std::size_t numVars, const Vec<N, expression>& sol) 
		: solvable_(numVars), solution_(sol) {} 

	expression& operator[](std::size_t i){ return solution_[i]; }
	const expression& operator[](std::size_t i) const { return solution_[i]; }

	bool solvable() const { return (solvable_ != -1); }
	bool unambigouoslySolvable() const { return (solvable_ == 0); }
	std::size_t numberVariables() const { return (solvable()) ? solvable_ : 0; }

	template<std::size_t D, typename P>
	Vec<N, double> solution(const Vec<D, P>& variables) const;
	Vec<N, double> solution() const;
};

///Represents a linear domain that can is used for domainedSolutionSet components.
///\ingroup math
class linearDomain
{
public:
	double minimum;
	double maximum;
};

///Represents a linear solution set with specified domains for each set componnent.
///Useful for computing the range the variables can take for a given domain.
///\ingroup math
template<std::size_t N>
class domainedSolutionSet
{
public:
	using expressionType = typename solutionSet<N>::expression;

	struct dependentDomain
	{
		std::vector<expressionType> min {};
		std::vector<expressionType> max {};

		double constMin = 0;
		double constMax = 1;
	};

public:
	solutionSet<N> solutionSet_;
	Vec<N, linearDomain> domains_;
	mutable std::vector<dependentDomain> dependentDomains_; //cache

public:
	domainedSolutionSet() = default;

	///Constructs the object and bakes its internal data.
	domainedSolutionSet(const solutionSet<N>& sset, const Vec<N, linearDomain>& domains);

	///Contructs all variables with the same domain and bakes its internal data.
	domainedSolutionSet(const solutionSet<N>& sset, const linearDomain& domains);

	///Computes its internal representation of the solutionSets components.
	///\exception nytl::invalid_solution_set if the solutionSet has no solution at all.
	///\exception nytl::invalid_domained_solution_set if there is no solutions for the given
	///combination of solutionSet and domains.
	void bake() const;

	///Returns a solution for the given parameters.
	///\paran seq Specifies the order in which the variables should be chosen. Must have
	///as many components as the solutionSet has variables.
	///\param minmax Specifies for each solutionSet variable whether the minimum or the maximum 
	///of the variables range should be chosen. Must have as many components as the solutionSet
	///has variables.
	///\param bake Specifies whether the internal representation should be baked before computing
	///the solution. If multiple solutions are computing wihthout changing the solutionSet in
	///the domains in the meantime this parameter can be 0 (for optimization).
	///\return A solution whose components are in their given domains while still 
	///matching the solutionSet.
	///\exception nytl::invalid_Vector_size if seq or minmax have less components than
	///the objects solutionSet has variables.
	///\exception nytl::no_baked_data if there is no/invalid internal data representation to use.
	///Can occur if bake == 0 and the data has never been succesfully baked or changed since 
	///the last bake.
	///\warning If bake == 1 the bake() member function is called so all all exceptions from
	///this function might be thrown then.
	dynVecd solution(const dynVecui& seq, const dynVecb& minmax, bool bake = 1) const;

	unsigned int numberVariables() const { return solutionSet_.numberVariables(); }
};


///A linear equtations with variable coefficients and a result.
///Represents a linear equotation with V variables and a result that have a precision of P.
///\ingroup math
template<std::size_t V, typename P>
class linearEquotation
{
public:
	Vec<V, P> vars;
	P result;

	linearEquotation& operator=(const Vec<V + 1, P>& values)
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
	Vec<E, equotation_type> equotations_;

public:
	linearEquotationSystem() = default;
	linearEquotationSystem(const mat<E, V +1, P>& m) 
		: equotations_(*reinterpret_cast<const Vec<E, equotation_type>*>(&m)) {}

	equotation_type& operator[](std::size_t i){ return equotations_[i]; }
	const equotation_type& operator[](std::size_t i) const { return equotations_[i]; }

	mat_type& asMat() { return *reinterpret_cast<mat_type*>(&equotations_); }
	const mat_type& asMat() const { return *reinterpret_cast<const mat_type*>(&equotations_); }

	solution_type solve() const;
};

#include <nytl/bits/linearSolver.inl>

}
