// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file A Vector implementation class.

#pragma once

#ifndef NYTL_INCLUDE_VEC
#define NYTL_INCLUDE_VEC

#include <nytl/fwd/vec.hpp> // nytl::Vec typedefs
#include <nytl/vecOps.hpp> // nytl::vec::print

#include <iterator> // std::reverse_iterator
#include <array> // std::array

namespace nytl {

/// \brief Basic Vector template class.
/// Implements the Vector concept from nytl/vecOps and there are all common vector
/// operations defined for it.
/// The default implementation uses stack storage and derives from std::array.
/// See the Vector concept and nytl/vecOps for more details.
/// \module vec
template<size_t D, typename T>
class Vec : public std::array<T, D> {
public:
	using Array = std::array<T, D>;
	using Size = typename Array::size_type;
	using Value = typename Array::value_type;
	using Reference = typename Array::reference;
	using ConstReference = typename Array::const_reference;

	/// Static sized vector implementation
	static constexpr auto staticSized = true;

	/// Rebinds the template to another dimension and precision.
	template<typename OT> using Rebind = Vec<D, OT>;

	/// Creates and returns a vector instance.
	/// Ignores the size since the type is static sized.
	template<Size S> static constexpr Vec<S, T> create() { return {}; }

	/// The (static/fixed) size of the type
	static constexpr size_t size() { return D; }

public:
	/// Explicitly casts the Vec to another Vec that may have
	/// a different precision or dimension. Will default construct
	/// any values that cannot be filled (e.g. vec3 -> vec4) or leave
	/// out the last values when the size of vector is shrinked (e.g.
	/// {1, 2, 3} -> {1, 2}).
	template<size_t OD, typename OT>
	constexpr explicit operator Vec<OD, OT>() const;
};

// Dynamic-sized specialization of Vec.
// See/include nytl/dyncVec.hpp
template<typename T> class Vec<0, T>;


// - implementation/operators -
template<size_t D, typename T>
template<size_t OD, typename OT>
constexpr Vec<D, T>::operator Vec<OD, OT>() const
{
	auto ret = Vec<OD, OT>{};
	if constexpr(!decltype(ret)::staticSized) ret.resize(this->size()); // dynamic size

	for(auto i = 0u; i < std::min(ret.size(), this->size()); ++i)
		ret[i] = (*this)[i];
	for(auto i = this->size(); i < ret.size(); ++i)
		ret[i] = {};
	return ret;
}

// - free operators -
template<size_t D, typename T, size_t OD, typename OT>
constexpr Vec<D, T>& operator+=(Vec<D, T>& a, const Vec<OD, OT>& b) noexcept
{
	vec::detail::assertSameDimensions(a, b);
	for(size_t i = 0; i < a.size(); ++i)
		a[i] += b[i];
	return a;
}

template<size_t D, typename T, size_t OD, typename OT>
constexpr Vec<D, T>& operator-=(Vec<D, T>& a, const Vec<OD, OT>& b) noexcept
{
	vec::detail::assertSameDimensions(a, b);
	for(size_t i = 0; i < a.size(); ++i)
		a[i] -= b[i];
	return a;
}

template<size_t D, typename T, typename OT>
constexpr Vec<D, T>& operator*=(Vec<D, T>& vec, OT fac)
{
	for(auto& val : vec)
		val *= fac;
	return vec;
}

template<size_t D, typename T, typename OT>
constexpr Vec<D, T>& operator/=(Vec<D, T>& vec, OT fac)
{
	for(auto& val : vec)
		val /= fac;
	return vec;
}

template<size_t D1, size_t D2, typename T1, typename T2>
constexpr auto operator+(const Vec<D1, T1>& a, const Vec<D2, T2>& b)
{
	vec::detail::assertSameDimensions(a, b);
	auto ret = Vec<std::min(D1, D2), decltype(a[0] + b[0])>{};
	if constexpr(!decltype(ret)::staticSized) ret.resize(a.size());

	for(auto i = 0u; i < ret.size(); ++i)
		ret[i] = a[i] + b[i];
	return ret;
}

template<size_t D1, size_t D2, typename T1, typename T2>
constexpr auto operator-(const Vec<D1, T1>& a, const Vec<D2, T2>& b)
{
	vec::detail::assertSameDimensions(a, b);
	auto ret = Vec<std::min(D1, D2), decltype(a[0] - b[0])>{};
	if constexpr(!decltype(ret)::staticSized) ret.resize(a.size());

	for(auto i = 0u; i < ret.size(); ++i)
		ret[i] = a[i] - b[i];
	return ret;
}

template<size_t D, typename T>
constexpr auto operator-(const Vec<D, T>& a)
{
	auto ret = Vec<D, decltype(-a[0])>{};
	if constexpr(!decltype(ret)::staticSized) ret.resize(a.size());

	for(auto i = 0u; i < ret.size(); ++i)
		ret[i] = -a[i];
	return ret;
}

template<size_t D, typename F, typename T>
constexpr auto operator*(const F& f, const Vec<D, T>& a)
{
	auto ret = Vec<D, decltype(f * a[0])>{};
	if constexpr(!decltype(ret)::staticSized) ret.resize(a.size());

	for(auto i = 0u; i < ret.size(); ++i)
		ret[i] = f * a[i];
	return ret;
}

template<size_t D1, size_t D2, typename T1, typename T2>
constexpr auto operator==(const Vec<D1, T1>& a, const Vec<D2, T2>& b)
{
	vec::detail::assertSameDimensions(a, b);
	for(auto i = 0u; i < a.size(); ++i)
		if(a[i] != b[i]) return false;
	return true;
}

template<size_t D1, size_t D2, typename T1, typename T2>
constexpr auto operator!=(const Vec<D1, T1>& a, const Vec<D2, T2>& b)
{
	return !(a == b);
}

template<size_t D, typename T>
std::ostream& operator<<(std::ostream& os, const Vec<D, T>& a)
{
	return vec::print(os, a);
}

} // namespace nytl

#endif // header guard
