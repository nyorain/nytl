// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Header that implementes several useful metaprogramming templates.

//TODO: naming

#pragma once

#ifndef NYTL_INCLUDE_TMP_HPP
#define NYTL_INCLUDE_TMP_HPP

#include <nytl/bits/tmpUtil.inl>
#include <utility>
#include <type_traits>
#include <ostream>

namespace nytl
{

//TUPLE
//tuple_erase_first
template<typename T> struct TupleEraseFirstT;

template<template<typename...> class T, typename Head, typename ... Tail>
struct TupleEraseFirstT<T<Head, Tail...>>
{
	using type = T<Tail...>;
};

template<typename T> using TupleEraseFirst = typename TupleEraseFirstT<T>::type;

//tuple_append
template<typename T, typename Append> struct TupleAppendT;

template<template<typename...> class T, typename... Body, typename Append>
struct TupleAppendT<T<Body...>, Append>
{
	using type = T<Body..., Append>;
};

template<typename T, typename Append> using TupleAppend = typename TupleAppendT<T, Append>::type;

//tuple_prepend
template<typename T, typename Append> struct TuplePrependT;

template<template<typename...> class T, typename... Body, typename Prepend>
struct TuplePrependT<T<Body...>, Prepend>
{
	using type = T<Prepend, Body...>;
};

template<typename T, typename Prepend> using TuplePrepend =
	typename TuplePrependT<T, Prepend>::type;

//type_tuple
template<typename T, std::size_t size, template<typename...> class Tuple = std::tuple>
struct TypeTupleT
{
	using type = TuplePrepend<typename TypeTupleT<T, size - 1>::type, T>;
};

template<typename T, template<typename...> class Tuple> struct TypeTupleT<T, 1, Tuple>
{
	using type = Tuple<T>;
};

template<typename T, std::size_t size, template<typename...> class Tuple = std::tuple>
using TypeTuple = typename TypeTupleT<T, size, Tuple>::type;




//SEQEUNCE
///\tparam Default Whether the value of this should be 0 if N is zero and there are no more
///sequence values
template<std::size_t N, typename Seq, bool D = false> struct SeqGetT;

template<std::size_t N, typename I, template<typename, I...> class T, bool D, I Head, I... Tail>
struct SeqGetT<N, T<I, Head, Tail...>, D>
{
	static constexpr auto value = SeqGetT<N - 1, T<I, Tail...>>::value;
};

template<typename I, template<typename, I...> class T, bool D, I Head, I... Tail>
struct SeqGetT<0, T<I, Head, Tail...>, D>
{
	static constexpr auto value = Head;
};

template<typename I, template<typename, I...> class T>
struct SeqGetT<0, T<I>, true>
{
	static constexpr auto value = 0;
};

template<std::size_t N, typename T, bool D = false> constexpr auto SeqGet = SeqGetT<N, T, D>::value;

//seq_erase_first
template<typename T> struct SeqEraseFirstT;

template<typename I, template<typename, I...> class T, I Head, I... Tail>
struct SeqEraseFirstT<T<I, Head, Tail...>>
{
	using type = T<I, Tail...>;
};

template<typename T> using SeqEraseFirst = typename SeqEraseFirstT<T>::type;


//seq_append
template<typename T, typename T::value_type Append> struct SeqAppendT;

template<typename I, template<typename, I...> class T, I... Body, I Append>
struct SeqAppendT<T<I, Body...>, Append>
{
	using type = T<I, Body..., Append>;
};

template<typename T, typename T::value_type Append> using SeqAppend =
	typename SeqAppendT<T, Append>::type;

//seq_prepend
template<typename T, typename T::value_type Prepend> struct SeqPrependT;

template<typename I, template<typename, I...> class T, I... Body, I Prepend>
struct SeqPrependT<T<I, Body...>, Prepend>
{
	using type = T<I, Prepend, Body...>;
};

template<typename T, typename T::value_type Prepend> using SeqPrepend =
	typename SeqPrependT<T, Prepend>::type;

//seq_merge
template<typename A, typename B> struct SeqMergeT;
template<typename I, template<typename, I...> class T, I... IdxA, I... IdxB>
struct SeqMergeT<T<I, IdxA...>, T<I, IdxB...>>
{
	using type = T<I, IdxA..., IdxB...>;
};

template<typename A, typename B> using SeqMerge = typename SeqMergeT<A, B>::type;

//seq_merge_renumber
template<typename A, typename B> struct SeqMergeRenumberT;
template<typename I, template<typename, I...> class T, I... IdxA, I... IdxB>
struct SeqMergeRenumberT<T<I, IdxA...>, T<I, IdxB...>>
{
	using type = T<I, IdxA..., (sizeof...(IdxA) + IdxB)...>;
};

template<typename A, typename B> using SeqMergeRenumber =
	typename SeqMergeRenumberT<A, B>::type;

//seq_print
template<typename T> struct SeqPrint;
template<typename I, template<typename, I...> class T, I... idx>
struct SeqPrint<T<I, idx...>>
{
	static std::ostream& print(std::ostream& o)
	{
		Expand{((void) (o << idx << " "), 0)...};
		return o;
	};
};

template<typename T> void seqPrint(std::ostream& os)
{
	SeqPrint<T>::print(os);
}

}

#endif //header guard
