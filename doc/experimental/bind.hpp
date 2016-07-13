/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 nyorain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <nytl/functionTraits.hpp>
#include <nytl/bits/apply.inl>
#include <type_traits>
#include <functional>

namespace nytl
{

namespace detail
{

template<int N, typename... Ts> using NthTypeOf =
    typename std::tuple_element<N, std::tuple<Ts...>>::type;

template<std::size_t N>
struct nth
{
	template<typename F, typename... T>
	constexpr static auto call(F f, T... t)
	{
		return nth<N - 1>::call(t...);
	}
};

template<>
struct nth<0>
{
	template<typename F, typename... T>
	constexpr static F call(F f, T... t)
	{
		return f;
	}
};

template<std::size_t I, typename C, typename... T> struct GetArgPlaceholder;

template<std::size_t I, typename... T>
struct GetArgPlaceholder<I, std::enable_if_t<std::is_placeholder<
	decltype(nth<I>::call(std::declval<T>()...))>::value>, T...>
{
	static constexpr auto number = std::is_placeholder<NthTypeOf<I, T...>>::value - 1;

	template<typename... VA>
	static auto call(T... t, VA... va)
	{
		return nth<number>::call(va...);
	}
};

template<std::size_t I, typename... T>
struct GetArgPlaceholder<I, std::enable_if_t<!std::is_placeholder<
	decltype(nth<I>::call(std::declval<T>()...))>::value>, T...>
{
	template<typename... VA>
	static auto call(T... t, VA... va)
	{
		return nth<I>::call(t...);
	}
};

///\tparam T: types of the given bind args
template<std::size_t I, typename C, typename... T> struct GetArg;

template<std::size_t I, typename... T>
struct GetArg<I, std::enable_if_t<std::less<void>{}(I, sizeof...(T))>, T...>
{
	///\tparam VA: types of args given to the function call
	template<typename... VA>
	static auto call(T... t, VA... va)
	{
		return GetArgPlaceholder<I, void, T...>::call(t..., va...);
	}
};

template<typename... T, std::size_t I>
struct GetArg<I, std::enable_if_t<std::greater<void>{}(I + 1, sizeof...(T))>, T...>
{
	template<typename... VA>
	static auto call(T... t, VA... va) -> decltype(nth<I - sizeof...(T)>::call(va...))
	{
		return nth<I - sizeof...(T)>::call(va...);
	}
};

template<typename I, typename... A>
struct TupleBuilder;

template<std::size_t... I, typename... A>
struct TupleBuilder<std::index_sequence<I...>, A...>
{
	template<typename... VA>
	static constexpr auto call(A... args, VA... vargs)
	{
		return std::make_tuple(GetArg<I, void, A...>::call(args..., vargs...)...);
	}
};

template<typename... A> constexpr auto PackSize = sizeof...(A);

template<typename T, typename F, typename C = void>
struct Binder;

template<typename F, typename R, typename... A>
struct Binder<R(A...), F,
	std::enable_if_t<std::is_same<R(A...), typename FunctionTraits<F>::Signature>::value>>
{
	template<typename... FA>
	auto bind(const F& func, FA&&... args)
	{
		auto fcopy = func;
		return ([=](auto&&... params)
		{
			using I = std::make_index_sequence<sizeof...(args) + sizeof...(params)>;
			auto tup = TupleBuilder<I, FA...>::call(args..., params...);
			return apply(fcopy, tup);
		});
	};
};

template<typename F, typename... A>
struct Binder<void(A...), F,
	std::enable_if_t<std::is_same<void(A...), typename FunctionTraits<F>::Signature>::value>>
{
	template<typename... FA>
	auto bind(const F& func, FA... args)
	{
		auto fcopy = func;
		return ([=](auto&&... params)
		{
			using I = std::make_index_sequence<sizeof...(A)>;
			auto tup = TupleBuilder<I, FA...>::call(args..., params...);
			apply(fcopy, tup);
		});
	};
};

}

template<typename S, typename F, typename... A>
auto bind(F&& func, A&&... args)
{
	detail::Binder<S, F> binder;
	return binder.bind(func, args...);
}

template<typename F, typename... A>
auto bind(F&& func, A&&... args)
{
	detail::Binder<typename FunctionTraits<F>::Signature, F> binder;
	return binder.bind(func, args...);
}

//example:
//TODO: current problem: the function returned by bind can be called with too many (then unused)
//paramters. Can probably be fixed by replacing TupleBuilder with something recursive that
//performs some kind of implicit checks.
//
// void func(int, int, std::string, int, int);
// void func(float);
// void b()
// {
// 	using namespace std::placeholders;
//
// 	auto f1 = nytl::bind<void(int, int, std::string, int, int)>(func, _1, 2, "yo", _1, _1);
// 	auto f2 = nytl::bind<void(float)>(func, 5.f);
//
// 	f1(7);
// 	f2(7);
// }

}
