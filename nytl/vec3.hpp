// Copyright (c) 2017-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file vec3 specialization (x,y,z members)

#pragma once

#ifndef NYTL_INCLUDE_VEC3
#define NYTL_INCLUDE_VEC3

#include <nytl/fwd/vec.hpp> // nytl::Vec declaration
#include <algorithm> // std::min

namespace nytl {

/// Specialization for 3 component Vec.
/// Holds x,y,z members that are more convenient to access.
/// Compatible with the default class definition.
template<typename T>
class Vec<3, T> {
public:
	T x;
	T y;
	T z;

public:
	static constexpr size_t size() { return 3; }

	constexpr const T* begin() const { return &x; }
	constexpr const T* end() const { return &z + 1; }
	constexpr T* begin() { return &x; }
	constexpr T* end() { return &z + 1; }

	constexpr T& front() { return x; }
	constexpr T& back() { return z; }

	constexpr const T& front() const { return x; }
	constexpr const T& back() const { return z; }

	constexpr T* data() { return &x; }
	constexpr const T* data() const { return &x; }

	// See the vec2 implementation for implementation reasoning.
	constexpr T& operator[](size_t i) {
		switch(i) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: return *static_cast<T*>(nullptr);
		}
	}

	constexpr const T& operator[](size_t i) const {
		switch(i) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: return *static_cast<const T*>(nullptr);
		}
	}

	// implemented in vec.hpp for all specializations
	template<size_t OD, typename OT>
	constexpr explicit operator Vec<OD, OT>() const {
		auto ret = Vec<OD, OT> {};
		for(auto i = 0u; i < std::min(size(), OD); ++i)
			ret[i] = (*this)[i];
		return ret;
	}
};

} // namespace nytl

#endif // header guard
