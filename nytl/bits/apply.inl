// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_APPLY_INL
#define NYTL_INCLUDE_APPLY_INL

#include <tuple>
#include <utility>
#include <functional>

//experimental::tuple::apply example implementation
//will be in c++17 standard
//http://en.cppreference.com/w/cpp/experimental/apply

namespace nytl
{
namespace detail
{

template <class F, class Tuple, std::size_t... I>
constexpr auto apply_impl(F&& f, Tuple&& t, std::index_sequence<I...> )
	-> decltype(std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...))
{
	//C++17:
	// return std::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
	return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
}

}

template <class F, class Tuple>
constexpr auto apply(F&& f, Tuple&& t)
	-> decltype(detail::apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
		std::make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>{}>{}))
{
	return detail::apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
		std::make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>{}>{});
}

}

#endif //header guard
