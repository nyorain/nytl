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

namespace detail
{

constexpr std::size_t dMin(std::size_t a, std::size_t b)
	{ return (a == dynamicSize || b == dynamicSize) ? dynamicSize : min(a, b); }

constexpr std::size_t dMax(std::size_t a, std::size_t b)
	{ return (a == dynamicSize || b == dynamicSize) ? dynamicSize : max(a, b); }

}

//general operators
///\relates nytl::vec
///Prints all values individually to an output stream.
template<std::size_t dim, typename T> std::ostream& 
operator<<(std::ostream& os, const vec<dim, T>& obj)
{
    const char* c = "";

    os << "(";

    for(const auto& val : obj)
    {
        os << c << val;
        c = "; ";
    }

    os << ")";

    return os;
}

///\relates nytl::vec
///Loads all components from an input stream.
template<std::size_t dim, typename T> std::istream& 
operator>>(std::istream& is, vec<dim, T>& obj)
{
    for(auto& val : obj)
    {
        is >> val;
    }

    return is;
}

//+todo: vec and vec unefficient atm, some proxy class 
///\relates nytl::vec
template<std::size_t dim, typename T, typename O>  
vec<dim, T> operator+(vec<dim, T> mvec, const O& other)
{
    mvec += other;
    return mvec;
}

///\relates nytl::vec
template<std::size_t dim, typename T, typename O>  
vec<dim, T> operator+(const O& other, vec<dim, T> mvec)
{
    mvec += other;
    return mvec;
}

///\relates nytl::vec
template<std::size_t dima, typename Ta, std::size_t dimb, typename Tb>  
auto operator+(const vec<dima, Ta>& a, const vec<dimb, Tb>& b) 
	-> vec<detail::dMax(dima, dimb), typename std::conditional<dima >= dimb, Ta, Tb>::type>
{
	using V = vec<detail::dMax(dima, dimb), typename std::conditional<dima >= dimb, Ta, Tb>::type>;

    auto ret = static_cast<V>(a);
    ret += static_cast<V>(b);
    return ret;
}

//-
///\relates nytl::vec
template<std::size_t dim, typename T, typename O>  
vec<dim, T> operator-(vec<dim, T> mvec, const O& other)
{
    mvec -= other;
    return mvec;
}

///\relates nytl::vec
template<std::size_t dim, typename T, typename OT>  
vec<dim, T> operator-(OT other, vec<dim, T> mvec)
{
    for(auto& val : mvec)
        val = other - val;

    return mvec;
}

///\relates nytl::vec
template<std::size_t dima, typename Ta, std::size_t dimb, typename Tb>  
auto operator-(const vec<dima, Ta>& a, const vec<dimb, Tb>& b) 
	-> vec<detail::dMax(dima, dimb), typename std::conditional<dima >= dimb, Ta, Tb>::type>
{
	using V = vec<detail::dMax(dima, dimb), typename std::conditional<dima >= dimb, Ta, Tb>::type>;

    auto ret = static_cast<V>(a);
    ret -= static_cast<V>(b);
    return ret;
}

//*
///\relates nytl::vec
template<std::size_t dim, typename T, typename O> 
vec<dim, T> operator*(vec<dim, T> mvec, const O& other)
{
    mvec *= other;
    return mvec;
}


///\relates nytl::vec
template<std::size_t dim, typename T, typename O> 
vec<dim, T> operator*(const O& other, vec<dim, T> mvec)
{
    mvec *= other;
    return mvec;
}

///\relates nytl::vec
template<std::size_t dima, typename Ta, std::size_t dimb, typename Tb>  
auto operator*(const vec<dima, Ta>& a, const vec<dimb, Tb>& b) 
	-> vec<detail::dMax(dima, dimb), typename std::conditional<dima >= dimb, Ta, Tb>::type>
{
	using V = vec<detail::dMax(dima, dimb), decltype(a[0] * b[0])>;

    auto ret = static_cast<V>(a);
    ret *= static_cast<V>(b);
    return ret;
}

//\
///\relates nytl::vec
template<std::size_t dim, typename T, typename O>  
vec<dim, T> operator/(vec<dim, T> mvec, const O& other)
{
    mvec /= other;
    return mvec;
}

///\relates nytl::vec
template<std::size_t dim, typename T, typename OT>
vec<dim, T> operator/(OT other, vec<dim, T> mvec)
{
    for(auto& val : mvec)
        val = other / val;

    return mvec;
}

///\relates nytl::vec
template<std::size_t dima, typename Ta, std::size_t dimb, typename Tb>
auto operator/(const vec<dima, Ta>& a, const vec<dimb, Tb>& b) 
	-> vec<detail::dMax(dima, dimb), typename std::conditional<dima >= dimb, Ta, Tb>::type>
{
	using V = vec<detail::dMax(dima, dimb), typename std::conditional<dima >= dimb, Ta, Tb>::type>;

    auto ret = static_cast<V>(a);
    ret /= static_cast<V>(b);
    return ret;
}

//%
///\relates nytl::vec
template<std::size_t dim, typename T, typename O>
vec<dim, T> operator%(vec<dim, T> mvec, const O& other)
{
    mvec %= other;
    return mvec;
}

///\relates nytl::vec
template<std::size_t dim, typename T, typename OT>
vec<dim, T> operator%(OT other, vec<dim, T> mvec)
{
    for(auto& val : mvec)
        val = other % val;

    return mvec;
}

///\relates nytl::vec
template<std::size_t dima, typename Ta, std::size_t dimb, typename Tb>
auto operator%(const vec<dima, Ta>& a, const vec<dimb, Tb>& b) 
	-> vec<detail::dMax(dima, dimb), typename std::conditional<dima >= dimb, Ta, Tb>::type>
{
	using V = vec<detail::dMax(dima, dimb), typename std::conditional<dima >= dimb, Ta, Tb>::type>;

    auto ret = static_cast<V>(a);
    ret %= static_cast<V>(b);
    return ret;
}

//equal
///\relates nytl::vec
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
vec<detail::dMin(dima, dimb), bool> equal(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    vec<detail::dMin(dima, dimb), bool> ret(min(veca.size(), vecb.size()));
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        ret[i] = (veca[i] == vecb[i]);
    return ret;
}

///\relates nytl::vec
template<std::size_t dim, typename Ta, typename Tb>  
vec<dim, bool> equal(const vec<dim, Ta>& veca, const Tb& value)
{
    vec<dim, bool> ret(veca.size());
    for(std::size_t i(0); i < dim; ++i)
        ret[i] = (veca[i] == value);
    return ret;
}

///\relates nytl::vec
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>  
vec<detail::dMin(dima, dimb), bool> notEqual(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    vec<detail::dMin(dima, dimb), bool> ret(min(veca.size(), vecb.size()));
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        ret[i] = (veca[i] != vecb[i]);
    return ret;
}

///\relates nytl::vec
template<std::size_t dim, typename Ta, typename Tb>  
vec<dim, bool> notEqual(const vec<dim, Ta>& veca, const Tb& value)
{
    vec<dim, bool> ret(veca.size());
    for(std::size_t i(0); i < dim; ++i)
        ret[i] = (veca[i] != value);
    return ret;
}

///\relates nytl::vec
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>  
vec<detail::dMin(dima, dimb), bool> lessThan(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    vec<detail::dMin(dima, dimb), bool> ret(min(veca.size(), vecb.size()));
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        ret[i] = veca[i] < vecb[i];
    return ret;
}

///\relates nytl::vec
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>  
vec<min(dima, dimb), bool> greaterThan(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    vec<min(dima, dimb), bool> ret(min(veca.size(), vecb.size()));
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        ret[i] = veca[i] > vecb[i];
    return ret;
}

///\relates nytl::vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
vec<detail::dMin(DA, DB), bool> lessThanEqual(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    vec<detail::dMin(DA, DB), bool> ret(min(veca.size(), vecb.size()));
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        ret[i] = veca[i] <= vecb[i];
    return ret;
}

///\relates nytl::vec
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>  
vec<detail::dMin(dima, dimb), bool> greaterThanEqual(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    vec<detail::dMin(dima, dimb), bool> ret(min(veca.size(), vecb.size()));
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        ret[i] = veca[i] >= vecb[i];
    return ret;
}

///\relates nytl::vec
template<std::size_t dim, typename Ta, typename Tb>  
vec<dim, bool> lessThan(const vec<dim, Ta>& veca, const Tb& value)
{
    vec<dim, bool> ret(veca.size());
    for(std::size_t i(0); i < veca.size(); ++i)
        ret[i] = veca[i] < value;
    return ret;
}

///\relates nytl::vec
template<std::size_t dim, typename Ta, typename Tb>  
vec<dim, bool> greaterThan(const vec<dim, Ta>& veca, const Tb& value)
{
    vec<dim, bool> ret(veca.size());
    for(std::size_t i(0); i < veca.size(); ++i)
        ret[i] = veca[i] > value;
    return ret;
}

///\relates nytl::vec
template<std::size_t dim, typename Ta, typename Tb>  
vec<dim, bool> lessThanEqual(const vec<dim, Ta>& veca, const Tb& value)
{
    vec<dim, bool> ret(veca.size());
    for(std::size_t i(0); i < veca.size(); ++i)
        ret[i] = veca[i] <= value;
    return ret;
}

///\relates nytl::vec
template<std::size_t dim, typename Ta, typename Tb>  
vec<dim, bool> greaterThanEqual(const vec<dim, Ta>& veca, const Tb& value)
{
    vec<dim, bool> ret(veca.size());
    for(std::size_t i(0); i < veca.size(); ++i)
        ret[i] = veca[i] >= value;
    return ret;
}

//todo: allow comparison of vec and value with operator? 
//vec<dim, bool> operator==(vec<dim, Ta> v, Tb value);
///\relates nytl::vec
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb> 
vec<detail::dMin(dima, dimb), bool> operator==(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    return equal(veca, vecb);
}

///\relates nytl::vec
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>  
vec<detail::dMin(dima, dimb), bool> operator!=(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    return notEqual(veca, vecb);
}

///\relates nytl::vec
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb> 
vec<detail::dMin(dima, dimb), bool> operator<(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    return lessThan(veca, vecb);
}

///\relates nytl::vec
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>  
vec<detail::dMin(dima, dimb), bool> operator>(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    return greaterThan(veca, vecb);
}

///\relates nytl::vec
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb> 
vec<detail::dMin(dima,dimb), bool> operator<=(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    return lessThanEqual(veca, vecb);
}

///\relates nytl::vec
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb> 
vec<detail::dMin(dima, dimb), bool> operator>=(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    return greaterThanEqual(veca, vecb);
}

//utility
///\relates nytl::vec
template<std::size_t dim, typename T>  
auto sum(const vec<dim, T>& v) -> decltype(v[0] + v[0])
{
    decltype(v[0] + v[0]) ret {};
    for(auto& val : v) ret += val;
    return ret;
}

///\relates nytl::vec
template<std::size_t dim, typename prec>
auto length(const vec<dim, prec>& v) -> decltype(sqrt(v[0] * v[0]))
{
    decltype(v[0] * v[0]) ret{};
    for(auto& val : v) ret += val * val;
    return std::sqrt(ret);
}

///\relates nytl::vec
///Alias function for length()
template<std::size_t dim, typename prec>
auto norm(const vec<dim, prec>& v) -> decltype(length(v))
{
    return length(v);
}

///\relates nytl::vec
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb> 
auto dot(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb) -> decltype(sum(veca * vecb))
{
    return sum(veca * vecb);
}

///\relates nytl::vec
template<typename Ta, typename Tb> 
auto cross(const vec<3, Ta>& veca, const vec<3, Tb>& vecb) -> vec<3, decltype(veca[0] * vecb[0])>
{
    return vec<3, decltype(veca[0] * vecb[0])>
		{
			veca[2] * veca[3] - veca[3] * vecb[2], 
			veca[3] * veca[1] - veca[1] * vecb[3], 
			veca[1] * veca[2] - veca[2] * vecb[1]
		};
}

///\relates nytl::vec
///Returns the angle between 2 vecs, always the smaller one that is <= PI. 
///Returns the angle in radiant form.
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb>
double angle(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    return std::acos(sum(veca * vecb) / (length(veca) * length(vecb)));
}

///\relates nytl::vec
///Returns the smallest angle between two lines with the given vectors as direction.
///The Returned angle is always <= PI/2. Returns the angle in radian form.
template<std::size_t dima, std::size_t dimb, typename Ta, typename Tb> 
double smallerAngle(const vec<dima, Ta>& veca, const vec<dimb, Tb>& vecb)
{
    return std::acos(abs(sum(veca * vecb)) / (length(veca) * length(vecb)));
}

//todo: cangle for 3-dimensional (or all) vectors
///\relates nytl::vec
///Returns the absolute, clockwise angle between two 2-dimensional vectors.
///Angle is returned in radian form.
template<typename Ta, typename Tb> 
double cangle(const vec<2, Ta>& veca, const vec<2, Tb>& vecb)
{
    auto val = atan2(veca.y, veca.x) - atan2(vecb.y, vecb.x);
    if(val <= 0) return (2 * cPi) + val;
    return val;
}

///\relates nytl::vec
template<std::size_t dim, typename T>
auto normalize(const vec<dim, T>& veca) -> decltype(veca / length(veca))
{
    return veca / length(veca);
}

///\relates nytl::vec
template<std::size_t dim, typename T>
auto distance(const vec<dim, T>& veca, const vec<dim, T>& vecb) -> decltype(length(veca - vecb))
{
    return length(vecb - veca);
}

///\relates nytl::vec
template<std::size_t dim, typename T> 
vec<dim, double> radians(vec<dim, T> veca)
{
    for(auto& val : veca) val = radians(val);
    return veca;
}

///\relates nytl::vec
template<std::size_t dim, typename T>  
vec<dim, double> degrees(vec<dim, T> veca)
{
    for(auto& val : veca) val = degrees(val);
    return veca;
}

///\relates nytl::vec
template<std::size_t dim, typename T>
vec<dim, T> abs(vec<dim, T> veca)
{
    for(auto& val : veca) val = std::abs(val);
    return veca;
}

///\relates nytl::vec
template<std::size_t dim, typename Ta, typename Tb, typename Tc>  
vec<dim, Ta> clamp(vec<dim, Ta> val, const vec<dim, Tb>& minVal, const vec<dim, Tc>& maxVal)
{
    for(std::size_t i(0); i < min(min(val.size()), minVal.size(), maxVal.size()); ++i) 
		val[i] = clamp(val[i], minVal[i], maxVal[i]);
    return val;
}

///\relates nytl::vec
template<std::size_t dim, typename Ta, typename Tb, typename Tc>  
vec<dim, Ta> clamp(vec<dim, Ta> val, const Tb& minVal, const Tc& maxVal)
{
    for(auto& v : val) v = clamp(v, minVal, maxVal);
    return val;
}

///\relates nytl::vec
template<std::size_t dim, typename Ta, typename Tb>  
vec<dim, Ta> mix(vec<dim, Ta> x, const vec<dim, Ta>& y, const vec<dim, Tb>& a)
{
    for(std::size_t i(0); i < min(min(x.size(), y.size()), a.size()); ++i) 
		x[i] = mix(x[i], y[i], a[i]);
    return x;
}

///\relates nytl::vec
template<std::size_t dim, typename Ta, typename Tb>  
vec<dim, Ta> mix(vec<dim, Ta> x, const vec<dim, Ta>& y, const Tb& a)
{
    for(std::size_t i(0); i < min(min(x.size(), y.size()), a.size()); ++i) 
		x[i] = mix(x[i], y[i], a);
    return x;
}

///\relates nytl::vec
template<std::size_t dim, typename Ta, typename Tb>  
vec<dim, Ta> mix(const Ta& x, const Ta& y, const vec<dim, Tb>& a)
{
	vec<dim, Ta> ret(a.size());
    for(std::size_t i(0); i < a.size(); ++i) ret[i] = mix(x, y, a[i]);
    return ret;
}

///\relates nytl::vec
template<std::size_t dim, typename T>  
raw<T> smallest(const vec<dim, T>& a)
{
    raw<T> ret = a[0];
    for(auto& val : a)
		if(val < ret) ret = val;

    return ret;
}

///\relates nytl::vec
template<std::size_t dim, typename T>  
raw<T> greatest(const vec<dim, T>& a)
{
    raw<T> ret = a[0];
    for(auto& val : a)
        if(val > ret) ret = val;

    return ret;
}

//boolean vec operations
///\relates nytl::vec
template<std::size_t dim>  
bool any(const vec<dim, bool>& v)
{
    for(auto val : v) if(val) return 1;
    return 0;
}

///\relates nytl::vec
template<std::size_t dim>  
bool all(const vec<dim, bool>& v)
{
    for(auto val : v) if(!val) return 0;
    return 1;
}

///\relates nytl::vec
template<std::size_t dim>  
bool none(const vec<dim, bool>& v)
{
    for(auto val : v) if(val) return 0;
    return 1;
}

///\relates nytl::vec
template<std::size_t dim, typename T>  
vec<dim, T> operator!(vec<dim, T> v)
{
    for(auto& val : v) val = !val;
    return v;
}

//component-wise
///\relates nytl::vec
template<std::size_t dim, typename prec>  
vec<dim, prec> max(const vec<dim, prec>& veca, const vec<dim, prec>& vecb)
{
    vec<dim, prec> ret(min(veca.size(), vecb.size()));
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        ret[i] = max(veca[i], vecb[i]);

    return ret;
}

///\relates nytl::vec
template<std::size_t dim, typename prec>  
vec<dim, prec> max(const vec<dim, prec>& veca, const prec& value)
{
    vec<dim, prec> ret(veca.size());
    for(std::size_t i(0); i < veca.size(); ++i)
        ret[i] = max(veca[i], value);

    return ret;
}

///\relates nytl::vec
template<std::size_t dim, typename prec>  
vec<dim, prec> min(const vec<dim, prec>& veca, const vec<dim, prec>& vecb)
{
    vec<dim, prec> ret(min(veca.size(), vecb.size()));
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        ret[i] = min(veca[i], vecb[i]);

    return ret;
}

///\relates nytl::vec
template<std::size_t dim, typename prec>  
vec<dim, prec> min(const vec<dim, prec>& veca, const prec& value)
{
    vec<dim, prec> ret(veca.size());
    for(std::size_t i(0); i < veca.size(); ++i)
        ret[i] = min(veca[i], value);
    
    return ret;
}

