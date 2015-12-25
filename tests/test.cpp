#include <nytl/nytl.hpp>
#include <iostream>
#include <fstream>
using namespace nytl;

int main()
{
	sequence<2, float, vec3f> myGradient;

	myGradient.addPoint({{0.f, 0.f}, {0.f, 0.f, 0.f}});
	myGradient.addPoint({{0.f, 10.f}, {0.f, 1.f, 0.f}});
	myGradient.addPoint({{10.f, 0.f}, {1.f, 0.f, 0.f}});
/*
	std::ofstream myF("test.txt");
	for(int y = -1; y < 11; ++y)
	{
		for(int x = -1; x < 11; ++x)
		{
			myF << "{(" << x << "," << y << "): " << myGradient.valueAt(vec2f(x,y)) << "} ";
		}
		myF << "\n";
	}
*/
	std::cout << myGradient.valueAt(vec2f(4, 4));
}
