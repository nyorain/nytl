// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Includes the unspecialized nytl::Simplex template class.

#pragma once

#ifndef NYTL_INCLUDE_SIMPLEX
#define NYTL_INCLUDE_SIMPLEX

#include <nytl/fwd/simplex.hpp> // nytl::Simplex default template parameter
#include <nytl/vec.hpp> // nytl::Vec

#include <type_traits> // std::enable_if
#include <cstdint> // std::size_t
#include <array> // std::array

namespace nytl {

/// \brief Templated abstraction of a simplex (the generalization of a triangle).
/// \details The Simplex<D, P, A> template class defines an unique area with 'A' dimensions
/// of 'P' precision in an 'D' dimensional space.
/// So e.g. Simplex<3, float, 2> describes a Triangle in a 3-dimensional space.
/// This template class does only work if 'D' >= 'A', since the dimension of the simplex
/// can not be higher than the dimension of the space that contains this simplex. You cannot
/// e.g. have a triangle in one dimensional space.
/// The area described by a simplex is the most trivial space-filling unique and unambiguous
/// geometrical figure in a space with the same dimensions as the simplex (e.g. a triangle
/// in 2 dimensions).
/// \requires 'P' must be a mathematical field over which the space is defined.
/// \requires 'D' must be greater or equal than 'A'.
/// \module simplex
template<std::size_t D, typename P, std::size_t A>
class Simplex {
public:
	static_assert(D >= A,
		"The Dimension of the Simplex cannot exceed the room dimension");

	static constexpr auto spaceDim = D; // dimensions of the space the simplex is in
	static constexpr auto simplexDim = A; // dimensions of the simplex itself
	static constexpr auto pointCount = A + 1; // number of points the simplex is defined by

	using Precision = P;
	using Point = Vec<D, P>;
	using PointArray = std::array<Point, pointCount>;
	using Size = std::size_t;

public:
	PointArray points_ {};

public:
	constexpr Simplex() noexcept = default; // use aggregate initialization
	~Simplex() noexcept = default;

	/// Returns the array of points this simplex is defined by.
	/// References A + 1 points.
	/// Can be used to access (read/change/manipulate) the points.
	/// Should be implemented by specializations.
	constexpr Point* points() noexcept { return points_.data(); }

	/// Returns the array of points this simplex is defined by.
	/// References A + 1 points.
	/// Can be used to const_iterate/read the points.
	/// Should be implemented by specializations.
	constexpr const Point* points() const noexcept { return points_.data(); }

	/// Converts the object to a Simplex with a different dimension or precision.
	/// Note that the area dimension A cannot be changed, only the space dimension D.
	/// This means simply that e.g. a Triangle cannot be converted into a line, but a triangle
	/// in 3 dimensional space can be converted to a triangle in 2 dimensional space (by simply
	/// stripping the 3rd dimension).
	/// Works only if the new D is still greater equal A.
	/// Should be implemented by specializations.
	template<Size OD, typename OP>
	constexpr explicit operator Simplex<OD, OP, A>() const noexcept {
		Simplex<OD, OP, A> ret {};
		for(auto i = 0u; i < points_.size(); ++i) {
			ret[i] = static_cast<Vec<OD, OP>>(points_[i]);
		}
		return ret;
	}
};

} // namespace nytl

#endif //header guard
