#include "test.hpp"
#include <cmath>

// testing both, vec and vecOps
#include <nytl/vec.hpp>
#include <nytl/vecOps.hpp>
#include <nytl/scalar.hpp>

// TODO: test component-wise (nytl::vec::cw) operations
// TODO: test cross product!

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

TEST_CASE("Inter-Sums are computed", "[addition]") {
	REQUIRE((a + b) == test::approx(a));
	REQUIRE((a - b) == test::approx(a));
	REQUIRE((a + a) == test::approx(Vec3{2.0, 4.0, 6.0}));
	REQUIRE((x + z) == test::approx(Vec3{2.0, 13.0, 6.0}));
	REQUIRE((b - x + x - x + b) == test::approx(-x));
	REQUIRE((f - f) == test::approx(b));
	REQUIRE(b == test::approx(-b));
	REQUIRE((Vec3{1.0, 1.0, 1.0} + Vec3{-1.0, 2.0, 0.0}) == test::approx(Vec3{0.0, 3.0, 1.0}));
}

TEST_CASE("Scalar multiplies are computed", "[scalar multiplication]") {
	REQUIRE((2 * a) == test::approx(a + a));
	REQUIRE((5 * b) == test::approx(b));
	REQUIRE((-1 * f) == test::approx(-f));
	REQUIRE((0 * e) == test::approx(b));
	REQUIRE((0.5 * y) == test::approx(Vec3{2.0, -1.5, 1.0}));
	REQUIRE((0.2 * z) == test::approx(z - 0.8 * z));
	REQUIRE((2 * x + y) == test::approx(Vec3{8.0, 3.0, 12.0}));
}

TEST_CASE("Multiplies are computed", "[multiplies]") {
	REQUIRE(nytl::vec::multiply(a) == test::approx(6.0));
	REQUIRE(nytl::vec::multiply(b) == test::approx(0.0));
	REQUIRE(nytl::vec::multiply(c) == test::approx(0.0));
	REQUIRE(nytl::vec::multiply(d) == test::approx(-1.0));
	REQUIRE(nytl::vec::multiply(e) == test::approx(-0.0001));
	REQUIRE(nytl::vec::multiply(f) == test::approx(-200000.0));
}

TEST_CASE("Intra-Sums are computed", "[sums]") {
	REQUIRE(nytl::vec::sum(a) == test::approx(6.0));
	REQUIRE(nytl::vec::sum(b) == test::approx(0.0));
	REQUIRE(nytl::vec::sum(c) == test::approx(1.0));
	REQUIRE(nytl::vec::sum(d) == test::approx(1.0));
	REQUIRE(nytl::vec::sum(e) == test::approx(0.0001));
	REQUIRE(nytl::vec::sum(f) == test::approx(596.0));
}

TEST_CASE("Dot products are computed", "[dot]") {
	REQUIRE(nytl::vec::dot(a, b) == Approx(0.0));
	REQUIRE(nytl::vec::dot(a, c) == Approx(1.0));
	REQUIRE(nytl::vec::dot(a, d) == Approx(4.0));
	REQUIRE(nytl::vec::dot(d, a) == Approx(4.0));
	REQUIRE(nytl::vec::dot(x, a) == Approx(23.0));
	REQUIRE(nytl::vec::dot(x, y) == Approx(9.0));
	REQUIRE(nytl::vec::dot(z, x) == Approx(35.0));
	REQUIRE(nytl::vec::dot(x, f) == Approx(1680.0));
	REQUIRE(nytl::vec::dot(y, d) == Approx(-5.0));
}

TEST_CASE("Lengths are computed", "[length]") {
	REQUIRE(nytl::vec::length(b) == test::approx(0.0));
	REQUIRE(nytl::vec::length(a) == test::approx(std::sqrt(14.0)));
	REQUIRE(nytl::vec::length(f) == test::approx(nytl::vec::length(-f)));
	REQUIRE(nytl::vec::length(2 * a) == test::approx(2 * nytl::vec::length(a)));
	REQUIRE(nytl::vec::length(1232 * a) == test::approx(1232 * nytl::vec::length(a)));
	REQUIRE(nytl::vec::length(-5 * a) == test::approx(5.0 * nytl::vec::length(a)));
	REQUIRE(nytl::vec::length(b) == test::approx(0.0));
	REQUIRE(nytl::vec::length(c) == test::approx(1.0));
	REQUIRE(nytl::vec::length(x) == test::approx(std::sqrt(38.0)));
	REQUIRE(nytl::vec::length(y) == test::approx(std::sqrt(29.0)));
	REQUIRE(nytl::vec::length(x - a) == test::approx(std::sqrt(6.0)));
	REQUIRE(nytl::vec::length(1.5 * (a + b + c)) == test::approx(1.5 * std::sqrt(17.0)));
}

TEST_CASE("Angles are computed", "[angles]") {
	constexpr Vec2 a2(1.0, 0.0);
	constexpr Vec2 b2(0.0, 1.0);
	constexpr Vec2 c2(1.0, 1.0);

	constexpr Vec3 a3(1.0, 0.0, -1.0);
	constexpr Vec3 b3(1.0, 0.0, 0.0);
	constexpr Vec3 c3(0.0, 1.0, 0.0);

	REQUIRE(nytl::vec::angle(a2, b2) == test::approx(nytl::radians(90.0)));
	REQUIRE(nytl::vec::angle(b2, a2) == test::approx(nytl::constants::pi / 2));
	REQUIRE(nytl::vec::angle(a2, c2) == test::approx(nytl::radians(45.0)));
	REQUIRE(nytl::vec::angle(c2, b2) == test::approx(nytl::constants::pi / 4));

	REQUIRE(nytl::vec::angle(a3, b3) == test::approx(nytl::constants::pi / 4));
	REQUIRE(nytl::vec::angle(c3, b3) == test::approx(nytl::radians(90.0)));
	REQUIRE(nytl::vec::angle(b3, c3) == test::approx(nytl::constants::pi / 2));
}

TEST_CASE("Distances are computed", "[distances]") {
	REQUIRE(nytl::vec::distance(a, b) == test::approx(nytl::vec::length(a)));
	REQUIRE(nytl::vec::distance(f, f) == test::approx(0.0));
	REQUIRE(nytl::vec::distance(x, y) == test::approx(nytl::vec::length(x - y)));
	REQUIRE(nytl::vec::distance(y, x) == test::approx(nytl::vec::length(x - y)));
}
