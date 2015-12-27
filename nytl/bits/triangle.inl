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

/*
namespace detail
{

template<std::size_t D, typename P>
struct SimplexContainsPoint<D, P, 2>
{
	static bool test(const triangle<D, P>& t, const vec<D, P>& v)
	{
	};
};

}

//test
template<std::size_t dim, typename prec> constexpr
bool intersects(const triangle<dim, prec>& t1, const triangle<dim, prec>& t2)
{
	return (intersects(t1.AB(), t2.AB()) || intersects(t1.AB(), t2.BC()) ||
			intersects(t1.AB(), t2.AC()) || intersects(t1.BC(), t2.AB()) ||
			intersects(t1.BC(), t2.BC()) || intersects(t1.BC(), t2.AC()) ||
			intersects(t1.AC(), t2.AB()) || intersects(t1.AC(), t2.BC()) ||
			insersects(t1.AC(), t2.AC()) || contains(t1, t2));
}
template<std::size_t dim, typename prec> constexpr
bool intersects(const triangle<dim, prec>& t1, const line<dim, prec>& l2)
{
	return (contains(t1, l2) || intersects(t1.AB(), l2) ||
			intersects(t1.BC(), l2) || intersects(t1.ac(), l2));
}

template<std::size_t dim, typename prec> constexpr
bool contains(const triangle<dim, prec>& t1, const triangle<dim, prec>& t2)
{
	return (contains(t1, t2.a) && contains(t1, t2.b) && contains(t1, t2.c));
}
template<std::size_t dim, typename prec> constexpr
bool contains(const triangle<dim, prec>& t1, const line<dim, prec>& l2)
{
	return (contains(t1, l2.a) && contains(t1, l2.b));
}
template<std::size_t dim, typename prec> NYTL_CPP14_CONSTEXPR
bool contains(const triangle<dim, prec>& t1, const vec<dim, prec>& v2)
{
    //http://math.stackexchange.com/questions/4322/check-whether-a-point-is-within-a-3d-triangle
    //todo: if point is not even on triangle plane
    double area2 = length(t1.b - t1.a) * length(t1.c - t1.a);
    double aa = length(t1.b - v2) * length(t1.c - v2) / area2;
    double bb = length(t1.c - v2) * length(t1.a - v2) / area2;
    double cc = 1 - aa - bb;

    if(aa < 0 || aa > 1) return 0;
    if(bb < 0 || bb > 1) return 0;
    if(cc < 0 || cc > 1) return 0;

    return 1;
}
*/

//operators
template<size_t dim, typename prec>
std::ostream& operator<<(std::ostream& os, const triangle<dim, prec>& obj)
{
    os << obj.a << " " << obj.b << " " << obj.c;
    return os;
}

