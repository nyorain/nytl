/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Jan Kelling
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
//header for template metaprogramming

#include <utility>

namespace nyutil
{

//TUPLE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//tuple_erase_first///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> struct tuple_erase_first;

template<template<typename...> class T, typename Head, typename ... Tail>
struct tuple_erase_first<T<Head, Tail...>>
{
    using type = T<Tail...>;
};

template<typename T> using tuple_erase_first_t = typename tuple_erase_first<T>::type;

//tuple_append///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, typename Append> struct tuple_append;

template<template<typename...> class T, typename... Body, typename Append>
struct tuple_append<T<Body...>, Append>
{
    using type = T<Body..., Append>;
};

template<typename T, typename Append> using tuple_append_t = typename tuple_append<T, Append>::type;

//tuple_prepend///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, typename Append> struct tuple_prepend;

template<template<typename...> class T, typename... Body, typename Prepend>
struct tuple_prepend<T<Body...>, Prepend>
{
    using type = T<Prepend, Body...>;
};

template<typename T, typename Prepend> using tuple_prepend_t = typename tuple_prepend<T, Prepend>::type;

//type_tuple///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, std::size_t size, template<typename...> class Tuple = std::tuple> struct type_tuple
{
    using type = typename tuple_prepend<typename type_tuple<T, size - 1>::type, T>::type;
};

template<typename T, template<typename...> class Tuple> struct type_tuple<T, 1, Tuple>
{
    using type = Tuple<T>;
};

template<typename T, std::size_t size, template<typename...> class Tuple = std::tuple> using type_tuple_t = typename type_tuple<T, size, Tuple>::type;




//SEQEUNCE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//seq_erase_first///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> struct seq_erase_first;

template<typename I, template<typename, I...> class T, I Head, I... Tail>
struct seq_erase_first<T<I, Head, Tail...>>
{
    using type = T<I, Tail...>;
};

template<typename T> using seq_erase_first_t = typename seq_erase_first<T>::type;


//seq_append///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, typename T::value_type Append> struct seq_append;

template<typename I, template<typename, I...> class T, I... Body, I Append>
struct seq_append<T<I, Body...>, Append>
{
    using type = T<I, Body..., Append>;
};

template<typename T, typename T::value_type Append> using seq_append_t = typename seq_append<T, Append>::type;

//seq_prepend///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, typename T::value_type Prepend> struct seq_prepend;

template<typename I, template<typename, I...> class T, I... Body, I Prepend>
struct seq_prepend<T<I, Body...>, Prepend>
{
    using type = T<I, Prepend, Body...>;
};

template<typename T, typename T::value_type Prepend> using seq_prepend_t = typename seq_prepend<T, Prepend>::type;

//seq_merge///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename A, typename B> struct seq_merge;
template<typename I, template<typename, I...> class T, I... IdxA, I... IdxB>
struct seq_merge<T<I, IdxA...>, T<I, IdxB...>>
{
    using type = T<I, IdxA..., IdxB...>;
};

template<typename A, typename B> using seq_merge_t = typename seq_merge<A, B>::type;

//seq_merge_renumber////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename A, typename B> struct seq_merge_renumber;
template<typename I, template<typename, I...> class T, I... IdxA, I... IdxB>
struct seq_merge_renumber<T<I, IdxA...>, T<I, IdxB...>>
{
    using type = T<I, IdxA..., (sizeof...(IdxA) + IdxB)...>;
};

template<typename A, typename B> using seq_merge_renumber_t = typename seq_merge_renumber<A, B>::type;

//seq_print////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> struct seq_print;
template<typename I, template<typename, I...> class T, I... idx>
struct seq_print<T<I, idx...>>
{
    static std::ostream& print(std::ostream& o)
    {
        int dummy[] = {((void) (o << idx << " "), 0)...};
        ((void)dummy);
        return o;
    };
};

}

