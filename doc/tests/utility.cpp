#include "test.hpp"

#include <nytl/callback.hpp>
#include <nytl/span.hpp>
#include <nytl/observe.hpp>
#include <nytl/typemap.hpp>
#include <nytl/utf.hpp>
#include <nytl/referenced.hpp>
#include <nytl/flags.hpp>
#include <nytl/convert.hpp>
#include <nytl/stringParam.hpp>
#include <nytl/clone.hpp>
#include <nytl/functionTraits.hpp>

#include <list>

// TODO: to test: functionTraits, nonCopyable, tuple (operations)

// - observe -
struct SomeClass {};
struct MyObserver : public nytl::Observer<SomeClass> {
	bool called {};
	void observeDestruction(SomeClass&) override { called = true; }
};

using ObsClass = nytl::ObservableWrapper<SomeClass>;

TEST_METHOD("[observer]") {
	{
		auto observer = MyObserver {};
		auto object = new ObsClass {};
		auto ptr = nytl::ObservingPtr<ObsClass>(object);
		EXPECT((ptr), true);

		object->addObserver(observer);
		delete object; // will trigger observer.observeDestruction(*object)

		EXPECT(observer.called, true);
		EXPECT((ptr), false);
	}
}

// - typemap -
TEST_METHOD("[typemap]") {
	{
		nytl::Typemap<std::string> typemap;

		nytl::add<int>(typemap, "int");
		nytl::add<float>(typemap, "float");
		nytl::add<std::string>(typemap, "std::string");

		auto iany = typemap.create("int");
		auto& i = (std::any_cast<int&>(iany) = 7);
		EXPECT(i, 7);

		auto sany = typemap.create("std::string");
		auto& s = (std::any_cast<std::string&>(sany) = "ayy");
		EXPECT(s, "ayy");

		auto dany = typemap.create("double");
		EXPECT(dany.has_value(), false);
		EXPECT_ERROR(typemap.id(typeid(double)), std::exception);

		bool found {};
		found = nytl::remove<int>(typemap);
		EXPECT(found, true);

		found = typemap.remove("int");
		EXPECT(found, false);

		found = typemap.remove("float");
		EXPECT(found, true);
		EXPECT_ERROR(typemap.id(typeid(int)), std::exception);
		EXPECT(typemap.typeInfo("float"), typeid(void));
		EXPECT(typemap.typeInfo("std::string"), typeid(std::string));
		EXPECT(typemap.exists("void"), false);
	}
}

//  - utf -
TEST_METHOD("[utf]") {
	std::string utf8 = u8"äöüßabêéè"; // some multi-char utf8 string

	EXPECT(nytl::charCount(utf8), 9u);
	EXPECT(std::string(nytl::nth(utf8, 0).data()), std::string(u8"ä"));
	EXPECT(nytl::toUtf16(utf8), u"äöüßabêéè");
	EXPECT(nytl::toUtf32(utf8), U"äöüßabêéè");
	EXPECT(nytl::toUtf8(nytl::toUtf16(utf8)), u8"äöüßabêéè");
	EXPECT(nytl::toUtf8(nytl::toUtf32(utf8)), u8"äöüßabêéè");

	std::uint8_t size;
	auto& a = nytl::nth(utf8, 4, size);
	EXPECT(size, 1u);
	EXPECT(std::string(&a, (unsigned int) size), u8"a");
	EXPECT_ERROR(nytl::nth(utf8, 10, size), std::out_of_range);
}

// - referenced -
TEST_METHOD("[referenced]") {
	bool deleted {};
	auto del = [&](const auto*){ deleted = true; };

	struct MyReferenced : public nytl::Referenced<MyReferenced, decltype(del)> {
		using nytl::Referenced<MyReferenced, decltype(del)>::Referenced;
	};

	MyReferenced obj(del);
	auto ref = nytl::IntrusivePtr<MyReferenced>(obj);
	EXPECT(ref->referenceCount(), 1u);

	{
		auto copy = ref;
		copy->ref();
		EXPECT(ref->referenceCount(), 3u);
		copy->unrefNodelete();
		EXPECT(ref->referenceCount(), 2u);
	}

	auto copy = ref;
	copy.reset();

	EXPECT(ref->referenceCount(), 1u);

	auto ptr = ref.get();
	ptr->ref();

	ref = {};
	EXPECT(ptr->referenceCount(), 1u);

	ptr->unref();
	EXPECT(ptr->referenceCount(), 0u);
	EXPECT(deleted, true);
}

// - flags -
enum class Enum {
	entry0 = 0,
	entry1 = 1,
	entry2 = 2,
	entry3 = 4
};

NYTL_FLAG_OPS(Enum)

TEST_METHOD("[flags]") {
	constexpr auto entry23 = Enum::entry2 | Enum::entry3;
	static_assert(entry23.value() == 6, "flags test #1");

	constexpr auto entry3 = entry23 & Enum::entry3;
	static_assert(entry3.value() == 4, "flags test #2");

	constexpr auto entryNot3 = ~Enum::entry3;
	static_assert((entryNot3 & Enum::entry0) == Enum::entry0, "flags test #3");
	static_assert((entryNot3 & Enum::entry1) == Enum::entry1, "flags test #4");
	static_assert((entryNot3 & Enum::entry2) == Enum::entry2, "flags test #5");
	static_assert((entryNot3) == true, "flags test #6");
}

// - convert -
TEST_METHOD("[convert]") {
	auto convertedFloat = nytl::convert<float>(7);
	EXPECT(convertedFloat, 7.f);

	int convertedInt = nytl::convert(23.0);
	EXPECT(convertedInt, 23);

	// TODO: c++17 convert update
	// auto floatArray = std::array<float, 5> {{1.f, 2.f, 3.f, 4.f, 5.f}};
	//
	// std::array<int, 5> convertedIntArray = nytl::convert(floatArray);
	// EXPECT(convertedIntArray[2], 3);
	//
	// auto convertedCharArray = nytl::convert<std::array<char, 5>>(floatArray);
	// EXPECT(convertedCharArray[3], 4);
	//
	// auto convertedDoubleArray = nytl::arrayCast<double>(floatArray);
	// EXPECT(convertedDoubleArray[0], 1.0);
	//
	// auto intVector = nytl::containerCast<std::vector<int>>(floatArray);
	// auto doubleList = nytl::containerCast<std::list<double>>(intVector);
	// EXPECT(doubleList.back(), 5.0);
}

// - stringParam -
constexpr void stringParamA(nytl::StringParam) {}
constexpr int stringParamB(nytl::SizedStringParam param) { return param.size(); }

TEST_METHOD("[stringParam]") {
	stringParamA("test");
	static_assert(stringParamB("lengthIs9") == 9, "stringParam test #1");
	EXPECT(stringParamB(std::string("length7")), 7);
}

// - clone -
struct CloneBase : public nytl::AbstractCloneable<CloneBase> {
	virtual int value() const = 0;
};

struct CloneDerived : public nytl::DeriveCloneable<CloneBase, CloneDerived> {
	int value_;
	int value() const override { return value_; }
};

struct CloneBase2 : public nytl::Cloneable<CloneBase2> {};
struct CloneDerived2 : public nytl::DeriveCloneable<CloneBase2, CloneDerived2> {};
struct CloneDerived3 : public nytl::DeriveCloneable<CloneDerived2, CloneDerived3> {};

TEST_METHOD("[clone]") {
	auto derived = CloneDerived {};
	derived.value_ = 42;

	auto ptr = static_cast<CloneBase*>(&derived);

	auto copy = nytl::clone(*ptr);
	auto moved = nytl::cloneMove(*ptr);

	EXPECT(copy->value(), 42);
	EXPECT(moved->value(), 42);
}


// - template meta programming -
template<typename T> using Expression1 = decltype(foooo(std::declval<T>()));
template<typename T> using Expression2 = decltype(foo(std::declval<T>()));
template<typename T> using Expression3 = decltype(std::round(std::declval<T>()));

TEST_METHOD("[tmp]") {
	static_assert(nytl::validExpression<Expression1, int> == false, "tmp:0");
	static_assert(nytl::validExpression<Expression2, int> == false, "tmp:1");
	static_assert(nytl::validExpression<Expression3, int> == true, "tmp:2");
}
