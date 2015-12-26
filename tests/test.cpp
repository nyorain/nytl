#include <nytl/nytl.hpp>
#include <iostream>
#include <fstream>
using namespace nytl;

int main()
{
	mat2f m(3, 8, 4, 6);
	std::cout << m << "\n";
	//std::cout << pivot(m) << " -> " << m << "\n";
	std::cout << luDecomposition(m) << "\n";
	std::cout << det(m) << "\n";
	
}
