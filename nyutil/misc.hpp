#pragma once

#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <sstream>
#include <cmath>

namespace nyutil
{

template<class... T> void unused(T&&...)
{ }

template<class U, class V, class ...t> auto memberCallback(U (V::*func)(t ...), V* obj)
{
    return ([=](t ... params)
    {
        return (obj->*func)(params ...);
    });
}

template<class U, class V, class ...t> auto memberCallback(U (V::*func)(t ...), V& obj)
{
    return ([=](t ... params)
    {
        return (obj.*func)(params ...);
    });
}

template<class ... Args> void printVars(std::ostream& out, Args ... args)
{
    int a[sizeof...(Args)] = { (out << args, 0)... };
    unused(a);
}


//credit thttp:/stackoverflow.com/users/13430/evan-teran
inline std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


inline std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

//dumpVector
template<class T> std::string dumpVector(const std::vector<T>& obj)
{
    const char* c = "";
    std::stringstream ss;

    ss << "(";

    for(unsigned int i(0); i < obj.size(); i++)
    {
        ss << c << obj[i];
        c = ",";
    }

    ss << ")";

    return ss.str();
}

//copyVector
template<class A, class B> void copyVector(const std::vector<A>& a, std::vector<B>& b)
{
    for(unsigned int i(0); i < a.size(); i++)
    {
        const B tmp = (B) a[i];
        b.push_back(tmp);
    }
}

template<class A, class B> std::vector<B> copyVector(const std::vector<A>& a)
{
    std::vector<B> ret;
    copyVector(a, ret);
    return ret;
}



template<class A, class B> std::vector<B> copyVectorLike(const A& a)
{
    std::vector<B> ret(a.size());
    for(auto& val : a)
    {
        ret.push_back(val);
    }
    return ret;
}


//functions
inline double absDistance(double x1, double y1, double x2, double y2){ return sqrt(pow(x2 - x1,2) + pow(y2 - y1,2)); }

template <class T> bool contains(std::vector<T> vec, T val)
{
    return(std::find(vec.begin(), vec.end(), val) != vec.end());
};

inline float randomFloat(float low, float high)
{
    return low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
}

inline int randomInt(int low, int high)
{
    return (int) low + rand() % (high - low);
}

}

