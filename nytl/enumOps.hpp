/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Jan Kelling
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

#include <type_traits>

///\file
///\brief Defines binary operators for enums.
///\details Inlcuding this file enables all binary operations for strongly-typed c++11 enum classes.
///If you want to use them using the real c++ operators (not explicitly calling the namespaced
///functions) you have to use the nytl::enumOps namespace.

namespace nytl
{

///Template class that can be specialized to inherit from std::true_type for enumOps enabled types.
template<typename E> struct EnumOpsType : public std::false_type {};

}


template<typename E, typename = typename std::enable_if<nytl::EnumOpsType<E>::value>::type> 
	E operator~(E a) { return static_cast<E>(~static_cast<std::underlying_type_t<E>>(a)); }

template<typename E, typename = typename std::enable_if<nytl::EnumOpsType<E>::value>::type> 
	E operator|(E a, E b) { return static_cast<E>(static_cast<std::underlying_type_t<E>>(a) | 
			static_cast<std::underlying_type_t<E>>(b)); }

template<typename E, typename = typename std::enable_if<nytl::EnumOpsType<E>::value>::type>
	E operator&(E a, E b) { return static_cast<E>(static_cast<std::underlying_type_t<E>>(a) & 
			static_cast<std::underlying_type_t<E>>(b)); }

template<typename E, typename = typename std::enable_if<nytl::EnumOpsType<E>::value>::type>
E operator^(E a, E b) { return static_cast<E>(static_cast<std::underlying_type_t<E>>(a) ^ 
		static_cast<std::underlying_type_t<E>>(b)); }

template<typename E, typename = typename std::enable_if<nytl::EnumOpsType<E>::value>::type>
E& operator|=(E& a, E b) { a = a | b; return a; } 

template<typename E, typename = typename std::enable_if<nytl::EnumOpsType<E>::value>::type>
E& operator&=(E& a, E b) { a = a & b; return a; } 

template<typename E, typename = typename std::enable_if<nytl::EnumOpsType<E>::value>::type>
E& operator^=(E& a, E b) { a = a ^ b; return a; } 

#define NYTL_ENABLE_ENUM_OPS(T) namespace nytl \
	{ template<> struct EnumOpsType<T> : public std::true_type{}; }
