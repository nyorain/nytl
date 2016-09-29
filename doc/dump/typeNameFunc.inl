// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <nytl/system.hpp>
#include <string>
#include <cstring>

namespace nytl
{

//detail typename helper
namespace detail
{

template<typename T>
struct TypeNameFuncHelper
{
	static const char* call()
	{
		#if defined(NYTL_COMPILER_MSC)
			constexpr auto prefix = "nytl::detail::TypeNameFuncHelper<";
			constexpr auto suffix1 = ">::call";
			constexpr auto suffix2 = "";
			constexpr auto typeRepeats = 1;
		#elif defined(NYTL_COMPILER_CLANG)
			 constexpr auto prefix =
				"static nytl::detail::StringWrapper nytl::detail::TypeNameFuncHelper<";
			 constexpr auto suffix1 = ">::call() [T = ";
			 constexpr auto suffix2 = "]";
			 constexpr auto typeRepeats = 2;
		#elif defined(NYTL_COMPILER_GNU)
			constexpr auto prefix = "static nytl::detail::StringWrapper "
		"nytl::detail::TypeNameFuncHelper<T>::call() [with T = ";
			 constexpr auto suffix1 = "]";
			 constexpr auto suffix2 = "";
			 constexpr auto typeRepeats = 1;
		#else
			 //#warning "nytl::TypeNameFuncHelper does not support your compiler."
			constexpr auto prefix = "";
			 constexpr auto suffix1 = "";
			 constexpr auto suffix2 = "";
			 constexpr auto typeRepeats = 1;
			return typeid(T).name();
		#endif

			 constexpr auto fLength = sizeof(NYTL_PRETTY_FUNCTION) - 1u;
			 constexpr auto pLength = sizeof(prefix) - 1u;
			 constexpr auto sLength = sizeof(suffix1) - 1u + sizeof(suffix2) - 1u;
			 constexpr auto tLength = (fLength - (pLength + sLength)) / typeRepeats;

			static char name[tLength + 1u] = {};
			std::memcpy(name, NYTL_PRETTY_FUNCTION + pLength, tLength - 1u);
			return name;
	}
};

template<typename T>
std::string typeNameFunc()
{
	return std::string(TypeNameFuncHelper<T>::call());
}

} //detail

} //nytl
