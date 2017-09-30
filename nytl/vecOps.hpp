// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Various operations for real vectors.
/// Over the whole files, template types names V or V* must fulfil the
/// vector concept specified in doc/vec.md

#pragma once

#ifndef NYTL_INCLUDE_VEC_OPS
#define NYTL_INCLUDE_VEC_OPS

#include <nytl/vec.hpp>
#include <nytl/tmpUtil.hpp> // nytl::templatize
#include <nytl/math.hpp> // nytl::accumulate

#include <functional> // std::plus, std::multiplies
#include <stdexcept> // std::invalid_argument
#include <cmath> // std::acos
#include <iosfwd> // std::ostream

namespace nytl {

/// \brief Sums up all values of the given vector using the + operator.
template<size_t D, typename T>
constexpr auto sum(const Vec<D, T>& a)
{
	decltype(a[0] + a[1]) ret {0};
	for(auto& val : a)
		ret += val;
	return ret;
}

/// \brief Multiplies all values of the given vector using the * operator.
template<size_t D, typename T>
constexpr auto multiply(const Vec<D, T>& a)
{
	decltype(a[0] * a[1]) ret {1};
	for(auto& val : a)
		ret *= val;
	return ret;
}

/// \brief Calculates the default (real) dot product for the given vectors.
/// Note that this follows the dot definition for real numbers and does
/// not automatically handle the dot definition for other structures.
template<size_t D, typename T1, typename T2>
constexpr auto dot(const Vec<D, T1>& a, const Vec<D, T2>& b)
{
	decltype(a[0] * b[0] + a[0] * b[0]) ret {0};
	for(auto i = 0u; i < D; ++i)
		ret += a[i] * b[i];

	return ret;
}

/// \brief Returns the euclidean norm (or length) of the given vector.
template<size_t D, typename T>
constexpr auto length(const Vec<D, T>& a)
{
	return std::sqrt(dot(a, a));
}

/// \brief Returns the euclidean distance between two vectors.
/// Another way to describe this operation is the length between the
/// difference of the given vectors.
template<size_t D, typename T1, typename T2>
constexpr auto distance(const Vec<D, T1>& a, const Vec<D, T2>& b)
{
	return length(a - b);
}

/// \brief Calculates the angle in radians between two vectors using the dot product.
/// Therefore it will always return the smaller between the both vectors on a
/// plane in which both vectors lay.
/// For two equal vectors, it will return always 0.0.
/// Does only work for real numbers and does not handle complex vectors.
/// \throws std::domain_error if at least one of the given vectors has a length of 0.
template<size_t D, typename T1, typename T2>
constexpr auto angle(const Vec<D, T1>& a, const Vec<D, T2>& b)
{
	auto l = length(a) * length(b);
	if(l == 0) {
		throw std::domain_error("nytl::angle: nullvector given");
	}

	// we do the clamp to work against rounding errors
	// (dot(a, b) / l) cannot return anything out of range [-1, 1]
	auto v = dot(a, b) / l;
	return std::acos(std::clamp<decltype(v)>(v, -1.0, 1.0));
}

/// \brief Calculates the cross product for two 3-dimensional vectors.
template<typename T1, typename T2>
constexpr auto cross(const Vec<3, T1>& a, const Vec<3, T2>& b)
{
	Vec<3, decltype(a[1] * b[2] - a[2] * b[1])> ret {};
	ret[0] = (a[1] * b[2]) - (a[2] * b[1]);
	ret[1] = (a[2] * b[0]) - (a[0] * b[2]);
	ret[2] = (a[0] * b[1]) - (a[1] * b[0]);
	return ret;
}

/// \brief Returns a normalization of the given vector for the euclidean norm.
/// \throws std::domain_error if the vector has the length 0.
template<size_t D, typename T>
constexpr auto normalized(const Vec<D, T>& a)
{
	auto l = length(a);
	if(l == T {0.0}) {		
		throw std::domain_error("nytl::normalized: nullvector given");
	}

	return (T {1.0} / l) * a;
}

/// \brief Normalizes the given vector in place. Note that this may
/// not work out as expected if its value type does not have the needed
/// precision (e.g. for an int vector).
/// \throws std::domain_error if the vector has the length 0.
template<size_t D, typename T>
constexpr auto normalize(Vec<D, T>& a) 
{
	auto l = length(a);
	if(l == T {0.0}) {		
		throw std::domain_error("nytl::normalized: nullvector given");
	}

	a *= T {1.0} / l;
}

/// \brief Prints the given vector to the given ostream.
/// If this function is used, header <ostream> must be included.
/// This function does not implement operator<< since this operator should only implemented
/// for the Vector implementation types.
/// \requires There must be an implementation of operator<<(std::ostream&, V::Value).
template<typename V>
std::ostream& print(std::ostream& os, const V& vec, 
	const char* start = "(", const char* end = ")", const char* sep = ", ")
{
	auto& tos = templatize<V>(os); // we don't want to include ostream
	tos << start;

	auto it = vec.begin();
	tos << *it;
	while(++it != vec.end())
		tos << sep << *it;

	tos << end;
	return os;
}

template<size_t D, typename T>
std::ostream& operator<<(std::ostream& os, const Vec<D, T>& a)
{
	return print(os, a);
}

// additional utility operators
namespace vec {
namespace operators {
	
template<size_t D, typename F, typename T>
constexpr auto operator*(const Vec<D, T>& a, const F& f)
{
	auto ret = Vec<D, decltype(a[0] * f)> {};
	for(auto i = 0u; i < D; ++i)
		ret[i] = a[i] * f;
	return ret;
}

template<size_t D, typename F, typename T>
constexpr auto operator/(const Vec<D, T>& a, const F& f)
{
	auto ret = Vec<D, decltype(a[0] / f)> {};
	for(auto i = 0u; i < D; ++i)
		ret[i] = a[i] / f;
	return ret;
}

template<size_t D, typename F, typename T>
constexpr auto operator/(const F& f, const Vec<D, T>& a)
{
	auto ret = Vec<D, decltype(f / a[0])> {};
	for(auto i = 0u; i < D; ++i)
		ret[i] = f / a[i];
	return ret;
}

} // namespace operators

namespace cw { // vec component-wise operations

/// \brief Returns a vector holding the component-wise maximum of the given vectors.
template<size_t D, typename T>
constexpr auto max(Vec<D, T> a, const Vec<D, T>& b)
{
	for(auto i = 0u; i < D; ++i)
		if(b[i] > a[i])
			a[i] = b[i];
	return a;
}

/// \brief Returns a vector holding the component-wise minimum of the given vectors.
template<size_t D, typename T>
constexpr auto min(Vec<D, T> a, const Vec<D, T>& b)
{
	for(auto i = 0u; i < D; ++i)
		if(b[i] < a[i])
			a[i] = b[i];
	return a;
}

/// \brief Returns the component-wise product of the given vectors.
template<size_t D, typename T1, typename T2>
constexpr auto multiply(const Vec<D, T1>& a, const Vec<D, T2>& b)
{
	Vec<D, decltype(a[0] * b[0])> ret {};
	for(auto i = 0u; i < D; ++i)
		ret[i] = a[i] * b[i];
	return ret;
}

/// \brief Returns the component-wise quotient of the given vectors.
template<size_t D, typename T1, typename T2>
constexpr auto divide(const Vec<D, T1>& a, const Vec<D, T2>& b)
{
	Vec<D, decltype(a[0] / b[0])> ret {};
	for(auto i = 0u; i < D; ++i)
		ret[i] = a[i] / b[i];
	return ret;
}

namespace operators {

template<size_t D, typename T1, typename T2>
constexpr Vec<D, T1>& operator*=(Vec<D, T1>& a, const Vec<D, T2>& b) noexcept
{
	for(size_t i = 0; i < D; ++i)
		a[i] *= b[i];
	return a;
}

template<size_t D, typename T1, typename T2>
constexpr Vec<D, T1>& operator/=(Vec<D, T1>& a, const Vec<D, T2>& b) noexcept
{
	for(size_t i = 0; i < D; ++i)
		a[i] /= b[i];
	return a;
}

template<size_t D, typename T1, typename T2>
constexpr auto operator*(const Vec<D, T1>& a, const Vec<D, T2>& b)
{
	return multiply(a, b);
}

template<size_t D, typename T1, typename T2>
constexpr auto operator/(const Vec<D, T1>& a, const Vec<D, T2>& b)
{
	return divide(a, b);
}

} // namespace operators

namespace ip { // inplace operations

// Various utility functions.
// Always modify the given vector in place and simply apply the similar
// named stl or nytl function on all components.
#define NYTL_VEC_IP_UTIL_FUNC(func) \
	template<size_t D, typename T> \
	constexpr void func(Vec<D, T>& vec) { \
		for(auto& val : vec) \
			val = std::func(val); \
	}

NYTL_VEC_IP_UTIL_FUNC(abs)
NYTL_VEC_IP_UTIL_FUNC(sin)
NYTL_VEC_IP_UTIL_FUNC(cos)
NYTL_VEC_IP_UTIL_FUNC(tan)
NYTL_VEC_IP_UTIL_FUNC(asin)
NYTL_VEC_IP_UTIL_FUNC(acos)
NYTL_VEC_IP_UTIL_FUNC(atan)
NYTL_VEC_IP_UTIL_FUNC(sqrt)
NYTL_VEC_IP_UTIL_FUNC(log)
NYTL_VEC_IP_UTIL_FUNC(exp)
NYTL_VEC_IP_UTIL_FUNC(exp2)
NYTL_VEC_IP_UTIL_FUNC(floor)
NYTL_VEC_IP_UTIL_FUNC(ceil)

#undef NYTL_VEC_IP_UTIL

template<size_t D, typename T1, typename T2>
constexpr void pow(Vec<D, T1>& a, T2 exp)
{
	for(auto& val : a)
		val = std::pow(val, exp);
}


} // namespace ip

#define NYTL_VEC_UTIL_FUNC(func) \
	template<size_t D, typename T> \
	constexpr void func(Vec<D, T> vec) { \
		ip::func(vec); \
		return vec; \
	}

NYTL_VEC_UTIL_FUNC(abs)
NYTL_VEC_UTIL_FUNC(sin)
NYTL_VEC_UTIL_FUNC(cos)
NYTL_VEC_UTIL_FUNC(tan)
NYTL_VEC_UTIL_FUNC(asin)
NYTL_VEC_UTIL_FUNC(acos)
NYTL_VEC_UTIL_FUNC(atan)
NYTL_VEC_UTIL_FUNC(sqrt)
NYTL_VEC_UTIL_FUNC(log)
NYTL_VEC_UTIL_FUNC(exp)
NYTL_VEC_UTIL_FUNC(exp2)
NYTL_VEC_UTIL_FUNC(floor)
NYTL_VEC_UTIL_FUNC(ceil)

#undef NYTL_VEC_UTIL_FUNC

template<size_t D, typename T1, typename T2>
constexpr void pow(Vec<D, T1> a, T2 exp)
{
	ip::pow(a, exp);
	return a;
}

} // namespace cw
} // namespace vec
} // namespace nytl

#undef nytl_assure
#endif // header guard
