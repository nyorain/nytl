#pragma once

#include <nytl/mat.hpp>
#include <nytl/vec.hpp>

#include <climits>

namespace nytl
{

///Represents the solution a linear equotation system can have.
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

///Represents a linear equotation with V variables and a result that have a precision of P.
template<std::size_t V, typename P>
class linearEquotation
{
public:
	vec<V, P> vars;
	P result;

	linearEquotation& operator=(const vec<V + 1, P>& values)
		{ vars = values; result = values.back(); return *this; }
};

///Represnts a linear equotation system with E equtations and V variables that have P precision.
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
