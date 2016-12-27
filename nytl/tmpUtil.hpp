// Copyright (c) 2016 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Various useful template helpers without dependencies.

#pragma once

#ifndef NYTL_INCLUDE_TMPUTIL
#define NYTL_INCLUDE_TMPUTIL

namespace nytl {

/// \brief Useful typedef for expanding the use of variadic template arguments.
/// In C++17, most of the Expand shortcuts can be done with fold expressions.
/// \code
/// // Prints the variadic template pack args to std::cout.
/// (void) nytl::Expand {((void) std::cout << args , 0), ...};
/// \endcode
using Expand = int[];

/// \brief Class that can be derived from to check if given template parameters are valid.
/// \details Really useful for template classes that use SFINAE.
/// \code
/// class D : public nytl::DeriveDummy<std::enable_if_t<Condition>> {};
/// \endcode
template<typename...> struct DeriveDummy {};

/// \ingroup utility
/// C++17 alias template for void, used to detect ill-formed types in a SFINAE-context.
/// \code
/// // Function foo must be callable with a value of type A.
/// template<typename A, typename = void_t<decltype(foo(std::declval<A>()))>>
/// void bar();
/// \endcode
template<typename...> using void_t = void;

/// \ingroup utility
/// Utility template function that can be used to hide unused compiler warnings.
/// Has usually no additional cost. Is meant as placeholder for future code.
/// \code
/// void foo(int a, int b, int c) { nytl::unused(a, b, c); } // to be implemented later on
/// \endcode
template<typename... T> void unused(T&&...) {}

/// \ingroup utility
/// Can be used to assure that variadic arguments have the same type.
/// \code
/// // implementation function that takes a variable number of integers
/// template<typename... Args>
/// void funcImpl(nytl::VariadicDummy<int, Args>... ints);
///
/// // public interface function that is called
/// // results in compile time error if not all arguments are convertible to integers
/// template<typename... Args>
/// void func(Args... args) { funcImpl<Args...>(args...); }
/// \endcode
template<typename A, typename> using Variadic = A;

/// \ingroup utility
/// Assures that the returned value can be used as template-dependent expressions.
/// This allows e.g. to defer operator of function lookup so that specific headers don't
/// have to be included by a template functions if a function is not used.
template<typename V, typename T>
decltype(auto) templatize(T& value) { return value; }


}

#endif //header guard
