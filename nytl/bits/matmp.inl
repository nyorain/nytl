// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_MATMP_INL
#define NYTL_INCLUDE_MATMP_INL

#include <nytl/bits/tmpUtil.inl>

#include <cstdint>
#include <memory>

namespace nytl
{

namespace detail
{

//makeRowRefVec
template<std::size_t Size, typename Seq = std::make_index_sequence<Size>>
struct MakeRowRefVec;

template<std::size_t Size, std::size_t... I>
struct MakeRowRefVec<Size, std::index_sequence<I...>>
{
	template<std::size_t R, std::size_t C, typename P>
	static RefVec<sizeof...(I), P> call(Vec<R, Vec<C, P>>& v, std::size_t i)
	{
		return RefVec<sizeof...(I), P>(v[I][i]...);
	}
};

//makeRowVec
template<std::size_t Size, typename Seq = std::make_index_sequence<Size>>
struct MakeRowVec;

template<std::size_t Size, std::size_t... I>
struct MakeRowVec<Size, std::index_sequence<I...>>
{
	template<size_t R, size_t C, typename P>
	static Vec<sizeof...(I), P> call(const Vec<R, Vec<C, P>>& v, std::size_t i)
	{
		return Vec<sizeof...(I), P>(v[I][i]...);
	}
};

//initMat
template<std::size_t Size, typename Seq = std::make_index_sequence<Size>>
struct InitMatData;

template<std::size_t Size, std::size_t... I>
struct InitMatData<Size, std::index_sequence<I...>>
{
	template<size_t R, size_t C, typename P, typename... Args>
	static void call(Vec<R, Vec<C, P>>& v, std::tuple<Args...> args)
	{
		Expand{0, ((v[I / C][I % C] = std::get<I>(args)), 0)... };
	}

	template<size_t R, size_t C, typename P>
	static void call(Vec<R, Vec<C, P>>& v, const P& val)
	{
		Expand{0, ((v[I / C][I % C] = val), 0)... };
	}
};

//copyMatData
template<std::size_t Size, typename Seq = std::make_index_sequence<Size>>
struct CopyMatData;

template<std::size_t Size, size_t... I>
struct CopyMatData<Size, std::index_sequence<I...>>
{
	template<size_t R, size_t C, typename P>
	static std::unique_ptr<P[]> call(const Vec<R, Vec<C, P>>& v)
	{
		return std::unique_ptr<P[]>(new P[R * C]{v[I / C][I % R]...});
	}
};

//makeRowRefVec
template<std::size_t Size, typename Seq = std::make_index_sequence<Size>>
struct MatTuple;

template<std::size_t Size, size_t... I>
struct MatTuple<Size, std::index_sequence<I...>>
{
	template<size_t R, size_t C, typename P>
	static TypeTuple<P, sizeof...(I)> call(const Vec<R, Vec<C, P>>& v)
	{
		return TypeTuple<P, sizeof...(I)>{v[I / C][I % R]...};
	}
};

}

}

#endif //header guard
