#pragma once

#include <ostream>

namespace nyutil
{

template<size_t dim, class T> class vec;

//typedefs
template<class T> using vec2 = vec<2, T>;
template<class T> using vec3 = vec<3, T>;
template<class T> using vec4 = vec<4, T>;

typedef vec2<float> vec2f;
typedef vec2<int> vec2i;
typedef vec2<unsigned int> vec2ui;
typedef vec2<double> vec2d;
typedef vec2<char> vec2c;
typedef vec2<unsigned char> vec2uc;
typedef vec2<long> vec2l;
typedef vec2<unsigned long> vec2ul;

typedef vec3<float> vec3f;
typedef vec3<int> vec3i;
typedef vec3<unsigned int> vec3ui;
typedef vec3<double> vec3d;
typedef vec3<char> vec3c;
typedef vec3<unsigned char> vec3uc;
typedef vec3<long> vec3l;
typedef vec3<unsigned long> vec3ul;

typedef vec4<float> vec4f;
typedef vec4<int> vec4i;
typedef vec4<unsigned int> vec4ui;
typedef vec4<double> vec4d;
typedef vec4<char> vec4c;
typedef vec4<unsigned char> vec4uc;
typedef vec4<long> vec4l;
typedef vec4<unsigned long> vec4ul;


//vec pod
template<size_t dim, class T> class vec
{
public:
    T data[dim];

public:
    vec() { setAllTo(T()); }
    ~vec() = default;

    vec(const vec<dim, T>& other) = default;
    vec<dim, T>& operator=(const vec<dim, T>& other) = default;

    vec(vec<dim, T>&& other) = default;
    vec<dim, T>& operator=(vec<dim, T>&& other) = default;

    T& operator[](size_t i){ return data[i]; }
    const T& operator[](size_t i) const { return data[i]; }

    template <size_t odim, class ot> vec<dim,T>& operator +=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) data[i] += other[i];  return *this; }
    template <size_t odim, class ot> vec<dim,T>& operator -=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) data[i] -= other[i];  return *this; }
    template <size_t odim, class ot> vec<dim,T>& operator *=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) data[i] *= other[i];  return *this; }
    template <size_t odim, class ot> vec<dim,T>& operator /=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) data[i] /= other[i];  return *this; }
    template <size_t odim, class ot> vec<dim,T>& operator %=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) data[i] %= other[i];  return *this; }

    vec<dim,T>& operator +=(const T& other){ for(auto& val : data) val += other;  return *this; }
    vec<dim,T>& operator -=(const T& other){ for(auto& val : data) val -= other;  return *this; }
    vec<dim,T>& operator *=(const T& other){ for(auto& val : data) val *= other;  return *this; }
    vec<dim,T>& operator /=(const T& other){ for(auto& val : data) val /= other;  return *this; }
    vec<dim,T>& operator %=(const T& other){ for(auto& val : data) val %= other;  return *this; }

    vec<dim, T> operator-() const { vec<dim,T> ret(*this); for(size_t i(0); i < dim; i++) ret[i] -= data[i]; return ret; }

    template <class ot> operator vec<dim, ot>() const { vec<dim, ot> ret; for(size_t i(0); i < dim; i++) ret[i] = data[i]; return ret; }
    template <size_t odim, class ot> operator vec<odim, ot>() const { vec<odim, ot> ret; for(size_t i(0); i < std::min(odim, dim); i++) ret[i] = data[i]; return ret; }

    void setAllTo(const T& value){ for(auto& val : data)val = value; }

    constexpr size_t size() const { return dim; }
};

//operators
template<size_t dim, class T> std::ostream& operator<<(std::ostream& os, const vec<dim, T>& obj)
{
    const char* c = "";

    os << "(";

    for(unsigned int i(0); i < dim; i++)
    {
        os << c << obj[i];
        c = ",";
    }

    os << ")";

    return os;
}


//+/////////////////////////////////////////
template<size_t dim, class T> vec<dim, T> operator+(vec<dim, T> mvec, const T& other)
{
    mvec += other;
    return mvec;
}

template<size_t dim, class T> vec<dim, T> operator+(const T& other, vec<dim, T> mvec)
{
    mvec += other;
    return mvec;
}

template<size_t dima, class Ta, size_t dimb, class Tb, class = std::enable_if_t<(dima >= dimb)>> vec<dima, Ta> operator+(vec<dima, Ta> a, const vec<dimb,Tb>& b)
{
    a += b;
    return a;
}

template<size_t dima, class Ta, size_t dimb, class Tb, class = std::enable_if_t<(dimb > dima)>> vec<dimb, Tb> operator+(const vec<dima, Ta>& a, vec<dimb,Tb> b)
{
    b += a;
    return b;
}



//-//////////////////////////
template<size_t dim, class T> vec<dim, T> operator-(vec<dim, T> mvec, const T& other)
{
    mvec -= other;
    return mvec;
}

template<size_t dim, class T> vec<dim, T> operator-(const T& other, vec<dim, T> mvec)
{
    for(unsigned int i(0); i < dim; i++)
    {
        mvec[i] = other - mvec[i];
    }

    return mvec;
}

template<size_t dima, class Ta, size_t dimb, class Tb, class = std::enable_if_t<(dima >= dimb)>> vec<dima, Ta> operator-(vec<dima, Ta> a, const vec<dimb,Tb>& b)
{
    a -= b;
    return a;
}

template<size_t dima, class Ta, size_t dimb, class Tb, class = std::enable_if_t<(dimb > dima)>> vec<dimb, Tb> operator-(const vec<dima, Ta>& a, const vec<dimb,Tb>& b)
{
    vec<dimb, Tb> ret(a);
    ret -= b;
    return ret;
}



//*////////////////////////////////////////
template<size_t dim, class T> vec<dim, T> operator*(vec<dim, T> mvec, const T& other)
{
    mvec *= other;
    return mvec;
}


template<size_t dim, class T> vec<dim, T> operator*(const T& other, vec<dim, T> mvec)
{
    mvec *= other;
    return mvec;
}

template<size_t dima, class Ta, size_t dimb, class Tb, class = std::enable_if_t<(dima >= dimb)>> vec<dima, Ta> operator*(vec<dima, Ta> a, const vec<dimb,Tb>& b)
{
    a *= b;
    return a;
}

template<size_t dima, class Ta, size_t dimb, class Tb, class = std::enable_if_t<(dimb > dima)>> vec<dimb, Tb> operator*(const vec<dima, Ta>& a, vec<dimb,Tb> b)
{
    b *= a;
    return b;
}




//\//////////////////////////////////////////////////////////
template<size_t dim, class T> vec<dim, T> operator/(vec<dim, T> mvec, const T& other)
{
    mvec /= other;
    return mvec;
}


template<size_t dim, class T> vec<dim, T> operator/(const T& other, vec<dim, T> mvec)
{
    for(unsigned int i(0); i < dim; i++)
    {
        mvec[i] = other / mvec[i];
    }

    return mvec;
}

template<size_t dima, class Ta, size_t dimb, class Tb, class = std::enable_if_t<(dima >= dimb)>> vec<dima, Ta> operator/(vec<dima, Ta> a, const vec<dimb,Tb>& b)
{
    a /= b;
    return a;
}

template<size_t dima, class Ta, size_t dimb, class Tb, class = std::enable_if_t<(dimb > dima)>> vec<dimb, Tb> operator/(const vec<dima, Ta>& a, const vec<dimb,Tb>& b)
{
    vec<dimb, Tb> ret(a);
    ret /= b;
    return ret;
}


//%////////////////////////////////////////////////////////////
template<size_t dim, class T> vec<dim, T> operator%(vec<dim, T> mvec, const T& other)
{
    mvec %= other;
    return mvec;
}


template<size_t dim, class T> vec<dim, T> operator%(const T& other, vec<dim, T> mvec)
{
    for(unsigned int i(0); i < dim; i++)
    {
        mvec[i] = other % mvec[i];
    }

    return mvec;
}

template<size_t dima, class Ta, size_t dimb, class Tb, class = std::enable_if_t<(dima >= dimb)>> vec<dima, Ta> operator%(vec<dima, Ta> a, const vec<dimb,Tb>& b)
{
    a %= b;
    return a;
}

template<size_t dima, class Ta, size_t dimb, class Tb, class = std::enable_if_t<(dimb > dima)>> vec<dimb, Tb> operator%(const vec<dima, Ta>& a, const vec<dimb,Tb>& b)
{
    vec<dimb, Tb> ret(a);
    ret %= b;
    return ret;
}


//equal/////////////////////////////////////////////////////////////////////////
template<size_t dim, class Ta, class Tb> bool operator==(const vec<dim, Ta>& a, const vec<dim, Tb>& b)
{
    for(unsigned int i(0); i < dim; i++)
    {
        if(a[i] != b[i])
            return false;
    }

    return true;
}

template<size_t dim, class Ta, class Tb> bool operator!=(const vec<dim, Ta>& a, const vec<dim, Tb>& b)
{
    for(unsigned int i(0); i < dim; i++)
    {
        if(a[i] != b[i])
            return true;
    }

    return false;
}

//vec specializations for vec2, vec3 and vec4
/////////////////////////////////////
//vec2
//no pod anymore?! behaves like one.
template<class T> class vec<2, T>
{
public:
	T x;
 	T y;

	vec(T a = T(), T b = T()) : x(a), y(b) {}
	~vec() = default;

	vec(const vec<2,T>& other) = default;
	vec<2, T>& operator=(const vec<2, T>& other) = default;

    vec(vec<2,T>&& other) = default;
    vec<2, T>& operator=(vec<2, T>&& other) = default;

	T& operator[](size_t i){ if(i == 0) return x; return y; }
	const T& operator[](size_t i) const { if(i == 0) return x; return y; }

	template <size_t odim, class ot> vec<2,T>& operator +=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 2); i++) (*this)[i] += other[i];  return *this; }
	template <size_t odim, class ot> vec<2,T>& operator -=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 2); i++) (*this)[i] -= other[i];  return *this; }
	template <size_t odim, class ot> vec<2,T>& operator *=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 2); i++) (*this)[i] *= other[i];  return *this; }
	template <size_t odim, class ot> vec<2,T>& operator /=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 2); i++) (*this)[i] /= other[i];  return *this; }
	template <size_t odim, class ot> vec<2,T>& operator %=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 2); i++) (*this)[i] %= other[i];  return *this; }

	vec<2,T>& operator +=(const T& other){ for(size_t i = 0; i < 2; i++) (*this)[i] += other;  return *this; }
	vec<2,T>& operator -=(const T& other){ for(size_t i = 0; i < 2; i++) (*this)[i] -= other;  return *this; }
	vec<2,T>& operator *=(const T& other){ for(size_t i = 0; i < 2; i++) (*this)[i] *= other;  return *this; }
	vec<2,T>& operator /=(const T& other){ for(size_t i = 0; i < 2; i++) (*this)[i] /= other;  return *this; }
	vec<2,T>& operator %=(const T& other){ for(size_t i = 0; i < 2; i++) (*this)[i] %= other;  return *this; }

	vec<2, T> operator-() const { return vec<2,T>(-x, -y); }

	template <class ot> operator vec<2, ot>() const { return vec<2,ot>(x, y); }
	template <size_t odim, class ot> operator vec<odim, ot>() const { vec<odim, ot> ret; if(odim > 0) ret[0] = x; if(odim > 1) ret[1] = x; return ret; }

	void setAllTo(const T& value){ x = value; y = value; }

	constexpr size_t size() const { return 2; }
};



/////////////////////////////////////
//vec3
template<class T> class vec<3, T>
{
public:
    T x;
    T y;
    T z;

	vec(T a = T(), T b = T(), T c = T()) : x(a), y(b), z(c) {}
	~vec() = default;

	vec(const vec<3,T>& other) = default;
    vec<3, T>& operator=(const vec<3, T>& other) = default;

    vec(vec<3,T>&& other) = default;
    vec<3, T>& operator=(vec<3, T>&& other) = default;

	T& operator[](size_t i){ if(i == 0) return x; if(i == 1) return y; return z; }
	const T& operator[](size_t i) const { if(i == 0) return x; if(i == 1) return y; return z; }

    template <size_t odim, class ot> vec<3,T>& operator +=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 3); i++) (*this)[i] += other[i]; return *this; }
    template <size_t odim, class ot> vec<3,T>& operator -=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 3); i++) (*this)[i] -= other[i]; return *this; }
    template <size_t odim, class ot> vec<3,T>& operator *=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 3); i++) (*this)[i] *= other[i]; return *this; }
    template <size_t odim, class ot> vec<3,T>& operator /=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 3); i++) (*this)[i] /= other[i]; return *this; }
    template <size_t odim, class ot> vec<3,T>& operator %=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 3); i++) (*this)[i] %= other[i]; return *this; }

    vec<3,T>& operator +=(const T& other){ for(size_t i = 0; i < 3; i++) (*this)[i] += other;  return *this; }
    vec<3,T>& operator -=(const T& other){ for(size_t i = 0; i < 3; i++) (*this)[i] -= other;  return *this; }
    vec<3,T>& operator *=(const T& other){ for(size_t i = 0; i < 3; i++) (*this)[i] *= other;  return *this; }
    vec<3,T>& operator %=(const T& other){ for(size_t i = 0; i < 3; i++) (*this)[i] %= other;  return *this; }

	vec<3, T> operator-() const { return vec<3, T>(-x, -y, -z); }

    template <class ot> operator vec<3, ot>() const { return vec<3,ot>(x, y, z); }
    template <size_t odim, class ot> operator vec<odim, ot>() const{ vec<odim, ot> ret; if(odim > 0) ret[0] = x; if(odim > 1) ret[1] = x; if(odim > 2) ret[2] = z; return ret; }

    void allTo(const T& value){ x = value; y = value; z = value; }

    ////as point
    vec2<T> xy() const { return vec2<T>(x,y); }
    vec2<T> yz() const { return vec2<T>(y,z); }
    vec2<T> xz() const { return vec2<T>(x,z); }

    constexpr size_t size() const { return 3; }
};



//////////////////////////////
//vec4

template<class T> class vec<4, T>
{
public:
    T x;
    T y;
    T z;
    T w;

	vec(T a = T(), T b = T(), T c = T(), T d = T()) : x(a), y(b), z(c), w(d) {}
	~vec() = default;

	vec(const vec<4,T>& other) = default;
    vec<4, T>& operator=(const vec<4, T>& other) = default;

    vec(vec<4,T>&& other) = default;
    vec<4, T>& operator=(vec<4, T>&& other) = default;

    T& operator[](size_t i){ if(i == 0) return x; if(i == 1) return y; if(i == 2)return z; return w; }
	const T& operator[](size_t i) const { if(i == 0) return x; if(i == 1) return y; if(i == 2)return z; return w; }

    template <size_t odim, class ot> vec<4,T>& operator +=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 4); i++) (*this)[i] += other[i];  return *this; }
    template <size_t odim, class ot> vec<4,T>& operator -=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 4); i++) (*this)[i] -= other[i];  return *this; }
    template <size_t odim, class ot> vec<4,T>& operator *=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 4); i++) (*this)[i] *= other[i];  return *this; }
    template <size_t odim, class ot> vec<4,T>& operator /=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 4); i++) (*this)[i] /= other[i];  return *this; }
    template <size_t odim, class ot> vec<4,T>& operator %=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, (size_t) 4); i++) (*this)[i] %= other[i];  return *this; }

    vec<4,T>& operator +=(const T& other){ for(size_t i = 0; i < 4; i++) (*this)[i] += other;  return *this; }
    vec<4,T>& operator -=(const T& other){ for(size_t i = 0; i < 4; i++) (*this)[i] -= other;  return *this; }
    vec<4,T>& operator *=(const T& other){ for(size_t i = 0; i < 4; i++) (*this)[i] *= other;  return *this; }
    vec<4,T>& operator /=(const T& other){ for(size_t i = 0; i < 4; i++) (*this)[i] /= other;  return *this; }
    vec<4,T>& operator %=(const T& other){ for(size_t i = 0; i < 4; i++) (*this)[i] %= other;  return *this; }

	vec<4, T> operator-() const { return vec<4, T>(-x, -y, -z, -w); }

    void allTo(const T& value){ x = value; y = value; z = value; w = value; }

    template <class ot> operator vec<4, ot>() const { return vec<4,ot>(x, y, z, w); }
    template <size_t odim, class ot> operator vec<odim, ot>() const{ vec<odim, ot> ret; if(odim > 0) ret[0] = x; if(odim > 1) ret[1] = x; if(odim > 2) ret[2] = z; if(odim > 3) ret[3] = w; return ret; }

    ////
    vec2<T> xy() const { return vec2<T>(x,y); }
    vec2<T> xz() const { return vec2<T>(x,z); }
    vec2<T> xw() const { return vec2<T>(x,w); }
    vec2<T> yz() const { return vec2<T>(y,z); }
    vec2<T> yw() const { return vec2<T>(y,w); }
    vec2<T> zw() const { return vec2<T>(z,w); }

    vec3<T> xyz() const { return vec3<T>(x,y,z); }
    vec3<T> xyw() const { return vec3<T>(x,y,w); }
    vec3<T> xzw() const { return vec3<T>(x,z,w); }
    vec3<T> yzw() const { return vec3<T>(y,z,w); }

    constexpr size_t size() const { return 4; }
};

////functions
////one
template<size_t dim, class prec> bool oneValueLess(const vec<dim, prec>& vec1, const vec<dim, prec>& vec2)
{
	for(size_t i(0); i < dim; i++)
	{
		if(vec1[i] < vec2[i])
			return 1;
	}
	return 0;
}

template<size_t dim, class prec> bool oneValueLessOrEqual(const vec<dim, prec>& vec1, const vec<dim, prec>& vec2)
{
	for(size_t i(0); i < dim; i++)
	{
		if(vec1[i] <= vec2[i])
			return 1;
	}
	return 0;
}

template<size_t dim, class prec> bool oneValueGreater(const vec<dim, prec>& vec1, const vec<dim, prec>& vec2)
{
	for(size_t i(0); i < dim; i++)
	{
		if(vec1[i] < vec2[i])
			return 1;
	}
	return 0;
}

template<size_t dim, class prec> bool oneValueGreaterOrEqual(const vec<dim, prec>& vec1, const vec<dim, prec>& vec2)
{
	for(size_t i(0); i < dim; i++)
	{
		if(vec1[i] >= vec2[i])
			return 1;
	}
	return 0;
}


////all
template<size_t dim, class prec> bool allValuesLess(const vec<dim, prec>& vec1, const vec<dim, prec>& vec2)
{
	for(size_t i(0); i < dim; i++)
	{
		if(vec1[i] >= vec2[i])
			return 0;
	}
	return 1;
}

template<size_t dim, class prec> bool allValuesLessOrEqual(const vec<dim, prec>& vec1, const vec<dim, prec>& vec2)
{
	for(size_t i(0); i < dim; i++)
	{
		if(vec1[i] > vec2[i])
			return 0;
	}
	return 1;
}


template<size_t dim, class prec> bool allValuesGreater(const vec<dim, prec>& vec1, const vec<dim, prec>& vec2)
{
	for(size_t i(0); i < dim; i++)
	{
		if(vec1[i] <= vec2[i])
			return 0;
	}
	return 1;
}


template<size_t dim, class prec> bool allValuesGreaterOrEqual(const vec<dim, prec>& vec1, const vec<dim, prec>& vec2)
{
	for(size_t i(0); i < dim; i++)


{
		if(vec1[i] < vec2[i])
			return 0;
	}
	return 1;
}

//weight
template<size_t dim, typename prec> prec weight(const vec<dim, prec>& v)
{
    prec ret;
    for(size_t i(0); i < dim; i++)
        ret += v[i];

    //std::cout << "weight: " << v << " --> " << ret << std::endl;
    return ret;
}

}
