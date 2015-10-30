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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//members
template<size_t dim, typename prec>
constexpr bool line<dim, prec>::definedFor(const prec& value, std::size_t dimension) const
{
    return ((std::min(a[dimension], b[dimension]) <= value) &&
            (std::max(a[dimension], b[dimension]) >= value));
}

template<size_t dim, typename prec>
NYUTIL_CPP14_CONSTEXPR vec<dim, prec> line<dim, prec>::valueAt(const prec& value, std::size_t dimension) const
{
    if(!definedFor(value, dimension))
    {
         //should this function rlly throw?
        throw std::logic_error("line::valueAt: line is not defined for [" + std::to_string(dimension) + "] == " + std::to_string(value));
        return vec<dim, prec>{};
    }
    else if(gradient()[dimension] == 0)
    {
        auto ret = a;
        ret[dimension] = value;
        return ret;
    }
    else
    {
        auto ret = a + ((value - a[dimension]) * gradient(dimension)); //todo: clearer expression
        ret[dimension] = value;
        return ret;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//tests and stuff
template<std::size_t dim, typename prec> NYUTIL_CPP14_CONSTEXPR 
bool intersects(const line<dim, prec>& l1, const line<dim, prec>& l2)
{
    float st = max(l1.min(0), l2.min(0));
    float en = min(l1.max(0), l2.max(0));

    if(st >= en) return false;

    for(std::size_t i(1); i < dim; ++i)
    {
        bool stV = l1.valueAt(st)[i] < l2.valueAt(st)[i];
        bool enV = l1.valueAt(en)[i] < l2.valueAt(en)[i];

        if(stV == enV) return false;
    }

    return true;
}
template<std::size_t dim, typename prec> constexpr 
bool intersects(const line<dim, prec>& l1, const vec<dim, prec>& v1)
{
    return (l1.definedFor(v1[0], 0) && l1.valueAt(v1[0], 0) == v1);
}

template<std::size_t dim, typename prec> NYUTIL_CPP14_CONSTEXPR 
vec<dim, prec> intersection(const line<dim, prec>& l1, const line<dim, prec>& l2)
{
    if(!intersects(l1, l2))
    {
	   	//should this function rlly throw?
        throw std::logic_error("line::intersection: lines do not intersect");        
		return vec<dim, prec>{};
    }

    float val = (l1.valueAt(0, 0)[1] - l2.valueAt(0, 0)[1]) / 
		(l2.gradient(0)[1] - l1.gradient(0)[1]);

    return l1.valueAt(val, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//utility
template<size_t dim, typename prec>
constexpr auto length(const line<dim, prec>& l) -> decltype(l.length())
{
    return l.length();
}
