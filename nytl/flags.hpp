// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Defines binary operators for enums.

#pragma once

#ifndef NYTL_INCLUDE_FLAGS_HPP
#define NYTL_INCLUDE_FLAGS_HPP

#include <nytl/fwd/flags.hpp>
#include <type_traits>

namespace nytl
{

template<typename T, typename U>
class Flags
{
public:
	constexpr Flags() noexcept = default;
	constexpr Flags(T bit) noexcept : value_(static_cast<U>(bit)) {}
	constexpr Flags(bool, T bit) noexcept : value_(~static_cast<U>(bit)) {}
	constexpr Flags(const Flags& rhs) noexcept : value_(rhs.value()) {}
	~Flags() noexcept = default;

	constexpr Flags& operator=(const Flags& r) noexcept { value_ = r.value(); return *this; }
	constexpr Flags& operator|=(const Flags& r) noexcept { value_ |= r.value(); return *this; }
	constexpr Flags& operator&=(const Flags& r) noexcept { value_ &= r.value_; return *this; }
	constexpr Flags& operator^=(const Flags& r) noexcept { value_ ^= r.value(); return *this; }
	constexpr Flags operator|(const Flags& r) const noexcept { return Flags(r) |= *this; }
	constexpr Flags operator&(const Flags& r) const noexcept { return Flags(r) &= *this; }
	constexpr Flags operator^(const Flags& r) const noexcept { return Flags(r) ^= *this; }
	constexpr operator bool() const noexcept { return (value()); }
	constexpr bool operator!() const noexcept { return !(value()); }
	constexpr bool operator==(const Flags& rhs) const noexcept { return value_ == rhs.value(); }
	constexpr bool operator!=(const Flags& rhs) const noexcept { return value_ != rhs.value(); }

	constexpr explicit operator U() const noexcept { return value_; }
	constexpr const U& value() const noexcept { return value_; }

protected:
	U value_ {};
};

template <typename T> Flags<T> constexpr operator|(T bit, const Flags<T>& flags) noexcept
	{ return flags | bit; }
template <typename T> Flags<T> constexpr operator&(T bit, const Flags<T>& flags) noexcept
	{ return flags & bit; }
template <typename T> Flags<T> constexpr operator^(T bit, const Flags<T>& flags) noexcept
	{ return flags ^ bit; }

}

#define NYTL_FLAG_OPS(T) \
	constexpr nytl::Flags<T> operator|(T a, T b) noexcept { return nytl::Flags<T>(a) | b; } \
	constexpr nytl::Flags<T> operator&(T a, T b) noexcept { return nytl::Flags<T>(a) & b; } \
	constexpr nytl::Flags<T> operator^(T a, T b) noexcept { return nytl::Flags<T>(a) ^ b; } \
	constexpr nytl::Flags<T> operator~(T bit) noexcept { return nytl::Flags<T>(false, bit); }

#endif //header guard
