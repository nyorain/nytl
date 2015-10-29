#include <nytl/nytl.hpp>

#include <string>
#include <iostream>

using namespace nytl;

int main()
{
    vec3f vv(1, 2, 3);
    vec4d va(3, 6, 4, 8);

    std::cout << lessThan(vv, va) << std::endl;

    rect2f a({100, 100}, {100, 100});
    rect2f b({150, 150}, {100, 100});
    line2f c({0, 0}, {100, 100});

    triangle3f tri;
    tri.a = {0, 0, 0};
    tri.b = {100, 0, 100};
    tri.c = {0, 100, 100};

    //auto vec = symmetricDifference(a, b);
    //std::cout << dumpContainer(vec) << std::endl;
    //std::cout << a.topLeft() << " " << b.top() << " " << a.left() << " ";

    std::cout << degrees(tri.angleA()) << "\n";
    std::cout << degrees(tri.angleB()) << "\n";
    std::cout << degrees(tri.angleC()) << "\n";
}
