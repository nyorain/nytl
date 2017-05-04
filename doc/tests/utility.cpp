#include "test.hpp"

#include <nytl/callback.hpp>
#include <nytl/span.hpp>
#include <nytl/utf.hpp>
#include <nytl/referenced.hpp>
#include <nytl/flags.hpp>
#include <nytl/convert.hpp>
#include <nytl/stringParam.hpp>
#include <nytl/functionTraits.hpp>

#include <list>

// TODO: to test: functionTraits, nonCopyable, tuple (operations)

// - referenced -
TEST(referenced) {
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

TEST(flags) {
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
TEST(convert) {
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

TEST(stringParam) {
	stringParamA("test");
	static_assert(stringParamB("lengthIs9\0discarded") == 9, "stringParam test #1");

	EXPECT(stringParamB(std::string("length7")), 7);

	EXPECT(nytl::StringParam("test") == "test", true);
	EXPECT(nytl::StringParam("test") == nytl::SizedStringParam("test"), true);
	EXPECT(nytl::StringParam("test") == "tes\0t", false);
	EXPECT(nytl::SizedStringParam("ayy") == "ayy\0oo", true);
	EXPECT(nytl::StringParam("nytl") == std::string("nytl"), true);
	EXPECT("nytl" == nytl::SizedStringParam("ny"), false);
}

// - clone -

// - template meta programming -
template<typename T> using Expression1 = decltype(foooo(std::declval<T>()));
template<typename T> using Expression2 = decltype(foo(std::declval<T>()));
template<typename T> using Expression3 = decltype(std::round(std::declval<T>()));

TEST(tmp) {
	static_assert(nytl::validExpression<Expression1, int> == false, "tmp:0");
	static_assert(nytl::validExpression<Expression2, int> == false, "tmp:1");
	static_assert(nytl::validExpression<Expression3, int> == true, "tmp:2");
}
