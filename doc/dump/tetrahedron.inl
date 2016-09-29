// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

template<std::size_t D, typename P>
double Tetrahedron<D, P>::size() const
{
	return detail::SimplexSize(*this);
}

template<std::size_t D, typename P>
Vec<D, P> Tetrahedron<D, P>::center() const
{
	return detail::SimplexCenter(*this);
}

template<std::size_t D, typename P>
bool Tetrahedron<D, P>::valid() const
{
	return detail::SimplexValid(*this);
}
