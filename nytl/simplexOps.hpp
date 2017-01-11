// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Various simplex realted operations.

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
	auto sum = accumulate(a.points(), a.points() + a.pointCount, Point {}, std::plus<>());
	return (1.0 / a.pointCount) * sum;
}

/// \brief Returns the total size/volume of the given simplex.
/// \module simplex
template<std::size_t D, typename P, std::size_t A>
auto size(const Simplex<D, P, A>& a)
{
	Mat<D, A, P> m;
	for(auto i = 0u; i < A; ++i) mat::col(m, i, a.points()[i + 1] - a.points()[0]);
	return std::abs(mat::determinant(m)) / factorial(D);
}

/// \brief Returns the barycentric coordinates for the given global vector and simplex.
/// \throws std::invalid_argument for an invalid simplex or if the given point has no
/// barycentric coordinates for the given simplex
/// \module simplex
template<std::size_t D, typename P, std::size_t A>
auto barycentric(const Simplex<D, P, A>& simplex, const typename Simplex<D, P, A>::Point& point)
{
	Mat<D + 1, A + 1, double> eqs;
	for(auto c = 0u; c < A + 1; ++c) {
		mat::col(eqs, c, simplex.points()[c] - simplex.points()[A]);
		eqs[D][c] = 1.0;
	}

	// [auto l, u, p, s] = mat::luDecomp(eqs); // TODO: C++17
	auto lups = mat::luDecomp(eqs);
	const auto& l = std::get<0>(lups);
	const auto& u = std::get<1>(lups);
	const auto& p = std::get<2>(lups);

	nytl::Vec<D + 1, double> vec;
	auto diff = point - simplex.points()[A];

	for(auto i = 0u; i < diff.size(); ++i) vec[i] = diff[i];
	vec[D] = 1.0;

	return mat::luEvaluate(l, u, p * vec);
}

/// \brief Returns a matrix that converts to barycentric coordinates whem multiplied.
/// \details
/// \throws std::invalid_argument for an invalid simplex.
/// \module simplex
template<std::size_t D, typename P, std::size_t A>
auto barycentricMatrix(const Simplex<D, P, A>& simplex)
{
	Mat<D, A, double> eqs;
	for(auto c = 0u; c < A; ++c)
		mat::col(eqs, c, simplex.points()[c] - simplex.points()[A]);

	return mat::inverse(eqs);
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
