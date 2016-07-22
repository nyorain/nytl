#include <nytl/nytl.hpp>
using namespace nytl;

#include <array>
#include <algorithm>

enum class enumType
{
	entry1,
	entry2,
	entry3,
	entry4
};

NYTL_ENABLE_ENUM_OPS(enumType)

namespace nytl
{

///Converter specialization used later.
///Note that this specialization MUST be in namespace nytl and have a static call function
///that takes an parameter of type O (second template arg) as argument and returns an object
///of type T (first template paramter of nytl::Converter).
///Note that nytl::Converter has a 3rd, unused template parameter that is defaulted to
///void and can be used for SFINAE stuff.
template<std::size_t N, typename T>
struct Converter<std::array<T, N>, nytl::Vec<N, T>>
{
	static std::array<T, N> call(const nytl::Vec<N, T>& other)
	{
		std::array<T, N> ret;
		std::copy(other.begin(), other.end(), ret.begin());
		return ret;
	}
};

}

int main()
{
	//enumOPs
	{
		auto e = (enumType::entry1 | enumType::entry2) & enumType::entry4;
		e |= e & enumType::entry3;
		e ^= enumType::entry4;
	}

	{
		auto i = nytl::convert<int>(7.f);
		auto f = nytl::convert<float>(42);

		nytl::Vec2f position;
		position = nytl::convert(nytl::Vec3i(5, 6, 3));

		std::array<float, 2> arr1 = nytl::convert(position);
		auto arr2 = nytl::convert<std::array<float, 2>>(position);

		nytl::unused(i, f, arr1, arr2);
	}

}
