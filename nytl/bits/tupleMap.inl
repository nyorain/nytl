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

#include <nytl/tmp.hpp>
#include <nytl/integer_sequence.hpp>
#include <tuple>

namespace nytl
{

namespace detail
{

template<typename OT, typename NT, std::size_t I = 0> 
struct TupleMapImpl; //unspecified

template<typename... OA, typename... NA, std::size_t I>
struct TupleMapImpl<std::tuple<OA...>, std::tuple<NA...>, I>
{
    using OrgTuple = std::tuple<OA...>;
    using NewTuple = std::tuple<NA...>;

    constexpr static const bool value = std::is_convertible<
			typename std::tuple_element<0, OrgTuple>::type, 
			typename std::tuple_element<0, NewTuple>::type
		>::value;

	using type = typename std::conditional<
        value,  //condition
        typename seq_prepend< //match
            typename TupleMapImpl<
                typename tuple_erase_first<OrgTuple>::type,
                typename tuple_erase_first<NewTuple>::type,
                I + 1
            >::type,
            I
        >::type,
        typename TupleMapImpl< //no match
            typename tuple_erase_first<OrgTuple>::type,
            NewTuple,
            I + 1
        >::type
    >::type;
};

template<typename... OrgLeft, std::size_t idx>
struct TupleMapImpl<std::tuple<OrgLeft...>, std::tuple<>, idx>
{
    using type = index_sequence<>;
};

template<std::size_t idx>
struct TupleMapImpl<std::tuple<>, std::tuple<>, idx>
{
    using type = index_sequence<>;
};

template<typename... NewLeft, std::size_t idx>
struct TupleMapImpl<std::tuple<>, std::tuple<NewLeft...>, idx>
{
    using type = index_sequence<>;

    //XXX ERROR. Should not happen! Indicates that args are not compatible.
    //how to show error if this is used, but only IF it is used (std::conditional must still work)?
};


//TupleMap
template<typename orgTup, typename newTup, typename seq = 
	typename detail::TupleMapImpl<orgTup, newTup>::type> 
struct TupleMap; //unspecified

template<typename... OA, typename... NA, std::size_t... I>
struct TupleMap<std::tuple<OA...>, std::tuple<NA...>, index_sequence<I...>>
{
    using NewTup = typename std::tuple<NA...>;
    using OrgTup = typename std::tuple<OA...>;
    using Seq = index_sequence<I...>;

    static constexpr NewTup map(OA&&... args) noexcept
    {
        return std::tuple<NA...>(std::forward<decltype(std::get<I>(OrgTup(args...)))>
				(std::get<I>(OrgTup(args...)))...);
    }
};

} //detail

} //nytl
