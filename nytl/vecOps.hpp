// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Various operations for real vectors.
/// Over the whole files, template types names V or V* must fulfil the
/// vector concept specified in doc/vec.md

#pragma once

#ifndef NYTL_INCLUDE_VEC_OPS
#define NYTL_INCLUDE_VEC_OPS

#include <nytl/assure.hpp> // nytl::assure
#include <nytl/tmpUtil.hpp> // nytl::templatize
#include <nytl/scalar.hpp> // nytl::accumulate

#include <functional> // std::plus, std::multiplies
#include <stdexcept> // std::invalid_argument
#include <cmath> // std::acos
#include <iosfwd> // std::ostream

namespace nytl::vec {
namespace detail {

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
template<typename R, typename T, typename V>
auto createVector(const V& v)
{
	if constexpr(V::staticSized) return R::template Rebind<T>::template create<V::size()>();
	else return R::template Rebind<T>::create(v.size());
}

} // namespace detail

/// \brief Sums up all values of the given vector using the + operator.
template<typename V>
constexpr auto sum(const V& a)
{
	decltype(a.get(0) + a.get(1)) ret = a.get(0);
	for(auto i = 1u; i < a.size(); ++i)
		ret += a.get(i);
	return ret;
}

/// \brief Multiplies all values of the given vector using the * operator.
template<typename V>
constexpr auto multiply(const V& a)
{
	decltype(a.get(0) * a.get(1)) ret = a.get(0);
	for(auto i = 1u; i < a.size(); ++i)
		ret *= a.get(i);
	return ret;
}

/// \brief Calculates the default dot product for the given vectors.
/// Note that this follows the dot definition for real numbers and does
/// not automatically handle the dot definition for complex numbers.
/// \expect Both vectors must have the same dimension
template<typename V1, typename V2>
constexpr auto dot(const V1& a, const V2& b)
{
	nytl_assure(a.staticSized && b.staticSized,
		a.size() == b.size(),
		"Invalid vector dimensions for dot operation");

	using RetType = decltype(a.get(0) * b.get(0) + a.get(0) * b.get(0));
	auto ret = RetType {};
	for(auto i = 0u; i < a.size(); ++i)
		ret += a.get(i) * b.get(i);

	return ret;
}

/// \brief Returns the euclidean norm (or length) of the given vector.
template<typename V>
constexpr auto length(const V& a)
{
	return std::sqrt(dot(a, a));
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
	nytl_assure(a.staticSized && b.staticSized,
		a.size() == b.size(),
		"Invalid vector dimensions for dot operation");

	auto la = length(a);
	auto lb = length(b);
	nytl_assure(false, la != 0 && lb != 0,
		"Invalid operation for nullvector");

	// We do this check here to output 0 for angle(a, a).
	// This might produce nan somtimes due to rounding errors.
	auto div = dot(a, b) / (la * lb);
	if(div > 1.0)
		return 0.0;

	return std::acos(div);
}

/// \brief Calculates the cross product for two 3-dimensional vectors.
/// \requires The given vectors shall be in the 3-dimensional space.
/// \throws std::domain_error if at least on of the input vectors does not have a size of 3.
template<typename V1, typename V2, typename R = V1>
constexpr auto cross(const V1& a, const V2& b)
{
	nytl_assure(a.staticSized && b.staticSized,
		a.size() == 3 && b.size() == 3,
		"Invalid vector dimensions for cross operation");

	auto ret = detail::createVector<R, decltype(a.get(0) * b.get(0) - a.get(0) * b.get(0)), 3>();
	ret.set(0, a.get(1) * b.get(2) - a.get(2) * b.get(1));
	ret.set(1, a.get(2) * b.get(0) - a.get(0) * b.get(2));
	ret.set(2, a.get(0) * b.get(1) - a.get(1) * b.get(0));
	return ret;
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

	tos << vec.get(0);
	for(auto i = 1u; i < vec.size(); ++i)
		tos << ", " << vec.get(i);

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
		func(vec.get(i));
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
	nytl_assure(a.staticSized && b.staticSized,
		a.size() == b.size(),
		"Vectors must have same dimension");

	for(auto i = 0u; i < a.size(); ++i)
		if(b.get(i) > a.get(i))
			a.set(i, b.get(i));
	return a;
}

/// \brief Returns a vector holding the component-wise maximum of the given Vectors.
/// \requires Type 'V' shall be a Vector type.
/// \requires The both given vectors shall have the same dimension.
template<typename V>
constexpr auto min(V a, const V& b)
{
	nytl_assure(a.staticSized && b.staticSized,
		a.size() == b.size(),
		"Vectors must have same dimension");

	for(auto i = 0u; i < a.size(); ++i)
		if(b.get(i) < a.get(i))
			a.set(i, b.get(i));
	return a;
}

/// \brief Multiplies the two vectors component wise
/// \requires Types 'V1', 'V2' shall be Vector types over the same space.
template<typename V1, typename V2, typename R = V1>
constexpr auto multiply(const V1& a, const V2& b)
{
	nytl_assure(a.staticSized && b.staticSized,
		a.size() == b.size(),
		"Vectors must have same dimension");

	auto ret = detail::createVector<R, decltype(a.get(0) * b.get(0))>(a);
	for(auto i = 0u; i < a.size(); ++i)
		ret.set(i, a.get(i) * b.get(i));
	return ret;
}

/// \brief Component-wise divides the first vector by the second one.
/// Will not perform any zero checks.
/// \requires Types 'V1', 'V2' shall be Vector types over the same space.
template<typename V1, typename V2, typename R = V1>
constexpr auto divide(const V1& a, const V2& b)
{
	nytl_assure(a.staticSized && b.staticSized,
		a.size() == b.size(),
		"Vectors must have same dimension");

	auto ret = detail::createVector<R, decltype(a.get(0) / b.get(0))>(a);
	for(auto i = 0u; i < a.size(); ++i)
		ret.set(i, a.get(i) / b.get(i));
	return ret;
}

} // namespace cw
} // namespace nytl::vec

#undef nytl_assure
#endif // header guard
