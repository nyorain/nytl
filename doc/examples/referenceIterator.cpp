#include <nytl/referenceIteration.hpp>
#include <vector>

int main()
{
	int a = 7;
	int b = 14;
	int c = -23;
	std::vector<int*> intvec {&a, &b, &c};

	for(auto& i : nytl::makeReferenceIteration(intvec))
	{
		std::cout << i;	
		++i;
	}

	for(auto it = nytl::makeReferenceIterator(intvec.begin()); it != intvec.end(); ++it)
	{
		std::cout << i;
	}
}
