#pragma once

#include <nyutil/vec.hpp>

namespace nyutil
{

template<size_t dim, typename prec>
class triangle
{
public:
    using value_type = prec;
    using vec_type = vec<dim, value_type>;
    using triangle_type = vec<dim, value_type>;

public:
    vec_type a;
    vec_type b;
    vec_type c;

public:
    triangle() = default;
    triangle(const vec_type& xa, const vec_type& xb, const vec_type& xc) : a(xa), b(xb), c(xc) {}

    inline bool contains(const vec_type& point) const;
    bool intersects(const triangle_type& tri) const { return 0; } //todo
    double size() const { return abs(b - a) * abs(c - a) * 0.5; }
};

//util
template<size_t dim, typename prec>
bool triangle<dim, prec>::contains(const vec<dim, prec>& p) const
{
    //http://math.stackexchange.com/questions/4322/check-whether-a-point-is-within-a-3d-triangle
    //todo: if point is not even on triangle plane
    double area2 = abs(b - a) * abs(c - a);
    double a = abs(b - p) * abs(c - p) / area2;
    double b = abs(c - p) * abs(a - p) / area2;
    double c = 1 - a - b;

    if(a < 0 || a > 1) return 0;
    if(b < 0 || b > 1) return 0;
    if(c < 0 || c > 1) return 0;

    return 1;
}

//typedefs
using triangle2 = triangle<2, float>;
using triangle3 = triangle<3, float>;
using triangle4 = triangle<4, float>;


}
