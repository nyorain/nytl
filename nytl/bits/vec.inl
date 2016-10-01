// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_VEC_INL
#define NYTL_INCLUDE_VEC_INL

namespace detail
{

constexpr std::size_t dMin(std::size_t a, std::size_t b)
	{ return (a == dynamicSize || b == dynamicSize) ? dynamicSize : std::min(a, b); }

constexpr std::size_t dMax(std::size_t a, std::size_t b)
	{ return (a == dynamicSize || b == dynamicSize) ? dynamicSize : std::max(a, b); }

}

//general operators
///\relates nytl::Vec
///Prints all values individually to an output stream.
template<std::size_t D, typename T> std::ostream&
operator<<(std::ostream& os, const Vec<D, T>& obj)
{
	const char* c = "";

	os << "(";

	for(const auto& val : obj)
	{
		os << c << val;
		c = "; ";
	}

	os << ")";

	return os;
}

///\relates nytl::Vec
///Loads all components from an input stream.
template<std::size_t D, typename T> std::istream&
operator>>(std::istream& is, Vec<D, T>& obj)
{
	for(auto& val : obj)
	{
		is >> val;
	}

	return is;
}

///\relates nytl::Vec
template<std::size_t D, typename T>
constexpr Vec<D, T> operator!(Vec<D, T> v)
{
	for(auto& val : v) val = !val;
	return v;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename O>
auto operator+(const Vec<D, T>& mVec, const O& other)
	-> Vec<D, decltype(mVec[0] + other)>
{
	Vec<D, decltype(mVec[0] + other)> ret = mVec;
	ret += other;
	return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename O>
auto operator+(const O& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other + mVec[0])>
{
	Vec<D, decltype(mVec[0] + other)> ret = mVec;
	ret += other;
	return ret;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>
auto operator+(const Vec<DA, TA>& a, const Vec<DB, TB>& b)
	-> Vec<detail::dMax(DA, DB), decltype(a[0] + b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] + b[0])>;

	auto ret = static_cast<V>(a);
	ret += static_cast<V>(b);
	return ret;
}

//-
///\relates nytl::Vec
template<std::size_t D, typename T, typename O>
auto operator-(Vec<D, T> mVec, const O& other)
	-> Vec<D, decltype(mVec[0] - other)>
{
	mVec -= other;
	return mVec;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename OT>
auto operator-(const OT& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other - mVec[0])>
{
	Vec<D, decltype(other - mVec[0])> ret(mVec.size());
	for(std::size_t i(0); i < ret.size(); ++i)
		ret[i] = other - mVec[i];

	return mVec;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>
auto operator-(const Vec<DA, TA>& a, const Vec<DB, TB>& b)
	-> Vec<detail::dMax(DA, DB), decltype(a[0] - b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] - b[0])>;

	auto ret = static_cast<V>(a);
	ret -= static_cast<V>(b);
	return ret;
}

//*
///\relates nytl::Vec
template<std::size_t D, typename T, typename O>
auto operator*(const Vec<D, T>& mVec, const O& other)
	-> Vec<D, decltype(mVec[0] * other)>
{
	Vec<D, decltype(mVec[0] * other)> ret = mVec;
	ret *= other;
	return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename O>
auto operator*(const O& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other * mVec[0])>
{
	Vec<D, decltype(mVec[0] * other)> ret = mVec;
	ret *= other;
	return ret;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>
auto operator*(const Vec<DA, TA>& a, const Vec<DB, TB>& b)
	-> Vec<detail::dMax(DA, DB), decltype(a[0] * b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] * b[0])>;

	auto ret = static_cast<V>(a);
	ret *= static_cast<V>(b);
	return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename O>
auto operator/(Vec<D, T> mVec, const O& other)
	-> Vec<D, decltype(mVec[0] / other)>
{
	mVec /= other;
	return mVec;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename OT>
auto operator/(const OT& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other / mVec[0])>
{
	Vec<D, decltype(other / mVec[0])> ret(mVec.size());
	for(std::size_t i(0); i < ret.size(); ++i)
		ret[i] = other / mVec[i];

	return mVec;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>
auto operator/(const Vec<DA, TA>& a, const Vec<DB, TB>& b)
	-> Vec<detail::dMax(DA, DB), decltype(a[0] / b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] / b[0])>;

	auto ret = static_cast<V>(a);
	ret /= static_cast<V>(b);
	return ret;
}

//%
///\relates nytl::Vec
template<std::size_t D, typename T, typename O>
auto operator%(Vec<D, T> mVec, const O& other)
	-> Vec<D, decltype(mVec[0] % other)>
{
	mVec %= other;
	return mVec;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename OT>
auto operator%(const OT& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other % mVec[0])>
{
	Vec<D, decltype(other % mVec[0])> ret(mVec.size());
	for(std::size_t i(0); i < ret.size(); ++i)
		ret[i] = other % mVec[i];

	return mVec;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>
auto operator%(const Vec<DA, TA>& a, const Vec<DB, TB>& b)
	-> Vec<detail::dMax(DA, DB), decltype(a[0] % b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] % b[0])>;

	auto ret = static_cast<V>(a);
	ret %= static_cast<V>(b);
	return ret;
}

//equal
///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA, DB), bool> equal(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
	Vec<detail::dMin(DA, DB), bool> ret(min(va.size(), vb.size()));
	for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
		ret[i] = (va[i] == vb[i]);
	return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>
Vec<D, bool> equal(const Vec<D, TA>& va, const TB& value)
{
	Vec<D, bool> ret(va.size());
	for(std::size_t i(0); i < D; ++i)
		ret[i] = (va[i] == value);
	return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA, DB), bool> notEqual(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
	Vec<detail::dMin(DA, DB), bool> ret(min(va.size(), vb.size()));
	for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
		ret[i] = (va[i] != vb[i]);
	return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>
Vec<D, bool> notEqual(const Vec<D, TA>& va, const TB& value)
{
	Vec<D, bool> ret(va.size());
	for(std::size_t i(0); i < D; ++i)
		ret[i] = (va[i] != value);
	return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA, DB), bool> lessThan(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
	Vec<detail::dMin(DA, DB), bool> ret(min(va.size(), vb.size()));
	for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
		ret[i] = va[i] < vb[i];
	return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<min(DA, DB), bool> greaterThan(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
	Vec<min(DA, DB), bool> ret(min(va.size(), vb.size()));
	for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
		ret[i] = va[i] > vb[i];
	return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA, DB), bool> lessThanEqual(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
	Vec<detail::dMin(DA, DB), bool> ret(min(va.size(), vb.size()));
	for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
		ret[i] = va[i] <= vb[i];
	return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA, DB), bool> greaterThanEqual(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
	Vec<detail::dMin(DA, DB), bool> ret(min(va.size(), vb.size()));
	for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
		ret[i] = va[i] >= vb[i];
	return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>
Vec<D, bool> lessThan(const Vec<D, TA>& va, const TB& value)
{
	Vec<D, bool> ret(va.size());
	for(std::size_t i(0); i < va.size(); ++i)
		ret[i] = va[i] < value;
	return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>
Vec<D, bool> greaterThan(const Vec<D, TA>& va, const TB& value)
{
	Vec<D, bool> ret(va.size());
	for(std::size_t i(0); i < va.size(); ++i)
		ret[i] = va[i] > value;
	return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>
Vec<D, bool> lessThanEqual(const Vec<D, TA>& va, const TB& value)
{
	Vec<D, bool> ret(va.size());
	for(std::size_t i(0); i < va.size(); ++i)
		ret[i] = va[i] <= value;
	return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>
Vec<D, bool> greaterThanEqual(const Vec<D, TA>& va, const TB& value)
{
	Vec<D, bool> ret(va.size());
	for(std::size_t i(0); i < va.size(); ++i)
		ret[i] = va[i] >= value;
	return ret;
}

//todo: allow comparison of Vec and value with operator?
//Vec<D, bool> operator==(Vec<D, TA> v, TB value);
///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA, DB), bool> operator==(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
	return equal(va, vb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA, DB), bool> operator!=(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
	return notEqual(va, vb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA, DB), bool> operator<(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
	return lessThan(va, vb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA, DB), bool> operator>(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
	return greaterThan(va, vb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA,DB), bool> operator<=(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
	return lessThanEqual(va, vb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA, DB), bool> operator>=(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
	return greaterThanEqual(va, vb);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB>
Vec<DA, bool> operator==(const Vec<DA, TA>& va, const TB& b)
{
	return equal(va, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB>
Vec<DA, bool> operator!=(const Vec<DA, TA>& va, const TB& b)
{
	return notEqual(va, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB>
Vec<DA, bool> operator<(const Vec<DA, TA>& va, const TB& b)
{
	return lessThan(va, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB>
Vec<DA, bool> operator>(const Vec<DA, TA>& va, const TB& b)
{
	return greaterThan(va, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB>
Vec<DA, bool> operator<=(const Vec<DA, TA>& va, const TB& b)
{
	return lessThanEqual(va, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB>
Vec<DA, bool> operator>=(const Vec<DA, TA>& va, const TB& b)
{
	return greaterThanEqual(va, b);
}

#endif //header guard
