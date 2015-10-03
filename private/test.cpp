#include <nyutil/vec.hpp>
#include <nyutil/transformable.hpp>
#include <string>
#include <iostream>

using namespace nyutil;

int main()
{
    vec3f u(2, 2, 1);
    vec3f v(-1, -1, 1);

    std::cout << "scalar: " << weight(u * v) << "\n";
    std::cout << "abs u: " << abs(u) << "\n";
    std::cout << "abs v: " << abs(v) << "\n";
    std::cout << "Y = " << angle(u, v) << "\n";
}
