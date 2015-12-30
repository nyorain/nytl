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

#include <nytl/tmp.hpp>
#include <nytl/integer_sequence.hpp>
#include <tuple>

namespace nytl
{

namespace detail
{

template<typename orgTup, typename newTup, std::size_t idx = 0> 
struct tupleMapImpl; //unspecified

template<typename... orgArgs, typename... newArgs, std::size_t idx>
struct tupleMapImpl<std::tuple<orgArgs...>, std::tuple<newArgs...>, idx>
{
    using orgTuple = std::tuple<orgArgs...>;
    using newTuple = std::tuple<newArgs...>;

    constexpr static const bool value = std::is_convertible<
			typename std::tuple_element<0, orgTuple>::type, 
			typename std::tuple_element<0, newTuple>::type
		>::value;

	using type = typename std::conditional<
        value,  //condition
        typename seq_prepend< //match
            typename tupleMapImpl<
                typename tuple_erase_first<orgTuple>::type,
                typename tuple_erase_first<newTuple>::type,
                idx + 1
            >::type,
            idx
        >::type,
        typename tupleMapImpl< //no match
            typename tuple_erase_first<orgTuple>::type,
            newTuple,
            idx + 1
        >::type
    >::type;
};

template<typename... OrgLeft, std::size_t idx>
struct tupleMapImpl<std::tuple<OrgLeft...>, std::tuple<>, idx>
{
    using type = index_sequence<>;
};

template<std::size_t idx>
struct tupleMapImpl<std::tuple<>, std::tuple<>, idx>
{
    using type = index_sequence<>;
};

template<typename... NewLeft, std::size_t idx>
struct tupleMapImpl<std::tuple<>, std::tuple<NewLeft...>, idx>
{
    using type = index_sequence<>;

    //XXX ERROR. Should not happen! Indicates that args are not compatible.
    //how to show error if this is used, but only IF it is used (std::conditional must still work)?
};


//tupleMap
template<typename orgTup, typename newTup, typename seq = 
	typename detail::tupleMapImpl<orgTup, newTup>::type> 
struct tupleMap; //unspecified

template<typename... orgArgs, typename... newArgs, std::size_t... idx>
struct tupleMap<std::tuple<orgArgs...>, std::tuple<newArgs...>, index_sequence<idx...>>
{
    using newTup = typename std::tuple<newArgs...>;
    using orgTup = typename std::tuple<orgArgs...>;
    using seq = index_sequence<idx...>;

    static constexpr std::tuple<newArgs...> map(orgArgs&&... args) noexcept
    {
        return std::tuple<newArgs...>(std::forward<decltype(std::get<idx>(orgTup(args...)))>
				(std::get<idx>(orgTup(args...)))...);
    }
};

} //detail

} //nytl
