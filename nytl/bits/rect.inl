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

//tests/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//intersects
template<std::size_t dim, typename prec> bool 
intersects(const rect<dim, prec>& r1, const rect<dim, prec>& r2)
{
    return all(r2.position < r1.position + r1.size) && all(r1.position < r2.position + r2.size);
}
template<std::size_t dim, typename prec> bool 
intersects(const rect<dim, prec>& r1, const line<dim, prec>& l2)
{
    //TODO: algorithm might be wrong, might have bugs, !important
    if(contains(r1, l2.a) || contains(r1, l2.b)) return 1;
    if(!l2.definedFor(r1.position[0], 0) || !l2.definedFor(r1.position[0] + r1.size[0], 0)) return 0;

    auto st = l2.valueAt(r1.position[0]);
    auto en = l2.valueAt(r1.position[0] + r1.size[0]);

    for(std::size_t i(1); i < dim; ++i)
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
template<std::size_t dim, typename prec> bool 
intersects(const rect<dim, prec>& a, const triangle<dim, prec>& b)
{
	return (contains(a, b.a) || contains(a, b.b) || contains(a, b.c));
}
template<std::size_t dim, typename prec> bool 
intersects(const line<dim, prec>& a, const rect<dim, prec>& b){ return intersects(b, a); }

template<std::size_t dim, typename prec> bool 
intersects(const triangle<dim, prec>& a, const rect<dim, prec>& b){ return interects(b, a); };


//contains
template<std::size_t dim, typename prec> bool 
contains(const rect<dim, prec>& r1, const rect<dim, prec>& r2)
{
    return contains(r1, r2.position) && contains(r1, r2.position + r2.size);
}
template<std::size_t dim, typename prec> bool 
contains(const rect<dim, prec>& r1, const line<dim, prec>& l2)
{
    return contains(r1, l2.a) && contains(r1, l2.b);
}
template<std::size_t dim, typename prec> bool 
contains(const rect<dim, prec>& r1, const triangle<dim, prec>& t2)
{
    return contains(r1, t2.a) && contains(r1, t2.b) && contains(r1, t2.c);
}
template<std::size_t dim, typename prec> bool 
contains(const rect<dim, prec>& r1, const vec<dim, prec>& v2)
{
    return all(r1.position <= v2) && all(v2 <= r1.position + r1.size);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//operators
//means: subtract rectb from recta and then return everything that is left from recta.
//so recta will be splitted into the rectb-part and the not-rectb-part. the not-rectb-part will be returned.
template<std::size_t dim, class prec>
std::vector<rect<dim, prec>> difference(const rect<dim, prec>& recta, const rect<dim, prec>& rectb)
{
    std::vector<rect<dim, prec>> ret;
    if(!intersects(recta, rectb)) return std::vector<rect<dim, prec>>{recta};

    for(std::size_t i(0); i < dim; ++i)
    {
        if(recta.position[i] < rectb.position[i]) //begin rect
        {
            auto pos = recta.position;
            for(std::size_t o(0); o < i; ++o)
                pos[o] = rectb.position[o];

            auto size = (recta.position + recta.size) - pos;
            size[i] = rectb.position[i] - pos[i];

            ret.push_back(rect<dim,prec>(pos, size));
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

            ret.push_back(rect<dim, prec>(pos, size));
        }
    }

    return ret;
}

template<std::size_t dim, typename prec>
rect<dim, prec> intersection(const rect<dim, prec>& recta, const rect<dim, prec>& rectb)
{
    auto pos = max(recta.topLeft(), rectb.topLeft());
    auto size = min(recta.bottomRight(), rectb.bottomRight()) - pos;
    return rect<dim, prec>(pos, size); //max and min component-wise
}

template<std::size_t dim, typename prec>
std::vector<rect<dim, prec>> combination(const rect<dim, prec>& recta, const rect<dim, prec>& rectb)
{
    auto ret = difference(recta, rectb);
    ret.push_back(rectb);
    return ret;
}

template<std::size_t dim, typename prec>
std::vector<rect<dim, prec>> symmetricDifference(const rect<dim, prec>& recta, const rect<dim, prec>& rectb)
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//as operators
template<std::size_t dim, typename prec>
rect<dim, prec> operator&(const rect<dim, prec>& recta, const rect<dim, prec>& rectb)
{
    return intersection(recta, rectb);
}

template<std::size_t dim, typename prec>
std::vector<rect<dim, prec>> operator^(const rect<dim, prec>& recta, const rect<dim, prec>& rectb)
{
    return symmeticDifference(recta, rectb);
}

template<std::size_t dim, typename prec>
std::vector<rect<dim, prec>> operator|(const rect<dim, prec>& recta, const rect<dim, prec>& rectb)
{
    return combination(recta, rectb);
}

//those 2 rlly needed?
/*
template<std::size_t dim, typename prec>
std::vector<rect<dim, prec>> operator+(const rect<dim, prec>& recta, const rect<dim, prec>& rectb)
{
    return combination(recta, rectb);
}

template<std::size_t dim, typename prec>
std::vector<rect<dim, prec>> operator-(const rect<dim, prec>& recta, const rect<dim, prec>& rectb)
{
    return combination(recta, rectb);
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//split
template<std::size_t dim, typename prec>
vec<2, triangle<dim, prec>> split(const rect<dim, prec>& r)
{
    return vec<2, triangle<dim, prec>>({r.topLeft(), r.topRight(), r.bottomRight()},
                                       {r.topLeft(), r.bottomLeft(), r.bottomRight()});
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ostream
template<size_t dim, typename T> std::ostream& operator<<(std::ostream& os, const rect<dim, T>& obj)
{
    os << "position: " << obj.position << " size: " << obj.size;
    return os;
}


