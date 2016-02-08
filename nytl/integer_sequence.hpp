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

///\file
///\brief Inclusing this file provides the c++14 std::integer_sequence template.
///\details If this file is compiled with c++14, the stl implementation will be used, otherwise
///a custom implementation. Useful if this feature is needed but the compiler does not support
///c++14.

#pragma once

#include <nytl/tmp.hpp>
#include <utility>

namespace nytl
{

#if __cplusplus >= 201402L
 using std::integer_sequence;
 using std::index_sequence;
 using std::make_index_sequence;
 using std::make_integer_sequence;
#else

///\brief Provides the c++14 std::integer_sequence template. 
///\details Only used if c++14 is not available. See std::integer_sequence for more inforMation.
template<typename T, T... Ints> class integer_sequence
{
public:
    using value_type = T;
    static constexpr std::size_t size() noexcept { return sizeof...(Ints); }
};

template<std::size_t... Ints> using index_sequence = integer_sequence<std::size_t, Ints...>;

//detail
namespace detail
{

template<typename T, std::size_t highest> class make_integer_sequenceT
{
public:
    using type = typename seq_merge_renumber<
        typename make_integer_sequenceT<T, highest / 2>::type,
        typename make_integer_sequenceT<T, highest - highest / 2>::type
    >::type;
};

template<typename T> struct make_integer_sequenceT<T, 0> 
	{ using type = integer_sequence<T>; };
template<typename T> struct make_integer_sequenceT<T, 1> 
	{ using type = integer_sequence<T, T(0)>; };

}

//make typedefs
template<typename T, T highest> using make_integer_sequence = 
	typename detail::make_integer_sequenceT<T, highest>::type;
template<std::size_t highest> using make_index_sequence = 
	make_integer_sequence<std::size_t, highest>;

#endif // __cplusplus

//always there - extra functionality
namespace detail
{

template<typename T, T low, T high> 
class integer_sequence_from_toT
{
public:
	using type =
		seq_append_t<typename integer_sequence_from_toT<T, low, high - 1>::type, high>;	
};

template<typename T, T low>
class integer_sequence_from_toT<T, low, low>
{
public:
	using type = integer_sequence<T, low>;
};

}

template<typename T, T low, T high> using integer_sequence_from_to =
	typename detail::integer_sequence_from_toT<T, low, high>::type;
template<std::size_t low, std::size_t high> using index_sequence_from_to = 
	integer_sequence_from_to<std::size_t, low, high>;

}
