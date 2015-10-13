#include <nyutil/vec.hpp>
#include <nyutil/transformable.hpp>
#include <nyutil/system.hpp>
#include <nyutil/cache.hpp>
#include <nyutil/triangle.hpp>
#include <nyutil/region.hpp>

#include <string>
#include <iostream>

using namespace nyutil;

int main()
{
    vec3f u(1, 1, 0);
    vec3f v(1, 0, 1);

    line3f l1(u, v);
    line2f l2 = l1;

    std::cout << length(l1) << "\n";
    std::cout << length(l2) << "\n";

    auto angl1 = cangle(u, v);
    auto angl2 = cangle(v, u);

    std::cout << "angle from u to v: " << angl1 / cDeg << "\n";
    std::cout << "angle from v to u: " << angl2 / cDeg << "\n";

    //std::cout << "abs u: " << abs(u) << "\n";
    //std::cout << "abs v: " << abs(v) << "\n";
    //std::cout << "Y = " << angle(u, v) / cDeg << "\n";
}
