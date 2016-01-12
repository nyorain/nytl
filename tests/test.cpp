#include <nytl/nytl.hpp>
#include <iostream>
#include <fstream>
using namespace nytl;

int main()
{/*
	mat43f m(0.4, 200, 0, 56,
			4, 0.2, 22, 66,
			0, 224, 22, 45);
*/	
	mat34f m(25, 1, 4, 111,
			0, 1, 7, 66,
			0, 1, 7, 66);

	linearEquotationSystem<3, 3, float> les(m);
	auto res = les.solve();

	std::cout << res.solvable() << "\n";
	std::cout << res.numberVariables() << "\n";
	std::cout << res.solution(vec<1, float>{1.f}) << "\n";
	
	for(auto& e : res.solution_)
	{
		std::cout << e.constPart << " + " << e.variablePart << "\n";
	}

/*
	std::cout << m << "\n";
	refMat(m);

	std::cout << m << "\n";
	rrefMat(m);

	std::cout << m << "\n";
*/
}
