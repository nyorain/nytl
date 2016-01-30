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
///\relates nytl::Vec
///Prints all values individually to an output stream.
template<std::size_t D, typename T> std::ostream& 
operator<<(std::ostream& os, const Vec<D, T>& obj)
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

///\relates nytl::Vec
///Loads all components from an input stream.
template<std::size_t D, typename T> std::istream& 
operator>>(std::istream& is, Vec<D, T>& obj)
{
    for(auto& val : obj)
    {
        is >> val;
    }

    return is;
}

//+todo: Vec and Vec unefficient atm, some proxy class 
///\relates nytl::Vec
template<std::size_t D, typename T, typename O>  
auto operator+(const Vec<D, T>& mVec, const O& other)
	-> Vec<D, decltype(mVec[0] + other)>
{
	Vec<D, decltype(mVec[0] + other)> ret = mVec;
	ret += other;
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename O>  
auto operator+(const O& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other + mVec[0])>
{
	Vec<D, decltype(mVec[0] + other)> ret = mVec;
	ret += other;
    return ret;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator+(const Vec<DA, TA>& a, const Vec<DB, TB>& b) 
	-> Vec<detail::dMax(DA, DB), decltype(a[0] + b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] + b[0])>;

    auto ret = static_cast<V>(a);
    ret += static_cast<V>(b);
    return ret;
}

//-
///\relates nytl::Vec
template<std::size_t D, typename T, typename O>  
auto operator-(Vec<D, T> mVec, const O& other)
	-> Vec<D, decltype(mVec[0] - other)>
{
    mVec -= other;
    return mVec;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename OT>  
auto operator-(const OT& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other - mVec[0])>
{
	Vec<D, decltype(other - mVec[0])> ret(mVec.size());
	for(std::size_t i(0); i < ret.size(); ++i)
		ret[i] = other - mVec[i];

    return mVec;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator-(const Vec<DA, TA>& a, const Vec<DB, TB>& b) 
	-> Vec<detail::dMax(DA, DB), decltype(a[0] - b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] - b[0])>;

    auto ret = static_cast<V>(a);
    ret -= static_cast<V>(b);
    return ret;
}

//*
///\relates nytl::Vec
template<std::size_t D, typename T, typename O>  
auto operator*(const Vec<D, T>& mVec, const O& other)
	-> Vec<D, decltype(mVec[0] * other)>
{
	Vec<D, decltype(mVec[0] * other)> ret = mVec;
	ret *= other;
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename O>  
auto operator*(const O& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other * mVec[0])>
{
	Vec<D, decltype(mVec[0] * other)> ret = mVec;
	ret *= other;
    return ret;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator*(const Vec<DA, TA>& a, const Vec<DB, TB>& b) 
	-> Vec<detail::dMax(DA, DB), decltype(a[0] * b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] * b[0])>;

    auto ret = static_cast<V>(a);
    ret *= static_cast<V>(b);
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename O>  
auto operator/(Vec<D, T> mVec, const O& other)
	-> Vec<D, decltype(mVec[0] / other)>
{
    mVec /= other;
    return mVec;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename OT>  
auto operator/(const OT& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other / mVec[0])>
{
	Vec<D, decltype(other / mVec[0])> ret(mVec.size());
	for(std::size_t i(0); i < ret.size(); ++i)
		ret[i] = other / mVec[i];

    return mVec;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator/(const Vec<DA, TA>& a, const Vec<DB, TB>& b) 
	-> Vec<detail::dMax(DA, DB), decltype(a[0] / b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] / b[0])>;

    auto ret = static_cast<V>(a);
    ret /= static_cast<V>(b);
    return ret;
}

//%
///\relates nytl::Vec
template<std::size_t D, typename T, typename O>  
auto operator%(Vec<D, T> mVec, const O& other)
	-> Vec<D, decltype(mVec[0] % other)>
{
    mVec %= other;
    return mVec;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename OT>  
auto operator%(const OT& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other % mVec[0])>
{
	Vec<D, decltype(other % mVec[0])> ret(mVec.size());
	for(std::size_t i(0); i < ret.size(); ++i)
		ret[i] = other % mVec[i];

    return mVec;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator%(const Vec<DA, TA>& a, const Vec<DB, TB>& b) 
	-> Vec<detail::dMax(DA, DB), decltype(a[0] % b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] % b[0])>;

    auto ret = static_cast<V>(a);
    ret %= static_cast<V>(b);
    return ret;
}

//equal
///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA, DB), bool> equal(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    Vec<detail::dMin(DA, DB), bool> ret(min(Veca.size(), Vecb.size()));
    for(std::size_t i(0); i < min(Veca.size(), Vecb.size()); ++i)
        ret[i] = (Veca[i] == Vecb[i]);
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, bool> equal(const Vec<D, TA>& Veca, const TB& value)
{
    Vec<D, bool> ret(Veca.size());
    for(std::size_t i(0); i < D; ++i)
        ret[i] = (Veca[i] == value);
    return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
Vec<detail::dMin(DA, DB), bool> notEqual(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    Vec<detail::dMin(DA, DB), bool> ret(min(Veca.size(), Vecb.size()));
    for(std::size_t i(0); i < min(Veca.size(), Vecb.size()); ++i)
        ret[i] = (Veca[i] != Vecb[i]);
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, bool> notEqual(const Vec<D, TA>& Veca, const TB& value)
{
    Vec<D, bool> ret(Veca.size());
    for(std::size_t i(0); i < D; ++i)
        ret[i] = (Veca[i] != value);
    return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
Vec<detail::dMin(DA, DB), bool> lessThan(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    Vec<detail::dMin(DA, DB), bool> ret(min(Veca.size(), Vecb.size()));
    for(std::size_t i(0); i < min(Veca.size(), Vecb.size()); ++i)
        ret[i] = Veca[i] < Vecb[i];
    return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
Vec<min(DA, DB), bool> greaterThan(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    Vec<min(DA, DB), bool> ret(min(Veca.size(), Vecb.size()));
    for(std::size_t i(0); i < min(Veca.size(), Vecb.size()); ++i)
        ret[i] = Veca[i] > Vecb[i];
    return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA, DB), bool> lessThanEqual(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    Vec<detail::dMin(DA, DB), bool> ret(min(Veca.size(), Vecb.size()));
    for(std::size_t i(0); i < min(Veca.size(), Vecb.size()); ++i)
        ret[i] = Veca[i] <= Vecb[i];
    return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
Vec<detail::dMin(DA, DB), bool> greaterThanEqual(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    Vec<detail::dMin(DA, DB), bool> ret(min(Veca.size(), Vecb.size()));
    for(std::size_t i(0); i < min(Veca.size(), Vecb.size()); ++i)
        ret[i] = Veca[i] >= Vecb[i];
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, bool> lessThan(const Vec<D, TA>& Veca, const TB& value)
{
    Vec<D, bool> ret(Veca.size());
    for(std::size_t i(0); i < Veca.size(); ++i)
        ret[i] = Veca[i] < value;
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, bool> greaterThan(const Vec<D, TA>& Veca, const TB& value)
{
    Vec<D, bool> ret(Veca.size());
    for(std::size_t i(0); i < Veca.size(); ++i)
        ret[i] = Veca[i] > value;
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, bool> lessThanEqual(const Vec<D, TA>& Veca, const TB& value)
{
    Vec<D, bool> ret(Veca.size());
    for(std::size_t i(0); i < Veca.size(); ++i)
        ret[i] = Veca[i] <= value;
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, bool> greaterThanEqual(const Vec<D, TA>& Veca, const TB& value)
{
    Vec<D, bool> ret(Veca.size());
    for(std::size_t i(0); i < Veca.size(); ++i)
        ret[i] = Veca[i] >= value;
    return ret;
}

//todo: allow comparison of Vec and value with operator? 
//Vec<D, bool> operator==(Vec<D, TA> v, TB value);
///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
Vec<detail::dMin(DA, DB), bool> operator==(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    return equal(Veca, Vecb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
Vec<detail::dMin(DA, DB), bool> operator!=(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    return notEqual(Veca, Vecb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
Vec<detail::dMin(DA, DB), bool> operator<(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    return lessThan(Veca, Vecb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
Vec<detail::dMin(DA, DB), bool> operator>(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    return greaterThan(Veca, Vecb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
Vec<detail::dMin(DA,DB), bool> operator<=(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    return lessThanEqual(Veca, Vecb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
Vec<detail::dMin(DA, DB), bool> operator>=(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    return greaterThanEqual(Veca, Vecb);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB> 
Vec<DA, bool> operator==(const Vec<DA, TA>& Veca, const TB& b)
{
    return equal(Veca, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB> 
Vec<DA, bool> operator!=(const Vec<DA, TA>& Veca, const TB& b)
{
    return notEqual(Veca, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB> 
Vec<DA, bool> operator<(const Vec<DA, TA>& Veca, const TB& b)
{
    return lessThan(Veca, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB> 
Vec<DA, bool> operator>(const Vec<DA, TA>& Veca, const TB& b)
{
    return greaterThan(Veca, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB> 
Vec<DA, bool> operator<=(const Vec<DA, TA>& Veca, const TB& b)
{
    return lessThanEqual(Veca, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB> 
Vec<DA, bool> operator>=(const Vec<DA, TA>& Veca, const TB& b)
{
    return greaterThanEqual(Veca, b);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
bool allEqual(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    for(std::size_t i(0); i < min(Veca.size(), Vecb.size()); ++i)
        if(Veca[i] != Vecb[i]) return 0;

    return 1;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
bool allEqual(const Vec<D, TA>& Veca, const TB& value)
{
    for(std::size_t i(0); i < D; ++i)
		if(Veca[i] != value) return 0;

	return 1; 
}

//utility
///\relates nytl::Vec
///\return The sum of all Vector components.
template<std::size_t D, typename T>  
auto sum(const Vec<D, T>& v) -> decltype(v[0] + v[0])
{
    decltype(v[0] + v[0]) ret {};
    for(auto& val : v) ret += val;
    return ret;
}

///\relates nytl::Vec
///\return The length of the Vector (square-root of the sum of all its component squared).
template<std::size_t D, typename T>
auto length(const Vec<D, T>& v) -> decltype(sqrt(v[0] * v[0]))
{
    decltype(v[0] * v[0]) ret{};
    for(auto& val : v) ret += val * val;
    return std::sqrt(ret);
}

///\relates nytl::Vec
///Alias function for length.
///\return the length (norm) of the given Vector.
template<std::size_t D, typename T>
auto norm(const Vec<D, T>& v) -> decltype(length(v))
{
    return length(v);
}

///\relates nytl::Vec
///\return The dot product of the given Vectors.
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
auto dot(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb) -> decltype(sum(Veca * Vecb))
{
    return sum(Veca * Vecb);
}

///\relates nytl::Vec
///Alias function for dot.
///\return The dot (scalar) product of the given Vectors.
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
auto scalar(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb) -> decltype(sum(Veca * Vecb))
{
    return sum(Veca * Vecb);
}

///\relates nytl::Vec
///\return The cross product for 2 3-Densional Vectors.
template<typename TA, typename TB> 
auto cross(const Vec<3, TA>& Veca, const Vec<3, TB>& Vecb) -> Vec<3, decltype(Veca[0] * Vecb[0])>
{
    return Vec<3, decltype(Veca[0] * Vecb[0])>
		{
			Veca[2] * Veca[3] - Veca[3] * Vecb[2], 
			Veca[3] * Veca[1] - Veca[1] * Vecb[3], 
			Veca[1] * Veca[2] - Veca[2] * Vecb[1]
		};
}

///\relates nytl::Vec
///\return The angle between 2 Vecs in radiant form. Returns always the smaller one; angle <= PI. 
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
double angle(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    return std::acos(sum(Veca * Vecb) / (length(Veca) * length(Vecb)));
}

///\relates nytl::Vec
///\return The smallest angle between two lines with the given Vectors as direction in radiant 
///form. The Returned angle is always <= PI/2. 
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
double smallerAngle(const Vec<DA, TA>& Veca, const Vec<DB, TB>& Vecb)
{
    return std::acos(abs(sum(Veca * Vecb)) / (length(Veca) * length(Vecb)));
}

//todo: cangle for 3-Densional (or all) Vectors
///\relates nytl::Vec
///Returns the absolute, clockwise angle between two 2-Densional Vectors.
///Angle is returned in radian form.
template<typename TA, typename TB> 
double cangle(const Vec<2, TA>& Veca, const Vec<2, TB>& Vecb)
{
    auto val = atan2(Veca.y, Veca.x) - atan2(Vecb.y, Vecb.x);
    if(val <= 0) return (2 * cPi) + val;
    return val;
}

///\relates nytl::Vec
template<std::size_t D, typename T>
auto normalize(const Vec<D, T>& Veca) -> decltype(Veca / length(Veca))
{
    return Veca / length(Veca);
}

///\relates nytl::Vec
template<std::size_t D, typename T>
auto distance(const Vec<D, T>& Veca, const Vec<D, T>& Vecb) -> decltype(length(Veca - Vecb))
{
    return length(Vecb - Veca);
}

///\relates nytl::Vec
template<std::size_t D, typename T> 
Vec<D, double> radians(Vec<D, T> Veca)
{
    for(auto& val : Veca) val = radians(val);
    return Veca;
}

///\relates nytl::Vec
template<std::size_t D, typename T>  
Vec<D, double> degrees(Vec<D, T> Veca)
{
    for(auto& val : Veca) val = degrees(val);
    return Veca;
}

///\relates nytl::Vec
template<std::size_t D, typename T>
Vec<D, T> abs(Vec<D, T> Veca)
{
    for(auto& val : Veca) val = std::abs(val);
    return Veca;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB, typename Tc>  
Vec<D, TA> clamp(Vec<D, TA> val, const Vec<D, TB>& minVal, const Vec<D, Tc>& maxVal)
{
    for(std::size_t i(0); i < min(min(val.size(), minVal.size()), maxVal.size()); ++i) 
		val[i] = clamp(val[i], minVal[i], maxVal[i]);
    return val;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB, typename Tc>  
Vec<D, TA> clamp(Vec<D, TA> val, const TB& minVal, const Tc& maxVal)
{
    for(auto& v : val) v = clamp(v, minVal, maxVal);
    return val;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, TA> mix(Vec<D, TA> x, const Vec<D, TA>& y, const Vec<D, TB>& a)
{
    for(std::size_t i(0); i < min(min(x.size(), y.size()), a.size()); ++i) 
		x[i] = mix(x[i], y[i], a[i]);
    return x;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, TA> mix(Vec<D, TA> x, const Vec<D, TA>& y, const TB& a)
{
    for(std::size_t i(0); i < min(min(x.size(), y.size()), a.size()); ++i) 
		x[i] = mix(x[i], y[i], a);
    return x;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, TA> mix(const TA& x, const TA& y, const Vec<D, TB>& a)
{
	Vec<D, TA> ret(a.size());
    for(std::size_t i(0); i < a.size(); ++i) ret[i] = mix(x, y, a[i]);
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>  
raw<T> smallest(const Vec<D, T>& a)
{
    raw<T> ret = a[0];
    for(auto& val : a)
		if(val < ret) ret = val;

    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>  
raw<T> greatest(const Vec<D, T>& a)
{
    raw<T> ret = a[0];
    for(auto& val : a)
        if(val > ret) ret = val;

    return ret;
}

//boolean Vec operations
///\relates nytl::Vec
template<std::size_t D>  
bool any(const Vec<D, bool>& v)
{
    for(auto val : v) if(val) return 1;
    return 0;
}

///\relates nytl::Vec
template<std::size_t D>  
bool all(const Vec<D, bool>& v)
{
    for(auto val : v) if(!val) return 0;
    return 1;
}

///\relates nytl::Vec
template<std::size_t D>  
bool none(const Vec<D, bool>& v)
{
    for(auto val : v) if(val) return 0;
    return 1;
}

///\relates nytl::Vec
template<std::size_t N, std::size_t D, typename T>
Vec<N, T> subVec(const Vec<D, T>& va, std::size_t pos = 0)
{
	return va.template subVec<N>(pos);
}

///\relates nytl::Vec
template<std::size_t D, typename T>
Vec<dynamicSize, T> subVec(const Vec<D, T>& va, std::size_t pos = 0, std::size_t size = -1)
{
	return va.subVec(pos, size);
}

///\relates nytl::Vec
template<std::size_t D, typename T>  
Vec<D, T> operator!(Vec<D, T> v)
{
    for(auto& val : v) val = !val;
    return v;
}

//component-wise
///\relates nytl::Vec
template<std::size_t D, typename T>  
Vec<D, T> max(const Vec<D, T>& Veca, const Vec<D, T>& Vecb)
{
    Vec<D, T> ret(min(Veca.size(), Vecb.size()));
    for(std::size_t i(0); i < min(Veca.size(), Vecb.size()); ++i)
        ret[i] = max(Veca[i], Vecb[i]);

    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>  
Vec<D, T> max(const Vec<D, T>& Veca, const T& value)
{
    Vec<D, T> ret(Veca.size());
    for(std::size_t i(0); i < Veca.size(); ++i)
        ret[i] = max(Veca[i], value);

    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>  
Vec<D, T> min(const Vec<D, T>& Veca, const Vec<D, T>& Vecb)
{
    Vec<D, T> ret(min(Veca.size(), Vecb.size()));
    for(std::size_t i(0); i < min(Veca.size(), Vecb.size()); ++i)
        ret[i] = min(Veca[i], Vecb[i]);

    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>  
Vec<D, T> min(const Vec<D, T>& Veca, const T& value)
{
    Vec<D, T> ret(Veca.size());
    for(std::size_t i(0); i < Veca.size(); ++i)
        ret[i] = min(Veca[i], value);
    
    return ret;
}

