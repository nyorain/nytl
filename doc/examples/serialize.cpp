#include <nytl/serialize.hpp>
#include <cassert>
#include <iostream>

//Serialized made classes
class BaseClass : public nytl::Serialized<BaseClass> {};
class DerivedClass : public nytl::DeriveSerialized<BaseClass, DerivedClass> {};

//DerivedTemplateClass
template<typename T>
class DerivedTC : public nytl::DeriveSerialized<BaseClass, DerivedTC<T>> {};

//To go the safe way and make it working across all platforms one must use a macro
NYTL_GEN_TYPENAME(BaseClass)
NYTL_GEN_TYPENAME(DerivedClass)
NYTL_GEN_TEMPLATE_TYPENAME(DerivedTC)

//It will also work without this macro but only with certain non-standard extensions and not
//safely cross platform (the types will have the same name on EVERY platform).
//So prefer using this macro for all types and templates parameters of types that are serialized.
//Also specify it for int since we use it in the main function:
NYTL_GEN_TYPENAME(int)

//Alternatively, you can also give classes a name on creation by deriving from
//nytl::Named or nytl::DerivedNamed.
class StaticNamed : public nytl::DeriveNamed<BaseClass, TS("StaticNamed")>
{};

int main()
{
	//Create a Serializer (might also be done global)
	//See nytl::Serializer or nytl::Typemap for more information
	nytl::DefaultSerializer serializer;

	//Add some random types to the typemap
	//notice how we dont have to specify their string identifier names here
	//we could also do that, then there would be no need for the macros used above to make
	//it work on all platforms.
	nytl::addType<DerivedClass>(serializer);
	nytl::addType<DerivedTC<int>>(serializer);
	nytl::addType<int>(serializer);

	//dynamically create an object by typeName
	auto object1 = serializer.create("int");
	auto object2 = serializer.create("DerivedTC<int>");

	//check typeName
	assert(nytl::any_cast<DerivedTC<int>>(object2).objectTypeName() == "DerivedTC<int>");
	
	std::cout << object1.empty() << "\n";
	std::cout << object2.empty() << "\n";
	
	//output all registered types (names)
	for(auto& type : serializer.types())
		std::cout << type.first << "\n";

	//load object from stream
	//e.g. by int 43 which will dynamically (!) create a int with the value of 43 and
	//return it type-erased as an nytl::Any object.
	//Note that you can also use common base classes instead of nytl::Any for a nytl::Serializer
	//(or more general speaking a nytl::Typemap).
	if(!serializer.createLoad(std::cin).empty()) std::cout << "succesful\n";
	else std::cout << "failed\n";

	return EXIT_SUCCESS;
}
