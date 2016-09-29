// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file
///\brief Includes several useful template functions.

#pragma once

#ifndef NYTL_INCLUDE_MISC_HPP
#define NYTL_INCLUDE_MISC_HPP

#include <nytl/bits/tmpUtil.inl>
#include <nytl/functionTraits.hpp>

#include <string>
#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <type_traits>

namespace nytl
{


///\ingroup function
///\{
///Produces a std::function from a member function with a given object.
template<typename U, typename V, typename... A>
std::function<U(A...)> memberCallback(U (V::*func)(A ...), typename std::remove_const<V>::type* obj)
{
	return ([func, obj](A... params)
	{
		return (obj->*func)(std::forward<A>(params)...);
	});
}

template<typename U, typename V, typename... A>
std::function<U(A...)> memberCallback(U (V::*func)(A...) const, const V* obj)
{
	return ([func, obj](A... params)
	{
		return (obj->*func)(std::forward<A>(params)...);
	});
}

template<typename U, typename V, typename... A>
std::function<U(A...)> memberCallback(U (V::*func)(A ...), typename std::remove_const<V>::type& obj)
{
	auto* tmp = &obj;
	return ([=](A... params)
	{
		return (tmp->*func)(std::forward<A>(params)...);
	});
}

template<typename U, typename V, typename... A>
std::function<U(A...)> memberCallback(U (V::*func)(A...) const, const V& obj)
{
	auto* tmp = &obj;
	return ([=](A ... params)
	{
		return (tmp->*func)(std::forward<A>(params)...);
	});
}
///\}

///\ingroup utility
///Prints the given args to the given output stream.
template<typename... Args> void printVars(std::ostream& out, Args ... args)
{
	Expand{(out << args, 0)...};
}


///\ingroup utility
inline std::vector<std::string>& split(const std::string &s, char delim,
		std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

///\ingroup utility
inline std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

///\ingroup utility
///Dumps a given iteratable container to a stringstream and returns its content.
///\param sep The sperator used after every container component.
template<typename T> std::string dumpContainer(const T& obj, const char* sep = ",\n")
{
	std::stringstream ss;

	bool first = 1;
	for(auto& val : obj)
	{
		if(!first) ss << sep;
		else first = 0;

		ss << val;
	}

	return ss.str();
}

}

#endif //header guard
