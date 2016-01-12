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

//member
template<std::size_t D, typename P>
double tetrahedron<D, P>::size() const
{
	return detail::simplexSize(*this);
}

template<std::size_t D, typename P>
vec<D, P> tetrahedron<D, P>::center() const
{
	return detail::simplexCenter(*this);
}

template<std::size_t D, typename P>
vec<4, double> tetrahedron<D, P>::barycentric(const vec<D, P>& v) const
{
	return detail::simplexBarycentric(*this, v);
}

template<std::size_t D, typename P>
bool tetrahedron<D, P>::valid() const
{
	return detail::simplexValid(*this);
}

//operators
///\relates tetrahedron
template<size_t D, typename P>
std::ostream& operator<<(std::ostream& os, const tetrahedron<D, P>& obj)
{
    os << obj.a << " " << obj.b << " " << obj.c << " " << obj.d;
    return os;
}

