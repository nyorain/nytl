#include "bugged.hpp"
#include <nytl/convert.hpp>
#include <array>
#include <vector>
#include <list>

// TODO: static assert invalid expressions (i.e. copy autocastable object)

TEST(basic) {
	auto convertedFloat = nytl::convert<float>(7);
	EXPECT(convertedFloat, 7.f);

	int convertedInt = nytl::convert(23.0);
	EXPECT(convertedInt, 23);

	auto floatArray = std::array<float, 5> {{1.f, 2.f, 3.f, 4.f, 5.f}};

	std::array<int, 5> convertedIntArray = nytl::convert(floatArray);
	EXPECT(convertedIntArray[2], 3);

	auto convertedCharArray = nytl::convert<std::array<char, 5>>(floatArray);
	EXPECT(convertedCharArray[3], 4);

	auto convertedDoubleArray = nytl::arrayCast<double>(floatArray);
	EXPECT(convertedDoubleArray[0], 1.0);

	auto intVector = nytl::containerCast<std::vector<int>>(floatArray);
	auto doubleList = nytl::containerCast<std::list<double>>(intVector);
	EXPECT(doubleList.back(), 5.0);
}
