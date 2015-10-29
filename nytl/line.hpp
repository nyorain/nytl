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

#include <nytl/vec.hpp>

namespace nytl
{

//line
template<size_t dim, typename prec>
class line
{
public:
    using value_type = prec;
    using vec_type = vec<dim, value_type>;

public:
    vec_type a;
    vec_type b;

public:
    line(const vec_type& xa, const vec_type& xb) noexcept : a(xa), b(xb) {}
    line() noexcept = default;

    vec_type getDifference() const { return b - a; }
    vec_type getGradient() const { return normalize(getDifference() / length(getDifference())); }
    vec_type getGradient(std::size_t dimension) const { return getGradient() / getGradient()[dimension]; }

    vec_type getValueAt(const prec& value, std::size_t dimension = 0) const
    {
        if(getGradient()[dimension] == 0)
        {
            auto ret = a;
            ret[dimension] = value;
            return ret;
        };

        auto ret = a + ((value - a[dimension]) * getGradient(dimension)); //todo: clearer expression
        ret[dimension] = value;
        return ret;
    }

    bool isDefinedFor(const prec& value, std::size_t dimension = 0) const
    {
        return std::min(a[dimension], b[dimension]) <= value && std::max(a[dimension], b[dimension]) >= value;
    }

    //conversion
    template<size_t odim, typename oprec>
    operator line<odim, oprec>() const { return line<odim, oprec>(a, b); }
};

template<size_t dim, typename prec> using ray = line<dim, prec>;
template<size_t dim, typename prec> using segment = line<dim, prec>;


//typedefs
template<typename prec> using line2 = line<2, prec>;
template<typename prec> using line3 = line<3, prec>;
template<typename prec> using line4 = line<4, prec>;

using line2f = line<2, float>;
using line3f = line<3, float>;
using line4f = line<4, float>;

using line2i = line<2, int>;
using line3i = line<3, int>;
using line4i = line<4, int>;

using line2ui = line<2, unsigned int>;
using line3ui = line<3, unsigned int>;
using line4ui = line<4, unsigned int>;

//util
template<size_t dim, typename prec>
auto length(const line<dim, prec>& l) -> decltype(length(l.getDifference()))
{
    return length(l.getDifference());
}

}
