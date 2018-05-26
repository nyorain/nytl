#include "test.hpp"
#include <nytl/approx.hpp>
#include <nytl/approxVec.hpp>

#include <cmath>
#include <iomanip>
#include <stdexcept>

// testing both, vec and vecOps
#include <nytl/vec.hpp>
#include <nytl/vecOps.hpp>
#include <nytl/math.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-const-variable"

// TODO: test component-wise (nytl::vcw) operations
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

constexpr auto c1 = static_cast<nytl::Vec2f>(nytl::Vec2u64{1243u, 432u});
constexpr auto c2 = static_cast<nytl::Vec4f>(nytl::Vec4u64{1243u, 432u, 1u, 2u});

template<typename A, typename B> using AddT =
	std::void_t<decltype(std::declval<A>() + std::declval<B>())>;
template<typename A, typename B> using DotT =
	std::void_t<decltype(nytl::dot(std::declval<A>(), std::declval<B>()))>;
template<typename A, typename B> using CrossT =
	std::void_t<decltype(nytl::cross(std::declval<A>(), std::declval<B>()))>;

static_assert(validExpression<AddT, Vec3f, Vec3f>);
static_assert(validExpression<CrossT, Vec2f, Vec2f>);
static_assert(!validExpression<AddT, Vec3f, Mat4f>);
static_assert(!validExpression<DotT, Vec3f, Mat4f>);
static_assert(!validExpression<CrossT, Vec3f, Vec2f>);
static_assert(!validExpression<CrossT, Vec4f, Vec3f>);

TEST(deduction) {
	auto a = nytl::Vec {1.f, 2, 3, 4.};
	static_assert(std::is_same_v<decltype(a), nytl::Vec4d>);

	auto b = nytl::Vec {1, 2};
	static_assert(std::is_same_v<decltype(b), nytl::Vec2i>);

	auto c = nytl::Vec {1u, 2u};
	static_assert(std::is_same_v<decltype(c), nytl::Vec2ui>);
}

TEST(basic) {
	auto cpy = d3a;
	cpy += d3c;
	cpy -= d3d;
	EXPECT(cpy, (Vec3d{3.0, 1.0, 2.0}));

	cpy = -cpy;
	EXPECT(cpy, (Vec3d{-3.0, -1.0, -2.0}));

	cpy *= -4;
	EXPECT(cpy, (Vec3d{12.0, 4.0, 8.0}));
	EXPECT(cpy != (Vec3d{12.0, 4.0, 1.0}), true);
}

TEST(vec_addition) {
	EXPECT(-d3a, nytl::approx(Vec3d{-1.0, -2.0, -3.0}));
	EXPECT(d3a + d3b, nytl::approx(d3a));
	EXPECT(d3a - d3b, nytl::approx(d3a));
	EXPECT(d3a + d3a, nytl::approx(Vec3d{2.0, 4.0, 6.0}));
	EXPECT(d3g + d3i, nytl::approx(Vec3d{2.0, 13.0, 6.0}));
	EXPECT(d3b - d3g + d3g - d3g + d3b, nytl::approx(-d3g));
	EXPECT(d3f - d3f, nytl::approx(d3b));
	EXPECT(d3b, nytl::approx(-d3b));
	EXPECT(d2a + i2a, nytl::approx(Vec2d{2.0, 4.0}));
	EXPECT(d3b - i3a, nytl::approx(Vec3d{1.0, 0.0, -2.0}));
	EXPECT(i5a + i5b, (Vec<5, int>{11, 22, -7, -16, 5}));

	// - should not compile -
	static_assert(!validExpression<AddT, decltype(d2a), decltype(d3a)>);
	static_assert(!validExpression<AddT, decltype(d3a), decltype(d2a)>);
	static_assert(!validExpression<AddT, decltype(i3a), decltype(i2a)>);
}

TEST(scalar_mult) {
	EXPECT(2 * d3a, nytl::approx(d3a + d3a));
	EXPECT(5 * d3b, nytl::approx(d3b));
	EXPECT(-1 * d3f, nytl::approx(-d3f));
	EXPECT(0 * d3e, nytl::approx(d3b));
	EXPECT(0.5 * d3h, nytl::approx(Vec3f{2.0, -1.5, 1.0}));
	EXPECT(0.2 * d3i, nytl::approx(d3i - 0.8 * d3i));
	EXPECT(2 * d3g + d3h, nytl::approx(Vec3f{8.0, 3.0, 12.0}));
	EXPECT(2 * i5a, (Vec<5, int>{2, 4, 6, 8, 10}));
}

TEST(multiplies) {
	EXPECT(nytl::multiply(d3a), nytl::approx(6.0));
	EXPECT(nytl::multiply(d3b), nytl::approx(0.0));
	EXPECT(nytl::multiply(d3c), nytl::approx(0.0));
	EXPECT(nytl::multiply(d3d), nytl::approx(-1.0));
	EXPECT(nytl::multiply(d3e), nytl::approx(-0.0001));
	EXPECT(nytl::multiply(d3f), nytl::approx(-200000.0));
	EXPECT(static_cast<unsigned int>(nytl::multiply(i7a)), nytl::factorial(7));
}

TEST(sums) {
	EXPECT(nytl::sum(d3a), nytl::approx(6.0));
	EXPECT(nytl::sum(d3b), nytl::approx(0.0));
	EXPECT(nytl::sum(d3c), nytl::approx(1.0));
	EXPECT(nytl::sum(d3d), nytl::approx(1.0));
	EXPECT(nytl::sum(d3e), nytl::approx(0.0001));
	EXPECT(nytl::sum(d3f), nytl::approx(596.0));
	EXPECT(nytl::sum(i7a), 1 + 2 + 3 + 4 + 5 + 6 + 7);
}

TEST(dot) {
	EXPECT(nytl::dot(d3a, d3b), nytl::approx(0.0));
	EXPECT(nytl::dot(d3a, d3c), nytl::approx(1.0));
	EXPECT(nytl::dot(d3a, d3d), nytl::approx(4.0));
	EXPECT(nytl::dot(d3d, d3a), nytl::approx(4.0));
	EXPECT(nytl::dot(d3g, d3a), nytl::approx(23.0));
	EXPECT(nytl::dot(d3g, d3h), nytl::approx(9.0));
	EXPECT(nytl::dot(d3i, d3g), nytl::approx(35.0));
	EXPECT(nytl::dot(d3g, d3f), nytl::approx(1680.0));
	EXPECT(nytl::dot(d3h, d3d), nytl::approx(-5.0));

	// - should not compile -
	static_assert(!validExpression<DotT, decltype(d3a), decltype(d2a)>);
}

TEST(length) {
	EXPECT(nytl::length(d3b), nytl::approx(0.0));
	EXPECT(nytl::length(d3a), nytl::approx(std::sqrt(14.0)));
	EXPECT(nytl::length(d3f), nytl::approx(nytl::length(-d3f)));
	EXPECT(nytl::length(2 * d3a), nytl::approx(2 * nytl::length(d3a)));
	EXPECT(nytl::length(1232 * d3a), nytl::approx(1232 * nytl::length(d3a)));
	EXPECT(nytl::length(-5 * d3a), nytl::approx(5.0 * nytl::length(d3a)));
	EXPECT(nytl::length(d3b), nytl::approx(0.0));
	EXPECT(nytl::length(d3c), nytl::approx(1.0));
	EXPECT(nytl::length(d3g), nytl::approx(std::sqrt(38.0)));
	EXPECT(nytl::length(d3h), nytl::approx(std::sqrt(29.0)));
	EXPECT(nytl::length(d3i), nytl::approx(std::sqrt(nytl::dot(d3i, d3i))));
	EXPECT(nytl::length(d3g - d3a), nytl::approx(std::sqrt(6.0)));
	EXPECT(nytl::length(1.5 * (d3a + d3b + d3c)), nytl::approx(1.5 * std::sqrt(17.0)));
	EXPECT(nytl::length(Vec3i{1, 2, 3}), nytl::approx(std::sqrt(14.0)));
}

TEST(angles) {
	// custom angle vectors
	constexpr Vec2d a {1.0, 0.0};
	constexpr Vec2d b {0.0, 1.0};
	constexpr Vec2i c {1, 1};

	EXPECT(nytl::angle(a, b), nytl::approx(nytl::radians(90.0)));
	EXPECT(nytl::angle(b, a), nytl::approx(nytl::constants::pi / 2));
	EXPECT(nytl::angle(a, c), nytl::approx(nytl::radians(45.0)));
	EXPECT(nytl::angle(c, b), nytl::approx(nytl::constants::pi / 4));

	constexpr Vec3f d {1.0, 0.0, -1.0};
	constexpr Vec3i e {1, 0, 0};
	constexpr Vec3ui f {0u, 1u, 0u};

	EXPECT(nytl::angle(d, e), nytl::approx(nytl::constants::pi / 4));
	EXPECT(nytl::angle(f, e), nytl::approx(nytl::radians(90.0)));
	EXPECT(nytl::angle(e, f), nytl::approx(nytl::constants::pi / 2));

	ERROR(nytl::angle(d3a, d3b), std::domain_error);
	ERROR(nytl::angle(d3b, d3b), std::domain_error);

	EXPECT(nytl::angle(d3g, d3g), nytl::approx(0.0));
	EXPECT(nytl::angle(d3g, d3h), nytl::approx(1.296246288593885243));
	EXPECT(nytl::angle(d2a, d2b), nytl::approx(0.46364760900080614903));
	EXPECT(nytl::angle(i5a, i5b), nytl::approx(1.8295137377985963845));

	// - should not compile - TODO
	// nytl::angle(a3, a2);
	// nytl::angle(b2, c3);
}

TEST(distances) {
	EXPECT(nytl::distance(d3a, d3b), nytl::approx(nytl::length(d3a)));
	EXPECT(nytl::distance(d3f, d3f), nytl::approx(0.0));
	EXPECT(nytl::distance(d3g, d3h), nytl::approx(nytl::length(d3g - d3h)));
	EXPECT(nytl::distance(d3h, d3g), nytl::approx(nytl::length(d3g - d3h)));
}

TEST(cross_product) {
	EXPECT(nytl::cross(d3a, d3b), nytl::approx(Vec3d{0.0, 0.0, 0.0}));
	EXPECT(nytl::cross(d3a, d3c), nytl::approx(Vec3d{0.0, 3.0, -2.0}));
	EXPECT(nytl::cross(d3c, d3a), nytl::approx(Vec3d{0.0, -3.0, 2.0}));
	EXPECT(nytl::cross(d3g, d3g), nytl::approx(Vec3d{0.0, 0.0, 0.0}));
	EXPECT(nytl::cross(d3f, d3h), nytl::approx(-nytl::cross(d3h, d3f)));
	EXPECT(nytl::cross(d3g, d3h), nytl::approx(Vec3d{21.0, 16.0, -18.0}));

	// - should not compile - TODO
	// nytl::cross(d3a, Vec2d{1.0, 2.0});
	// nytl::cross(Vec2d{2.0, 3.0}, Vec2d{1.0, 2.0});
	// nytl::cross(Vec4d{1.0, 2.0, 3.0, 4.0}, d3b);
}

TEST(component_wise) {
	using namespace nytl::vec::cw;

	// abs
	EXPECT(abs(d3a), nytl::approx(d3a));
	EXPECT(abs(-d3a), nytl::approx(d3a));

	auto v1 = nytl::Vec {-2.f, -10000.f};
	EXPECT(abs(v1), nytl::approx(-v1));
	EXPECT(abs(-v1), nytl::approx(-v1));

	// clamp
	EXPECT(clamp(v1, -1.f, 1.f), (nytl::Vec {-1.f, -1.f}));
	EXPECT(clamp(d3a, 1., 1.), (nytl::Vec {1.f, 1.f, 1.f}));
	EXPECT(clamp(d2a, d2a, d2a), d2a);
	EXPECT(clamp(i5b, i5b, i5b), i5b);

	// pow
	EXPECT(pow(d3f, 1), d3f);
}
