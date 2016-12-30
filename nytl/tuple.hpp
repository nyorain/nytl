// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Various useful tuple meta operations.

#pragma once

#ifndef NYTL_INCLUDE_TUPLE
#define NYTL_INCLUDE_TUPLE

#include <tuple> // std::tuple
#include <type_traits> // std::enable_if
#include <cstdlib> // std::size_t

namespace nytl {
namespace detail {

template<typename Tuple> struct TupleEraseFirstT;
template<typename Head, typename ... Tail>
struct TupleEraseFirstT<std::tuple<Head, Tail...>> {
	using type = std::tuple<Tail...>;
};

template<typename Tuple, typename Append> struct TupleAppendT;
template<typename... Body, typename Append>
struct TupleAppendT<std::tuple<Body...>, Append> {
	using type = std::tuple<Body..., Append>;
};

template<typename Tuple, typename Append> struct TuplePrependT;
template<typename... Body, typename Prepend>
struct TuplePrependT<std::tuple<Body...>, Prepend> {
	using type = std::tuple<Prepend, Body...>;
};

template<typename T, std::size_t size>
struct TypeTupleT {
	using type = typename TuplePrependT<typename TypeTupleT<T, size - 1>::type, T>::type;
};
template<typename T>
struct TypeTupleT<T, 1> {
	using type = std::tuple<T>;
};

}

/// Erases the first type from std::tuple type 'T'.
/// \module utility
template<typename T>
using TupleEraseFirst = typename detail::TupleEraseFirstT<T>::type;

/// Appends type 'A' to std::tuple type 'T'.
/// \module utility
template<typename T, typename A>
using TupleAppend = typename detail::TupleAppendT<T, A>::type;

/// Prepends type 'P' to std::tuple type 'T'.
/// \module utility
template<typename T, typename P>
using TuplePrepend = typename detail::TuplePrependT<T, P>::type;

/// A std::tuple that contains 'C' entries of type 'T'.
/// \module utility
template<typename T, std::size_t C>
using TypeTuple = typename detail::TypeTupleT<T, C>::type;

/// \ingroup utility
/// Can be used to assure the variadic parameter pack 'Args' has size 'C' and each parameter
/// can be converted to type 'T'.
template<typename T, std::size_t C, typename... Args>
using EnableIfVariadicType =
	typename std::enable_if_t<std::is_convertible<std::tuple<Args...>, TypeTuple<T, C>>::value>;

} // namespace nytl

#endif // header guard
