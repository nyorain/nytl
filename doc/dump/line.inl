// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

//implementation specialization
namespace detail
{

template<std::size_t D, typename P>
struct SimplexContainsPoint<D, P, 1>
{
	static bool call(const Simplex<D, P, 1>& a, const Vec<D, P>& v)
	{
		return (a.definedAt(v[0]) && all(a.valueAt(v[0]) == v));
	}
};

template<std::size_t D, typename P>
struct SimplexIntersects<D, P, 1>
{
	static bool call(const Simplex<D, P, 1>& la, const Simplex<D, P, 1>& lb)
	{
		Mat<D, 3, double> eqs;
		eqs.col(0) = la.b - la.a;
		eqs.col(1) = -lb.b + lb.a;
		eqs.col(2) = lb.a - la.a;

		rrefMat(eqs);

		//unsolveable
		if((all(Vec2d(eqs.row(D - 1)) == 0) && eqs.row(D - 1)[2] != 0)) return false;

		//solveable, but not in Line segment
		if(any(eqs.col(2) >= 1) || any(eqs.col(2) <= 0)) return false;

		return true;
	};
};

} //detail

template<size_t D, typename P> bool
Line<D, P>::definedAt(const P& value, std::size_t dim) const
{
	return ((smallestValue(dim) <= value) &&
			(greatestValue(dim) >= value));
}

template<size_t D, typename P> Vec<D, P>
Line<D, P>::valueAt(const P& value, std::size_t dim) const
{
	if(!definedAt(value, dim))
	{
		//throw?
		return {};
	}
	else if(gradient()[dim] == 0)
	{
		auto ret = a;
		ret[dim] = value;
		return ret;
	}
	else
	{
		auto ret = a + ((value - a[dim]) * gradient(dim));
		return ret;
	}
}

template<std::size_t D, typename P>
bool Line<D, P>::valid() const
{
	return detail::SimplexValid(*this);
}
