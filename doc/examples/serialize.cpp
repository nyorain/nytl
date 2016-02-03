#include <nytl/serialize.hpp>

class BaseClass : nytl::Serialized<BaseClass, TS_("BaseClass")>
{
};

class DerviedClass : nytl::DeriveSerialized<BaseClass, DerivedClass, TS_("DerivedClass")>
{
};

template<typename T>
class DerivedTC : nytl::DeriveSerialized<BaseClass, DerivedTC<T>, 
	TS_("DerivedTC"), TS_("<"), TS_(typeName<T>()), TS_(">")>
{
};

int main()
{
}
