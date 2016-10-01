#include <nytl/vec.hpp>
#include <nytl/simplex.hpp>
#include <nytl/mat.hpp>
#include <nytl/transform.hpp>
#include <nytl/line.hpp>
#include <nytl/rect.hpp>
#include <nytl/triangle.hpp>
#include <nytl/refVec.hpp>
#include <nytl/dynVec.hpp>
#include <nytl/vecOps.hpp>
using namespace nytl;

#include <type_traits>
#include <cassert>

int main()
{
	//Vec operations type test
	{
		Vec2f a(100.f, 200.f);
		Vec3i b{300, 100, 55};

		static_assert(std::is_same<decltype(a + b), Vec3f>::value, "");
		static_assert(std::is_same<decltype(b + a), Vec3f>::value, "");

		static_assert(std::is_same<decltype(b - a), Vec3f>::value, "");
		static_assert(std::is_same<decltype(a * b), Vec3f>::value, "");
		static_assert(std::is_same<decltype(b / a), Vec3f>::value, "");
	}

	//Vec operations test
	{
		Vec3d a(5, 10, 20);
		Vec<6, float> b(1, 2, 3, 4, 5, 6);

		assert(allEqual(a + b, b + a));
		assert(allEqual(-a - b, -b - a));
		assert(allEqual(a * b, b * a));
		assert(allEqual((1 / a) * b, b / a));

		assert(allOf((a + b) == Vec<6, float>(6, 12, 23, 4, 5, 6)));
		assert(allOf((b * a) == Vec<6, float>(5, 20, 60, 4, 5, 6)));
	}

	{
		// constexpr auto joined = join(Vec3i(1, 2, 3), Vec4i(4, 5, 6, 7));
		
		constexpr auto v1 = Vec3i(1, 2, 3);
		constexpr auto v2 = Vec4i(4, 5, 6, 7);

		constexpr auto joined = join(v1, v2);

		static_assert(joined.size() == 7, "");
		static_assert(allEqual(joined, Vec<7, int>(1, 2, 3, 4, 5, 6, 7)), "");
	}

	return EXIT_SUCCESS;
}
