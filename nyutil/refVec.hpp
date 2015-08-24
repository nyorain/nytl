#pragma once

#include <nyutil/vec.hpp>

#include <functional>

namespace nyutil
{

//todo
//specializations
//operators (like vec)

template<size_t dim, class T> class refVec;

//is p/typedefs
template<class T> using refVec2 = refVec<2, T>;
template<class T> using refVec3 = refVec<3, T>;
template<class T> using refVec4 = refVec<4, T>;

typedef refVec2<float> refVec2f;
typedef refVec2<int> refVec2i;
typedef refVec2<unsigned int> refVec2ui;
typedef refVec2<double> refVec2d;
typedef refVec2<char> refVec2c;
typedef refVec2<unsigned char> refVec2uc;
typedef refVec2<long> refVec2l;
typedef refVec2<unsigned long> refVec2ul;

typedef refVec3<float> refVec3f;
typedef refVec3<int> refVec3i;
typedef refVec3<unsigned int> refVec3ui;
typedef refVec3<double> refVec3d;
typedef refVec3<char> refVec3c;
typedef refVec3<unsigned char> refVec3uc;
typedef refVec3<long> refVec3l;
typedef refVec3<unsigned long> refVec3ul;

typedef refVec4<float> refVec4f;
typedef refVec4<int> refVec4i;
typedef refVec4<unsigned int> refVec4ui;
typedef refVec4<double> refVec4d;
typedef refVec4<char> refVec4c;
typedef refVec4<unsigned char> refVec4uc;
typedef refVec4<long> refVec4l;
typedef refVec4<unsigned long> refVec4ul;

template<size_t dim, class T> class refVec
{
public:
   	T* data[dim];

	refVec() = default;
	refVec(const refVec<dim, T>& other) = default;
	refVec(vec<dim, T>& pref) { setRef(pref); }

	refVec<dim, T>& operator=(const refVec<dim, T>& other){ for(size_t i(0); i < dim; i++)data[i] = other.data[i]; return *this; } //todo: prob. this operator should not exist
	refVec<dim, T>& operator=(const vec<dim, T>& other){ for(size_t i(0); i < dim; i++)*data[i] = other[i]; return *this; }

	T& operator[](size_t i){ return *data[i]; }
	const T& operator[](size_t i) const { return *data[i]; }

	template <size_t odim, class ot> refVec<dim,T>& operator +=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) *data[i] += other[i];  return *this; }
    template <size_t odim, class ot> refVec<dim,T>& operator -=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) *data[i] -= other[i];  return *this; }
    template <size_t odim, class ot> refVec<dim,T>& operator *=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) *data[i] *= other[i];  return *this; }
    template <size_t odim, class ot> refVec<dim,T>& operator /=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) *data[i] /= other[i];  return *this; }
    template <size_t odim, class ot> refVec<dim,T>& operator %=(const vec<odim, ot>& other){ for(size_t i = 0; i < std::min(odim, dim); i++) *data[i] %= other[i];  return *this; }

    refVec<dim,T>& operator +=(const T& other){ for(size_t i = 0; i < dim; i++) *data[i] += other;  return *this; }
    refVec<dim,T>& operator -=(const T& other){ for(size_t i = 0; i < dim; i++) *data[i] -= other;  return *this; }
    refVec<dim,T>& operator *=(const T& other){ for(size_t i = 0; i < dim; i++) *data[i] *= other;  return *this; }
    refVec<dim,T>& operator /=(const T& other){ for(size_t i = 0; i < dim; i++) *data[i] /= other;  return *this; }
    refVec<dim,T>& operator %=(const T& other){ for(size_t i = 0; i < dim; i++) *data[i] %= other;  return *this; }


	template <class ot> operator vec<dim, ot>() const { vec<dim, ot> ret; for(size_t i(0); i < dim; i++)ret[i] = *data[i]; return ret; }
	template <size_t odim, class ot> operator vec<odim, ot>() const  { vec<odim, ot> ret; for(size_t i(0); i < std::min(odim, dim); i++)ret[i] = *data[i]; return ret; }

    constexpr size_t size() const { return dim; }

    void setRef(vec<dim, T>& other){ for(size_t i(0); i < dim; i++)data[i] = &other[i]; }
    void setRefAt(size_t i, T& pref){ data[i] = &pref; };
};


//operator
template<size_t dim, class T> std::ostream& operator<<(std::ostream& os, const refVec<dim, T>& obj)
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



template<size_t dim, class T> vec<dim, T> operator+(refVec<dim, T> mvec, const T& other)
{
    mvec += other;
    return mvec;
}
template<size_t dim, class T> vec<dim, T> operator+(const T& other, refVec<dim, T> mvec)
{
    mvec += other;
    return mvec;
}
template<size_t dima, class Ta, size_t dimb, class Tb> vec<dima, Ta> operator+(refVec<dima, Ta> mvec, const vec<dimb, Tb>& other)
{
    mvec += other;
    return mvec;
}
template<size_t dima, class Ta, size_t dimb, class Tb> vec<dima, Ta> operator+(vec<dima, Ta> a, const refVec<dimb,Tb>& b)
{
    a += (vec<dimb, Tb>) b;
    return a;
}


template<size_t dim, class T> vec<dim, T> operator-(refVec<dim, T> mvec, const T& other)
{
    mvec -= other;
    return mvec;
}
template<size_t dim, class T> vec<dim, T> operator-(const T& other, refVec<dim, T> mvec)
{
    for(unsigned int i(0); i < dim; i++)
    {
        mvec[i] = other - mvec[i];
    }
    return mvec;
}
template<size_t dima, class Ta, size_t dimb, class Tb> vec<dima, Ta> operator-(refVec<dima, Ta> mvec, const vec<dimb, Tb>& other)
{
    mvec -= other;
    return mvec;
}
template<size_t dima, class Ta, size_t dimb, class Tb> vec<dima, Ta> operator-(vec<dima, Ta> a, const refVec<dimb,Tb>& b)
{
    a -= (vec<dimb, Tb>) b;
    return a;
}




template<size_t dim, class T> vec<dim, T> operator*(refVec<dim, T> mvec, const T& other)
{
    mvec *= other;
    return mvec;
}
template<size_t dim, class T> vec<dim, T> operator*(const T& other, refVec<dim, T> mvec)
{
    mvec *= other;
    return mvec;
}
template<size_t dima, class Ta, size_t dimb, class Tb> vec<dima, Ta> operator*(refVec<dima, Ta> mvec, const vec<dimb, Tb>& other)
{
    mvec *= other;
    return mvec;
}
template<size_t dima, class Ta, size_t dimb, class Tb> vec<dima, Ta> operator*(vec<dima, Ta> a, const refVec<dimb,Tb>& b)
{
    a *= (vec<dimb, Tb>) b;
    return a;
}




template<size_t dim, class T> vec<dim, T> operator/(refVec<dim, T> mvec, const T& other)
{
    mvec /= other;
    return mvec;
}
template<size_t dim, class T> vec<dim, T> operator/(const T& other, refVec<dim, T> mvec)
{
    for(unsigned int i(0); i < dim; i++)
    {
        mvec[i] = other / mvec[i];
    }
    return mvec;
}
template<size_t dima, class Ta, size_t dimb, class Tb> vec<dima, Ta> operator/(refVec<dima, Ta> mvec, const vec<dimb, Tb>& other)
{
    mvec /= other;
    return mvec;
}
template<size_t dima, class Ta, size_t dimb, class Tb> vec<dima, Ta> operator/(vec<dima, Ta> a, const refVec<dimb,Tb>& b)
{
    a /= (vec<dimb, Tb>) b;
    return a;
}


//specialization

}

