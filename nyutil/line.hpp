#pragma once

#include <nyutil/vec.hpp>

namespace nyutil
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
    line(const vec_type& xa, const vec_type& xb) : a(xa), b(xb) {}
    line() = default;

    vec_type getDifference() const { return b - a; }

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
