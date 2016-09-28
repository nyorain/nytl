// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_TMPUTIL_INL
#define NYTL_INCLUDE_TMPUTIL_INL

namespace nytl
{

///\brief Useful typedef for expanding the use of variadic template arguments.
using Expand = int[];

///\brief Class that can be derived from to check if given template parameters are valid.
///\details Really useful for template classes that use SFINAE.
template<typename...> struct DeriveDummy {};

///\ingroup utility
///C++17 alias template for void, used to detect ill-formad types in a SFINAE-context.
template<typename...> using void_t = void;

///\ingroup utility
///Utility template function that can be used to hide unused compiler warnings.
///Has usually no additional cost. Is meant as placeholder for future code.
template<typename... T> void unused(T&&...) {}

}

#endif //header guard
