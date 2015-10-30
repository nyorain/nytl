#include <nytl/nytl.hpp>
using namespace nytl;

int main()
{
	line2f l1{{0,0}, {100, 100}};
	line2f l2{{100, 100}, {0, 0}};

	std::cout << "hallllooooo\n"; 
	std::cout << intersects(l1, l2) << "\n";	

	return -1;
}
