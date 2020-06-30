// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file A Vector implementation class.

#pragma once

#ifndef NYTL_INCLUDE_VEC
#define NYTL_INCLUDE_VEC

#include <nytl/fwd/vec.hpp> // nytl::Vec typedefs

// specializations
#include <nytl/vec2.hpp> // nytl::Vec<2, T>
#include <nytl/vec3.hpp> // nytl::Vec<3, T>

#include <array> // std::array
#include <algorithm> // std::min

namespace nytl {

/// \brief Basic Vector template class.
/// Basically a std::array with vector semantics. There are various operators
/// and utility provided for dealing with it.
/// \module vec
template<size_t D, typename T>
class Vec : public std::array<T, D> {
public:
	/// The (static/fixed) size of the type
	static constexpr size_t size() { return D; }

	/// Explicitly casts the Vec to another Vec that may have
	/// a different precision or dimension. Will default construct
	/// any values that cannot be filled (e.g. vec3 -> vec4) or leave
	/// out the last values when the size of vector is shrinked (e.g.
	/// {1, 2, 3} -> {1, 2}).
	template<size_t OD, typename OT>
	constexpr explicit operator Vec<OD, OT>() const {
		auto ret = Vec<OD, OT> {};
		for(auto i = 0u; i < std::min(D, OD); ++i)
			ret[i] = (*this)[i];
		return ret;
	}
};

template<typename... Args>
Vec(Args&&... args) ->
	Vec<sizeof...(Args), std::common_type_t<Args...>>;

// - implementation/operators -
// - free operators -
template<size_t D, typename T1, typename T2>
constexpr Vec<D, T1>& operator+=(Vec<D, T1>& a, const Vec<D, T2>& b) {
	for(size_t i = 0; i < D; ++i)
		a[i] += b[i];
	return a;
}

template<size_t D, typename T1, typename T2>
constexpr Vec<D, T1>& operator-=(Vec<D, T1>& a, const Vec<D, T2>& b) {
	for(size_t i = 0; i < D; ++i)
		a[i] -= b[i];
	return a;
}

template<size_t D, typename T, typename F>
constexpr Vec<D, T>& operator*=(Vec<D, T>& vec, const F& fac) {
	for(auto i = 0u; i < D; ++i)
		vec[i] *= fac;
	return vec;
}

template<size_t D, typename T1, typename T2>
constexpr auto operator+(const Vec<D, T1>& a, const Vec<D, T2>& b) {
	Vec<D, decltype(a[0] + b[0])> ret {};
	for(auto i = 0u; i < D; ++i)
		ret[i] = a[i] + b[i];
	return ret;
}

template<size_t D, typename T1, typename T2>
constexpr auto operator-(const Vec<D, T1>& a, const Vec<D, T2>& b) {
	Vec<D, decltype(a[0] - b[0])> ret {};
	for(auto i = 0u; i < D; ++i)
		ret[i] = a[i] - b[i];
	return ret;
}

template<size_t D, typename T>
constexpr auto operator-(Vec<D, T> a) {
	for(auto i = 0u; i < D; ++i)
		a[i] = -a[i];
	return a;
}

template<size_t D, typename T>
constexpr auto operator+(const Vec<D, T>& a) {
	return a;
}

template<size_t D, typename F, typename T>
constexpr auto operator*(const F& f, const Vec<D, T>& a)
		-> Vec<D, decltype(f * a[0])> {
	Vec<D, decltype(f * a[0])> ret {};
	for(auto i = 0u; i < D; ++i)
		ret[i] = f * a[i];
	return ret;
}

template<size_t D, typename T1, typename T2>
constexpr auto operator==(const Vec<D, T1>& a, const Vec<D, T2>& b) {
	for(auto i = 0u; i < D; ++i)
		if(a[i] != b[i])
			return false;
	return true;
}

template<size_t D1, size_t D2, typename T1, typename T2>
constexpr auto operator!=(const Vec<D1, T1>& a, const Vec<D2, T2>& b) {
	return !(a == b);
}

} // namespace nytl

#endif // header guard
