// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Defines utility templates to get inforMation about callable types.

#pragma once

#ifndef NYTL_INCLUDE_FUNCTION_TRAITS
#define NYTL_INCLUDE_FUNCTION_TRAITS

#include <tuple> // std::tuple
#include <type_traits> // std::false_type

namespace nytl {
namespace detail {
	template<typename T, typename = void>
	struct IsCallableImpl;
} // namespace detail

/// Metaprogramming template to check if a type can be called.
/// \module function
template<typename T>
constexpr bool isCallable = detail::IsCallableImpl<T>::value;

/// Metaprogramming template to retrieve information about a function type.
/// \module function
template<typename F> struct FunctionTraits;

/// Default FunctionTraits specializations for a raw signature.
/// All other (valid) FunctionTraits will inherit from this type.
template<typename Ret, typename... Args>
struct FunctionTraits<Ret(Args...)> {
	/// A tuple containing all arguments the function takes with all its qualifiers.
	using ArgTuple = std::tuple<Args...>;

	/// The type the function returns
	using ReturnType = Ret;

	/// The whole signature of the function.
	/// For e.g. `int foo(const int&, std::string)` this would be `int(const int&, std::string)`
	using Signature = Ret(Args...);

	/// The number of arguments this function takes
	constexpr static auto ArgSize = std::tuple_size<ArgTuple>::value;

	/// Can be used to retrieve the ith function parameter type.
	/// Example: `typename nytl::FunctionTraits<std::memcpy>::template ArgType<1>` is an
	/// alias for `const void*` since this is the second std::memcpy parameter
	/// \note Indexing of parameters starts (as usual) with 0, so ArgType<1> refers to the
	/// second argument.
	template<std::size_t I>
	using ArgType = typename std::tuple_element_t<I, ArgTuple>;
};

// - FunctionTraits specializations -
// Function pointer
template<typename Ret, typename... Args>
struct FunctionTraits<Ret(*)(Args...)> : public FunctionTraits<Ret(Args...)> {};

// Member function pointer
template<typename C, typename Ret, typename... Args>
struct FunctionTraits<Ret(C::*)(Args...)> : public FunctionTraits<Ret(Args...)> {
	using Class = C;
};

// Const member function pointer
template<typename C, typename Ret, typename... Args>
struct FunctionTraits<Ret(C::*)(Args...) const> : public FunctionTraits<Ret(Args...)> {
	using Class = const C;
};

// functor, class
template<typename F>
struct FunctionTraits : public FunctionTraits<decltype(&F::operator())> {};

// various additional qualifiers
template<typename F>
struct FunctionTraits<F&> : public FunctionTraits<F> {};

template<typename F>
struct FunctionTraits<F&&> : public FunctionTraits<F> {};


namespace detail {

// default implementation for not callable non-class types such as int
template<typename T, typename>
struct IsCallableImpl : public std::false_type {};

// specialization for int. Contains SFINAE check
template<typename T>
struct IsCallableImpl<T, typename std::enable_if<std::is_class<T>::value>::type> {
	template <typename U> static decltype(&U::operator(), void(), std::true_type()) test(int);
	template <typename> static std::false_type test(...);
	static constexpr auto value = decltype(test<T>(0))::value;
};

// member function pointer
template<typename R, typename S, typename... Args>
struct IsCallableImpl<R(S::*)(Args...)> : public std::true_type {};

// const member function pointer
template<typename R, typename S, typename... Args>
struct IsCallableImpl<R(S::*)(Args...) const> : public std::true_type {};

// function pointer
template<typename R, typename... Args>
struct IsCallableImpl<R(*)(Args...)> : public std::true_type {};

// function reference
template<typename R, typename... Args>
struct IsCallableImpl<R(&)(Args...)> : public std::true_type {};

// raw signature
// treated as callable
template<typename R, typename... Args>
struct IsCallableImpl<R(Args...)> : public std::true_type {};

} // namespace detail
} // namespace nytl

#endif // header guard
