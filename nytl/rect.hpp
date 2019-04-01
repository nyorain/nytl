// Copyright (c) 2017-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file Defines the Rect template class.

#pragma once

#ifndef NYTL_INCLUDE_RECT
#define NYTL_INCLUDE_RECT

#include <nytl/fwd/rect.hpp> // nytl::Rect typedefs
#include <nytl/vec.hpp> // nytl::Vec
#include <cstddef> // std::size_t

namespace nytl {

/// \brief Templated class that represents the hyperrectangle (n-box) concept.
/// \tparam D The dimension of the hyperrectangle.
/// \tparam P The precision of the hyperrectangle.
/// \details The hyperrectangle is the generalization of a rectangle for higher dimensions.
/// It represents an area that is aligned with the spaces dimensions at a given position with
/// a given size. There exist various operators for the Rect template class e.g. to check for
/// intersection, compute unions or differences.
/// See nytl/vecOps.hpp for various operations on rectangles.
/// \module rect
template<std::size_t D, typename T>
class Rect {
public:
	static constexpr std::size_t dim = D;

	using Value = T;
	using Size = std::size_t;
	using VecType = Vec<dim, Value>;

public:
	VecType position;
	VecType size;

public:
	/// Converts the Rect to another Rect object of different dimension and/or precision.
	template<Size OD, class OP>
	explicit operator Rect<OD, OP>() const
		{ return Rect<OD, OP>{static_cast<Vec<OD, OP>>(position), static_cast<Vec<OD, OP>>(size)}; }
};

template<std::size_t D, typename T1, typename T2>
Rect(const nytl::Vec<D, T1>&, const nytl::Vec<D, T2>&)
	-> Rect<D, std::common_type_t<T1, T2>>;

} // namespace nytl

#endif // header guard
