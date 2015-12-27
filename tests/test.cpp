#include <nytl/simplex.hpp>
#include <iostream>
#include <fstream>
using namespace nytl;

int main()
{
	simplex<2, float> t{vec2f{0.f, 0.f}, vec2f{0.f, 100.f}, vec2f{100.f, 0.f}};
	std::cout << t.barycentric({50, 50});	
}
