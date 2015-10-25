#include <nyutil/nyutil.hpp>

#include <string>
#include <iostream>

using namespace nyutil;



class myClass : public multiCache<unsigned int>
{
public:
    int a;

    using multiCache<unsigned int>::invalidateCache;
};

float getResult(const myClass& mc)
{
    class myCache : public cacheBase<myCache>
    {
    public:
        float result;
    };

    if(mc.getCache(0))
    {
        std::cout << "found " << ((myCache*)(mc.getCache(0)))->result << "\n";
        return ((myCache*)(mc.getCache(0)))->result;
    }

    myCache result;
    result.result = mc.a * 22 / 233.3f;
    mc.storeCache(0, make_unique<myCache>(result));

    std::cout << "computed " << result.result << "\n";

    return result.result;
}

int main()
{
    myClass c;;
    c.a = 10;

    myClass d;
    d.a = 7;

    getResult(c);
    getResult(c);

    getResult(d);
    getResult(d);
    d.invalidateCache();

    getResult(d);
    getResult(d);

    seq_print<make_integer_sequence<int, 1300>>::print(std::cout) << "\n";
}
