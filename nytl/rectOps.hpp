// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_RECT_OPS
#define NYTL_INCLUDE_RECT_OPS

#include <nytl/rect.hpp> // nytl::Rect
#include <nytl/vecOps.hpp> // nytl::sum
#include <nytl/simplex.hpp> // nytl::Simplex
#include <utility> // std::pair

namespace nytl {

/// Returns the total size of a given Rect.
/// \module rectOps
template<std::size_t D, typename T>
auto size(const Rect<D, T>& rect)
{
	return multiply(rect.size);
}

/// Returns the center of a given Rect
/// Returns a vector in the same space of position and size of the rect.
/// \module rectOps
template<std::size_t D, typename T>
auto center(const Rect<D, T>& rect)
{
	return rect.position + (0.5 * rect.size);
}

/// Returns whether the given Rect contains the given point (as Vec).
/// Returns true if the lies on the outline of the given rect.
/// \requires Types 'T1' and 'T2' must describe the same field and be comparable.
/// \module rectOps
template<std::size_t D, typename T1, typename T2>
bool contains(const Rect<D, T1>& rect, const Vec<D, T2>& point)
{
	for(auto i = 0u; i < point.size(); ++i) {
		if(rect.position[i] > point[i]) return false;
		if(rect.position[i] + rect.size[i] < point[i]) return false;
	}

	return true;
}

/// Returns whether the given Rect contains the given point (as Vec).
/// Returns false if the lies on the outline of the given rect.
/// \requires Types 'T1' and 'T2' must describe the same field and be comparable.
/// \module rectOps
template<std::size_t D, typename T1, typename T2>
bool containsReal(const Rect<D, T1>& rect, const Vec<D, T2>& point)
{
	for(auto i = 0u; i < point.size(); ++i) {
		if(rect.position[i] >= point[i]) return false;
		if(rect.position[i] + rect.size[i] <= point[i]) return false;
	}

	return true;
}

/// Returns whether the the two given Rects intersect.
/// Will return true if they only touch each other.
/// \requires Types 'T1' and 'T2' must describe the same field and be comparable.
/// \module rectOps
template<std::size_t D, typename T1, typename T2>
bool intersects(const Rect<D, T1>& a, const Rect<D, T2>& b)
{
	return contains(a, b.position) || contains(a, b.position + b.size) ||
		contains(b, a.position) || contains(b, a.position + size);
}

/// Returns whether the the two given Rects intersect.
/// Will return false if they only touch each other.
/// \requires Types 'T1' and 'T2' must describe the same field and be comparable.
/// \module rectOps
template<std::size_t D, typename T1, typename T2>
bool intersectsReal(const Rect<D, T1>& a, const Rect<D, T2>& b)
{
	return containsReal(a, b.position) || containsReal(a, b.position + b.size) ||
		containsReal(b, a.position) || containsReal(b, a.position + size);
}

} // namespace nytl

#endif // header guard
