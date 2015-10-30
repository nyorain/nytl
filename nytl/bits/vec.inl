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

/**
@file
@brief This file is marvelous.
*/

//minmax todo: extra header
#if __cplusplus >= 201402L
    using std::min;
    using std::max;

#else
    template<typename T> constexpr T min(const T& a, const T& b) { return a < b ? a : b; }
    template<typename T> constexpr T max(const T& a, const T& b) { return a > b ? a : b; }

#endif // c++14

//maxType

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//general operators//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///\relates nytl::vec
template<std::size_t dim, typename T> std::ostream& operator<<(std::ostream& os, const vec<dim, T>& obj)
{
    const char* c = "";

    os << "(";

    for(unsigned int i(0); i < dim; i++)
    {
        os << c << obj[i];
        c = "; ";
    }

    os << ")";

    return os;
}

//todo
template<std::size_t dim, typename T> std::istream& operator>>(std::istream& is, vec<dim, T>& obj)
{
    for(unsigned int i(0); i < dim; i++)
    {
        is >> obj[i];
    }

    return is;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//todo: vec and vec unefficient atm
//+///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t dim, typename T, typename O>
NYUTIL_CPP14_CONSTEXPR vec<dim, T> operator+(vec<dim, T> mvec, const O& other)
{
    mvec += other;
    return mvec;
}

template<std::size_t dim, typename T, typename O>
NYUTIL_CPP14_CONSTEXPR vec<dim, T> operator+(const O& other, vec<dim, T> mvec)
{
    mvec += other;
    return mvec;
}

template<std::size_t dima, typename Ta, std::size_t dimb, typename Tb, typename ttype = typename std::conditional<(dima > dimb), Ta, Tb>::type>
NYUTIL_CPP14_CONSTEXPR vec<max(dima, dimb), ttype> operator+(const vec<dima, Ta>& a, const vec<dimb, Tb>& b)
{
    vec<max(dima, dimb), ttype> ret = (vec<max(dima, dimb), ttype>)a;
    ret += (vec<max(dima, dimb), ttype>)b;
    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t dim, typename T, typename O>
NYUTIL_CPP14_CONSTEXPR vec<dim, T> operator-(vec<dim, T> mvec, const O& other)
{
    mvec -= other;
    return mvec;
}

template<std::size_t dim, typename T, typename O>
NYUTIL_CPP14_CONSTEXPR vec<dim, T> operator-(const O& other, vec<dim, T> mvec)
{
    for(std::size_t i(0); i < dim; i++)
        mvec[i] = other - mvec[i];

    return mvec;
}

template<std::size_t dima, typename Ta, std::size_t dimb, typename Tb, typename ttype = typename std::conditional<(dima > dimb), Ta, Tb>::type>
NYUTIL_CPP14_CONSTEXPR vec<max(dima, dimb), ttype> operator-(const vec<dima, Ta>& a, const vec<dimb, Tb>& b)
{
    vec<max(dima, dimb), ttype> ret = (vec<max(dima, dimb), ttype>)a;
    ret -= (vec<max(dima, dimb), ttype>)b;
    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t dim, typename T, typename O>
NYUTIL_CPP14_CONSTEXPR  vec<dim, T> operator*(vec<dim, T> mvec, const O& other)
{
    mvec *= other;
    return mvec;
}


template<std::size_t dim, typename T, typename O>
NYUTIL_CPP14_CONSTEXPR  vec<dim, T> operator*(const O& other, vec<dim, T> mvec)
{
    mvec *= other;
    return mvec;
}

template<std::size_t dima, typename Ta, std::size_t dimb, typename Tb, typename ttype = typename std::conditional<(dima > dimb), Ta, Tb>::type>
NYUTIL_CPP14_CONSTEXPR vec<max(dima, dimb), ttype> operator*(const vec<dima, Ta>& a, const vec<dimb, Tb>& b)
{
    vec<max(dima, dimb), ttype> ret = (vec<max(dima, dimb), ttype>)a;
    ret *= (vec<max(dima, dimb), ttype>)b;
    return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//\////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t dim, typename T, typename O>
NYUTIL_CPP14_CONSTEXPR vec<dim, T> operator/(vec<dim, T> mvec, const O& other)
{
    mvec /= other;
    return mvec;
}


template<std::size_t dim, typename T, typename O>
NYUTIL_CPP14_CONSTEXPR vec<dim, T> operator/(const O& other, vec<dim, T> mvec)
{
    for(std::size_t i(0); i < dim; i++)
        mvec[i] = other / mvec[i];

    return mvec;
}

template<std::size_t dima, typename Ta, std::size_t dimb, typename Tb, typename ttype = typename std::conditional<(dima > dimb), Ta, Tb>::type>
NYUTIL_CPP14_CONSTEXPR vec<max(dima, dimb), ttype> operator/(const vec<dima, Ta>& a, const vec<dimb, Tb>& b)
{
    vec<max(dima, dimb), ttype> ret = (vec<max(dima, dimb), ttype>)a;
    ret /= (vec<max(dima, dimb), ttype>)b;
    return ret;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//%//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t dim, typename T, typename O>
NYUTIL_CPP14_CONSTEXPR  vec<dim, T> operator%(vec<dim, T> mvec, const O& other)
{
    mvec %= other;
    return mvec;
}


template<std::size_t dim, typename T, typename O>
NYUTIL_CPP14_CONSTEXPR vec<dim, T> operator%(const O& other, vec<dim, T> mvec)
{
    for(std::size_t i(0); i < dim; i++)
        mvec[i] = other % mvec[i];

    return mvec;
}

template<std::size_t dima, typename Ta, std::size_t dimb, typename Tb, typename ttype = typename std::conditional<(dima > dimb), Ta, Tb>::type>
NYUTIL_CPP14_CONSTEXPR vec<max(dima, dimb), ttype> operator%(const vec<dima, Ta>& a, const vec<dimb, Tb>& b)
{
    vec<max(dima, dimb), ttype> ret = (vec<max(dima, dimb), ttype>)a;
    ret %= (vec<max(dima, dimb), ttype>)b;
    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//equal/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR vec<min(dima, dimb), bool> equal(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    vec<min(dima, dimb), bool> ret {};
    for(std::size_t i(0); i < min(dima, dimb); i++)
        ret[i] = (veca[i] == vecb[i]);
    return ret;
}

template<std::size_t dim, typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR vec<dim, bool> equal(const vec<dim, Ta>& veca, const Tb& value)
{
    vec<dim, bool> ret {};
    for(std::size_t i(0); i < dim; i++)
        ret[i] = (veca[i] == value);
    return ret;
}

template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR vec<min(dima, dimb), bool> notEqual(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    vec<min(dima, dimb), bool> ret {};
    for(std::size_t i(0); i < min(dima, dimb); i++)
        ret[i] = (veca[i] != vecb[i]);
    return ret;
}

template<std::size_t dim, typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR vec<dim, bool> notEqual(const vec<dim, Ta>& veca, const Tb& value)
{
    vec<dim, bool> ret {};
    for(std::size_t i(0); i < dim; i++)
        ret[i] = (veca[i] != value);
    return ret;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//greater/less//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//vec
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR vec<min(dima, dimb), bool> lessThan(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    vec<min(dima, dimb), bool> ret {};
    for(std::size_t i(0); i < min(dima, dimb); i++)
        ret[i] = veca[i] < vecb[i];
    return ret;
}
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR vec<min(dima, dimb), bool> greaterThan(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    vec<min(dima, dimb), bool> ret {};
    for(std::size_t i(0); i < min(dima, dimb); i++)
        ret[i] = veca[i] > vecb[i];
    return ret;
}
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR vec<min(dima, dimb), bool> lessThanEqual(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    vec<min(dima, dimb), bool> ret {};
    for(std::size_t i(0); i < min(dima, dimb); i++)
        ret[i] = veca[i] <= vecb[i];
    return ret;
}
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR vec<min(dima, dimb), bool> greaterThanEqual(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    vec<min(dima, dimb), bool> ret {};
    for(std::size_t i(0); i < min(dima, dimb); i++)
        ret[i] = veca[i] >= vecb[i];
    return ret;
}

//value
template<std::size_t dim, typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR vec<dim, bool> lessThan(const vec<dim, Ta>& veca, const Tb& value)
{
    vec<dim, bool> ret;
    for(std::size_t i(0); i < dim; i++)
        ret[i] = veca[i] < value;
    return ret;
}
template<std::size_t dim, typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR vec<dim, bool> greaterThan(const vec<dim, Ta>& veca, const Tb& value)
{
    vec<dim, bool> ret;
    for(std::size_t i(0); i < dim; i++)
        ret[i] = veca[i] > value;
    return ret;
}
template<std::size_t dim, typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR vec<dim, bool> lessThanEqual(const vec<dim, Ta>& veca, const Tb& value)
{
    vec<dim, bool> ret;
    for(std::size_t i(0); i < dim; i++)
        ret[i] = veca[i] <= value;
    return ret;
}
template<std::size_t dim, typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR vec<dim, bool> greaterThanEqual(const vec<dim, Ta>& veca, const Tb& value)
{
    vec<dim, bool> ret;
    for(std::size_t i(0); i < dim; i++)
        ret[i] = veca[i] >= value;
    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//operator form//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//todo: allow comparison of vec and value with operator? "" vec<dim, bool> operator==(vec<dim, Ta> v, Tb value); ""
//equal/not equal
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
constexpr auto operator==(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb) -> decltype(equal(veca, vecb))
{
    return equal(veca, vecb);
}

template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
constexpr auto operator!=(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb) -> decltype(notEqual(veca, vecb))
{
    return notEqual(veca, vecb);
}
//less/greater
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
constexpr auto operator<(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb) -> decltype(lessThan(veca, vecb))
{
    return lessThan(veca, vecb);
}

template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
constexpr auto operator>(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb) -> decltype(greaterThan(veca, vecb))
{
    return greaterThan(veca, vecb);
}

template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
constexpr auto operator<=(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb) -> decltype(lessThanEqual(veca, vecb))
{
    return lessThanEqual(veca, vecb);
}

template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
constexpr auto operator>=(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb) -> decltype(lessThanEqual(veca, vecb))
{
    return greaterThanEqual(veca, vecb);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//utility//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//all vector weights added (todo: is there some official name for this?)
template<std::size_t dim, typename prec>
NYUTIL_CPP14_CONSTEXPR auto weight(const vec<dim, prec>& v) -> decltype(v[0] + v[0]) //to make reference types disappear
{
    decltype(v[0] + v[0]) ret {};
    for(auto& val : v) ret += val;
    return ret;
}

//length
template<std::size_t dim, typename prec>
NYUTIL_CPP14_CONSTEXPR auto length(const vec<dim, prec>& v) -> decltype(sqrt(v[0]))
{
    decltype(sqrt(v[0])) ret{};
    for(auto& val : v) ret += val * val;
    return std::sqrt(ret);
}

//length alias
template<std::size_t dim, typename prec>
constexpr auto norm(const vec<dim, prec>& v) -> decltype(length(v))
{
    return length(v);
}

//dot
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
constexpr auto dot(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb) -> decltype(weight(veca * vecb))
{
    return weight(veca * vecb);
}

//cross, only vec3
template<typename Ta, typename Tb>
constexpr auto cross(const vec<3, Ta>& veca, const vec<3, Tb>& vecb) -> vec<3, decltype(veca[0] * vecb[0])>
{
    return vec<3, decltype(veca[0] * vecb[0])>
        (veca[2] * veca[3] - veca[3] * vecb[2], veca[3] * veca[1] - veca[1] * vecb[3], veca[1]* veca[2] - veca[2]* vecb[1]);
}

//angle between 2 vecs, always the smaller one < 180degrees
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
constexpr float angle(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    return std::acos(weight(veca * vecb) / (length(veca) * length(vecb)));
}

//absoulte counter-clockwise angle, todo: 3D, possible?
template<typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR float cangle(const vec<2, Ta>& veca, const vec<2, Tb>& vecb)
{
    auto val = atan2(veca.y, veca.x) - atan2(vecb.y, vecb.x);
    if(val <= 0) return (2 * cPi) + val;
    return val;
}

//normalize
template<std::size_t dim, typename T>
constexpr vec<dim, float> normalize(const vec<dim, T>& veca)
{
    return (vec<dim, float>(veca)) / length(veca);
}

//distance
template<std::size_t dim, typename T>
constexpr float distance(const vec<dim, T>& veca, const vec<dim, T>& vecb)
{
    return length(vecb - veca);
}

//radiance/degrees
template<std::size_t dim, typename T>
NYUTIL_CPP14_CONSTEXPR vec<dim, float> radiance(vec<dim, T> veca)
{
    for(auto& val : veca) val = radiance(val);
    return veca;
}

template<std::size_t dim, typename T>
NYUTIL_CPP14_CONSTEXPR vec<dim, float> degrees(vec<dim, T> veca)
{
    for(auto& val : veca) val = degrees(val);
    return veca;
}

//abs
template<std::size_t dim, typename T>
NYUTIL_CPP14_CONSTEXPR vec<dim, T> abs(vec<dim, T> veca)
{
    for(auto& val : veca) val = std::abs(val);
    return veca;
}

//clamp
template<std::size_t dim, typename Ta, typename Tb, typename Tc>
NYUTIL_CPP14_CONSTEXPR vec<dim, Ta> clamp(vec<dim, Ta> val, const vec<dim, Tb>& minVal, const vec<dim, Tc>& maxVal)
{
    for(std::size_t i(0); i < dim; ++i) val[i] = clamp(val[i], minVal[i], maxVal[i]);
    return val;
}

//mix
template<std::size_t dim, typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR vec<dim, Ta> mix(vec<dim, Ta> x, const vec<dim, Ta>& y, const vec<dim, Tb>& a)
{
    for(std::size_t i(0); i < dim; ++i) x[i] = mix(x[i], y[i], a[i]);
    return x;
}

template<std::size_t dim, typename Ta, typename Tb>
NYUTIL_CPP14_CONSTEXPR vec<dim, Ta> mix(vec<dim, Ta> x, const vec<dim, Ta>& y, const Tb& a)
{
    for(std::size_t i(0); i < dim; ++i) x[i] = mix(x[i], y[i], a);
    return x;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//boolean vec operations//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t dim>
NYUTIL_CPP14_CONSTEXPR bool any(const vec<dim, bool>& v)
{
    for(auto val : v) if(val) return 1;
    return 0;
}

template<std::size_t dim>
NYUTIL_CPP14_CONSTEXPR bool all(const vec<dim, bool>& v)
{
    for(auto val : v) if(!val) return 0;
    return 1;
}

template<std::size_t dim>
NYUTIL_CPP14_CONSTEXPR bool none(const vec<dim, bool>& v)
{
    for(auto val : v) if(val) return 0;
    return 1;
}

//not
template<std::size_t dim, typename T>
NYUTIL_CPP14_CONSTEXPR vec<dim, T> operator!(vec<dim, T> v)
{
    for(auto& val : v) val = !val;
    return v;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//component-wise//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t dim, typename prec>
NYUTIL_CPP14_CONSTEXPR vec<dim, prec> max(const vec<dim, prec>& veca, const vec<dim, prec>& vecb)
{
    vec<dim, prec> ret{};
    for(std::size_t i(0); i < dim; ++i)
    {
        ret[i] = max(veca[i], vecb[i]);
    }
    return ret;
}

template<std::size_t dim, typename prec>
NYUTIL_CPP14_CONSTEXPR vec<dim, prec> max(const vec<dim, prec>& veca, const prec& value)
{
    vec<dim, prec> ret{};
    for(std::size_t i(0); i < dim; ++i)
    {
        ret[i] = max(veca[i], value);
    }
    return ret;
}

template<std::size_t dim, typename prec>
NYUTIL_CPP14_CONSTEXPR vec<dim, prec> min(const vec<dim, prec>& veca, const vec<dim, prec>& vecb)
{
    vec<dim, prec> ret{};
    for(std::size_t i(0); i < dim; ++i)
    {
        ret[i] = min(veca[i], vecb[i]);
    }
    return ret;
}

template<std::size_t dim, typename prec>
NYUTIL_CPP14_CONSTEXPR vec<dim, prec> min(const vec<dim, prec>& veca, const prec& value)
{
    vec<dim, prec> ret{};
    for(std::size_t i(0); i < dim; ++i)
    {
        ret[i] = min(veca[i], value);
    }
    return ret;
}
