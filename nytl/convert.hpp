// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file Utilities to implement easier conversions and casts.

#pragma once

#ifndef NYTL_INCLUDE_CONVERT
#define NYTL_INCLUDE_CONVERT

#include <nytl/tmpUtil.hpp> // nytl::void_t

#include <array> // std::array
#include <cstdint> // std::size_t

namespace nytl {

/// \brief Template struct for general conversions between types.
/// Specialize this template with a static 'call' function to enable explicit conversions
/// from type O to type T using the [nytl::convert]() function.
/// Works out of the box for objects that can be converted using static_cast.
/// \module utility
template<typename From, typename To, typename = void>
struct Converter;

/// \brief Can be used as some general and extendable conversion mechanism.
/// This can be used to implement free conversion operations in a consistent way.
/// To extend the types that can be converted by this call beyond static_cast and the container
/// cast provided by nytl, specialize [nytl::Converter]().
/// \module utility
template<typename T, typename O>
auto convert(const O& other)
{
	return Converter<T, O>::call(other);
}

/// \brief Will automatically try to convert the referenced object into whatever type is requested.
/// Uses nytl::convert (i.e. specializations of nytl::Converter<From, To>) to convert the
/// object. Therefore works for static_cast convertible objects out of the box.
/// \note This is only a temporary wrapper around an object, it therefore does not own it.
/// This object should generally not be used outside of temporary expressions.
/// \module utility
template<typename T>
struct AutoCastable {
	template<typename O> operator O() const { return convert<O>(*object_); }
	const T* object_;
};

/// \brief Alternative convert overload that will convert to the requested type automatically.
/// Instead of [nytl::convert<T, O>(const O&)]() this does not require to specify the
/// exact type the given object should be converted to, but instead returns an object
/// of type nytl::AutoCastable<T> that will convert to whichever type it is requested to
/// convert to, e.g. by using it in a function call or assignment expression.
/// \note Should not be used together with auto. Always keep in mind that this call returns
/// only a temporary expression and it should never be used in another way.
/// \note This might be really evil.
/// \module utility
template<typename O>
auto convert(const O& other)
{
	return AutoCastable<O>{&other};
}

/// \brief Casts an array to an array of another type.
/// \requires Values of type 'U' must be convertible to type 'T' using
/// nytl::convert (i.e. static_cast).
/// \module utility
template<typename T, typename U, std::size_t I,
	typename = decltype(convert<T>(std::declval<std::array<U, I>>()[0]))>
constexpr auto arrayCast(const std::array<U, I>& array)
{
	std::array<T, I> ret {};
	for(auto i = 0u; i < I; ++i) ret[i] = convert<T>(array[i]);
	return ret;
}

/// \brief Converts container object of type 'U' to container of type 'T'.
/// Can be used to convert between different containers and different container value types.
/// \module utility
template<typename T, typename U, std::size_t I>
constexpr auto containerCast(const U& con)
{
	T ret;
	ret.resize(con.size());

	for(auto f = con.cbegin(), t = ret.begin(); f != con.cend(); ++f, ++t)
		*t = nytl::convert<*f>();

	return ret;
}

// - general static_cast Converter -
template<typename From, typename To>
struct Converter<From, To, void_t<decltype(static_cast<To>(std::declval<From>()))>> {
	static To call(const From& other) { return static_cast<To>(other); }
};

template<typename From, typename To, std::size_t I> using ValidArrayCast =
	void_t<decltype(arrayCast<To>(std::declval<std::array<From, I>>()))>;

// - arrayCast Converter -
template<typename From, typename To, std::size_t I>
struct Converter<std::array<From, I>, std::array<To, I>, ValidArrayCast<From, To, I>> {
	static To call(const From& other) { return arrayCast<To>(other); }
};

// - containerCast Converter -
template<typename From, typename To>
struct Converter<From, To, void_t<decltype(containerCast<To>(std::declval<From>()))>> {
	static To call(const From& other) { return containerCast<To>(other); }
};

}

#endif // header guard
