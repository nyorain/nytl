// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_TYPEMAP_INL
#define NYTL_INCLUDE_TYPEMAP_INL

#include <nytl/bits/tmpUtil.inl>
#include <istream>
#include <memory>
#include <any>

namespace nytl
{

namespace detail
{

template<typename T, typename = void>
struct Loader
{
	static bool call(std::istream& is, T& obj)
	{
		is >> obj;
		return 1;
	}
};

template<typename T>
struct Loader<T, void_t<decltype(T{}.load(std::declval<std::istream>()))>>
{
	static bool call(std::istream& is, T& obj)
	{
		return obj.load(is);
	}
};

} //detail

///\ingroup utility
///\brief Load an object from an std::istream via operator/member function/custom overload.
///If your type does neither overload the >> operator nor have a load member function,
///specialize this template function with your own type.
template<typename T>
bool load(std::istream& is, T& obj)
{
	return detail::Loader<T>::call(is, obj);
}


namespace detail
{

//TODO: perfect forwarding needed here? Args/CArgs are explicitly specified...
//make_unique_wrapper, returns a unique_ptr for usual types and a void* for void type
template<typename Base, typename T, typename... Args> struct CreateWrapper
{
    static std::unique_ptr<Base> call(Args... args) { return std::make_unique<T>(args...); }
};

template<typename T, typename... Args> struct CreateWrapper<void, T, Args...>
{
    static void* call(Args... args) { return new T(args...); };
};

template<typename T, typename... Args> struct CreateWrapper<std::any, T, Args...>
{
    static std::any call(Args... args) { return Any(T(args...)); };
};

//createLoad
template<typename Base, typename T, typename... Args> struct CreateLoadWrapper
{
    static std::unique_ptr<Base> call(std::istream& is, Args... args)
	{
		auto ret = std::make_unique<T>(args...);
		if(!load(is, *ret)) return nullptr;
		return ret;
	}
};

template<typename T, typename... Args> struct CreateLoadWrapper<void, T, Args...>
{
    static void* call(std::istream& is, Args... args)
	{
		auto ret = new T(args...);
		if(!load(is, *ret)) return nullptr;
		return ret;
	};
};

template<typename T, typename... Args> struct CreateLoadWrapper<std::any, T, Args...>
{
    static std::any call(std::istream& is, Args... args)
	{
		auto ret = T(args...);
		if(!load(is, ret)) return {};
		return {ret};
	};
};

//wrapper for checking if an object is valid
template<typename T>
struct CheckValidWrapper
{
	static bool call(const T& obj){ return obj != nullptr; }
};

template<>
struct CheckValidWrapper<std::any>
{
	static bool call(const std::any& obj){ return obj.has_value(); }
};

} //namespace detail

} //namespace nytl

#endif //header guard
