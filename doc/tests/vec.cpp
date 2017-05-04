#include "test.hpp"
#include "approx.hpp"
#include "approxMath.hpp"

#include <cmath>
#include <iomanip>
#include <stdexcept>

// testing both, vec and vecOps
#include <nytl/vec.hpp>
#include <nytl/vecOps.hpp>
#include <nytl/scalar.hpp>

// TODO: test component-wise (nytl::vec::cw) operations
// TODO: more testvectors, use other sizes and types

// NOTE: for a real test, uncomment the should not compile statements
// one-by-one and assure that they not compile

using namespace nytl;

// collection of 3d vecs

// just a few random vectors with different types
// no special meaning
// double
constexpr Vec2d d2a {1.0, 2.0};
constexpr Vec2d d2b {0.0, 12.0};
constexpr Vec2d d2c {-5.0, 2.5};

constexpr Vec3d d3a {1.0, 2.0, 3.0};
constexpr Vec3d d3b {0.0, 0.0, 0.0};
constexpr Vec3d d3c {1.0, 0.0, 0.0};
constexpr Vec3d d3d {-1.0, 1.0, 1.0};
constexpr Vec3d d3e {0.0001, 1.0, -1.0};
constexpr Vec3d d3f {100.0, 500.0, -4.0};
constexpr Vec3d d3g {2.0, 3.0, 5.0};
constexpr Vec3d d3h {4.0, -3.0, 2.0};
constexpr Vec3d d3i {0.0, 10.0, 1.0};

constexpr Vec4d d4a{1.0, 2.7, 3.87, 8.22};
constexpr Vec4d d4b{0.0, -2.334, 0.0, -103.4};
constexpr Vec4d d4c{1.0, 2.7, 3.87, 8.22};

// int
constexpr Vec2i i2a {1, 2};
constexpr Vec3i i3a {-1, 0, 2};
constexpr Vec4i i4a {5, -2, 12, 0};

constexpr nytl::Vec<5, int> i5a {1, 2, 3, 4, 5};
constexpr nytl::Vec<5, int> i5b {10, 20, -10, -20, 0};
constexpr nytl::Vec<7, int> i7a {1, 2, 3, 4, 5, 6, 7};
constexpr nytl::Vec<7, int> i7b {-1, 0, 0, 0, 1, 4, 5};

TEST(vec_addition) {
	EXPECT(-d3a, test::approx(Vec3d{-1.0, -2.0, -3.0}));
	EXPECT(d3a + d3b, test::approx(d3a));
	EXPECT(d3a - d3b, test::approx(d3a));
	EXPECT(d3a + d3a, test::approx(Vec3d{2.0, 4.0, 6.0}));
	EXPECT(d3g + d3i, test::approx(Vec3d{2.0, 13.0, 6.0}));
	EXPECT(d3b - d3g + d3g - d3g + d3b, test::approx(-d3g));
	EXPECT(d3f - d3f, test::approx(d3b));
	EXPECT(d3b, test::approx(-d3b));
	EXPECT(d2a + i2a, test::approx(Vec2d{2.0, 4.0}));
	EXPECT(d3b - i3a, test::approx(Vec3d{1.0, 0.0, -2.0}));
	EXPECT(i5a + i5b, test::approx(Vec<5, int>{11, 22, -7, -16, 5}));

	// - should not compile - TODO
	// d2a + d3a;
	// d3a + d2a;
	// i3a + i2a;
}

TEST(scalar_mult) {
	EXPECT(2 * d3a, test::approx(d3a + d3a));
	EXPECT(5 * d3b, test::approx(d3b));
	EXPECT(-1 * d3f, test::approx(-d3f));
	EXPECT(0 * d3e, test::approx(d3b));
	EXPECT(0.5 * d3h, test::approx(Vec3f{2.0, -1.5, 1.0}));
	EXPECT(0.2 * d3i, test::approx(d3i - 0.8 * d3i));
	EXPECT(2 * d3g + d3h, test::approx(Vec3f{8.0, 3.0, 12.0}));
	EXPECT(2 * i5a, test::approx(Vec<5, int>{2, 4, 6, 8, 10}));
}

TEST(multiplies) {
	EXPECT(nytl::vec::multiply(d3a), test::approx(6.0));
	EXPECT(nytl::vec::multiply(d3b), test::approx(0.0));
	EXPECT(nytl::vec::multiply(d3c), test::approx(0.0));
	EXPECT(nytl::vec::multiply(d3d), test::approx(-1.0));
	EXPECT(nytl::vec::multiply(d3e), test::approx(-0.0001));
	EXPECT(nytl::vec::multiply(d3f), test::approx(-200000.0));
	EXPECT(static_cast<unsigned int>(nytl::vec::multiply(i7a)), nytl::factorial(7));
}

TEST(sums) {
	EXPECT(nytl::vec::sum(d3a), test::approx(6.0));
	EXPECT(nytl::vec::sum(d3b), test::approx(0.0));
	EXPECT(nytl::vec::sum(d3c), test::approx(1.0));
	EXPECT(nytl::vec::sum(d3d), test::approx(1.0));
	EXPECT(nytl::vec::sum(d3e), test::approx(0.0001));
	EXPECT(nytl::vec::sum(d3f), test::approx(596.0));
	EXPECT(nytl::vec::sum(i7a), 1 + 2 + 3 + 4 + 5 + 6 + 7);
}

TEST(dot) {
	EXPECT(nytl::vec::dot(d3a, d3b), test::approx(0.0));
	EXPECT(nytl::vec::dot(d3a, d3c), test::approx(1.0));
	EXPECT(nytl::vec::dot(d3a, d3d), test::approx(4.0));
	EXPECT(nytl::vec::dot(d3d, d3a), test::approx(4.0));
	EXPECT(nytl::vec::dot(d3g, d3a), test::approx(23.0));
	EXPECT(nytl::vec::dot(d3g, d3h), test::approx(9.0));
	EXPECT(nytl::vec::dot(d3i, d3g), test::approx(35.0));
	EXPECT(nytl::vec::dot(d3g, d3f), test::approx(1680.0));
	EXPECT(nytl::vec::dot(d3h, d3d), test::approx(-5.0));

	// - should not compile -
	// nytl::vec::dot(d3a, d2a);
}

TEST(length) {
	EXPECT(nytl::vec::length(d3b), test::approx(0.0));
	EXPECT(nytl::vec::length(d3a), test::approx(std::sqrt(14.0)));
	EXPECT(nytl::vec::length(d3f), test::approx(nytl::vec::length(-d3f)));
	EXPECT(nytl::vec::length(2 * d3a), test::approx(2 * nytl::vec::length(d3a)));
	EXPECT(nytl::vec::length(1232 * d3a), test::approx(1232 * nytl::vec::length(d3a)));
	EXPECT(nytl::vec::length(-5 * d3a), test::approx(5.0 * nytl::vec::length(d3a)));
	EXPECT(nytl::vec::length(d3b), test::approx(0.0));
	EXPECT(nytl::vec::length(d3c), test::approx(1.0));
	EXPECT(nytl::vec::length(d3g), test::approx(std::sqrt(38.0)));
	EXPECT(nytl::vec::length(d3h), test::approx(std::sqrt(29.0)));
	EXPECT(nytl::vec::length(d3i), test::approx(std::sqrt(nytl::vec::dot(d3i, d3i))));
	EXPECT(nytl::vec::length(d3g - d3a), test::approx(std::sqrt(6.0)));
	EXPECT(nytl::vec::length(1.5 * (d3a + d3b + d3c)), test::approx(1.5 * std::sqrt(17.0)));
	EXPECT(nytl::vec::length(Vec3i{1, 2, 3}), test::approx(std::sqrt(14.0)));
}

TEST(angles) {
	// custom angle vectors
	constexpr Vec2d a {1.0, 0.0};
	constexpr Vec2d b {0.0, 1.0};
	constexpr Vec2i c {1, 1};

	EXPECT(nytl::vec::angle(a, b), test::approx(nytl::radians(90.0)));
	EXPECT(nytl::vec::angle(b, a), test::approx(nytl::constants::pi / 2));
	EXPECT(nytl::vec::angle(a, c), test::approx(nytl::radians(45.0)));
	EXPECT(nytl::vec::angle(c, b), test::approx(nytl::constants::pi / 4));

	constexpr Vec3f d {1.0, 0.0, -1.0};
	constexpr Vec3i e {1, 0, 0};
	constexpr Vec3ui f {0u, 1u, 0u};

	EXPECT(nytl::vec::angle(d, e), test::approx(nytl::constants::pi / 4));
	EXPECT(nytl::vec::angle(f, e), test::approx(nytl::radians(90.0)));
	EXPECT(nytl::vec::angle(e, f), test::approx(nytl::constants::pi / 2));

	ERROR(nytl::vec::angle(d3a, d3b), std::domain_error);
	ERROR(nytl::vec::angle(d3b, d3b), std::domain_error);

	EXPECT(nytl::vec::angle(d3g, d3g), test::approx(0.0));
	EXPECT(nytl::vec::angle(d3g, d3h), test::approx(1.296246288593885243));
	EXPECT(nytl::vec::angle(d2a, d2b), test::approx(0.46364760900080614903));
	EXPECT(nytl::vec::angle(i5a, i5b), test::approx(1.8295137377985963845));

	// - should not compile -
	// nytl::vec::angle(a3, a2);
	// nytl::vec::angle(b2, c3);
}

TEST(distances) {
	EXPECT(nytl::vec::distance(d3a, d3b), test::approx(nytl::vec::length(d3a)));
	EXPECT(nytl::vec::distance(d3f, d3f), test::approx(0.0));
	EXPECT(nytl::vec::distance(d3g, d3h), test::approx(nytl::vec::length(d3g - d3h)));
	EXPECT(nytl::vec::distance(d3h, d3g), test::approx(nytl::vec::length(d3g - d3h)));

	// - should not compile - TODO
	// nytl::vec::distance(Vec2d{2.0, 3.0}, d3a);
	// nytl::vec::distance(Vec4d{2.0, 3.0, 0.0, 1.0}, d2a);
}

TEST(cross_product) {
	EXPECT(nytl::vec::cross(d3a, d3b), test::approx(Vec3d{0.0, 0.0, 0.0}));
	EXPECT(nytl::vec::cross(d3a, d3c), test::approx(Vec3d{0.0, 3.0, -2.0}));
	EXPECT(nytl::vec::cross(d3c, d3a), test::approx(Vec3d{0.0, -3.0, 2.0}));
	EXPECT(nytl::vec::cross(d3g, d3g), test::approx(Vec3d{0.0, 0.0, 0.0}));
	EXPECT(nytl::vec::cross(d3f, d3h), test::approx(-nytl::vec::cross(d3h, d3f)));
	EXPECT(nytl::vec::cross(d3g, d3h), test::approx(Vec3d{21.0, 16.0, -18.0}));

	// - should not compile -
	// nytl::vec::cross(d3a, Vec2d{1.0, 2.0});
	// nytl::vec::cross(Vec2d{2.0, 3.0}, Vec2d{1.0, 2.0});
	// nytl::vec::cross(Vec4d{1.0, 2.0, 3.0, 4.0}, d3b);
}
