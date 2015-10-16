#include <nyutil/integer_sequence.hpp>
#include <nyutil/compFunc.hpp>

#include <string>
#include <iostream>

using namespace nyutil;

template<typename T> struct print_seq;
template<size_t... idx> struct print_seq<index_sequence<idx...>>
{
    static void print()
    {
        int dummy[] = {((void) (std::cout << idx << "\n"), 0)...};
    };
};

int main()
{
    print_seq<detail::tupleMapImpl<std::tuple<int, float>, std::tuple<int, float>>::type>::print();
}
