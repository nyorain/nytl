#include "test.hpp"

#include <nytl/matOps.hpp>
#include <nytl/mat.hpp>
#include <nytl/approx.hpp>
#include <nytl/approxVec.hpp>
#include <nytl/vecOps.hpp>
#include <nytl/vec.hpp>

#include <array>
#include <type_traits>
#include <limits>
#include <cfloat>
#include <iostream>
#include <stack>
#include <tuple>

TEST(basic) {
	auto r1 = nytl::Vec<1, double>{2.0};
	auto r2 = nytl::Vec<1, double>{1.0};

	nytl::Mat<2, 1, double> x {{r1, r2}};
	nytl::Mat<1, 3, double> y {1.0, 2.0, 3.0};
	nytl::Mat<4, 1, double> z {-1.0, 0.0, 1.0, -2.0};
	nytl::Mat<2, 3, double> a {{nytl::Vec3d{1.0, 2.0, -1.0}, nytl::Vec3d{0.0, 2.0, 1.0}}};
	nytl::Mat<4, 2, double> b {{nytl::Vec2d{1.0, 1.0}, {0.0, 2.0}, {3.0, -1.0}, {-1.0, 2.0}}};

	nytl::Mat<4, 1, double> r2bx {6.0, 4.0, 10.0, 0.0};
	nytl::Mat<4, 3, double> rba {{
		nytl::Vec3d{1.0, 4.0, 0.0},
		{0.0, 4.0, 2.0},
		{3.0, 4.0, -4.0},
		{-1.0, 2.0, 3.0}
	}};
	nytl::Mat<4, 4, double> rzzt {
		1.0, 0.0, -1.0, 2.0,
		0.0, 0.0, 0.0, 0.0,
		-1.0, 0.0, 1.0, -2.0,
		2.0, 0.0, -2.0, 4.0
	};
	nytl::Mat<2, 1, double> rayt {2, 7};
	nytl::Mat<4, 3, double> rzy {
		-1.0, -2.0, -3.0,
		0.0, 0.0, 0.0,
		1.0, 2.0, 3.0,
		-2.0, -4.0, -6.0
	};

	EXPECT(2 * (b * x), nytl::approx(r2bx));
	EXPECT(b * a, nytl::approx(rba));
	EXPECT(z * nytl::transpose(z), nytl::approx(rzzt));
	EXPECT(a * nytl::transpose(y), nytl::approx(rayt));
	EXPECT(z * y, nytl::approx(rzy));

	// - should not compile -
	// a * r1;
	// y * a;
	// a + nytl::mat::transpose(b);
	// y * y;
	// 3 - nytl::mat::transpose(x) * x;
}

TEST(echolon) {
	nytl::Mat<3, 5, double> a {
		2.0, 1.0, -1.0, 8.0, 80.0,
		-3.0, -1.0, 2.0, -11.0, -110.0,
		-2.0, 1.0, 2.0, -3.0, -30.0
	};

	nytl::Mat<3, 5, double> reduced {
		1.0, 0.0, 0.0, 2.0, 20.0,
		0.0, 1.0, 0.0, 3.0, 30.0,
		0.0, 0.0, 1.0, -1.0, -10.0
	};

	nytl::reducedRowEcholon(a);
	EXPECT(a, nytl::approx(reduced));
}

// tests the lu decomposition operations
TEST(lu_decomp_1) {
	nytl::Mat<3, 3, double> a {
		2.0, 2.0, 3.0,
		1.0, 1.0, -1.0,
		1.0, 0.0, 2.0,
	};

	auto lups = nytl::luDecomp(a);
	const auto& l = lups.lower;
	const auto& u = lups.upper;
	const auto& p = lups.perm;

	EXPECT(l * u, nytl::approx(p * a));

	auto lups2 = nytl::luDecomp(p * a);
	const auto& l2 = lups2.lower;
	const auto& u2 = lups2.upper;
	const auto& p2 = lups2.perm;

	EXPECT(l2 * u2, nytl::approx(p * a));
	EXPECT(p2, (nytl::identity<3, double>()));
}

TEST(lu_decomp_2) {
	nytl::Mat<3, 3, double> a {
		3.0, -.1, -.2,
		0.1, 7, -.3,
		.3, -.2, 10
	};

	nytl::Vec<3, double> b {7.85, -19.3, 71.4};

	auto lups = nytl::luDecomp(a);
	const auto& l = lups.lower;
	const auto& u = lups.upper;
	const auto& p = lups.perm;

	EXPECT(l * u, nytl::approx(p * a));

	auto x2 = nytl::luEvaluate(l, u, b);
	EXPECT(a * x2, nytl::approx(p * b));

	// the manually (right) computed solution
	nytl::Vec<3, double> x {3.0, -2.5, 7.0};
	EXPECT(a * x, nytl::approx(p * b));
}

// tests the inverse and determinant operations
TEST(inverse) {
	{
		nytl::Mat<5, 5, double> a {
			1, -2, 3, 5, 8,
			0, -1, -1, 2, 3,
			2, 4, -1, 3, 1,
			0, 0, 5, 0, 0,
			1, 3, 0, 4, -1
		};

		auto lups = nytl::luDecomp(a);
		const auto& l = lups.lower;
		const auto& u = lups.upper;
		const auto& p = lups.perm;
		EXPECT(l * u, nytl::approx(p * a));

		EXPECT(nytl::determinant(a), nytl::approx(-135.0));
		EXPECT(nytl::invertible(a), true);

		auto inv = nytl::inverse(a);
		auto inv1 = nytl::inverse(lups);
		EXPECT(inv, nytl::approx(inv1));

		auto iinv = nytl::inverse(inv);
		EXPECT(a, nytl::approx(iinv));

		auto id = nytl::identity<5, double>();
		EXPECT(a * inv, nytl::approx(id));
		EXPECT(inv * a, nytl::approx(id));
	}

	{
		nytl::Mat<5, 5, double> a {{
			nytl::Vec<5, double>{1, -2, 3, 5, 8},
			{0, -1, -1, 0, 3},
			{2, 4, -1, 10, 1},
			{0, 0, 5, 0, 0},
			{1, 3, 0, 5, -1}
		}};

		auto lups = nytl::luDecomp(a);
		const auto& l = lups.lower;
		const auto& u = lups.upper;
		const auto& p = lups.perm;
		EXPECT(l * u, nytl::approx(p * a));

		EXPECT(nytl::determinant(a), nytl::approx(0.0));
		EXPECT(nytl::invertible(a), false);

		// undefined behvaiour now
		// ERROR(nytl::inverse(a), std::invalid_argument);
		// ERROR(nytl::inverse(lups), std::invalid_argument);
	}
}