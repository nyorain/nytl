// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Defines a utility to implement easy conversions without conversion operator.

#pragma once

#ifndef NYTL_INCLUDE_CONVERT_HPP
#define NYTL_INCLUDE_CONVERT_HPP

#include <type_traits>

namespace nytl
{

///Specialize this template with a static call function to enable explicit conversions
///from type O to type T using the nytl::convert function.
///By default it will just perform a static cast.
///\sa nytl::convert
template<typename T, typename O, typename = void>
struct Converter;

template<typename T, typename O>
struct Converter<T, O, std::void_t<decltype(static_cast<T>(std::declval<O>()))>>
{
	static O call(const T& other) { return static_cast<O>(other); }
};

///The convert function can be used to call specialization of the Converted struct.
///This can be used to implement free conversion operations in a consistent way.
template<typename T, typename O>
auto convert(const O& other)
{
	return Converter<T, O>::call(other);
}

///Utility wrapper around type O that will convert into any type to which the stored object
///can be converted to.
template<typename O>
class AutoCastable
{
public:
	template<typename T>
	operator T() const { return convert<T>(*data_); }

public:
	const O* data_;
};

///Convert overload that only takes one template parameter which will be deduced.
///Returns a wrapper object that will implicitly cast into any object into which the given
///object of type O can be converted.
///alternative name: auto_cast
template<typename O> AutoCastable<O> convert(const O& other) { return {&other}; }

}

#endif //header guard
