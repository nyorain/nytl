// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file Contains various utility functions for dealing with Vec objects.

#pragma once

#ifndef NYTL_INCLUDE_VEC_OPS
#define NYTL_INCLUDE_VEC_OPS

#include <nytl/vec.hpp>

namespace nytl
{

//utility
///\relates nytl::Vec
///\return The sum of all Vector components.
template<std::size_t D, typename T>
constexpr auto sum(const Vec<D, T>& v) -> decltype(v[0] + v[0])
{
    auto ret = decltype(v[0] + v[0]) {};
    for(auto& val : v) ret += val;
    return ret;
}

///\relates nytl::Vec
///\return The product of all Vector components.
template<std::size_t D, typename T>
constexpr auto multiply(const Vec<D, T>& v) -> decltype(v[0] * v[0])
{
    auto ret = decltype(v[0] * v[0]) (1);
    for(auto& val : v) ret *= val;
    return ret;
}

///\relates nytl::Vec
///\return The length of the Vector (square-root of the sum of all its component squared).
template<std::size_t D, typename T>
constexpr auto length(const Vec<D, T>& v) -> decltype(sqrt(v[0] * v[0]))
{
    decltype(v[0] * v[0]) ret{};
    for(auto& val : v) ret += val * val;
    return std::sqrt(ret);
}

///\relates nytl::Vec
///Alias function for length.
///\return the length (norm) of the given Vector.
template<std::size_t D, typename T>
constexpr auto norm(const Vec<D, T>& v) -> decltype(length(v))
{
    return length(v);
}

///\relates nytl::Vec
///\return The dot product of the given Vectors.
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
constexpr auto dot(const Vec<DA, TA>& va, const Vec<DB, TB>& vb) -> decltype(sum(va * vb))
{
    return sum(va * vb);
}

///\relates nytl::Vec
///Alias function for dot.
///\return The dot (scalar) product of the given Vectors.
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
constexpr auto scalar(const Vec<DA, TA>& va, const Vec<DB, TB>& vb) -> decltype(sum(va * vb))
{
    return sum(va * vb);
}

///\relates nytl::Vec
///\return The cross product for 2 3-Densional Vectors.
template<typename TA, typename TB>
constexpr auto cross(const Vec<3, TA>& va, const Vec<3, TB>& vb) -> Vec<3, decltype(va[0] * vb[0])>
{
    return Vec<3, decltype(va[0] * vb[0])>
		{
			va[2] * va[3] - va[3] * vb[2],
			va[3] * va[1] - va[1] * vb[3],
			va[1] * va[2] - va[2] * vb[1]
		};
}

///\relates nytl::Vec
///\return The angle between 2 Vecs in radiant form. Returns always the smaller one; angle <= PI.
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
constexpr double angle(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    return std::acos(sum(va * vb) / (length(va) * length(vb)));
}

///\relates nytl::Vec
///\return The smallest angle between two Lines with the given Vectors as diRection in radiant
///form. The Returned angle is always <= PI/2.
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
constexpr double smallerAngle(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    return std::acos(abs(sum(va * vb)) / (length(va) * length(vb)));
}

//todo: cangle for 3-Densional (or all) Vectors
///\relates nytl::Vec
///\return The absolute, clockwise angle between two 2-Densional Vectors in radian form.
template<typename TA, typename TB>
constexpr double cangle(const Vec<2, TA>& va, const Vec<2, TB>& vb)
{
    auto val = atan2(va.y, va.x) - atan2(vb.y, vb.x);
    if(val <= 0) return (2 * cPi) + val;
    return val;
}

///\relates nytl::Vec
///\return A normalized (i.e. length = 1, preserving the direction) copy of the given Vec.
template<std::size_t D, typename T>
constexpr auto normalize(const Vec<D, T>& va) -> decltype(va / length(va))
{
    return va / length(va);
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>
constexpr auto pow(Vec<D, TA> base, const TB& exp) -> Vec<D, decltype(std::pow(base[0], exp))>
{
	for(auto& val : base)
	   val = std::pow(val, exp);

	return base;
}

///\relates nytl::Vec
///\return The distance between two points represented as Vecs.
template<std::size_t D, typename T>
constexpr auto distance(const Vec<D, T>& va, const Vec<D, T>& vb) -> decltype(length(va - vb))
{
    return length(vb - va);
}

///\relates nytl::Vec
template<std::size_t D, typename T>
constexpr Vec<D, double> radians(Vec<D, T> va)
{
    for(auto& val : va) val = radians(val);
    return va;
}

///\relates nytl::Vec
template<std::size_t D, typename T>
constexpr Vec<D, double> degrees(Vec<D, T> va)
{
    for(auto& val : va) val = degrees(val);
    return va;
}

///\relates nytl::Vec
template<std::size_t D, typename T>
constexpr Vec<D, T> abs(Vec<D, T> va)
{
	using std::abs;
    for(auto& val : va) val = abs(val);
    return va;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB, typename Tc>
constexpr Vec<D, TA> clamp(Vec<D, TA> val, const Vec<D, TB>& minVal, const Vec<D, Tc>& maxVal)
{
    for(std::size_t i(0); i < min(min(val.size(), minVal.size()), maxVal.size()); ++i)
		val[i] = clamp(val[i], minVal[i], maxVal[i]);
    return val;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB, typename Tc>
constexpr Vec<D, TA> clamp(Vec<D, TA> val, const TB& minVal, const Tc& maxVal)
{
    for(auto& v : val) v = clamp(v, minVal, maxVal);
    return val;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>
constexpr Vec<D, TA> mix(Vec<D, TA> x, const Vec<D, TA>& y, const Vec<D, TB>& a)
{
    for(std::size_t i(0); i < min(min(x.size(), y.size()), a.size()); ++i)
		x[i] = mix(x[i], y[i], a[i]);
    return x;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>
constexpr Vec<D, TA> mix(Vec<D, TA> x, const Vec<D, TA>& y, const TB& a)
{
    for(std::size_t i(0); i < min(min(x.size(), y.size()), a.size()); ++i)
		x[i] = mix(x[i], y[i], a);
    return x;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>
constexpr Vec<D, TA> mix(const TA& x, const TA& y, const Vec<D, TB>& a)
{
	Vec<D, TA> ret(a.size());
    for(std::size_t i(0); i < a.size(); ++i) ret[i] = mix(x, y, a[i]);
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>
constexpr auto smallest(const Vec<D, T>& a)
{
    auto ret = a[0];
    for(auto& val : a)
		if(val < ret) ret = val;

    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>
constexpr auto greatest(const Vec<D, T>& a)
{
    auto ret = a[0];
    for(auto& val : a)
        if(val > ret) ret = val;

    return ret;
}

//boolean Vec operations
///\relates nytl::Vec
template<std::size_t D>
constexpr bool any(const Vec<D, bool>& v)
{
    for(auto val : v) if(val) return true;
    return false;
}

///\relates nytl::Vec
template<std::size_t D>
constexpr bool all(const Vec<D, bool>& v)
{
    for(auto val : v) if(!val) return false;
    return true;
}

///\relates nytl::Vec
template<std::size_t D>
constexpr bool none(const Vec<D, bool>& v)
{
    for(auto val : v) if(val) return false;
    return true;
}

///\relates nytl::Vec
///Helper function accessing the nytl::Vec::subvec member template.
template<std::size_t N, std::size_t D, typename T>
constexpr Vec<N, T> subVec(const Vec<D, T>& va, std::size_t pos = 0)
{
	return va.template subVec<N>(pos);
}

///\relates nytl::Vec
template<std::size_t D, typename T>
constexpr Vec<D, T> operator!(Vec<D, T> v)
{
    for(auto& val : v) val = !val;
    return v;
}

//component-wise
///\relates nytl::Vec
template<std::size_t D, typename T>
constexpr Vec<D, T> max(const Vec<D, T>& va, const Vec<D, T>& vb)
{
    Vec<D, T> ret(min(va.size(), vb.size()));
    for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
        ret[i] = max(va[i], vb[i]);

    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>
constexpr Vec<D, T> max(const Vec<D, T>& va, const T& value)
{
    Vec<D, T> ret(va.size());
    for(std::size_t i(0); i < va.size(); ++i)
        ret[i] = max(va[i], value);

    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>
constexpr Vec<D, T> min(const Vec<D, T>& va, const Vec<D, T>& vb)
{
    Vec<D, T> ret(min(va.size(), vb.size()));
    for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
        ret[i] = min(va[i], vb[i]);

    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>
constexpr Vec<D, T> min(const Vec<D, T>& va, const T& value)
{
    Vec<D, T> ret(va.size());
    for(std::size_t i(0); i < va.size(); ++i)
        ret[i] = min(va[i], value);

    return ret;
}

}

#endif //header guard
