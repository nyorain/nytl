#include <nytl/vecOps.hpp>
#include <nytl/vec.hpp>

#include <array>
#include <type_traits>
#include <limits>
#include <iostream>

constexpr double pi = 3.141592653589793238;

void test1()
{
	int failed = 0u;
	using Vec3 = nytl::Vec<3, double>;

	Vec3 a {};
	a[0] = 1.0;
	a[1] = 2.0;
	a[2] = 3.0;

	CHECK_EXPECT(almostEqual(nytl::dot(a, a), 14.0));

	using DynVec = nytl::Vec<nytl::dynamicSize, double>;
	auto b = DynVec::create(5);
	b[0] = 1.0;
	b[1] = 1.0;
	b[2] = 1.0;
	b[3] = 0.0;
	b[4] = 2.0;

	CHECK_EXPECT(almostEqual(nytl::dot(b, b), 7.0));

	std::cerr << "test1: " << failed << " tests failed\n";
}

void test2()
{
	int failed = 0u;
	using Vec3 = Vec<3, double>;

	constexpr Vec3 a (1.0, 2.0, 3.0);
	constexpr Vec3 b (1.0, 0.0, -1.0);
	constexpr Vec3 c (3.0, 4.0, 0.0);
	constexpr Vec3 d (1.0, 0.0, 0.0);
	constexpr Vec3 e (0.0, 1.0, 0.0);

	CHECK_EXPECT(almostEqual(nytl::sum(a), 6.0));
	CHECK_EXPECT(almostEqual(nytl::sum(b), 0.0));
	CHECK_EXPECT(almostEqual(nytl::multiply(a), 6.0));

	CHECK_EXPECT(almostEqual(nytl::dot(a, b), -2.0));
	CHECK_EXPECT(almostEqual(nytl::norm(c), 5.0));
	CHECK_EXPECT(almostEqual(nytl::angle(b, d), pi / 4));
	CHECK_EXPECT(almostEqual(nytl::angle(d, e), pi / 2));
	CHECK_EXPECT(almostEqual(nytl::distance(a, c), nytl::norm(a - c)));
	CHECK_EXPECT(almostEqual(nytl::distance(a, c), nytl::norm(c - a)));
	CHECK_EXPECT(almostEqual(nytl::norm(5.0 * a), 5 * nytl::norm(a)));

	std::cerr << "test2: " << failed << " tests failed\n";
}

constexpr int test3()
{
	int failed = 0u;
	using Vec3 = nytl::Vec<3, double>;

	Vec3 a {};
	a.x = 1.0;
	// a[1] = 2.0;
	// a[2] = 3.0;

	CHECK_EXPECT(almostEqual(nytl::dot(a, a), 14.0));
	return failed;
}

int main()
{
	test1();
	test2();
}
