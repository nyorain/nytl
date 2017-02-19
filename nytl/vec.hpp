// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file A Vector implementation class.

#pragma once

#ifndef NYTL_INCLUDE_VEC
#define NYTL_INCLUDE_VEC

#include <nytl/fwd/vec.hpp> // nytl::Vec typedefs
#include <nytl/vecOps.hpp> // nytl::vec::print

#include <cstdint> // std::size_t
#include <iterator> // std::reverse_iterator
#include <vector> // std::vector

// TODO: C++17: remove custom constructors since they can be aggregate initialized

namespace nytl {

/// \brief Base class for different Vector implementations.
/// \tparam V The Vector type derived from this class. It must take two parameters, a dimension
/// and a type.
/// The deriving class must only implement a few function to make VecBase work:
///   - <T* data()> and <const T* data() const> that point to the contigouos vector data
///   - <Size T::size()> that returns the current size.
/// \tparam D The dimension of the vector type deriving.
/// \tparam D The value type of the vector type deriving.
template<template<std::size_t, typename> typename V, std::size_t D, typename T>
class VecBase {
public:
	using Size = std::size_t;
	using Value = T;
	using VecType = V<D, T>;
	using Reference = Value&;
	using ConstReference = const Value&;
	using Pointer = Value*;
	using ConstPointer = const Value*;
	using Iterator = Pointer;
	using ConstIterator = ConstPointer;
	using ReverseIterator = std::reverse_iterator<Iterator>;
	using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
	using Difference = std::ptrdiff_t;

	template<Size OD, typename OT> using Rebind = V<OD, OT>;
	static constexpr V<D, T> create(Size) { return V<D,T>{}; }

	static constexpr auto dim = D;

public:
	template<Size OD, typename OT>
	constexpr VecType& operator+=(const V<OD, OT>& lhs)
	{
		for(Size i = 0; i < min(lhs.size(), vSelf().size()); ++i) (*this)[i] += lhs[i];
		return *this;
	}

	template<Size OD, typename OT>
	constexpr VecType& operator-=(const V<OD, OT>& lhs)
	{
		for(Size i = 0; i < min(lhs.size(), vSelf().size()); ++i) (*this)[i] -= lhs[i];
		return *this;
	}

	template<typename OT>
	constexpr VecType& operator*=(OT lhs)
		{ for(auto& val : *this) val *= lhs; return *this; }

	template<typename OT>
	constexpr VecType& operator/=(OT lhs)
		{ for(auto& val : *this) val /= lhs; return *this; }

	constexpr Iterator begin() noexcept { return &(*this)[0]; }
	constexpr ConstIterator begin() const noexcept { return &(*this)[0]; }
	constexpr ConstIterator cbegin() const noexcept { return &(*this)[0]; }

	constexpr Iterator end() noexcept { return begin() + vSelf().size(); }
	constexpr ConstIterator end() const noexcept { return begin() + vSelf().size(); }
	constexpr ConstIterator cend() const noexcept { return begin() + vSelf().size(); }

	constexpr ReverseIterator rbegin() noexcept { return ReverseIterator(end()); }
	constexpr ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator(cend()); }
	constexpr ConstReverseIterator crbegin() const noexcept { return ConstReverseIterator(cend()); }

	constexpr ReverseIterator rend() noexcept { return ReverseIterator(begin()); }
	constexpr ConstReverseIterator rend() const noexcept { return ConstReverseIterator(cbegin()); }
	constexpr ConstReverseIterator crend() const noexcept { return ConstReverseIterator(cbegin()); }

	constexpr Reference operator[](Size i){ return (vSelf().data())[i]; }
	constexpr ConstReference operator[](Size i) const { return (vSelf().data())[i]; }

	constexpr Reference at(Size i) { check(i); return (*this)[i]; }
	constexpr ConstReference at(Size i) const { check(i); return (*this)[i]; }

	constexpr Reference front() noexcept { return (*this)[0]; }
	constexpr ConstReference front() const noexcept { return (*this)[0]; }

	constexpr Reference back() noexcept { return (*this)[dim - 1]; }
	constexpr ConstReference back() const noexcept { return (*this)[dim - 1]; }

	template<Size OS, typename OT>
	constexpr explicit operator Vec<OS, OT>() const
	{
		auto ret = Vec<OS, OT>::create(vSize());
		for(auto i = 0u; i < std::min(ret.size(), vSize()); ++i) ret[i] = (*this)[i];
		for(auto i = std::min(ret.size(), vSize()); i < ret.size(); ++i) ret[i] = {};
		return ret;
	}

private:
	constexpr void check(Size i) const { if(i >= vSize()) throw std::out_of_range("nytl::Vec"); }
	constexpr auto& vSelf() { return static_cast<VecType&>(*this); }
	constexpr const auto& vSelf() const { return static_cast<const VecType&>(*this); }
	constexpr auto vSize() const { return vSelf().size(); }
};

/// \brief A static-sized Vector template on the stack.
/// Default implementation with an arbitary size.
/// \notes Since this vector is created on the stack, its dimensions should not be too large,
/// otherwise it may result in a stack overflow.
template<std::size_t D, typename T>
class Vec : public VecBase<Vec, D, T> {
public:
	T data_[D];

	// TODO: C++17
	constexpr Vec(std::initializer_list<T> list)
	{
		if(list.size() != size()) throw std::logic_error("nytl::Vec::Vec: Invalid init list");
		for(auto i = 0u; i < size(); ++i) data_[i] = *(list.begin() + i);
	}

	constexpr Vec() = default;

	constexpr T* data() { return data_; }
	constexpr const T* data() const { return data_; }

	static constexpr std::size_t size() { return D; }
};

/// \brief 2-dimensional nytl::Vec specialization.
/// Its values can be accessed over the x and y members.
template<typename T>
class Vec<2, T> : public VecBase<Vec, 2, T> {
public:
	[[deprecated("Use operator[]. Will be removed in next version")]]
	T x;

	[[deprecated("Use operator[]. Will be removed in next version")]]
	T y;

	constexpr Vec() = default;
	constexpr Vec(T xx, T yy) : x(xx), y(yy) {}

	constexpr T* data() { return &(*this[0]); }
	constexpr const T* data() const { return &(*this[0]); }

	static constexpr std::size_t size() { return 2; }
};

/// \brief 3-dimensional nytl::Vec specialization.
/// Its values can be accessed over the x, y and z members.
template<typename T>
class Vec<3, T> : public VecBase<Vec, 3, T> {
public:
	[[deprecated("Use operator[]. Will be removed in next version")]]
	T x;

	[[deprecated("Use operator[]. Will be removed in next version")]]
	T y;

	[[deprecated("Use operator[]. Will be removed in next version")]]
	T z;

	constexpr Vec() = default;
	constexpr Vec(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}

	constexpr T* data() { return &(*this[0]); }
	constexpr const T* data() const { return &(*this[0]); }

	static constexpr std::size_t size() { return 3; }
};

/// \brief 4-dimensional nytl::Vec specialization.
/// Its values can be accessed over the x, y, z and w members.
template<typename T>
class Vec<4, T> : public VecBase<Vec, 4, T> {
public:
	[[deprecated("Use operator[]. Will be removed in next version")]]
	T x;

	[[deprecated("Use operator[]. Will be removed in next version")]]
	T y;

	[[deprecated("Use operator[]. Will be removed in next version")]]
	T z;

	[[deprecated("Use operator[]. Will be removed in next version")]]
	T w;

	constexpr Vec() = default;
	constexpr Vec(T xx, T yy, T zz, T ww) : x(xx), y(yy), z(zz), w(ww) {}

	constexpr T* data() { return &(*this[0]); }
	constexpr const T* data() const { return &(*this[0]); }

	static constexpr std::size_t size() { return 4; }
};

constexpr auto dynamicSize = 0u;

/// \brief Dynamic-sized nytl::Vec specialization.
template<typename T>
class Vec<dynamicSize, T> : public VecBase<Vec, dynamicSize, T> {
public:
	static Vec<dynamicSize, T> create(std::size_t size)
	{
		Vec<dynamicSize,T> ret;
		ret.data_.resize(size);
		return ret;
	}

	std::vector<T> data_;

	T* data() { return data_.data(); }
	const T* data() const { return data_.data(); }

	std::size_t size() const { return data_.size(); }
};

// - operators -
template<std::size_t D1, std::size_t D2, typename T1, typename T2>
constexpr auto operator+(const Vec<D1, T1>& a, const Vec<D2, T2>& b)
{
	auto ret = Vec<std::min(D1, D2), decltype(a[0] + b[0])>::create(std::min(a.size(), b.size()));
	for(auto i = 0u; i < ret.size(); ++i)
		ret[i] = a[i] + b[i];
	return ret;
}

template<std::size_t D1, std::size_t D2, typename T1, typename T2>
constexpr auto operator-(const Vec<D1, T1>& a, const Vec<D2, T2>& b)
{
	auto ret = Vec<std::min(D1, D2), decltype(a[0] - b[0])>::create(std::min(a.size(), b.size()));
	for(auto i = 0u; i < ret.size(); ++i)
		ret[i] = a[i] - b[i];
	return ret;
}

template<std::size_t D, typename T>
constexpr auto operator-(const Vec<D, T>& a)
{
	auto ret = Vec<D, decltype(-a[0])>::create(a.size());
	for(auto i = 0u; i < ret.size(); ++i)
		ret[i] = -a[i];
	return ret;
}

template<std::size_t D, typename F, typename T>
constexpr auto operator*(const F& f, const Vec<D, T>& a)
{
	auto ret = Vec<D, decltype(f * a[0])>::create(a.size());
	for(auto i = 0u; i < ret.size(); ++i)
		ret[i] = f * a[i];
	return ret;
}

template<std::size_t D, typename T1, typename T2>
constexpr auto operator==(const Vec<D, T1>& a, const Vec<D, T2>& b)
{
	for(auto i = 0u; i < a.size(); ++i)
		if(a[i] != b[i]) return false;
	return true;
}

template<std::size_t D, typename T1, typename T2>
constexpr auto operator!=(const Vec<D, T1>& a, const Vec<D, T2>& b)
{
	return !(a == b);
}

template<std::size_t D, typename T>
std::ostream& operator<<(std::ostream& os, const Vec<D, T>& a)
{
	return vec::print(os, a);
}

} // namespace nytl

#endif // header guard
