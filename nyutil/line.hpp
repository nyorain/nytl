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
};

template<size_t dim, typename prec> using ray = line<dim, prec>;
template<size_t dim, typename prec> using segment = line<dim, prec>;


//typedefs
using line2 = line<2, float>;
using line3 = line<3, float>;
using line4 = line<4, float>;

using segment2 = segment<2, float>;
using segment3 = segment<3, float>;
using segment4 = segment<4, float>;

using ray2 = ray<2, float>;
using ray3 = ray<3, float>;
using ray4 = ray<4, float>;

}
