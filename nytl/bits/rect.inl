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


///\relates nytl::rect
template<std::size_t D, typename P> bool 
intersects(const rect<D, P>& r1, const rect<D, P>& r2)
{
    return all(r2.position < r1.position + r1.size) && all(r1.position < r2.position + r2.size);
}

///\relates nytl::rect
template<std::size_t D, typename P> bool 
intersects(const rect<D, P>& r1, const line<D, P>& l2)
{
    //TODO: algorithm might be wrong, might have bugs, !important
    if(contains(r1, l2.a) || contains(r1, l2.b)) return 1;
    if(!l2.definedFor(r1.position[0], 0) || !l2.definedFor(r1.position[0] + r1.size[0], 0)) 
		return 0;

    auto st = l2.valueAt(r1.position[0]);
    auto en = l2.valueAt(r1.position[0] + r1.size[0]);

    for(std::size_t i(1); i < D; ++i)
    {
        int stV = 1;
        int enV = 1;

        if(st[i] < r1.position[i]) stV = 0;
        else if(st[i] > r1.position[i] + r1.size[i]) stV = 2;

        if(en[i] < r1.position[i]) enV = 0;
        else if(en[i] > r1.position[i] + r1.size[i]) enV = 2;

        if(stV == enV && stV != 1 && enV != 1) return false;
    }

    return true;
}
///\relates nytl::rect
template<std::size_t D, typename P> bool 
intersects(const rect<D, P>& a, const triangle<D, P>& b)
{
	return (contains(a, b.a) || contains(a, b.b) || contains(a, b.c));
}
///\relates nytl::rect
template<std::size_t D, typename P> bool 
intersects(const line<D, P>& a, const rect<D, P>& b){ return intersects(b, a); }

///\relates nytl::rect
template<std::size_t D, typename P> bool 
intersects(const triangle<D, P>& a, const rect<D, P>& b){ return interects(b, a); }


//contains
///\relates nytl::rect
template<std::size_t D, typename P> bool 
contains(const rect<D, P>& r1, const rect<D, P>& r2)
{
    return contains(r1, r2.position) && contains(r1, r2.position + r2.size);
}
///\relates nytl::rect
template<std::size_t D, typename P> bool 
contains(const rect<D, P>& r1, const line<D, P>& l2)
{
    return contains(r1, l2.a) && contains(r1, l2.b);
}
///\relates nytl::rect
template<std::size_t D, typename P> bool 
contains(const rect<D, P>& r1, const vec<D, P>& v2)
{
    return all(r1.position <= v2) && all(v2 <= r1.position + r1.size);
}


///\relates nytl::rect
///\brief Returns the difference between two rectangles [AND ~].
///\details Tt subtracts the second rect from the first one and returns the rest of the first one. 
///Since the result cant be expressed as a single rect, it is a vector of those. 
///Asymmetrical operator.
template<std::size_t D, class P>
std::vector<rect<D, P>> difference(const rect<D, P>& recta, const rect<D, P>& rectb)
{
    std::vector<rect<D, P>> ret;
    if(!intersects(recta, rectb)) return std::vector<rect<D, P>>{recta};

    for(std::size_t i(0); i < D; ++i)
    {
        if(recta.position[i] < rectb.position[i]) //begin rect
        {
            auto pos = recta.position;
            for(std::size_t o(0); o < i; ++o)
                pos[o] = rectb.position[o];

            auto size = (recta.position + recta.size) - pos;
            size[i] = rectb.position[i] - pos[i];

            ret.push_back(rect<D,P>(pos, size));
        }

        if(recta.position[i] + recta.size[i] > rectb.position[i] + rectb.size[i]) //end rect
        {
            auto pos = recta.position;
            pos[i] = rectb.position[i] + rectb.size[i];
            for(std::size_t o(0); o < i; ++o)
                pos[o] = std::max(rectb.position[o], recta.position[o]);

            auto size = (recta.position + recta.size) - pos;
            for(std::size_t o(0); o < i; ++o)
                size[o] = (rectb.position[o] + rectb.size[o]) - pos[o];

            ret.push_back(rect<D, P>(pos, size));
        }
    }

    return ret;
}

///\relates nytl::rect
///\brief Returns the rectangle in which area the two paramater rectangles intersect [AND]. 
///\details Symmetrical operator.
template<std::size_t D, typename P>
rect<D, P> intersection(const rect<D, P>& recta, const rect<D, P>& rectb)
{
    auto pos = max(recta.topLeft(), rectb.topLeft());
    auto size = min(recta.bottomRight(), rectb.bottomRight()) - pos;
    return rect<D, P>(pos, size); //max and min component-wise
}

///\relates nytl::rect
///\brief Returns the union of two rects [OR]. 
//\details Since it combines them the result cannot be expressed in a single rectangle and is 
///therefore a vector. Symmetrical operator.
template<std::size_t D, typename P>
std::vector<rect<D, P>> combination(const rect<D, P>& recta, const rect<D, P>& rectb)
{
    auto ret = difference(recta, rectb);
    ret.push_back(rectb);
    return ret;
}

///\relates nytl::rect
///\brief Returns the symmetric difference between two rectangles [XOR].
///\details Basically just the area where exactly one of them is placed. Symmetrical operator.
template<std::size_t D, typename P>
std::vector<rect<D, P>> symmetricDifference(const rect<D, P>& recta, const rect<D, P>& rectb)
{
    //return combination(difference(recta, rectb), difference(rectb, recta));
    auto result = combination(recta, rectb);
    auto ret = result;

    for(std::size_t i(0); i < result.size(); i++)
    {
        auto vvec = difference(result[i], intersection(recta, rectb));
        if(!vvec.empty())
        {
            ret[i] = vvec[0];
            ret.insert(ret.cend(), vvec.cbegin() + 1, vvec.cend());
        }
    }

    return ret;
}


///\relates nytl::rect
template<std::size_t D, typename P>
rect<D, P> operator&(const rect<D, P>& recta, const rect<D, P>& rectb)
{
    return intersection(recta, rectb);
}

///\relates nytl::rect
template<std::size_t D, typename P>
std::vector<rect<D, P>> operator^(const rect<D, P>& recta, const rect<D, P>& rectb)
{
    return symmeticDifference(recta, rectb);
}

///\relates nytl::rect
template<std::size_t D, typename P>
std::vector<rect<D, P>> operator|(const rect<D, P>& recta, const rect<D, P>& rectb)
{
    return combination(recta, rectb);
}

///\relates nytl::rect simplex
///\brief Returns a simplex representation of the rects area.
///\details There are many ways to represent a rect area by multiple simplexes, this functions 
///returns one of them.
///\return A vec with D! simplexes of the same dimension and precision as the given rect.
template<std::size_t D, typename P>
vec<fac(D), simplex<D, P>> split(const rect<D, P>& r)
{
	//TODO
}

///\relates nytl::rect
template<size_t D, typename T> 
std::ostream& operator<<(std::ostream& os, const rect<D, T>& obj)
{
    os << "position: " << obj.position << " size: " << obj.size;
    return os;
}

