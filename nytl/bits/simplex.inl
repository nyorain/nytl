// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_SIMPLEX_INL
#define NYTL_INCLUDE_SIMPLEX_INL

#ifdef DOXYGEN
namespace nytl{
#endif

///\relates Simplex
///Outputs all Simplex points to an ostream.
template<std::size_t D, typename P, std::size_t A>
std::ostream& operator<<(std::ostream& os, const Simplex<D, P, A>& s)
{
	for(auto& p : s.points())
		os << p;

	return os;
}

//member
template<std::size_t D, typename P, std::size_t A>
double Simplex<D, P, A>::size() const
{
	SquareMat<D, P> m;
	for(std::size_t i(1); i <= A; ++i) m.col(i) = points()[i] - points()[0];
	return std::abs(det(m)) / fac(D);
}

template<std::size_t D, typename P, std::size_t A>
Vec<D, P> Simplex<D, P, A>::center() const
{
	return (sum(points()) / points().size());
}

template<std::size_t D, typename P, std::size_t A>
template<std::size_t OD, typename OP>
Simplex<D, P, A>::operator Simplex<OD, OP, A>() const
{
	return Simplex<OD, OP, A>{points()};
}

//equality
///\relates Simplex
template<std::size_t D, typename P, std::size_t A>
	bool operator==(const Simplex<D, P, A>& a, const Simplex<D, P, A>& b)
{
	for(std::size_t i(0); i < A + 1; ++i)
		if(!all(a.points()[i] == b.points()[i])) return 0;

	return 1;
}

#ifdef DOXYGEN
} //nytl
#endif

#endif //header guard
