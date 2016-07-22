#include <nytl/convert.hpp>
#include <nytl/vec.hpp>

#include <array>
#include <algorithm>

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
		std::copy(ret.begin(), ret.end(), other.begin());
		return ret;
	}
};

}

int main()
{
	//works for all conversion that can be achieved using a static cast
	auto i = nytl::convert<int>(7.f);
	auto f = nytl::convert<float>(42);

	//note that if the the variable is already declared with a type, the explicit template
	//parameter for the convert function is not even needed.
	nytl::Vec2f position;
	position = nytl::convert(nytl::Vec3i(5, 6, 3));

	//The thing convert is really useful for are custom conversions.
	//Usually this would take up more space and would not be as convinient.
	//In most cases provide a nytl::Converter specialization is proabaly still more effort,
	//so it is only really useful if the conversion is done often.
	//This would the case be e.g. when writing an application that usues nytl, glm and assimp
	//and therefore has to convert between their 3 vector representations rather frequently.
	//Can also be useful if the conversion is rather complex.
	std::array<float, 2> arr1 = nytl::convert(position);
	auto arr2 = nytl::convert<std::array<float, 2>>(position);
}
