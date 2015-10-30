#include <string>
#include <iostream>

#include <nytl/nytl.hpp>

using namespace nytl;

int main()
{

    region2f r1 {};
    region2f r2 {};

    r1.add({0, 0, 100, 100});
    r2.add({50, 50, 100, 100});

    r1 = r1 | r2;
    for(auto& rr : r1.getRects())
        std::cout << rr << "\n";

    std::cout << r1.getRects().size() << "\n";
    std::cout << r2.getRects().size() << "\n\n";

    std::cout << contains(r1, vec2f(20, 20)) << "\n";
    std::cout << contains(r1, vec2f(70, 70)) << "\n";
    std::cout << contains(r1, vec2f(100, 100)) << "\n";
    std::cout << contains(r1, vec2f(1, 1)) << "\n";
    std::cout << contains(r1, vec2f(145, 145)) << "\n";
/*
    rect2f r1 {0,0,100,100};
    rect2f r2 {50,50,100,100};

    std::cout << dumpContainer(difference(r1, r2)) << "\n";
*/

    //auto v3 = vec2f(100, 100) - vec2f(300, 300);
}
