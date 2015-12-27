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

//implementation specialization
namespace detail
{

template<std::size_t D, typename P>
struct SimplexContainsPoint<D, P, 1>
{
	static bool test(const simplex<D, P, 1>& a, const vec<D, P>& v)
	{
		return (a.definedAt(v[0]) && all(a.valueAt(v[0]) == v));
	}
};

} //detail

template<size_t D, typename P> bool 
line<D, P>::definedAt(const P& value, std::size_t dim) const
{
    return ((smallestValue(dim) <= value) &&
            (greatestValue(dim) >= value));
}

template<size_t D, typename P> vec<D, P>
line<D, P>::valueAt(const P& value, std::size_t dim) const
{
    if(!definedFor(value, dim))
    {
		sendWarning("nytl::line::valueAt: line not defined at ", value, ", dim ", dim);
        return {};
    }
    else if(gradient()[dim] == 0)
    {
        auto ret = a;
        ret[dim] = value;
        return ret;
    }
    else
    {
        auto ret = a + ((value - a[dim]) * gradient(dim));
        return ret;
    }
}

//tests and stuff
/*
template<std::size_t D, typename P> NYTL_CPP14_CONSTEXPR
bool intersects(const line<D, P>& l1, const line<D, P>& l2)
{
    float st = max(l1.min(0), l2.min(0));
    float en = min(l1.max(0), l2.max(0));

    if(st >= en) return false;

    for(std::size_t i(1); i < D; ++i)
    {
        bool stV = l1.valueAt(st)[i] < l2.valueAt(st)[i];
        bool enV = l1.valueAt(en)[i] < l2.valueAt(en)[i];

        if(stV == enV) return false;
    }

    return true;
}
template<std::size_t D, typename P> constexpr
bool intersects(const line<D, P>& l1, const vec<D, P>& v1)
{
    return (l1.definedFor(v1[0], 0) && l1.valueAt(v1[0], 0) == v1);
}

template<std::size_t D, typename P> NYTL_CPP14_CONSTEXPR
vec<D, P> intersection(const line<D, P>& l1, const line<D, P>& l2)
{
    if(!intersects(l1, l2))
    {
	   	//should this function rlly throw?
        throw std::logic_error("line::intersection: lines do not intersect");
		return vec<D, P>{};
    }

    float val = (l1.valueAt(0, 0)[1] - l2.valueAt(0, 0)[1]) /
		(l2.gradient(0)[1] - l1.gradient(0)[1]);

    return l1.valueAt(val, 0);
}
*/

//utility
template<size_t D, typename P>
double length(const line<D, P>& l)
{
    return l.length();
}

