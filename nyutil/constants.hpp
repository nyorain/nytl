#pragma once

namespace nyutil
{

constexpr const double cPi = 3.14159265359;
constexpr const double cDeg = cPi / 180.0;

template<typename prec>
prec degrees(const prec& rad)
{
    return rad / cDeg;
}

template<typename prec>
prec radians(const prec& deg)
{
    return deg * cDeg;
}

#if __cplusplus >= 201402L
    #define NYUTIL_CPP14_CONSTEXPR constexpr
#else
    #define NYUTIL_CPP14_CONSTEXPR
#endif



}
