#include <nyutil/typemap.hpp>
#include <string>
#include <iostream>

using namespace nyutil;
typemap<const char*> myMap {};
const char cc[] = "myType";

/////
template<typename Identifier, typename Base, typename Derived, typemap<Identifier, Base>& M, Identifier I>
class regBase
{
public:
    const static unsigned int registerID_;
};

template<typename Identifier, typename Base, typename Derived, typemap<Identifier, Base>& M, Identifier I>
const unsigned int regBase<Identifier, Base, Derived, M, I>::registerID_ = registerType<Derived>(myMap, I);

////
class myType : public regBase<const char*, void, myType, myMap, &cc>
{
public:
    const static unsigned int regID_;
};

int main()
{
    //registerType<int>(myMap, "int");
    //int* a = (int*)myMap.createObject("int");
    myType* b = (myType*)myMap.createObject("myType");
   // myType* c = (myType*)myMap.createObject("myType1");

    //*a = 7;
    //std::cout << *a << "\n";
    std::cout << b << "\n";
   // std::cout << c << "\n";
}
