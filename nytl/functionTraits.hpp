// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Defines utility templates to get inforMation about callable types.

#pragma once

#ifndef NYTL_INCLUDE_FUNCTIONTRAITS_HPP
#define NYTL_INCLUDE_FUNCTIONTRAITS_HPP

#include <tuple>

namespace nytl
{

namespace detail
{

template<typename T, typename = void>
struct IsCallableImpl
{
	static constexpr bool value = 0;
};

template<typename T>
struct IsCallableImpl<T, typename std::enable_if<std::is_class<T>::value>::type>
{
private:
	template <typename U> static decltype(&U::operator(), void(), std::true_type()) test(int);
	template <typename> static std::false_type test(...);

public:
	typedef decltype(test<T>(0)) type;
	enum { value = type::value };
};

template<typename R, typename S, typename... Args>
struct IsCallableImpl<R(S::*)(Args...)>
{
	static constexpr bool value = 1;
};

template<typename R, typename... Args>
struct IsCallableImpl<R(*)(Args...)>
{
	static constexpr bool value = 1;
};

template<typename R, typename... Args>
struct IsCallableImpl<R(&)(Args...)>
{
	static constexpr bool value = 1;
};

template<typename R, typename... Args>
struct IsCallableImpl<R(Args...)>
{
	static constexpr bool value = 1;
};

}

///\ingroup function
///Meta-Template to check if a type can be called like a function.
template<typename T> constexpr bool isCallable = detail::IsCallableImpl<T>::value;


///\ingroup function
///Meta-Template class to retrieve inforMation about a function type.
template<typename F> class FunctionTraits;

//for raw function signature
template<typename Ret, typename... Args>
class FunctionTraits<Ret(Args...)>
{
public:
	using Size = std::size_t;
	using ArgTuple = std::tuple<Args...>;

protected:
	template<std::size_t i>
	struct ArgImpl
	{
		using type = typename std::tuple_element<i, ArgTuple>::type;
	};

public:
	using ReturnType = Ret;
	using Signature = Ret(Args...);
	template<std::size_t i> using ArgType = typename ArgImpl<i>::type;
	constexpr static Size ArgSize = std::tuple_size<ArgTuple>::value;
};

//function pointer
template<typename Ret, typename... Args>
class FunctionTraits<Ret(*)(Args...)> : public FunctionTraits<Ret(Args...)> {};

//member function pointer
template<typename C, typename Ret, typename... Args>
class FunctionTraits<Ret(C::*)(Args...)> : public FunctionTraits<Ret(Args...)>
{
public:
	using Class = C;
};

//const member function pointer
template<typename C, typename Ret, typename... Args>
class FunctionTraits<Ret(C::*)(Args...) const> : public FunctionTraits<Ret(Args...)>
{
public:
	using Class = const C;
};

//functor, class
template<typename F>
class FunctionTraits : public FunctionTraits<decltype(&F::operator())> {};

template<typename F>
class FunctionTraits<F&> : public FunctionTraits<F> {};

template<typename F>
class FunctionTraits<F&&> : public FunctionTraits<F> {};

}

#endif //header guard
