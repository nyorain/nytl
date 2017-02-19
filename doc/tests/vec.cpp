#include "test.hpp"
#include "approx.hpp"
#include <cmath>

// testing both, vec and vecOps
#include <nytl/vec.hpp>
#include <nytl/vecOps.hpp>
#include <nytl/scalar.hpp>

// TODO: test component-wise (nytl::vec::cw) operations
// TODO: more testvectors, use other sizes and types

using namespace nytl;

constexpr Vec3d a{1.0, 2.0, 3.0};
constexpr Vec3d b{0.0, 0.0, 0.0};
constexpr Vec3d c{1.0, 0.0, 0.0};
constexpr Vec3d d{-1.0, 1.0, 1.0};
constexpr Vec3d e{0.0001, 1.0, -1.0};
constexpr Vec3d f{100.0, 500.0, -4.0};

constexpr Vec3d x{2.0, 3.0, 5.0};
constexpr Vec3d y{4.0, -3.0, 2.0};
constexpr Vec3d z{0.0, 10.0, 1.0};

TEST_METHOD("[vec-addition]") {
	EXPECT(a + b, test::approx(a));
	EXPECT(a - b, test::approx(a));
	EXPECT(a + a, test::approx(Vec3d{2.0, 4.0, 6.0}));
	EXPECT(x + z, test::approx(Vec3d{2.0, 13.0, 6.0}));
	EXPECT(b - x + x - x + b, test::approx(-x));
	EXPECT(f - f, test::approx(b));
	EXPECT(b, test::approx(-b));
	EXPECT((Vec3d{1.0, 1.0, 1.0} + Vec3d{-1.0, 2.0, 0.0}), (test::approx(Vec3d{0.0, 3.0, 1.0})));
	EXPECT((Vec2i{1, 2} - Vec2d{1.0, 2.0}), (test::approx(Vec2f{0.0, 0.0})));

	// - should not compile - TODO
}

TEST_METHOD("[scalar multiplication]") {
	EXPECT((2 * a), test::approx(a + a));
	EXPECT((5 * b), test::approx(b));
	EXPECT((-1 * f), test::approx(-f));
	EXPECT((0 * e), test::approx(b));
	EXPECT((0.5 * y), test::approx(Vec3f{2.0, -1.5, 1.0}));
	EXPECT((0.2 * z), test::approx(z - 0.8 * z));
	EXPECT((2 * x + y), test::approx(Vec3f{8.0, 3.0, 12.0}));
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

	// - should not compile -
	// nytl::vec::dot(a, Vec2{1.0, 2.0});
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
	EXPECT(nytl::vec::length(z), test::approx(std::sqrt(nytl::vec::dot(z, z))));
	EXPECT(nytl::vec::length(x - a), test::approx(std::sqrt(6.0)));
	EXPECT(nytl::vec::length(1.5 * (a + b + c)), test::approx(1.5 * std::sqrt(17.0)));
	EXPECT(nytl::vec::length(Vec3i{1, 2, 3}), test::approx(std::sqrt(14.0)));
}

TEST_METHOD("[angles]") {
	constexpr Vec2d a2{1.0, 0.0};
	constexpr Vec2d b2{0.0, 1.0};
	constexpr Vec2i c2{1, 1};

	constexpr Vec3f a3{1.0, 0.0, -1.0};
	constexpr Vec3i b3{1, 0, 0};
	constexpr Vec3ui c3{0u, 1u, 0u};

	std::cout << nytl::vec::dot(b2, c2) << "\n";
	std::cout << nytl::vec::length(b2) * nytl::vec::length(c2) << "\n";

	EXPECT(nytl::vec::angle(x, x), test::approx(0.0));
	EXPECT(nytl::vec::angle(a2, b2), test::approx(nytl::radians(90.0)));
	EXPECT(nytl::vec::angle(b2, a2), test::approx(nytl::constants::pi / 2));
	EXPECT(nytl::vec::angle(a2, c2), test::approx(nytl::radians(45.0)));
	EXPECT(nytl::vec::angle(c2, b2), test::approx(nytl::constants::pi / 4));

	EXPECT(nytl::vec::angle(a3, b3), test::approx(nytl::constants::pi / 4));
	EXPECT(nytl::vec::angle(static_cast<Vec3i>(c3), b3), test::approx(nytl::radians(90.0)));
	EXPECT(nytl::vec::angle(b3, static_cast<Vec3i>(c3)), test::approx(nytl::constants::pi / 2));

	EXPECT_ERROR(nytl::vec::angle(a, b), std::domain_error);
	EXPECT_ERROR(nytl::vec::angle(b, b), std::domain_error);

	// - should not compile -
	// nytl::vec::angle(a3, a2);
	// nytl::vec::angle(b2, c3);
}

TEST_METHOD("[distances]") {
	EXPECT(nytl::vec::distance(a, b), test::approx(nytl::vec::length(a)));
	EXPECT(nytl::vec::distance(f, f), test::approx(0.0));
	EXPECT(nytl::vec::distance(x, y), test::approx(nytl::vec::length(x - y)));
	EXPECT(nytl::vec::distance(y, x), test::approx(nytl::vec::length(x - y)));

	// - should not compile - TODO
	// nytl::vec::distance(Vec2{2.0, 3.0}, a);
	// nytl::vec::distance(Vec4{2.0, 3.0. 0.0, 1.0}, x);
}

TEST_METHOD("[cross-product]") {
	EXPECT(nytl::vec::cross(a, b), test::approx(Vec3d{0.0, 0.0, 0.0}));
	EXPECT(nytl::vec::cross(a, c), test::approx(Vec3d{0.0, 3.0, -2.0}));
	EXPECT(nytl::vec::cross(c, a), test::approx(Vec3d{0.0, -3.0, 2.0}));
	EXPECT(nytl::vec::cross(x, x), test::approx(Vec3d{0.0, 0.0, 0.0}));
	EXPECT(nytl::vec::cross(f, y), test::approx(-nytl::vec::cross(y, f)));
	EXPECT(nytl::vec::cross(x, y), test::approx(Vec3d{21.0, 16.0, -18.0}));

	// - should not compile -
	// nytl::vec::cross(x, Vec2{1.0, 2.0});
	// nytl::vec::cross(Vec2{2.0, 3.0}, Vec2{1.0, 2.0});
	// nytl::vec::cross(nytl::Vec4{1.0, 2.0, 3.0, 4.0}, f);
}
