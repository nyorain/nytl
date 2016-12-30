#include "test.hpp"
#include <nytl/vecOps.hpp>
#include <nytl/vec.hpp>

#include <array>
#include <type_traits>
#include <limits>
#include <iostream>

void test1()
{
	using Vec3 = nytl::Vec<3, double>;

	Vec3 a {};
	a[0] = 1.0;
	a[1] = 2.0;
	a[2] = 3.0;

	CHECK_EXPECT(nytl::vec::dot(a, a), 14.0);

	using DynVec = nytl::Vec<nytl::dynamicSize, double>;
	auto b = DynVec::create(5);
	b[0] = 1.0;
	b[1] = 1.0;
	b[2] = 1.0;
	b[3] = 0.0;
	b[4] = 2.0;

	CHECK_EXPECT(nytl::vec::dot(b, b), 7.0);
}

void test2()
{
	using Vec3 = nytl::Vec<3, double>;

	constexpr Vec3 a (1.0, 2.0, 3.0);
	constexpr Vec3 b (1.0, 0.0, -1.0);
	constexpr Vec3 c (3.0, 4.0, 0.0);
	constexpr Vec3 d (1.0, 0.0, 0.0);
	constexpr Vec3 e (0.0, 1.0, 0.0);

	CHECK_EXPECT(nytl::vec::sum(a), 6.0);
	CHECK_EXPECT(nytl::vec::sum(b), 0.0);
	CHECK_EXPECT(nytl::vec::multiply(a), 6.0);

	CHECK_EXPECT(nytl::vec::dot(a, b), -2.0);
	CHECK_EXPECT(nytl::vec::norm(c), 5.0);
	CHECK_EXPECT(nytl::vec::angle(b, d), nytl::constants::pi / 4);
	CHECK_EXPECT(nytl::vec::angle(d, e), nytl::constants::pi / 2);
	CHECK_EXPECT(nytl::vec::distance(a, c), nytl::vec::norm(a - c));
	CHECK_EXPECT(nytl::vec::distance(a, c), nytl::vec::norm(c - a));
	CHECK_EXPECT(nytl::vec::norm(5.0 * a), 5 * nytl::vec::norm(a));
}

int main()
{
	test1();
	test2();
	
	std::cout << (failed ? std::to_string(failed) : "no") << " tests failed!\n";
}
