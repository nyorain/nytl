// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_LINE_INL
#define NYTL_INCLUDE_LINE_INL

template<size_t D, typename P> bool
Line<D, P>::definedAt(const P& value, std::size_t dim) const
{
    return ((smallestValue(dim) <= value) &&
            (greatestValue(dim) >= value));
}

template<size_t D, typename P> Vec<D, P>
Line<D, P>::valueAt(const P& value, std::size_t dim) const
{
	//Will NOT check for defined at, can be done by function user, if needed
    if(gradient()[dim] == 0)
	{
		return a;
	}
    else
    {
        auto ret = a + ((value - a[dim]) * gradient(dim));
        return ret;
    }
}

///\ingroup math
///Returns the length of the given line.
///Just a wrapper around the Line::length function.
template<size_t D, typename P>
float length(const Line<D, P>& line)
{
	return line.length();
}

#endif //header guard
