#include "bugged.hpp"
#include <nytl/typemap.hpp>

TEST(basic) {
	nytl::Typemap<std::string> typemap;

	nytl::add<int>(typemap, "int");
	nytl::add<float>(typemap, "float");
	nytl::add<std::string>(typemap, "std::string");

	// try for registered values
	auto iany = typemap.create("int");
	EXPECT(iany.has_value(), true);
	EXPECT(std::any_cast<int>(&iany) == nullptr, false);
	EXPECT(std::any_cast<unsigned int>(&iany) == nullptr, true);
	EXPECT(typemap.id(typeid(int)) == nullptr, false);
	EXPECT(*typemap.id(typeid(int)), "int");

	auto sany = typemap.create("std::string");
	EXPECT(sany.has_value(), true);
	EXPECT(std::any_cast<std::string>(&sany) == nullptr, false);
	EXPECT(std::any_cast<int>(&sany) == nullptr, true);

	// double, long were not registered
	auto dany = typemap.create("double");
	EXPECT(dany.has_value(), false);
	EXPECT(typemap.id(typeid(long)), nullptr);
	EXPECT(typemap.remove("double"), false);

	EXPECT(nytl::remove<int>(typemap), true);
	EXPECT(typemap.remove("int"), false);

	EXPECT(typemap.id(typeid(int)), nullptr);
	EXPECT(typemap.typeInfo("float"), typeid(float));
	EXPECT(typemap.typeInfo("double"), typeid(void));
	EXPECT(typemap.typeInfo("int"), typeid(void));
	EXPECT(typemap.typeInfo("std::string"), typeid(std::string));
	EXPECT(typemap.exists("void"), false);
	EXPECT(typemap.remove("float"), true);
	EXPECT(typemap.remove("float"), false);
	EXPECT(typemap.create("int").has_value(), false);
}

struct Base { virtual ~Base() = default; };
struct Derived1 : public Base {};
struct Derived2 : public Base {};
struct Derived3 : public Derived2 {};

TEST(inheritance) {
	nytl::Typemap<std::string, Base> typemap;
	nytl::add<Base>(typemap, "Base");
	nytl::add<Derived1>(typemap, "Derived1");
	nytl::add<Derived2>(typemap, "Derived2");
	nytl::add<Derived3>(typemap, "Derived3");

	auto base = typemap.create("Base");
	EXPECT(base.get() == nullptr, false);
	EXPECT(dynamic_cast<Derived1*>(base.get()) == nullptr, true);
	EXPECT(dynamic_cast<Derived2*>(base.get()) == nullptr, true);

	auto derived1 = typemap.create("Derived1");
	EXPECT(derived1.get() == nullptr, false);
	EXPECT(dynamic_cast<Derived1*>(derived1.get()) == nullptr, false);
	EXPECT(dynamic_cast<Derived3*>(derived1.get()) == nullptr, true);

	auto empty = typemap.create("int");
	EXPECT(empty.get() == nullptr, true);
	EXPECT(typemap.id(typeid(int)), nullptr);
	EXPECT(typemap.id(typeid(Base)) == nullptr, false);

	EXPECT(*typemap.id(typeid(Base)), "Base");
	EXPECT(typemap.exists("Derived3"), true);
	EXPECT(typemap.exists("Derived4"), false);
	EXPECT(typemap.template exists<Derived2>(), true);
	EXPECT(typemap.template exists<std::string>(), false);
	EXPECT(*typemap.template id<Derived3>(), "Derived3");

	// Add a second time
	nytl::add<Derived3>(typemap, "Derived3:2");
	auto derived3 = typemap.create("Derived3:2");
	EXPECT(derived3.get() == nullptr, false);
	EXPECT(dynamic_cast<Derived3*>(derived3.get()) == nullptr, false);

	// Override
	nytl::add<Derived2>(typemap, "Derived3:2");
	auto derived2 = typemap.create("Derived3:2");
	EXPECT(derived2.get() == nullptr, false);
	EXPECT(dynamic_cast<Derived3*>(derived2.get()) == nullptr, true);
	EXPECT(dynamic_cast<Derived2*>(derived2.get()) == nullptr, false);
}

TEST(intID) {
	nytl::Typemap<unsigned int, std::any, double> typemap;
	nytl::add<std::int32_t>(typemap, 320);
	nytl::add<std::int64_t>(typemap, 640);
	nytl::add<std::uint32_t>(typemap, 321);
	nytl::add<std::uint64_t>(typemap, 641);
	nytl::add<float>(typemap, 322);
	nytl::add<double>(typemap, 642);

	auto int32 = typemap.create(320, 5.0);
	EXPECT(int32.has_value(), true);
	EXPECT(std::any_cast<std::int32_t>(&int32) == nullptr, false);
	EXPECT(std::any_cast<std::uint32_t>(&int32) == nullptr, true);
	EXPECT(std::any_cast<std::int32_t>(int32), 5);

	auto doubl = typemap.create(642, 0.42);
	EXPECT(doubl.has_value(), true);
	EXPECT(std::any_cast<std::int32_t>(&doubl) == nullptr, true);
	EXPECT(std::any_cast<double>(&doubl) == nullptr, false);
	EXPECT(std::any_cast<double>(doubl), 0.42);
}
