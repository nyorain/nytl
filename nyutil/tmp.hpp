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
    using type = T<I, Body..., Prepend>;
};

template<typename T, typename T::value_type Prepend> using seq_prepend_t = typename seq_prepend<T, Prepend>::type;

}

