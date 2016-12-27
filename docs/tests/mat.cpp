#include "test.hpp"

#include <nytl/matOps.hpp>
#include <nytl/mat.hpp>
#include <nytl/vecOps.hpp>
#include <nytl/vec.hpp>

#include <array>
#include <type_traits>
#include <limits>
#include <cfloat>
#include <iostream>
#include <stack>

static_assert(sizeof(std::vector<int>) <= 24);

// tests the echolon form operations
void echolon()
{
	{
		nytl::Mat<double, 3, 5> a {
			{2.0, 1.0, -1.0, 8.0, 80.0},
		 	{-3.0, -1.0, 2.0, -11.0, -110.0},
		 	{-2.0, 1.0, 2.0, -3.0, -30.0}
		};

		nytl::Mat<double, 3, 5> reduced {
			{1.0, 0.0, 0.0, 2.0, 20.0},
		 	{0.0, 1.0, 0.0, 3.0, 30.0},
		 	{0.0, 0.0, 1.0, -1.0, -10.0}
		};

		nytl::mat::reducedRowEcholon(a);
		CHECK_EXPECT(a, reduced);
	}
}

// tests the lu decomposition operations
void lu()
{
	{
		nytl::Mat<double, 3, 3> a {
			{2.0, 2.0, 3.0},
			{1.0, 1.0, -1.0},
			{1.0, 0.0, 2.0},
		};

		auto lups = nytl::mat::luDecomp(a);
		const auto& l = std::get<0>(lups);
		const auto& u = std::get<1>(lups);
		const auto& p = std::get<2>(lups);

		CHECK_EXPECT(l * u, p * a);
	}

	{
		nytl::Mat<double, 3, 3> a {
			{3.0, -.1, -.2},
			{0.1, 7, -.3},
			{.3, -.2, 10}
		};

		nytl::Vec<3, double> b {7.85, -19.3, 71.4};
		nytl::Vec<3, double> x {3.0, -2.5, 7.0};

		auto lups = nytl::mat::luDecomp(a);
		const auto& l = std::get<0>(lups);
		const auto& u = std::get<1>(lups);
		const auto& p = std::get<2>(lups);

		CHECK_EXPECT(l * u, p * a);
		CHECK_EXPECT(nytl::mat::luEvaluate(l, u, b), p * x)
	}
}

// tests the inverse and determinant operations
void inverse()
{
	{
		nytl::Mat<double, 5, 5> a {
			{1, -2, 3, 5, 8},
			{0, -1, -1, 2, 3},
			{2, 4, -1, 3, 1},
			{0, 0, 5, 0, 0},
			{1, 3, 0, 4, -1}
		};

		// std::cout << a << "\n";
		auto lups = nytl::mat::luDecomp(a);
		const auto& l = std::get<0>(lups);
		const auto& u = std::get<1>(lups);
		auto inv = nytl::mat::inverse(a);

		// std::cout << l * u << "\n";
		// std::cout << nytl::mat::determinant(a) << "\n";
		// std::cout << inv << "\n";
		// std::cout << a * inv << "\n";
		// std::cout << nytl::vec::dot(nytl::mat::row(a, 0), nytl::mat::col(inv, 4)) << "\n";
		// std::cout << 5 * inv[0][4] + 3 * inv[0][4] + 8 * inv[1][4] << "\n";
		// std::cout.precision(100);
		// std::cout << testEqual(inv[0][4], -inv[1][4]) << "\n";
		// std::cout << 8 * inv[0][4] << "\n";
		// std::cout << 8 * inv[1][4] << "\n";
		std::cout << inv[0][4] + inv[1][4] << "\n";
		// std::cout << l << "\n" << u << "\n";

		// CHECK_EXPECT(nytl::mat::determinant(a), -135.0);
		//
		auto inverse = nytl::mat::inverse(a);
		// std::cout << a * inverse << "\n";


		nytl::Mat<double, 5, 5> identity;
		nytl::mat::identity(identity);

		CHECK_EXPECT(a * inverse, identity);
		// CHECK_EXPECT(inverse * a, identity);
		// std::cout << DBL_MIN << "\n";
	}
}

int main()
{
	// echolon();
	// lu();
	inverse();

	std::cout << (failed ? std::to_string(failed) : "no") << " tests failed!\n";
}
