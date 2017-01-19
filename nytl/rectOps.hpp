// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Various operations and checks for Rects.

#pragma once

#ifndef NYTL_INCLUDE_RECT_OPS
#define NYTL_INCLUDE_RECT_OPS

#include <nytl/tmpUtil.hpp> // nytl::templatize
#include <nytl/rect.hpp> // nytl::Rect
#include <nytl/vecOps.hpp> // nytl::sum
#include <nytl/simplex.hpp> // nytl::Simplex

#include <utility> // std::pair
#include <vector> // std::vector
#include <iosfwd> // std::ostream

namespace nytl {

/// \brief Prints the given Rect.
/// Simply outputs the position and size of the Rect.
/// \module rectOps
template<std::size_t D, typename T>
std::ostream& print(std::ostream& os, const Rect<D, T>& rect)
{
	auto& tos = templatize<T>(os);
	tos << "{";
	vec::print(tos, rect.position);
	vec::print(tos, rect.size);
	tos << "}";
	return tos;
}

/// \brief Returns the total size of a given Rect.
/// \module rectOps
template<std::size_t D, typename T>
constexpr auto size(const Rect<D, T>& rect)
{
	return multiply(rect.size);
}

/// \brief Returns the center of a given Rect
/// Returns a vector in the same space of position and size of the rect.
/// \module rectOps
template<std::size_t D, typename T>
constexpr auto center(const Rect<D, T>& rect)
{
	return rect.position + (0.5 * rect.size);
}

/// \brief Returns whether the given Rect contains the given point (as Vec).
/// Returns true if the lies on the outline of the given rect.
/// \requires Types 'T1' and 'T2' must describe the same field and be comparable.
/// \module rectOps
template<std::size_t D, typename T1, typename T2>
constexpr bool contains(const Rect<D, T1>& rect, const Vec<D, T2>& point)
{
	for(auto i = 0u; i < point.size(); ++i) {
		if(rect.position[i] > point[i]) return false;
		if(rect.position[i] + rect.size[i] < point[i]) return false;
	}

	return true;
}

/// \brief Returns whether the given Rect contains the given point (as Vec).
/// Returns false if the lies on the outline of the given rect.
/// \requires Types 'T1' and 'T2' must describe the same field and be comparable.
/// \module rectOps
template<std::size_t D, typename T1, typename T2>
constexpr bool containsReal(const Rect<D, T1>& rect, const Vec<D, T2>& point)
{
	for(auto i = 0u; i < point.size(); ++i) {
		if(rect.position[i] >= point[i]) return false;
		if(rect.position[i] + rect.size[i] <= point[i]) return false;
	}

	return true;
}

/// \brief Returns whether the the two given Rects intersect.
/// Will return true if they only touch each other.
/// \requires Types 'T1' and 'T2' must describe the same field and be comparable.
/// \module rectOps
template<std::size_t D, typename T1, typename T2>
constexpr bool intersects(const Rect<D, T1>& a, const Rect<D, T2>& b)
{
	return contains(a, b.position) || contains(a, b.position + b.size) ||
		contains(b, a.position) || contains(b, a.position + size);
}

/// \brief Returns whether the the two given Rects intersect.
/// Will return false if they only touch each other.
/// \requires Types 'T1' and 'T2' must describe the same field and be comparable.
/// \module rectOps
template<std::size_t D, typename T1, typename T2>
constexpr bool intersectsReal(const Rect<D, T1>& a, const Rect<D, T2>& b)
{
	return containsReal(a, b.position) || containsReal(a, b.position + b.size) ||
		containsReal(b, a.position) || containsReal(b, a.position + size);
}

/// \brief Returns the intersection between the given Rects.
/// Note that two Rects in any dimensions can only interset in another Rect.
/// \module rectOps
template<std::size_t D, typename T>
constexpr Rect<D, T> intersection(const Rect<D, T>& a, const Rect<D, T>& b)
{
	auto pos = vec::cw::max(a.position, b.position);
	auto end = vec::cw::min(a.position + size, b.position + b.size);

	// check if there is no intersection
	for(auto i = 0u; i < pos.size(); ++i)
		if(pos[i] > end[i]) return {{}, {}};

	return {pos, size - pos};
}

/// \brief Returns the difference of the first Rect to the second Rect.
/// Effectively returns the parts of the first Rect that are not part of the second one.
/// Returns the resulting Rects as a vector since the count depends on the layout
/// of the Rects. If the Rects have no intersection, just returns a vector with
/// only the first Rect and if they are the same, returns an empty vector.
/// \notes This operations is not symmetric, i.e. difference(a, b) != difference(b, a).
/// \notes Also see [nytl::rectOps::symmetricDifference]().
/// In general is symmetricDifference(a, b) = difference(a, b) | difference(b, a);
/// \module rectOps
template<std::size_t D, class T>
std::vector<Rect<D, T>> difference(const Rect<D, T>& a, const Rect<D, T>& b)
{
	static constexpr auto inRange = [](T start, T size, T value) {
		return (start < value && value < start + size);
	};

	std::vector<Rect<D, T>> ret;
	for(std::size_t i(0); i < D; ++i) {

		// rect before intersection
		if(inRange(a.position[i], a.size[i], b.position[i])) {
			auto pos = a.position;
			for(std::size_t o(0); o < i; ++o)
				pos[o] = b.position[o];

			auto size = (a.position + a.size) - pos;
			size[i] = b.position[i] - pos[i];

			ret.push_back({pos, size});
		}

		// rect after intersection
		if(inRange(b.position[i] + b.size[i], a.position[i], a.size[i])) {
			auto pos = a.position;
			pos[i] = b.position[i] + b.size[i];
			for(std::size_t o(0); o < i; ++o)
				pos[o] = std::max(b.position[o], a.position[o]);

			auto size = (a.position + a.size) - pos;
			for(std::size_t o(0); o < i; ++o)
				size[o] = (b.position[o] + b.size[o]) - pos[o];

			ret.push_back({pos, size});
		}
	}

	return ret;
}

} // namespace nytl

#endif // header guard
