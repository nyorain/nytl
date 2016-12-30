// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Mathematical field related definitions.

#pragma once

#ifndef NYTL_INCLUDE_FIELD
#define NYTL_INCLUDE_FIELD

#include <cmath> // std::abs
#include <complex> // std::complex

namespace nytl {

/// Base class for FieldTrait specializations with zero and one from the real numbers
/// and implementations for mathematical operations inside the stl.
template<typename T>
struct FieldTraitsBase {
	static constexpr auto zero = T {0};
	static constexpr auto one = T {1};

	static constexpr auto abs(T value) { return std::abs(value); }
	static constexpr auto sqrt(T value) { return std::sqrt(value); }

	static constexpr auto sin(T value) { return std::sin(value); }
	static constexpr auto cos(T value) { return std::cos(value); }
	static constexpr auto tan(T value) { return std::tan(value); }

	static constexpr auto asin(T value) { return std::asin(value); }
	static constexpr auto acos(T value) { return std::acos(value); }
	static constexpr auto atan(T value) { return std::atan(value); }
};

/// \brief Specifies traits of a mathematical field
/// Should be specialized for used custom fields to make the various nytl operations work.
/// For custom fields, specializations must common mathematical operations (if available),
/// see [nytl::FieldTraitsBase]() which uses the std implementations.
/// Additionally one has to specify the zero and one values for the field, as well as providing
/// a FullPrecision typedef to the type that should be used when e.g. division on two arbitary
/// field types is performed (useful to make int able to represent the field of real numbers).
template<typename T, typename = void>
struct FieldTraits : FieldTraitsBase<T> {
	using FullPrecision = double;
};

/// \brief FieldTraits specialization for std::complex numbers.
/// Note that all common mathematical functions are also defined for complex numbers.
template<typename T>
struct FieldTraits<std::complex<T>> {
	using FullPrecision = std::complex<double>;
};

} // namespace nytl

#endif // header guard
