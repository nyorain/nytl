#include "test.hpp"
#include <nytl/simplex.hpp>
#include <nytl/simplexOps.hpp>

int main()
{
	nytl::Simplex<2, double> triangle {{{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}}};

	CHECK_EXPECT(nytl::size(triangle), 0.5);
	CHECK_EXPECT(nytl::center(triangle), nytl::Vec2d(1.0/3, 1.0/3));
	CHECK_EXPECT(nytl::barycentric(triangle, {0.5, 0.5}), nytl::Vec3d(0.0, 0.5, 0.5));

	// nytl::Simplex<3, double> tetra {{{
	// 	{0.0, 0.0, 0.0},
	// 	{1.0, 0.0, 0.0},
	// 	{0.0, 1.0, 0.0},
	// 	{0.0, 0.0, 1.0}
	// }}};
	// auto vec = nytl::Vec3d(0.5, 0.5, 0.0) - tetra.points()[3];
	// std::cout << nytl::barycentricMatrix(tetra) * vec << "\n";

	std::cout << (failed ? std::to_string(failed) : "no") << " tests failed!\n";
}
