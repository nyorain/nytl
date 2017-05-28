#include "bugged.hpp"
#include <nytl/approx.hpp>
#include <nytl/approxSpec.hpp>
#include <nytl/vec.hpp>
#include <nytl/mat.hpp>

TEST(basic) {
	EXPECT(nytl::approx(3.1, 0.2), 3.0);
	EXPECT(nytl::approx(3.1f), 3.1f);
	EXPECT(nytl::approx(0.0), -0.0);

	EXPECT(nytl::approx(1.0 + nytl::defaultApproxEpsilon), 1.0);
	EXPECT(nytl::approx(1.0 - nytl::defaultApproxEpsilon), 1.0);
}

TEST(complex) {
	EXPECT(nytl::approx(std::complex<double>(0.0, 0.0)), std::complex<double>(-0.0, 0.0));
	EXPECT(nytl::approx(std::complex<double>(1.0, 1.0), 0.2), std::complex<double>(1.1, 0.9));
}

TEST(vec) {
	nytl::Vec3f v3f {1.f, 2.f, 3.f};
	EXPECT(nytl::approx(v3f), v3f);
	EXPECT(nytl::approx(v3f), (nytl::Vec3d {1.0, 2.0, 3.0}));
	EXPECT(nytl::approx(v3f, 0.1), (nytl::Vec3d {1.1, 2.1, 3.1}));
	EXPECT(nytl::approx(v3f, 0.2), (nytl::Vec3d {0.9, 2.2, 2.8}));
}

TEST(mat) {
	nytl::Mat2d m2d {1.0, 2.0, 3.0, 4.0};
	EXPECT(nytl::approx(m2d), m2d);
	EXPECT(nytl::approx(m2d, 0.1), (nytl::Mat2d {1.1, 1.9, 3.1, 4.1}));
	EXPECT(nytl::approx(m2d, 10.0), (nytl::Mat2d {11.0, 12.0, 13.0, 14.0}));
}
