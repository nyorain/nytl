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
template<std::size_t D, typename T> std::ostream& 
operator<<(std::ostream& os, const vec<D, T>& obj)
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
template<std::size_t D, typename T> std::istream& 
operator>>(std::istream& is, vec<D, T>& obj)
{
    for(auto& val : obj)
    {
        is >> val;
    }

    return is;
}

//+todo: vec and vec unefficient atm, some proxy class 
///\relates nytl::vec
template<std::size_t D, typename T, typename O>  
auto operator+(const vec<D, T>& mvec, const O& other)
	-> vec<D, decltype(mvec[0] + other)>
{
	vec<D, decltype(mvec[0] + other)> ret = mvec;
	ret += other;
    return ret;
}

///\relates nytl::vec
template<std::size_t D, typename T, typename O>  
auto operator+(const O& other, const vec<D, T>& mvec)
	-> vec<D, decltype(other + mvec[0])>
{
	vec<D, decltype(mvec[0] + other)> ret = mvec;
	ret += other;
    return ret;
}

///\relates nytl::vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator+(const vec<DA, TA>& a, const vec<DB, TB>& b) 
	-> vec<detail::dMax(DA, DB), decltype(a[0] + b[0])>
{
	using V = vec<detail::dMax(DA, DB), decltype(a[0] + b[0])>;

    auto ret = static_cast<V>(a);
    ret += static_cast<V>(b);
    return ret;
}

//-
///\relates nytl::vec
template<std::size_t D, typename T, typename O>  
auto operator-(vec<D, T> mvec, const O& other)
	-> vec<D, decltype(mvec[0] - other)>
{
    mvec -= other;
    return mvec;
}

///\relates nytl::vec
template<std::size_t D, typename T, typename OT>  
auto operator-(const OT& other, const vec<D, T>& mvec)
	-> vec<D, decltype(other - mvec[0])>
{
	vec<D, decltype(other - mvec[0]) ret(mvec.size());
	for(std::size_t i(0); i < ret.size(); ++i)
		ret[i] = other - mvec[i];

    return mvec;
}

///\relates nytl::vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator-(const vec<DA, TA>& a, const vec<DB, TB>& b) 
	-> vec<detail::dMax(DA, DB), decltype(a[0] - b[0])>
{
	using V = vec<detail::dMax(DA, DB), decltype(a[0] - b[0])>;

    auto ret = static_cast<V>(a);
    ret -= static_cast<V>(b);
    return ret;
}

//*
///\relates nytl::vec
template<std::size_t D, typename T, typename O>  
auto operator*(const vec<D, T>& mvec, const O& other)
	-> vec<D, decltype(mvec[0] * other)>
{
	vec<D, decltype(mvec[0] * other)> ret = mvec;
	ret *= other;
    return ret;
}

///\relates nytl::vec
template<std::size_t D, typename T, typename O>  
auto operator*(const O& other, const vec<D, T>& mvec)
	-> vec<D, decltype(other * mvec[0])>
{
	vec<D, decltype(mvec[0] * other)> ret = mvec;
	ret *= other;
    return ret;
}

///\relates nytl::vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator*(const vec<DA, TA>& a, const vec<DB, TB>& b) 
	-> vec<detail::dMax(DA, DB), decltype(a[0] * b[0])>
{
	using V = vec<detail::dMax(DA, DB), decltype(a[0] * b[0])>;

    auto ret = static_cast<V>(a);
    ret *= static_cast<V>(b);
    return ret;
}

///\relates nytl::vec
template<std::size_t D, typename T, typename O>  
auto operator/(vec<D, T> mvec, const O& other)
	-> vec<D, decltype(mvec[0] / other)>
{
    mvec /= other;
    return mvec;
}

///\relates nytl::vec
template<std::size_t D, typename T, typename OT>  
auto operator/(const OT& other, const vec<D, T>& mvec)
	-> vec<D, decltype(other / mvec[0])>
{
	vec<D, decltype(other / mvec[0]) ret(mvec.size());
	for(std::size_t i(0); i < ret.size(); ++i)
		ret[i] = other / mvec[i];

    return mvec;
}

///\relates nytl::vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator/(const vec<DA, TA>& a, const vec<DB, TB>& b) 
	-> vec<detail::dMax(DA, DB), decltype(a[0] / b[0])>
{
	using V = vec<detail::dMax(DA, DB), decltype(a[0] / b[0])>;

    auto ret = static_cast<V>(a);
    ret /= static_cast<V>(b);
    return ret;
}

//%
///\relates nytl::vec
template<std::size_t D, typename T, typename O>  
auto operator%(vec<D, T> mvec, const O& other)
	-> vec<D, decltype(mvec[0] % other)>
{
    mvec %= other;
    return mvec;
}

///\relates nytl::vec
template<std::size_t D, typename T, typename OT>  
auto operator%(const OT& other, const vec<D, T>& mvec)
	-> vec<D, decltype(other % mvec[0])>
{
	vec<D, decltype(other % mvec[0]) ret(mvec.size());
	for(std::size_t i(0); i < ret.size(); ++i)
		ret[i] = other % mvec[i];

    return mvec;
}

///\relates nytl::vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator%(const vec<DA, TA>& a, const vec<DB, TB>& b) 
	-> vec<detail::dMax(DA, DB), decltype(a[0] % b[0])>
{
	using V = vec<detail::dMax(DA, DB), decltype(a[0] % b[0])>;

    auto ret = static_cast<V>(a);
    ret %= static_cast<V>(b);
    return ret;
}

//equal
///\relates nytl::vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
vec<detail::dMin(DA, DB), bool> equal(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    vec<detail::dMin(DA, DB), bool> ret(min(veca.size(), vecb.size()));
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        ret[i] = (veca[i] == vecb[i]);
    return ret;
}

///\relates nytl::vec
template<std::size_t D, typename TA, typename TB>  
vec<D, bool> equal(const vec<D, TA>& veca, const TB& value)
{
    vec<D, bool> ret(veca.size());
    for(std::size_t i(0); i < D; ++i)
        ret[i] = (veca[i] == value);
    return ret;
}

///\relates nytl::vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
vec<detail::dMin(DA, DB), bool> notEqual(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    vec<detail::dMin(DA, DB), bool> ret(min(veca.size(), vecb.size()));
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        ret[i] = (veca[i] != vecb[i]);
    return ret;
}

///\relates nytl::vec
template<std::size_t D, typename TA, typename TB>  
vec<D, bool> notEqual(const vec<D, TA>& veca, const TB& value)
{
    vec<D, bool> ret(veca.size());
    for(std::size_t i(0); i < D; ++i)
        ret[i] = (veca[i] != value);
    return ret;
}

///\relates nytl::vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
vec<detail::dMin(DA, DB), bool> lessThan(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    vec<detail::dMin(DA, DB), bool> ret(min(veca.size(), vecb.size()));
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        ret[i] = veca[i] < vecb[i];
    return ret;
}

///\relates nytl::vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
vec<min(DA, DB), bool> greaterThan(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    vec<min(DA, DB), bool> ret(min(veca.size(), vecb.size()));
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
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
vec<detail::dMin(DA, DB), bool> greaterThanEqual(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    vec<detail::dMin(DA, DB), bool> ret(min(veca.size(), vecb.size()));
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        ret[i] = veca[i] >= vecb[i];
    return ret;
}

///\relates nytl::vec
template<std::size_t D, typename TA, typename TB>  
vec<D, bool> lessThan(const vec<D, TA>& veca, const TB& value)
{
    vec<D, bool> ret(veca.size());
    for(std::size_t i(0); i < veca.size(); ++i)
        ret[i] = veca[i] < value;
    return ret;
}

///\relates nytl::vec
template<std::size_t D, typename TA, typename TB>  
vec<D, bool> greaterThan(const vec<D, TA>& veca, const TB& value)
{
    vec<D, bool> ret(veca.size());
    for(std::size_t i(0); i < veca.size(); ++i)
        ret[i] = veca[i] > value;
    return ret;
}

///\relates nytl::vec
template<std::size_t D, typename TA, typename TB>  
vec<D, bool> lessThanEqual(const vec<D, TA>& veca, const TB& value)
{
    vec<D, bool> ret(veca.size());
    for(std::size_t i(0); i < veca.size(); ++i)
        ret[i] = veca[i] <= value;
    return ret;
}

///\relates nytl::vec
template<std::size_t D, typename TA, typename TB>  
vec<D, bool> greaterThanEqual(const vec<D, TA>& veca, const TB& value)
{
    vec<D, bool> ret(veca.size());
    for(std::size_t i(0); i < veca.size(); ++i)
        ret[i] = veca[i] >= value;
    return ret;
}

//todo: allow comparison of vec and value with operator? 
//vec<D, bool> operator==(vec<D, TA> v, TB value);
///\relates nytl::vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
vec<detail::dMin(DA, DB), bool> operator==(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    return equal(veca, vecb);
}

///\relates nytl::vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
vec<detail::dMin(DA, DB), bool> operator!=(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    return notEqual(veca, vecb);
}

///\relates nytl::vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
vec<detail::dMin(DA, DB), bool> operator<(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    return lessThan(veca, vecb);
}

///\relates nytl::vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
vec<detail::dMin(DA, DB), bool> operator>(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    return greaterThan(veca, vecb);
}

///\relates nytl::vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
vec<detail::dMin(DA,DB), bool> operator<=(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    return lessThanEqual(veca, vecb);
}

///\relates nytl::vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
vec<detail::dMin(DA, DB), bool> operator>=(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    return greaterThanEqual(veca, vecb);
}

///\relates nytl::vec
template<std::size_t DA, typename TA, typename TB> 
vec<detail::dMin(DA, DB), bool> operator==(const vec<DA, TA>& veca, const TB& b)
{
    return equal(veca, b);
}

///\relates nytl::vec
template<std::size_t DA, typename TA, typename TB> 
vec<detail::dMin(DA, DB), bool> operator!=(const vec<DA, TA>& veca, const TB& b)
{
    return notEqual(veca, b);
}

///\relates nytl::vec
template<std::size_t DA, typename TA, typename TB> 
vec<detail::dMin(DA, DB), bool> operator<(const vec<DA, TA>& veca, const TB& b)
{
    return lessThan(veca, b);
}

///\relates nytl::vec
template<std::size_t DA, typename TA, typename TB> 
vec<detail::dMin(DA, DB), bool> operator>(const vec<DA, TA>& veca, const TB& b)
{
    return greaterThan(veca, b);
}

///\relates nytl::vec
template<std::size_t DA, typename TA, typename TB> 
vec<detail::dMin(DA, DB), bool> operator<=(const vec<DA, TA>& veca, const TB& b)
{
    return lessThanEqual(veca, b);
}

///\relates nytl::vec
template<std::size_t DA, typename TA, typename TB> 
vec<detail::dMin(DA, DB), bool> operator>=(const vec<DA, TA>& veca, const TB& b)
{
    return greaterThanEqual(veca, b);
}

///\relates nytl::vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
bool allEqual(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        if(veca[i] != vecb[i]) return 0;

    return 1;
}

///\relates nytl::vec
template<std::size_t D, typename TA, typename TB>  
bool allEqual(const vec<D, TA>& veca, const TB& value)
{
    for(std::size_t i(0); i < D; ++i)
		if(veca[i] != value) return 0;

	return 1; 
}

//utility
///\relates nytl::vec
///\return The sum of all vector components.
template<std::size_t D, typename T>  
auto sum(const vec<D, T>& v) -> decltype(v[0] + v[0])
{
    decltype(v[0] + v[0]) ret {};
    for(auto& val : v) ret += val;
    return ret;
}

///\relates nytl::vec
///\return The length of the vector (square-root of the sum of all its component squared).
template<std::size_t D, typename T>
auto length(const vec<D, T>& v) -> decltype(sqrt(v[0] * v[0]))
{
    decltype(v[0] * v[0]) ret{};
    for(auto& val : v) ret += val * val;
    return std::sqrt(ret);
}

///\relates nytl::vec
///Alias function for length.
///\return the length (norm) of the given vector.
template<std::size_t D, typename T>
auto norm(const vec<D, T>& v) -> decltype(length(v))
{
    return length(v);
}

///\relates nytl::vec
///\return The dot product of the given vectors.
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
auto dot(const vec<DA, TA>& veca, const vec<DB, TB>& vecb) -> decltype(sum(veca * vecb))
{
    return sum(veca * vecb);
}

///\relates nytl::vec
///Alias function for dot.
///\return The dot (scalar) product of the given vectors.
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
auto scalar(const vec<DA, TA>& veca, const vec<DB, TB>& vecb) -> decltype(sum(veca * vecb))
{
    return sum(veca * vecb);
}

///\relates nytl::vec
///\return The cross product for 2 3-Densional vectors.
template<typename TA, typename TB> 
auto cross(const vec<3, TA>& veca, const vec<3, TB>& vecb) -> vec<3, decltype(veca[0] * vecb[0])>
{
    return vec<3, decltype(veca[0] * vecb[0])>
		{
			veca[2] * veca[3] - veca[3] * vecb[2], 
			veca[3] * veca[1] - veca[1] * vecb[3], 
			veca[1] * veca[2] - veca[2] * vecb[1]
		};
}

///\relates nytl::vec
///\return The angle between 2 vecs in radiant form. Returns always the smaller one; angle <= PI. 
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
double angle(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    return std::acos(sum(veca * vecb) / (length(veca) * length(vecb)));
}

///\relates nytl::vec
///\return The smallest angle between two lines with the given vectors as direction in radiant 
///form. The Returned angle is always <= PI/2. 
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
double smallerAngle(const vec<DA, TA>& veca, const vec<DB, TB>& vecb)
{
    return std::acos(abs(sum(veca * vecb)) / (length(veca) * length(vecb)));
}

//todo: cangle for 3-Densional (or all) vectors
///\relates nytl::vec
///Returns the absolute, clockwise angle between two 2-Densional vectors.
///Angle is returned in radian form.
template<typename TA, typename TB> 
double cangle(const vec<2, TA>& veca, const vec<2, TB>& vecb)
{
    auto val = atan2(veca.y, veca.x) - atan2(vecb.y, vecb.x);
    if(val <= 0) return (2 * cPi) + val;
    return val;
}

///\relates nytl::vec
template<std::size_t D, typename T>
auto normalize(const vec<D, T>& veca) -> decltype(veca / length(veca))
{
    return veca / length(veca);
}

///\relates nytl::vec
template<std::size_t D, typename T>
auto distance(const vec<D, T>& veca, const vec<D, T>& vecb) -> decltype(length(veca - vecb))
{
    return length(vecb - veca);
}

///\relates nytl::vec
template<std::size_t D, typename T> 
vec<D, double> radians(vec<D, T> veca)
{
    for(auto& val : veca) val = radians(val);
    return veca;
}

///\relates nytl::vec
template<std::size_t D, typename T>  
vec<D, double> degrees(vec<D, T> veca)
{
    for(auto& val : veca) val = degrees(val);
    return veca;
}

///\relates nytl::vec
template<std::size_t D, typename T>
vec<D, T> abs(vec<D, T> veca)
{
    for(auto& val : veca) val = std::abs(val);
    return veca;
}

///\relates nytl::vec
template<std::size_t D, typename TA, typename TB, typename Tc>  
vec<D, TA> clamp(vec<D, TA> val, const vec<D, TB>& minVal, const vec<D, Tc>& maxVal)
{
    for(std::size_t i(0); i < min(min(val.size()), minVal.size(), maxVal.size()); ++i) 
		val[i] = clamp(val[i], minVal[i], maxVal[i]);
    return val;
}

///\relates nytl::vec
template<std::size_t D, typename TA, typename TB, typename Tc>  
vec<D, TA> clamp(vec<D, TA> val, const TB& minVal, const Tc& maxVal)
{
    for(auto& v : val) v = clamp(v, minVal, maxVal);
    return val;
}

///\relates nytl::vec
template<std::size_t D, typename TA, typename TB>  
vec<D, TA> mix(vec<D, TA> x, const vec<D, TA>& y, const vec<D, TB>& a)
{
    for(std::size_t i(0); i < min(min(x.size(), y.size()), a.size()); ++i) 
		x[i] = mix(x[i], y[i], a[i]);
    return x;
}

///\relates nytl::vec
template<std::size_t D, typename TA, typename TB>  
vec<D, TA> mix(vec<D, TA> x, const vec<D, TA>& y, const TB& a)
{
    for(std::size_t i(0); i < min(min(x.size(), y.size()), a.size()); ++i) 
		x[i] = mix(x[i], y[i], a);
    return x;
}

///\relates nytl::vec
template<std::size_t D, typename TA, typename TB>  
vec<D, TA> mix(const TA& x, const TA& y, const vec<D, TB>& a)
{
	vec<D, TA> ret(a.size());
    for(std::size_t i(0); i < a.size(); ++i) ret[i] = mix(x, y, a[i]);
    return ret;
}

///\relates nytl::vec
template<std::size_t D, typename T>  
raw<T> smallest(const vec<D, T>& a)
{
    raw<T> ret = a[0];
    for(auto& val : a)
		if(val < ret) ret = val;

    return ret;
}

///\relates nytl::vec
template<std::size_t D, typename T>  
raw<T> greatest(const vec<D, T>& a)
{
    raw<T> ret = a[0];
    for(auto& val : a)
        if(val > ret) ret = val;

    return ret;
}

//boolean vec operations
///\relates nytl::vec
template<std::size_t D>  
bool any(const vec<D, bool>& v)
{
    for(auto val : v) if(val) return 1;
    return 0;
}

///\relates nytl::vec
template<std::size_t D>  
bool all(const vec<D, bool>& v)
{
    for(auto val : v) if(!val) return 0;
    return 1;
}

///\relates nytl::vec
template<std::size_t D>  
bool none(const vec<D, bool>& v)
{
    for(auto val : v) if(val) return 0;
    return 1;
}

///\relates nytl::vec
template<std::size_t D, typename T>  
vec<D, T> operator!(vec<D, T> v)
{
    for(auto& val : v) val = !val;
    return v;
}

//component-wise
///\relates nytl::vec
template<std::size_t D, typename T>  
vec<D, T> max(const vec<D, T>& veca, const vec<D, T>& vecb)
{
    vec<D, T> ret(min(veca.size(), vecb.size()));
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        ret[i] = max(veca[i], vecb[i]);

    return ret;
}

///\relates nytl::vec
template<std::size_t D, typename T>  
vec<D, T> max(const vec<D, T>& veca, const T& value)
{
    vec<D, T> ret(veca.size());
    for(std::size_t i(0); i < veca.size(); ++i)
        ret[i] = max(veca[i], value);

    return ret;
}

///\relates nytl::vec
template<std::size_t D, typename T>  
vec<D, T> min(const vec<D, T>& veca, const vec<D, T>& vecb)
{
    vec<D, T> ret(min(veca.size(), vecb.size()));
    for(std::size_t i(0); i < min(veca.size(), vecb.size()); ++i)
        ret[i] = min(veca[i], vecb[i]);

    return ret;
}

///\relates nytl::vec
template<std::size_t D, typename T>  
vec<D, T> min(const vec<D, T>& veca, const T& value)
{
    vec<D, T> ret(veca.size());
    for(std::size_t i(0); i < veca.size(); ++i)
        ret[i] = min(veca[i], value);
    
    return ret;
}

