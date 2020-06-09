// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file vec2 specialization (x,y members)

#pragma once

#ifndef NYTL_INCLUDE_VEC2
#define NYTL_INCLUDE_VEC2

#include <nytl/fwd/vec.hpp> // nytl::Vec declaration
#include <algorithm> // std::min
#include <stdexcept> // std::out_of_range

namespace nytl {

/// Specialization for 2 component Vec.
/// Holds x,y members that are more convenient to access.
/// Compatible with the default class definition.
template<typename T>
class Vec<2, T> {
public:
	T x;
	T y;

public:
	static constexpr size_t size() { return 2; }

	// constexpr Vec() = default;
	// constexpr explicit Vec(const T& all) : x(all), y(all) {}
	// constexpr Vec(const T& x_, const T& y_) : x(x_), y(y_) {}

	constexpr const T* begin() const { return &x; }
	constexpr const T* end() const { return &y + 1; }
	constexpr T* begin() { return &x; }
	constexpr T* end() { return &y + 1; }

	constexpr T& front() { return x; }
	constexpr T& back() { return y; }

	constexpr const T& front() const { return x; }
	constexpr const T& back() const { return y; }

	constexpr T* data() { return &x; }
	constexpr const T* data() const { return &x; }

	// We could use (data()[i]) but this conflicts constexpr (gcc 7).
	// Stl convention is not to check bounds and therefore never throw
	// from operator[] (and so does the default Vec implementation).
	// But we implicitly have to check bounds here and all other alternatives
	// are worse so we throw in the case of out-of-range. It's almost free.
	constexpr T& operator[](size_t i) {
		switch(i) {
			case 0: return x;
			case 1: return y;
			default: throw std::out_of_range("Vec2[]");
		}
	}

	constexpr const T& operator[](size_t i) const {
		switch(i) {
			case 0: return x;
			case 1: return y;
			default: throw std::out_of_range("Vec2[]");
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
