#include <string>
#include <iostream>

#include <nytl/nytl.hpp>

using namespace nytl;

int main()
{
    potential_ptr<int> i = make_unique<int>(7);
    auto ptr = i.get();

    std::cout << i.owned() << std::endl;

    int a = 8;
    i.reset(a);
    *i = 9;

    std::cout << i.owned() << std::endl;
}
