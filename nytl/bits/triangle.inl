// Copyright (c) 2016 nyorain 
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_TRIANGLE_INL
#define NYTL_INCLUDE_TRIANGLE_INL

//member
template<std::size_t D, typename P>
double Triangle<D, P>::size() const
{
	return 0.5 * length(ab()) * length(bc()) * std::sin(angleB());
}

template<std::size_t D, typename P>
Vec<D, P> Triangle<D, P>::center() const
{
	return (a + b + c) / 3;

}

///Efficient barycentric coordinates implementation for triangles.
template<typename P>
Vec3f barycentric(const Triangle<2, P>& tri, const Vec<2, P>& cart)
{
	if(tri.size() == 0.f) throw std::logic_error("nytl::barycentric: invalid triangle");

	auto v0 = tri.b - tri.a;
	auto v1 = tri.c - tri.a;
	auto v2 = cart - tri.a;
	auto d = 1.f / (v0.x * v1.y - v1.x * v0.y);

	Vec3f ret;
	ret.x = (v2.x * v1.y - v1.x * v2.y) * d;
	ret.y = (v0.x * v2.y - v2.x * v0.y) * d;
	ret.z = 1.f - (ret.x + ret.y);
	return ret;
}

///Returns whether the given triangle contains the given point
template<typename P>
bool contains(const Triangle<2, P>& tri, const Vec<2, P>& p)
{
	if(tri.size() == 0.f) return false;
	auto bar = barycentric(tri, p);
	return (all(bar <= 1.f) && all(bar >= 0.f) && sum(bar) == 1.f);
}

#endif //header guard
