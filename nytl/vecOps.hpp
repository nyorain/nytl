// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Various operations for real vectors.
/// Over the whole files, template types names V or V* must fulfil the
/// vector concept specified in doc/vec.md

#pragma once

#ifndef NYTL_INCLUDE_VEC_OPS
#define NYTL_INCLUDE_VEC_OPS

#include <nytl/tmpUtil.hpp> // nytl::templatize
#include <nytl/scalar.hpp> // nytl::accumulate

#include <functional> // std::plus, std::multiplies
#include <stdexcept> // std::invalid_argument
#include <cmath> // std::acos
#include <iosfwd> // std::ostream

namespace nytl::vec {
namespace detail {

/// \brief Helper that asserts that the given vectors have the same dimension.
template<typename V1, typename V2>
struct AssertSameDimensions {

	static constexpr void call(const V1& a, const V2& b)
	{
		if constexpr(V1::staticSized && V2::staticSized) {
			static_assert(V1::size() == V2::size(), "nytl::vec: vectors must have same dimension");
		} else {
			if(a.size() != b.size())
				throw std::invalid_argument("nytl::vec: vectors must have same dimension");
		}
	}
};

/// \brief Helper that asserts that a vector of type V1 has dimension Dim.
template<unsigned int Dim, typename V>
struct AssertDimension {
	static constexpr void call(const V& a)
	{
		if constexpr(V::staticSized) {
			static_assert(V::size() == Dim, "nytl::vec: vector must have specified dimension");
		} else {
			if(a.size() != Dim)
				throw std::invalid_argument("nytl::vec: vector must have specified dimension");
		}
	}
};

/// \brief Asserts that the both given vectors have the same dimension.
/// Will result in a compile time error if possible, otherwise throws
/// std::invalid_argument.
template<typename V1, typename V2>
constexpr void assertSameDimensions(const V1& a, const V2& b)
{
	AssertSameDimensions<V1, V2>::call(a, b);
}

/// \brief Asserts that the given vector has dimension Dim.
/// Will result in a compile time error if possible, otherwise throws
/// std::invalid_argument.
template<unsigned int Dim, typename V>
constexpr void assertDimension(const V& a)
{
	AssertDimension<Dim, V>::call(a);
}

/// \brief Creates a new vector from implementation 'V' with value type 'T'
/// and size 'S'
template<typename V, typename T, std::size_t S>
auto createVector()
{
	if constexpr(V::staticSized) return V::template Rebind<T>::template create<S>();
	else return V::template Rebind<T>::create(S);

}

/// \brief Creates a new vector from the same implementation and size as the given vector
/// with value type T.
template<typename T, typename V>
auto createVector(const V& v)
{
	if constexpr(V::staticSized) return V::template Rebind<T>::template create<V::size()>();
	else return V::template Rebind<T>::create(v.size());
}

} // namespace detail

/// Vec operations without argument checking
/// Useful for extra performance gains when things like
/// vector dimension are known.
namespace nocheck {

/// \brief Like dot, but no sanity checks are performed.
template<typename V1, typename V2>
constexpr auto dot(const V1& a, const V2& b)
{
	using RetType = decltype(a[0] * b[0] + a[0] * b[0]);
	auto ret = RetType {};
	for(auto i = 0u; i < a.size(); ++i)
		ret += a[i] * b[i];

	return ret;
}

/// \brief Like angle, but no sanity checks are performed.
/// There might be unexpected results e.g. when both vectors are
/// equal due to rounding errors.
template<typename V1, typename V2>
constexpr auto angle(const V1& a, const V2& b)
{
	return std::acos(nocheck::dot(a, b) / (length(a) * length(b)));
}

/// \brief Like cross, but no sanity checks are performed.
template<typename V1, typename V2>
constexpr auto cross(const V1& a, const V2& b)
{
	auto ret = detail::createVector<V1, decltype(a[0] * b[0] - a[0] * b[0]), 3>();
	ret[0] = (a[1] * b[2]) - (a[2] * b[1]);
	ret[1] = (a[2] * b[0]) - (a[0] * b[2]);
	ret[2] = (a[0] * b[1]) - (a[1] * b[0]);
	return ret;
}

/// \brief Like normalize, but no sanity checks are performed.
template<typename V>
constexpr auto normalize(const V& a)
{
	return (typename V::Value {1} / length(a)) * a;
}

} // namespace nocheck

/// \brief Sums up all values of the given vector using the + operator.
template<typename V>
constexpr auto sum(const V& a)
{
	return accumulate(a.begin(), a.end(), typename V::Value{0}, std::plus<>());
}

/// \brief Multiplies all values of the given vector using the * operator.
template<typename V>
constexpr auto multiply(const V& a)
{
	return accumulate(a.begin(), a.end(), typename V::Value {1}, std::multiplies<>());
}

/// \brief Calculates the default dot product for the given vectors.
/// Note that this follows the dot definition for real numbers and does
/// not automatically handle the dot definition for complex numbers.
/// \throws std::invalid_argument if the size of the input vectors differs.
template<typename V1, typename V2>
constexpr auto dot(const V1& a, const V2& b)
{
	detail::assertSameDimensions(a, b);
	return nocheck::dot(a, b);
}

/// \brief Returns the euclidean norm (or length) of the given vector.
template<typename V>
constexpr auto length(const V& a)
{
	return std::sqrt(nocheck::dot(a, a));
}

/// \brief Returns the euclidean distance between two vectors.
/// Another way to describe this operation is the length between the
/// difference of the given vectors.
/// \requires The both given vectors shall have the same dimension.
/// Will not check for this and simply subtract them from each other.
template<typename V1, typename V2>
constexpr auto distance(const V1& a, const V2& b)
{
	return length(a - b);
}

/// \brief Calculates the angle in radians between two vectors using the dot product.
/// Therefore it will always return the smaller between the both vectors on a
/// plane in which both vectors lay.
/// For two equal vectors, it will return always 0.0.
/// Does only work for real numbers and does not handle complex vectors.
/// \throws std::invalid_argument if the size of the input vectors differs.
/// \throws std::domain_error if at least one of the given vectors has a length of 0.
template<typename V1, typename V2>
constexpr auto angle(const V1& a, const V2& b)
{
	detail::assertSameDimensions(a, b);

	auto la = length(a);
	auto lb = length(b);
	if(la == 0.0 || lb == 0.0)
		throw std::domain_error("nytl::vec::angle: both vectors are null");

	// We do this check here to output 0 for angle(a, a).
	// This might produce nan somtimes due to rounding errors.
	auto div = nocheck::dot(a, b) / (la * lb);
	if(div > 1.0)
		return 0.0;

	return std::acos(div);
}

/// \brief Calculates the cross product for two 3-dimensional vectors.
/// \requires The given vectors shall be in the 3-dimensional space.
/// \throws std::domain_error if at least on of the input vectors does not have a size of 3.
template<typename V1, typename V2>
constexpr auto cross(const V1& a, const V2& b)
{
	detail::assertDimension<3>(a);
	detail::assertDimension<3>(b);

	return nocheck::cross(a, b);
}

/// \brief Returns a normalization of the given vector for the euclidean norm.
/// \throws std::domain_error if the vector has the length 0.
template<typename V>
constexpr auto normalize(const V& a)
{
	auto la = length(a);
	if(la == typename V::Value{0})
		throw std::domain_error("nytl::vec::normalize: vector has length 0");

	return (typename V::Value {1} / la) * a;
}

/// \brief Prints the given vector to the given ostream.
/// If this function is used, header <ostream> must be included.
/// This function does not implement operator<< since this operator should only implemented
/// for the Vector implementation types.
/// \requires There must be an implementation of operator<<(std::ostream&, V::Value).
template<typename V>
std::ostream& print(std::ostream& os, const V& vec)
{
	auto& tos = templatize<V>(os); // we don't want to include ostream
	tos << "(";

	auto it = vec.begin();
	tos << *it;
	while(++it != vec.end())
	tos << ", " << *it;

	tos << ")";
	return os;
}

namespace cw { // component-wise operations
namespace ip { // inplace operations

/// \brief Applies the given function to every value in the given vector.
template<typename V, typename F>
constexpr void apply(V& vec, F&& func)
{
	for(auto i = 0u; i < vec.size(); ++i)
		func(vec[i]);
}

// Various utility functions.
// Always modify the given vector in place and simply apply the similar
// named stl or nytl function on all components.
template<typename V>
constexpr void abs(V& vec)
	{ apply(vec, [](auto& x){ x = std::abs(x); }); }

template<typename V>
constexpr void degrees(V& vec)
	{ apply(vec, [](auto& x){ x = nytl::degrees(x); }); }

template<typename V>
constexpr void radians(V& vec)
	{ apply(vec, [](auto& x){ x = nytl::radians(x); }); }

template<typename V>
constexpr void sin(V& vec)
	{ apply(vec, [](auto& x){ x = std::sin(x); }); }

template<typename V>
constexpr void cos(V& vec)
	{ apply(vec, [](auto& x){ x = std::cos(x); }); }

template<typename V>
constexpr void tan(V& vec)
	{ apply(vec, [](auto& x){ x = std::cos(x); }); }

template<typename V>
constexpr void asin(V& vec)
	{ apply(vec, [](auto& x){ x = std::asin(x); }); }

template<typename V>
constexpr void acos(V& vec)
	{ apply(vec, [](auto& x){ x = std::acos(x); }); }

template<typename V>
constexpr void atan(V& vec)
	{ apply(vec, [](auto& x){ x = std::atan(x); }); }

template<typename V>
constexpr void exp(V& vec)
	{ apply(vec, [](auto& x){ x = std::exp(x); }); }

template<typename V>
constexpr void log(V& vec)
	{ apply(vec, [](auto& x){ x = std::log(x); }); }

template<typename V>
constexpr void sqrt(V& vec)
	{ apply(vec, [](auto& x){ x = std::sqrt(x); }); }

template<typename V>
constexpr void exp2(V& vec)
	{ apply(vec, [](auto& x){ x = std::exp2(x); }); }

template<typename V>
constexpr void floor(V& vec)
	{ apply(vec, [](auto& x){ x = std::floor(x); }); }

template<typename V>
constexpr void ceil(V& vec)
	{ apply(vec, [](auto& x){ x = std::ceil(x); }); }

template<typename V, typename T>
constexpr void pow(V& vec, const T& e)
	{ apply(vec, [&e](auto& x){ x = std::pow(x, e); }); }

} // namespace ip

/// \brief Returns a vector holding the component-wise maximum of the given Vectors.
/// \requires Type 'V' shall be a Vector type.
/// \requires The both given vectors shall have the same dimension.
template<typename V>
constexpr auto max(V a, const V& b)
{
	detail::assertSameDimensions(a, b);

	for(auto i = 0u; i < a.size(); ++i)
		if(b[i] > a[i])
			a[i] = b[i];
	return a;
}

/// \brief Returns a vector holding the component-wise maximum of the given Vectors.
/// \requires Type 'V' shall be a Vector type.
/// \requires The both given vectors shall have the same dimension.
template<typename V>
constexpr auto min(V a, const V& b)
{
	detail::assertSameDimensions(a, b);

	for(auto i = 0u; i < a.size(); ++i)
		if(b[i] < a[i])
			a[i] = b[i];
	return a;
}

/// \brief Multiplies the two vectors component wise
/// \requires Types 'V1', 'V2' shall be Vector types over the same space.
template<typename V1, typename V2>
constexpr auto multiply(const V1& a, const V2& b)
{
	detail::assertSameDimensions(a, b);

	auto ret = detail::createVector<decltype(a[0] * b[0])>(a);
	for(auto i = 0u; i < a.size(); ++i)
		ret[i] = a[i] * b[i];
	return ret;
}

/// \brief Component-wise divides the first vector by the second one.
/// Will not perform any zero checks.
/// \requires Types 'V1', 'V2' shall be Vector types over the same space.
template<typename V1, typename V2>
constexpr auto divide(const V1& a, const V2& b)
{
	detail::assertSameDimensions(a, b);

	auto ret = detail::createVector<decltype(a[0] / b[0])>(a);
	for(auto i = 0u; i < a.size(); ++i)
		ret[i] = a[i] / b[i];
	return ret;
}

} // namespace cw
} // namespace nytl::vec

#endif // header guard
