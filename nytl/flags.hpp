// Copyright (c) 2017-2020 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// The file defines binary operators for enums as well as the nytl::Flags class.
/// Instead of simply defining bitwise operations for enumerations, e.g.
/// 'Enum | Enum -> Enum', we add the 'Flags' wrapper class signalling that multiple
/// bits might be set, resulting in operations like
/// 'Enum | Enum -> nytl::Flags<Enum>'.
/// This can be useful to differentiate for a function whether it expects
/// one value from a bitmask enum or can accept multiple values.
/// For a specific usecase see e.g. the Vulkan API that separates in
/// functions/structs whether it wants one bit of a bitmask enum or 'flags'.

#pragma once

#ifndef NYTL_INCLUDE_FLAGS
#define NYTL_INCLUDE_FLAGS

#include <nytl/fwd/flags.hpp> // nytl::Flags default template parameter

namespace nytl {

/// Can be used to invert the given value on Flags construction
/// Can be used like this: `nytl::Flags<Enum>(nytl::invertFlags, Enum::value)`.
struct InvertFlags {};
constexpr InvertFlags invertFlags {};

/// Can be used to combine multiple values from the same enumeration.
/// Use the NYTL_FLAG_OPS macro to define binary operations on the
/// enumeration that result in a nytl::Flags object for it.
/// Requires: Each value in the enumerations should have exactly one bit set and
/// all values should have different bits set so they can be combined.
/// T: The enum type from which values should be combined.
/// U: The raw type to store the values in. By default the underlying type of
///    the enum as reported by std::underlying_type<T>
template<typename T, typename U>
class Flags {
public:
	constexpr Flags() noexcept = default;
	constexpr Flags(T bit) noexcept : value_(static_cast<U>(bit)) {}
	constexpr Flags(InvertFlags, T bit) noexcept : value_(~static_cast<U>(bit)) {}
	~Flags() noexcept = default;

	constexpr Flags& operator|=(const Flags& r) noexcept { value_ |= r.value(); return *this; }
	constexpr Flags& operator&=(const Flags& r) noexcept { value_ &= r.value_; return *this; }
	constexpr Flags& operator^=(const Flags& r) noexcept { value_ ^= r.value(); return *this; }

	constexpr Flags operator|(const Flags& r) const noexcept { return Flags(r) |= *this; }
	constexpr Flags operator&(const Flags& r) const noexcept { return Flags(r) &= *this; }
	constexpr Flags operator^(const Flags& r) const noexcept { return Flags(r) ^= *this; }

	constexpr bool operator==(const Flags& rhs) const noexcept { return value_ == rhs.value(); }
	constexpr bool operator!=(const Flags& rhs) const noexcept { return value_ != rhs.value(); }

	constexpr const U& value() const noexcept { return value_; }
	constexpr operator U() const noexcept { return value_; }

	bool any() const { return value_ != U{}; }
	bool none() const { return value_ == U{}; }

	bool allOf(const Flags<T, U>& b) const { return (*this & b) == b; }
	bool anyOf(const Flags<T, U>& b) const { return (*this & b).any(); }
	bool noneOf(const Flags<T, U>& b) const { return (*this & b).none(); }

public: // public to make it a standard layout type
	U value_ {};
};

// Can be used to explicitly set/unset flags.
template<typename T, typename U>
Flags<T, U> set(Flags<T, U> a, const Flags<T, U>& b, bool doSet) {
	return doSet ? a |= b : a &= ~b;
}

// - binary flags operators -
template<typename T> constexpr
Flags<T> operator|(T bit, const Flags<T>& flags) noexcept
	{ return flags | bit; }

template<typename T> constexpr
Flags<T> operator&(T bit, const Flags<T>& flags) noexcept
	{ return flags & bit; }

template<typename T> constexpr
Flags<T> operator^(T bit, const Flags<T>& flags) noexcept
	{ return flags ^ bit; }

} // namespace nytl

/// Can be used for an enum to generate binary operations resulting in nytl::Flags.
/// Can be used like this: `enum class Enum {}; NYTL_FLAG_OPS(Enum)` which will
/// make results like `Enum::value1 | Enum::value2` automatically result in a
/// `nytl::Flags<Enum>` object holding the union of the given values.
/// Inversion of flags or enum values will actually inverse the underlaying value.
/// Therefore equal comparisions with flags can be error prone and one should prefer to
/// just check whether flags contain a specific value. The follwing static_assertion will fail:
/// ```cpp
/// enum class Enum { value1 = 1, value2 = 2 };
/// NYTL_FLAG_OPS(Enum)
/// static_assert(~Enum::value1 == Enum::value2, "will fail due to nytl::Flags");
/// ```
/// \module utility
#define NYTL_FLAG_OPS(T) \
	constexpr nytl::Flags<T> operator|(T a, T b) noexcept { return nytl::Flags<T>(a) | b; } \
	constexpr nytl::Flags<T> operator&(T a, T b) noexcept { return nytl::Flags<T>(a) & b; } \
	constexpr nytl::Flags<T> operator^(T a, T b) noexcept { return nytl::Flags<T>(a) ^ b; } \
	constexpr nytl::Flags<T> operator~(T bit) noexcept { return {nytl::invertFlags, bit}; }

#endif // header guard
