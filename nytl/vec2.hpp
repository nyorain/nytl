// Copyright (c) 2017-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file optional vec2 specialization (x,y members)

#pragma once

#ifndef NYTL_INCLUDE_VEC2
#define NYTL_INCLUDE_VEC2

#include <nytl/fwd/vec.hpp>

namespace nytl {

// TODO: provide other (mainly pointless but still) stl-like utility
// to keep the compatibility promise.

/// Specialization for 2 component Vec.
/// Holds x,y members that are more convenient to access.
/// Compatible with the default class definition.
template<typename T>
class Vec<2, T> {
public:
	using Value = T;
	static constexpr size_t size() { return 2; }

public:
	T x;
	T y;

public:
	constexpr auto* begin() const { return &x; }
	constexpr auto* end() const { return &y + 1; }
	constexpr auto* begin() { return &x; }
	constexpr auto* end() { return &y + 1; }

	constexpr auto& front() { return x; }
	constexpr auto& back() { return y; }

	constexpr auto& operator[](size_t i) const {
		return (i == 0) ? x : y;
	}

	constexpr auto& operator[](size_t i) {
		return (i == 0) ? x : y;
	}

	template<size_t OD, typename OT>
	constexpr explicit operator Vec<OD, OT>() const;
};

} // namespace nytl

#endif // header guard
