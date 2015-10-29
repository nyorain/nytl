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

#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <sstream>
#include <cmath>

namespace nytl
{

template<class... T> void unused(T&&...)
{ }

template<class U, class V, class ...t> std::function<U(t...)> memberCallback(U (V::*func)(t ...), V* obj) //auto vs function?
{
    return ([func, obj](t ... params)
    {
        return (obj->*func)(params ...);
    });
}

template<class U, class V, class ...t> std::function<U(t...)> memberCallback(U (V::*func)(t ...), V& obj)
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


inline std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}


inline std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

//dumpVector
template<class T> std::string dumpContainer(const T& obj)
{
    const char* c = "";
    std::stringstream ss;

    //ss << "(";

    bool first = 1;
    for(auto& val : obj)
    {
        if(!first) ss << ", ";
        else first = 0;

        ss << val;
    }

    //ss << ")";

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



template<class B, class A> std::vector<B> copyVectorLike(const A& a)
{
    std::vector<B> ret(a.size());

    size_t i = 0;
    for(auto& val : a)
    {
        ret[i] = val;
        i++;
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

