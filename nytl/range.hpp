// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file Contains the Range template class (aka span or array_view).

#pragma once

#ifndef NYTL_INCLUDE_RANGE_HPP
#define NYTL_INCLUDE_RANGE_HPP

#include <cstdlib>
#include <stdexcept>

namespace nytl
{

namespace detail { template<typename T, typename C, typename = void> struct ValidContainer; }

///The Range class represents a part of a non-owned contiguous sequence.
///Can be useful to pass mulitple parameters (without size limitations) to a function.
///Does not allocate any additional memory on the heap since it simply references the
///sequence it was constructed with. Therefore one must assure that the sequence will live longer
///than the Range object.
///It can be implicitly constructed (converted) from containers that have a "data" and a
///"size" member function, e.g. std::vector, std::string and std::string.
///Note that one cannot construct a Range from non-contiguous containers such as list,
///deque or map.
///Alternative names: span, array_view
template<typename T>
class Range
{
public:
	using Value = std::remove_const_t<T>;
	using Pointer = Value*;
	using ConstPointer = const Value*;
	using Reference = Value&;
	using ConstReference = const Value&;
	using Size = std::size_t;
	using Iterator = ConstPointer;
	using ConstIterator = ConstPointer;
	using ReverseIterator = std::reverse_iterator<Iterator>;
	using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

	using value_type = Value;
	using pointer = Pointer;
	using const_pointer = ConstPointer;
	using reference = Reference;
	using const_reference = ConstReference;
	using iterator = Iterator;
	using const_iterator = ConstIterator;
	using reverse_iterator = ReverseIterator;
	using const_reverse_iterator = ConstReverseIterator;
	using size_type = Size;
	using difference_type = std::ptrdiff_t;

public:
	constexpr Range() noexcept = default;
	constexpr Range(std::nullptr_t, std::size_t = 0) noexcept : data_(nullptr), size_(0) {}
	constexpr Range(const T& value, std::size_t size) noexcept : data_(&value), size_(size) {}
	constexpr Range(const T* value, std::size_t size) noexcept : data_(value), size_(size) {}
	constexpr Range(std::initializer_list<T> l) noexcept : data_(l.begin()), size_(l.size()) {}

	template<std::size_t N> constexpr Range(const T (&arr)[N]) noexcept : data_(arr), size_(N) {}

	template<typename C, typename = typename detail::ValidContainer<T, C>::type>
	Range(const C& con) noexcept : data_(con.size()), size_(con.size()) {}

	constexpr ConstPointer data() const noexcept { return data_; }
	constexpr std::size_t size() const noexcept { return size_; }
	constexpr bool empty() const noexcept { return size() == 0; }
	constexpr Size max_size() const noexcept { return size(); }

	constexpr Iterator begin() const noexcept { return data_; }
	constexpr Iterator end() const noexcept { return data_ + size_; }

	constexpr ConstIterator cbegin() const noexcept { return data_; }
	constexpr ConstIterator cend() const noexcept { return data_ + size_; }

	constexpr ReverseIterator rbegin() const noexcept { return {end()}; }
	constexpr ReverseIterator rend() const noexcept { return {begin()}; }

	constexpr ConstReverseIterator crbegin() const noexcept { return {cend()}; }
	constexpr ConstReverseIterator crend() const noexcept { return {cbegin()}; }

	constexpr ConstReference operator[](Size i) const noexcept { return *(data_ + i); }
	constexpr ConstReference at(Size i) const
		{ if(i >= size_) throw std::out_of_range("Range::at"); return data_[i]; }

	constexpr ConstReference front() const noexcept { return *data_; }
	constexpr ConstReference back() const noexcept { return *(data_ + size_); }

	constexpr Range<T> slice(Size pos, Size size) const noexcept { return {data_ + pos, size}; }

	///\{
	///Those function can be used to copy the range to an owned container.
	///range.as<std::vector>() will convert into an vector of the range type (T).
	///range.as<std::vector<float>>() will convert into an float-vector (if possible).
	template<typename C> C as() const { return C(data_, data_ + size_); }
	template<template<class...> class C> C<T> as() const { return C<T>(data_, data_ + size_); }
	///\}

protected:
	ConstPointer data_ = nullptr;
	Size size_ = 0;
};

namespace detail
{

template<typename T, typename C>
struct ValidContainer<T, C,
	typename std::enable_if<
		std::is_convertible<
			decltype(*std::declval<C>().data()),
			const T&
		>::value &&
		std::is_convertible<
			decltype(std::declval<C>().size()),
			std::size_t
		>::value
	>::type
> { using type = void; };

}

}

#endif //header guard
