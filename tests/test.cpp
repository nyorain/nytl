#include <nytl/nytl.hpp>
#include <iostream>
#include <fstream>
using namespace nytl;

int main()
{
	vec<dynamicSize, int> a(vec3f{7, 6, 2});
	std::cout << (a == vec4f(7, 6 ,2, 222)) << "\n";

	return EXIT_SUCCESS;
}
