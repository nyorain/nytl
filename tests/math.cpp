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
	//vec operations type test
	{
		vec2f a(100.f, 200.f);
		vec3i b{300, 100, 55};

		static_assert(std::is_same<decltype(a + b), vec3f>::value, "");
		static_assert(std::is_same<decltype(b + a), vec3f>::value, "");

		static_assert(std::is_same<decltype(b - a), vec3f>::value, "");
		static_assert(std::is_same<decltype(a * b), vec3f>::value, "");
		static_assert(std::is_same<decltype(b / a), vec3f>::value, "");
	}

	//vec operations test
	{
		vec3d a(5, 10, 20);
		vec<6, float> b(1, 2, 3, 4, 5, 6);

		assert(allEqual(a + b, b + a));
		assert(allEqual(-a - b, -b - a));
		assert(allEqual(a * b, b * a));
		assert(allEqual((1 / a) * b, b / a));

		assert(all((a + b) == vec<6, float>(6, 12, 23, 4, 5, 6)));
		assert(all((b * a) == vec<6, float>(5, 20, 60, 4, 5, 6)));
	}

	//simplex operations test
	{
		{
			line3f a({0.f, 0.f, 0.f}, {100.f, -100.f, 50.f});

			assert(contains(a, vec3f{50.f, -50.f, 25.f}));
			//assert(contains(a, vec2i{0, -0}));
			//assert(contains(a, vec1f{0.f}));

			//assert(!contains(a, vec4f{50.f, 50.f, 25.f, -44.6f}));
			assert(!contains(a, vec3f{101.f, -101.f, 50.5f}));
			//assert(!contains(a, vec1f{10.f}));

			assert(all(barycentric(a, vec3f{50.f, -50.f, 25.f}) == vec2f(0.5, 0.5)));
			assert(all(barycentric(a, vec3f{0.f, 0.f, 0.f}) == vec2f(1, 0)));
			assert(all(barycentric(a, vec3f{100.f, -100.f, 50.f}) == vec2f(0, 1)));

			bool thrown = 0;
			try
			{
				barycentric(a, vec3f{0.f, 0.f, -5.f}); //do not exist
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
