// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_TUPLEMAP_INL
#define NYTL_INCLUDE_TUPLEMAP_INL

#include <nytl/tmp.hpp>
#include <nytl/bits/apply.inl>

#include <tuple>
#include <utility>

namespace nytl
{

template<typename A, typename B>
struct ConvertException : public std::true_type {};

template<typename A, typename B>
struct IsConvertible
{
	using RawA = std::remove_reference_t<std::remove_cv_t<A>>;
	using RawB = std::remove_reference_t<std::remove_cv_t<B>>;

	static constexpr auto value = std::is_convertible<A, B>::value &&
		ConvertException<RawA, RawB>::value;
};

namespace detail
{

template<typename OT, typename NT, std::size_t I = 0>
struct TupleMapImpl; //unspecified

template<typename... OA, typename... NA, std::size_t I>
struct TupleMapImpl<std::tuple<OA...>, std::tuple<NA...>, I>
{
	using OrgTuple = std::tuple<OA...>;
	using NewTuple = std::tuple<NA...>;

	constexpr static const bool value = IsConvertible<
			typename std::tuple_element<0, OrgTuple>::type,
			typename std::tuple_element<0, NewTuple>::type
		>::value;

	using type = typename std::conditional<
		value,  //condition
		SeqPrepend< //match
			typename TupleMapImpl<
				TupleEraseFirst<OrgTuple>,
				TupleEraseFirst<NewTuple>,
				I + 1
			>::type,
			I
		>,
		typename TupleMapImpl< //no match
			TupleEraseFirst<OrgTuple>,
			NewTuple,
			I + 1
		>::type
	>::type;
};

template<typename... OrgLeft, std::size_t idx>
struct TupleMapImpl<std::tuple<OrgLeft...>, std::tuple<>, idx>
{
	using type = std::index_sequence<>;
};

template<std::size_t idx>
struct TupleMapImpl<std::tuple<>, std::tuple<>, idx>
{
	using type = std::index_sequence<>;
};

template<typename... NewLeft, std::size_t idx>
struct TupleMapImpl<std::tuple<>, std::tuple<NewLeft...>, idx>
{
	using type = std::index_sequence<std::size_t(-1)>;
};


//TupleMap
template<typename OrgTup, typename NewTup, typename R,
	typename Seq = typename detail::TupleMapImpl<OrgTup, NewTup>::type,
	typename Seq2 = std::make_index_sequence<Seq::size()>>
struct TupleMap; //unspecified

template<typename... OA, typename... NA, std::size_t... I, std::size_t... N, typename R>
struct TupleMap<std::tuple<OA...>, std::tuple<NA...>, R,
	std::index_sequence<I...>, std::index_sequence<N...>>
{
	using NewTup = typename std::tuple<NA...>;
	using OrgTup = typename std::tuple<OA...>;
	using Seq = std::index_sequence<I...>;

	template<typename F>
	static constexpr auto map(F f, OA... args) noexcept
	{
		unused(args...); //gcc bug, warns they are not used
		OrgTup orgTup(std::forward<OA>(args)...);

		return apply(f, NewTup(
			static_cast<std::tuple_element_t<N, NewTup>>(
			std::forward<std::tuple_element_t<I, OrgTup>>(
			std::get<I>(orgTup)))...));
	}
};

//void specialization
template<typename... OA, typename... NA, std::size_t... I, std::size_t... N>
struct TupleMap<std::tuple<OA...>, std::tuple<NA...>, void,
	std::index_sequence<I...>, std::index_sequence<N...>>
{
	using NewTup = typename std::tuple<NA...>;
	using OrgTup = typename std::tuple<OA...>;
	using Seq = std::index_sequence<I...>;

	template<typename F>
	static constexpr auto map(F f, OA... args) noexcept
	{
		unused(args...); //gcc bug, warns they are not used
		OrgTup orgTup(std::forward<OA>(args)...);

		apply(f, NewTup(
			static_cast<std::tuple_element_t<N, NewTup>>(
			std::forward<std::tuple_element_t<I, OrgTup>>(
			std::get<I>(orgTup)))...));
	}
};


} //detail

} //nytl

#endif //header guard
