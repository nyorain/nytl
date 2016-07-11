#include <nytl/vec.hpp>
#include <nytl/refVec.hpp>
#include <nytl/dynVec.hpp>
#include <iostream>

int main()
{
	//initialization
	nytl::Vec2f a(2, 3);
	nytl::Vec3d b{2., 3., 4.};
	nytl::Vec2ui c = {5, 8};
	auto d = nytl::Vec3c{5, 7, 2};

	//some operations
	nytl::dot(a, b);
	nytl::cross(b, nytl::Vec3d{0., 2., 0.});

	nytl::any(a > b);
	nytl::all(c == d);
	nytl::none(d <= a);

	//subvecs
	//dynVec
	auto e = b.subVec(1); //position = 1 -> {3., 4.}
	auto f = d.subVec(0, 2); //position = 0, size = 2 -> {5, 7}

	//static vec
	auto g = nytl::subVec<2>(a); //exactly the same as a, position = 0, size = 2
	auto h = nytl::subVec<2>(b, 1); //position = 1, size = 2 -> {3., 4.}
}
