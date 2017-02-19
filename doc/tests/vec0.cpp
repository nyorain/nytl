#include "old/test-old.hpp"
#include "approx.hpp"
#include <cmath>

// testing both, vec and vecOps
#include <nytl/vec.hpp>
#include <nytl/vecOps.hpp>
#include <nytl/scalar.hpp>

// TODO: test component-wise (nytl::vec::cw) operations

using Vec3 = nytl::Vec<3, double>;
using Vec2 = nytl::Vec<2, double>;

constexpr Vec3 a(1.0, 2.0, 3.0);
constexpr Vec3 b(0.0, 0.0, 0.0);
constexpr Vec3 c(1.0, 0.0, 0.0);
constexpr Vec3 d(-1.0, 1.0, 1.0);
constexpr Vec3 e(0.0001, 1.0, -1.0);
constexpr Vec3 f(100.0, 500.0, -4.0);

constexpr Vec3 x(2.0, 3.0, 5.0);
constexpr Vec3 y(4.0, -3.0, 2.0);
constexpr Vec3 z(0.0, 10.0, 1.0);

TEST_METHOD("[vec-addition]") {
	EXPECT(a + b, test::approx(a));
	EXPECT(a - b, test::approx(a));
	EXPECT(a + a, test::approx(Vec3{2.0, 4.0, 6.0}));
	EXPECT(x + z, test::approx(Vec3{2.0, 13.0, 6.0}));
	EXPECT(b - x + x - x + b, test::approx(-x));
	EXPECT(f - f, test::approx(b));
	EXPECT(b, test::approx(-b));
	EXPECT((Vec3{1.0, 1.0, 1.0} + Vec3{-1.0, 2.0, 0.0}), (test::approx(Vec3{0.0, 3.0, 1.0})));
}

TEST_METHOD("[scalar multiplication]") {
	EXPECT((2 * a), test::approx(a + a));
	EXPECT((5 * b), test::approx(b));
	EXPECT((-1 * f), test::approx(-f));
	EXPECT((0 * e), test::approx(b));
	EXPECT((0.5 * y), test::approx(Vec3{2.0, -1.5, 1.0}));
	EXPECT((0.2 * z), test::approx(z - 0.8 * z));
	EXPECT((2 * x + y), test::approx(Vec3{8.0, 3.0, 12.0}));
}

TEST_METHOD("[multiplies]") {
	EXPECT(nytl::vec::multiply(a), test::approx(6.0));
	EXPECT(nytl::vec::multiply(b), test::approx(0.0));
	EXPECT(nytl::vec::multiply(c), test::approx(0.0));
	EXPECT(nytl::vec::multiply(d), test::approx(-1.0));
	EXPECT(nytl::vec::multiply(e), test::approx(-0.0001));
	EXPECT(nytl::vec::multiply(f), test::approx(-200000.0));
}

TEST_METHOD("[sums]") {
	EXPECT(nytl::vec::sum(a), test::approx(6.0));
	EXPECT(nytl::vec::sum(b), test::approx(0.0));
	EXPECT(nytl::vec::sum(c), test::approx(1.0));
	EXPECT(nytl::vec::sum(d), test::approx(1.0));
	EXPECT(nytl::vec::sum(e), test::approx(0.0001));
	EXPECT(nytl::vec::sum(f), test::approx(596.0));
}

TEST_METHOD("[dot]") {
	EXPECT(nytl::vec::dot(a, b), test::approx(0.0));
	EXPECT(nytl::vec::dot(a, c), test::approx(1.0));
	EXPECT(nytl::vec::dot(a, d), test::approx(4.0));
	EXPECT(nytl::vec::dot(d, a), test::approx(4.0));
	EXPECT(nytl::vec::dot(x, a), test::approx(23.0));
	EXPECT(nytl::vec::dot(x, y), test::approx(9.0));
	EXPECT(nytl::vec::dot(z, x), test::approx(35.0));
	EXPECT(nytl::vec::dot(x, f), test::approx(1680.0));
	EXPECT(nytl::vec::dot(y, d), test::approx(-5.0));
}

TEST_METHOD("[length]") {
	EXPECT(nytl::vec::length(b), test::approx(0.0));
	EXPECT(nytl::vec::length(a), test::approx(std::sqrt(14.0)));
	EXPECT(nytl::vec::length(f), test::approx(nytl::vec::length(-f)));
	EXPECT(nytl::vec::length(2 * a), test::approx(2 * nytl::vec::length(a)));
	EXPECT(nytl::vec::length(1232 * a), test::approx(1232 * nytl::vec::length(a)));
	EXPECT(nytl::vec::length(-5 * a), test::approx(5.0 * nytl::vec::length(a)));
	EXPECT(nytl::vec::length(b), test::approx(0.0));
	EXPECT(nytl::vec::length(c), test::approx(1.0));
	EXPECT(nytl::vec::length(x), test::approx(std::sqrt(38.0)));
	EXPECT(nytl::vec::length(y), test::approx(std::sqrt(29.0)));
	EXPECT(nytl::vec::length(x - a), test::approx(std::sqrt(6.0)));
	EXPECT(nytl::vec::length(1.5 * (a + b + c)), test::approx(1.5 * std::sqrt(17.0)));
}

TEST_METHOD("[angles]") {
	constexpr Vec2 a2(1.0, 0.0);
	constexpr Vec2 b2(0.0, 1.0);
	constexpr Vec2 c2(1.0, 1.0);

	constexpr Vec3 a3(1.0, 0.0, -1.0);
	constexpr Vec3 b3(1.0, 0.0, 0.0);
	constexpr Vec3 c3(0.0, 1.0, 0.0);

	EXPECT(nytl::vec::angle(a2, b2), test::approx(nytl::radians(90.0)));
	EXPECT(nytl::vec::angle(b2, a2), test::approx(nytl::constants::pi / 2));
	EXPECT(nytl::vec::angle(a2, c2), test::approx(nytl::radians(45.0)));
	EXPECT(nytl::vec::angle(c2, b2), test::approx(nytl::constants::pi / 4));

	EXPECT(nytl::vec::angle(a3, b3), test::approx(nytl::constants::pi / 4));
	EXPECT(nytl::vec::angle(c3, b3), test::approx(nytl::radians(90.0)));
	EXPECT(nytl::vec::angle(b3, c3), test::approx(nytl::constants::pi / 2));
}

TEST_METHOD("[distances]") {
	EXPECT(nytl::vec::distance(a, b), test::approx(nytl::vec::length(a)));
	EXPECT(nytl::vec::distance(f, f), test::approx(0.0));
	EXPECT(nytl::vec::distance(x, y), test::approx(nytl::vec::length(x - y)));
	EXPECT(nytl::vec::distance(y, x), test::approx(nytl::vec::length(x - y)));
}

TEST_METHOD("[cross-product]") {
	EXPECT(nytl::vec::cross(a, b), test::approx(Vec3{0.0, 0.0, 0.0}));
	EXPECT(nytl::vec::cross(a, c), test::approx(Vec3{0.0, 3.0, -2.0}));
	EXPECT(nytl::vec::cross(c, a), test::approx(Vec3{0.0, -3.0, 2.0}));
	EXPECT(nytl::vec::cross(x, x), test::approx(Vec3{0.0, 0.0, 0.0}));
	EXPECT(nytl::vec::cross(f, y), test::approx(-nytl::vec::cross(y, f)));
	EXPECT(nytl::vec::cross(x, y), test::approx(Vec3{21.0, 16.0, -18.0}));
}
