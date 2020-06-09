// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file vec3 specialization (x,y,z members)

#pragma once

#ifndef NYTL_INCLUDE_VEC3
#define NYTL_INCLUDE_VEC3

#include <nytl/fwd/vec.hpp> // nytl::Vec declaration
#include <algorithm> // std::min
#include <stdexcept> // std::out_of_range

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

	// constexpr Vec() = default;
	// constexpr explicit Vec(const T& all) : x(all), y(all), z(all) {}
	// constexpr Vec(const T& x_, const T& y_, const T& z_) : x(x_), y(y_), z(z_) {}

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
			default: throw std::out_of_range("Vec3[]");
		}
	}

	constexpr const T& operator[](size_t i) const {
		switch(i) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: throw std::out_of_range("Vec3[]");
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
