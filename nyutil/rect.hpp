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

#include <nyutil/vec.hpp>
#include <nyutil/triangle.hpp>

#include <vector>

namespace nyutil
{
//rect
template<size_t dim, class prec> class rect;

template<class prec> using rect2 = rect<2, prec>;
template<class prec> using rect3 = rect<3, prec>;
template<class prec> using rect4 = rect<4, prec>;

typedef rect<2, int> rect2i;
typedef rect<2, unsigned int> rect2ui;
typedef rect<2, double> rect2d;
typedef rect<2, float> rect2f;
typedef rect<2, char> rect2c;
typedef rect<2, unsigned char> rect2uc;
typedef rect<2, long> rect2l;
typedef rect<2, unsigned long> rect2ul;

typedef rect<3, int> rect3i;
typedef rect<3, unsigned int> rect3ui;
typedef rect<3, double> rect3d;
typedef rect<3, float> rect3f;
typedef rect<3, char> rect3c;
typedef rect<3, unsigned char> rect3uc;
typedef rect<3, long> rect3l;
typedef rect<3, unsigned long> rect3ul;

typedef rect<4, int> rect4i;
typedef rect<4, unsigned int> rect4ui;
typedef rect<4, double> rect4d;
typedef rect<4, float> rect4f;
typedef rect<4, char> rect4c;
typedef rect<4, unsigned char> rect4uc;
typedef rect<4, long> rect4l;
typedef rect<4, unsigned long> rect4ul;


template<class prec> std::vector<rect<2, prec>> rectSubtract(rect<2, prec>& sub, const rect<2, prec>& other);

//rect
template<size_t dim, class prec> class rect
{
public:
	vec<dim, prec> position;
	vec<dim, prec> size;

public:
	rect(vec<dim, prec> pposition = vec<dim, prec>(), vec<dim, prec> psize = vec<dim, prec>()) noexcept : position(pposition), size(psize) {}
    ~rect() noexcept = default;

    rect(const rect<dim, prec>& other) noexcept = default;
	rect& operator=(const rect<dim, prec>& other) noexcept = default;

    rect(rect<dim, prec>&& other) noexcept = default;
	rect& operator=(rect<dim, prec>&& other) noexcept = default;

    //
	bool intersects(const rect<dim, prec>& other) const { return(allValuesLess(other.position, position + size) && allValuesLess(position, other.position + other.size));  }
	bool contains(const vec<dim, prec>& other) const { return(allValuesLess(position, other) && allValuesLess(other, position + size)); }
	bool contains(const rect<dim, prec>& other) const { return(allValuesLess(position, other.position) && allValuesLess(other.position + other.size, position + size)); }

	vec<dim, double> center() const { return (double)(position + (size / 2.d)); };

	template<size_t odim, class oprec> operator rect<odim, oprec>() const { return rect<odim, oprec>(position, size); }
};

//rect2
template<class prec> class rect<2, prec>
{
public:
	vec<2, prec> position;
	vec<2, prec> size;

public:
	rect(vec2<prec> pposition, vec2<prec> psize = vec2<prec>()) noexcept : position(pposition), size(psize) {}
	rect(prec x = prec(), prec y = prec(), prec width = prec(), prec height = prec()) noexcept : position(x,y), size(width,height) {}

	~rect() noexcept = default;

	rect(const rect2<prec>& other) noexcept = default;
	rect& operator=(const rect2<prec>& other) noexcept = default;

    rect(rect2<prec>&& other) noexcept = default;
	rect& operator=(rect2<prec>&& other) noexcept = default;

    //
	bool intersects(const rect2<prec>& other) const { return(allValuesLess(other.position, position + size) && allValuesLess(position, other.position + other.size));  }
	bool contains(const vec2<prec>& other) const { return(allValuesLess(position, other) && allValuesLess(other, position + size)); }
	bool contains(const rect<2, prec>& other) const { return(allValuesLess(position, other.position) && allValuesLess(other.position + other.size, position + size)); }

	vec<2, double> center() const { return (double)(position + (size / 2.d)); };

    vec<2, prec> topLeft() const { return position; }
	vec<2, prec> topRight() const { return position + vec<2, prec>(size.x, 0); }
	vec<2, prec> bottomLeft() const { return position + vec<2, prec>(0, size.y);}
	vec<2, prec> bottomRight() const { return position + size; }

	const prec& left() const { return position.x; }
	const prec right() const { return position.x + size.x; }
	const prec& top() const { return position.y; }
	const prec bottom() const { return position.y + size.y; }

    prec& left() { return position.x; }
	prec& top() { return position.y; }

	const prec& width() const { return size.x; }
	const prec& height() const { return size.y; }

    prec& width() { return size.x; }
	prec& height() { return size.x; }

    //conversion
	template<size_t odim, class oprec>
	operator rect<odim, oprec>() const { return rect<odim, oprec>(position, size); }
};

//util
//todo: subtract in any dimension
template<class prec> std::vector<rect<2, prec>> subtract(const rect<2, prec>& sub, const rect<2, prec>& other)
{
    //todo
    std::vector<rect<2, prec>> ret;
    if(!sub.contains(other)) return ret;


    if(sub.left() < other.left() && other.left() < sub.right()) //rect on the left
    {
        rect<2, prec> r;
        r.position = sub.position;
        r.size.x = other.position.x - sub.position.x;
        r.size.y = sub.size.y;

        ret.push_back(r);
    }

    if(sub.top() < other.top() && other.top() < sub.bottom()) //rect on top
    {
        rect<2, prec> r;
        r.position.x = std::max(sub.position.x, other.position.x);
        r.position.y = sub.position.y;
        r.size.x = sub.right() - r.position.x;
        r.size.y = other.top() - sub.top();

        ret.push_back(r);
    }

    if(other.right() < sub.right()) //rect on the right
    {
        rect<2, prec> r;
        r.position = other.topRight();
        r.size.x = sub.right() - other.right();
        r.size.y = sub.bottom() - r.top();

        ret.push_back(r);
    }

    if(other.bottom() < sub.bottom()) //rect on bottom
    {
        rect<2, prec> r;
        r.position = other.bottomLeft();
        r.size.x = other.size.x;
        r.size.y = sub.bottom() - other.bottom();

        ret.push_back(r);
    }

    return ret;
}

template<size_t dim, typename prec>
vec<2, triangle<dim, prec>> split(const rect<dim, prec>& r)
{
    return vec<2, triangle<dim, prec>>({r.topLeft(), r.topRight(), r.bottomRight()},
                                       {r.topLeft(), r.bottomLeft(), r.bottomRight()});
}

//operator
//stream: todo, istream check for correct type (?)
template<size_t dim, typename T> std::ostream& operator<<(std::ostream& os, const rect<dim, T>& obj)
{
    os << "rect{\n\t" << obj.position << ";\n\t" << obj.size << "\n}" << std::endl;
    return os;
}

}
