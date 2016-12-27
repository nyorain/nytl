// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_SIMPLEX_OPS
#define NYTL_INCLUDE_SIMPLEX_OPS

#include <nytl/simplex.hpp> // nytl::Simplex
#include <nytl/scalar.hpp> // nytl::accumulate
#include <nytl/mat.hpp> // nytl::Mat
#include <nytl/matOps.hpp> // nytl::mat::determinant
#include <iosfwd> // std::ostream

namespace nytl {

/// \brief Outputs all Simplex points to an ostream.
/// \module simplex
template<std::size_t D, typename P, std::size_t A>
std::ostream& operator<<(std::ostream& os, const Simplex<D, P, A>& s)
{
	for(auto& p : s.points()) os << p;
	return os;
}

/// \brief Checks two given simplices for equality.
/// They can only be equal if all of their points are equal.
/// \module simplex
template<std::size_t D, typename P, std::size_t A>
bool operator==(const Simplex<D, P, A>& a, const Simplex<D, P, A>& b)
{
	for(auto i = 0u; i < a.pointCount; ++i)
		if(a.points()[i] != b.points()[i]) return false;
	return true;
}

/// \brief Returns the center point of the given simplex.
/// \module simplex
template<std::size_t D, typename P, std::size_t A>
auto center(const Simplex<D, P, A>& a)
{
	using Point = typename Simplex<D, P, A>::Point;
	auto sum = accumulate(a.points(), a.points() + a.pointSize, std::plus<>(), Point {});
	return (1 / a.pointSize) * sum;
}

template<std::size_t D, typename P, std::size_t A>
auto size(const Simplex<D, P, A>& a)
{
	Mat<D, P> m;
	for(auto i = 0u; i < A + 1; ++i) mat::col(m, i) = points()[i] - points()[0];
	return std::abs(det(m)) / fac(D);
}

/// \brief Checks two given simplices for inequality.
/// They are inequal as soon as one of their points is not equal.
/// \module simplex
template<std::size_t D, typename P, std::size_t A>
bool operator!=(const Simplex<D, P, A>& a, const Simplex<D, P, A>& b)
{
	return !(a == b);
}

} // namespace nytl

#endif // header guard
