#pragma once

#include <nyutil/vec.hpp>

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

	rect(vec<dim, prec> pposition = vec<dim, prec>(), vec<dim, prec> psize = vec<dim, prec>()) : position(pposition), size(psize) {}
	rect(const rect<dim, prec>& other): position(other.position), size(other.size) {}
	template<size_t odim, class oprec> rect(const vec<odim, oprec>& other) : position(other.position), size(other.size) {}

	bool intersects(const rect<dim, prec>& other) const { return(allValuesLess(other.position, position + size) && allValuesLess(position, other.position + other.size));  }
	bool contains(const vec<dim, prec>& other) const { return(allValuesLess(position, other) && allValuesLess(other, position + size)); }
	bool contains(const rect<dim, prec>& other) const { return(allValuesLess(position, other.position) && allValuesLess(other.position + other.size, position + size)); }

	vec<dim, double> center() const { return (double)(position + size) / 2.d; };

	template<size_t odim, class oprec> operator rect<odim, oprec>() const { return rect<odim, oprec>(position, size); }
};

//rect2
template<class prec> class rect<2, prec>
{
public:
	vec<2, prec> position;
	vec<2, prec> size;

	rect(vec2<prec> pposition, vec2<prec> psize = vec2<prec>()) : position(pposition), size(psize) {}
	rect(prec x = prec(), prec y = prec(), prec width = prec(), prec height = prec()) : position(x,y), size(width,height) {}

	rect(const rect2<prec>& other): position(other.position), size(other.size) {}
	template<size_t odim, class oprec> rect(const vec<odim, oprec>& other) : position(other.position), size(other.size) {}

	bool intersects(const rect2<prec>& other) const { return(allValuesLess(other.position, position + size) && allValuesLess(position, other.position + other.size));  }
	bool contains(const vec2<prec>& other) const { return(allValuesLess(position, other) && allValuesLess(other, position + size)); }
	bool contains(const rect<2, prec>& other) const { return(allValuesLess(position, other.position) && allValuesLess(other.position + other.size, position + size)); }

	vec<2, double> center() const { return (double)(position + size) / 2.d; };

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
	const prec& height() const { return size.x; }

    prec& width() { return size.x; }
	prec& height() { return size.x; }

	std::vector<rect<2, prec>> subtract(const rect<2, prec>& other){ return rectSubtract(*this, other); };

	template<size_t odim, class oprec> operator rect<odim, oprec>() const { return rect<odim, oprec>(position, size); }
};

template<class prec> std::vector<rect<2, prec>> rectSubtract(rect<2, prec>& sub, const rect<2, prec>& other)
{
    //todo: all

    std::vector<rect<2, prec>> ret;

/*
    if(sub.left() < other.left() < sub.right())
    {
        ret.left() = sub.left();
        ret.top() = sub.top();
        ret.width() = other.left() - sub.left();
        ret.height() = sub.bottom() - other.bottom();

        sub.left() = other.left();
        sub.width() = other.top() - sub.top();
    }
*/

    return ret;
}

}
