#include <nytl/vec.hpp>
#include <nytl/simplex.hpp>
#include <nytl/mat.hpp>
#include <nytl/transform.hpp>
#include <nytl/line.hpp>
#include <nytl/triangle.hpp>
#include <nytl/tetrahedron.hpp>
#include <nytl/refVec.hpp>
#include <nytl/dynVec.hpp>
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

		assert(all((a + b) == Vec<6, float>(6, 12, 23, 4, 5, 6)));
		assert(all((b * a) == Vec<6, float>(5, 20, 60, 4, 5, 6)));
	}

	//simplex operations test
	{
		{
			Line3f a({0.f, 0.f, 0.f}, {100.f, -100.f, 50.f});

			assert(contains(a, Vec3f{50.f, -50.f, 25.f}));
			//assert(contains(a, Vec2i{0, -0}));
			//assert(contains(a, Vec1f{0.f}));

			//assert(!contains(a, Vec4f{50.f, 50.f, 25.f, -44.6f}));
			assert(!contains(a, Vec3f{101.f, -101.f, 50.5f}));
			//assert(!contains(a, Vec1f{10.f}));

			assert(all(barycentric(a, Vec3f{50.f, -50.f, 25.f}) == Vec2f(0.5, 0.5)));
			assert(all(barycentric(a, Vec3f{0.f, 0.f, 0.f}) == Vec2f(1, 0)));
			assert(all(barycentric(a, Vec3f{100.f, -100.f, 50.f}) == Vec2f(0, 1)));

			bool thrown = 0;
			try
			{
				barycentric(a, Vec3f{0.f, 0.f, -5.f}); //do not exist
			}
			catch(const std::exception&) //TODO: specify exception
			{
				thrown = 1;
			}

			assert(thrown);
		}
	}

	return EXIT_SUCCESS;
}
