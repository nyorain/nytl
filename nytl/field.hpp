// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Mathematical field related definitions.

#pragma once

#ifndef NYTL_INCLUDE_FIELD
#define NYTL_INCLUDE_FIELD

#include <cmath> // std::abs
#include <complex> // std::complex

namespace nytl {

/// The data type of a field that offers full precision.
/// Needed so that e.g. even int can be used to represent the real numbers.
template<typename T>
struct FullPrecisionField {
	using type = double;
};

/// FullPrecisionField specialization for complex numbers.
template<typename T>
struct FullPrecisionField<std::complex<T>> {
	using type = std::complex<double>;
};

/// \brief Specifies traits of a mathematical field
/// Should be specialized for used custom fields to make the various nytl operations work.
/// For custom fields, specializations must common mathematical operations (if available),
/// see [nytl::FieldTraitsBase]() which uses the std implementations.
/// Additionally one has to specify the zero and one values for the field, as well as providing
/// a FullPrecision typedef to the type that should be used when e.g. division on two arbitary
/// field types is performed (useful to make int able to represent the field of real numbers).
template<typename T>
struct FieldTraits {
	using FullPrecision = typename FullPrecisionField<T>::type;

	static constexpr T zero = T {0};
	static constexpr T one = T {1};

	static constexpr auto abs(FullPrecision value) { return std::abs(value); }
	static constexpr auto sqrt(FullPrecision value) { return std::sqrt(value); }

	static constexpr auto sin(FullPrecision value) { return std::sin(value); }
	static constexpr auto cos(FullPrecision value) { return std::cos(value); }
	static constexpr auto tan(FullPrecision value) { return std::tan(value); }

	static constexpr auto asin(FullPrecision value) { return std::asin(value); }
	static constexpr auto acos(FullPrecision value) { return std::acos(value); }
	static constexpr auto atan(FullPrecision value) { return std::atan(value); }
};

template<typename T> constexpr T FieldTraits<T>::zero;
template<typename T> constexpr T FieldTraits<T>::one;

} // namespace nytl

#endif // header guard
