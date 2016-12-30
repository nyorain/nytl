#include "test.hpp"

#include <nytl/callback.hpp>
#include <nytl/span.hpp>
#include <nytl/observe.hpp>
#include <nytl/typemap.hpp>
#include <nytl/utf.hpp>
#include <nytl/referenced.hpp>

// TODO: clone, connection, convert, flags, stringParam, tuple (operations)

// - callback -
void callback()
{
	{
		nytl::Callback<void()> a;

		int called {};
		auto inc = [&]{ ++called; };
		a += inc;
		a();

		CHECK_EXPECT(called, 1);
		called = 0;

		a = inc;
		a += inc;
		auto conn1 = a.add(inc);
		a.add([&](nytl::Connection conn){ ++called; conn.disconnect(); });

		a();
		CHECK_EXPECT(called, 4);
		called = 0;

		conn1.disconnect();
		a();
		CHECK_EXPECT(called, 2);
		called = 0;

		a = inc;
		a();
		CHECK_EXPECT(called, 1);
	}
}

// - span -
void foo(nytl::Span<std::string> names, int& count)
{
	for(auto& name : names) count += name.size();

	if(!names.empty()) {
		names.front() = "first name";
		names.back() = "last name";
	}

	if(names.size() <= 2) return;

	for(auto& name : names.slice(2, names.size() - 2)) ++count, nytl::unused(name);
	for(auto& name : names.slice<2>(0)) ++count, nytl::unused(name);
}

void bar(nytl::Span<const std::string, 3>) {}
void baz(nytl::Span<const std::string, 5>) {}

void span()
{
	int count {};

	std::array<std::string, 3> namesArray {"foo", "bar", "baz"};
	foo(namesArray, count);

	CHECK_EXPECT(namesArray.front(), "first name");
	CHECK_EXPECT(namesArray.back(), "last name");
	CHECK_EXPECT(count, 3 * 3 + 1 + 2);

	bar(namesArray);
	CHECK_ERROR(baz(namesArray), std::exception);

	std::vector<std::string> namesVector {"foo", "bar", "baz", "abz", "bla"};

	count = 0;
	foo(namesVector, count);
	CHECK_EXPECT(namesVector.front(), "first name");
	CHECK_EXPECT(namesVector.back(), "last name");
	CHECK_EXPECT(count, 5 * 3 + 3 + 2);

	baz(namesVector);
	CHECK_ERROR(bar(namesVector), std::exception);

	bar({namesVector.data(), 3});

	count = 0;
	foo({*namesVector.data(), 4}, count);
}

// - observe -
struct SomeClass {};
struct MyObserver : public nytl::Observer<SomeClass> {
	bool called {};
	void observeDestruction(SomeClass&) override { called = true; }
};

using ObsClass = nytl::ObservableWrapper<SomeClass>;

void observe()
{
	{
		auto observer = MyObserver {};
		auto object = new ObsClass {};
		auto ptr = nytl::ObservingPtr<ObsClass>(object);
		CHECK_EXPECT((ptr), true);

		object->addObserver(observer);
		delete object; // will trigger observer.observeDestruction(*object)

		CHECK_EXPECT(observer.called, true);
		CHECK_EXPECT((ptr), false);
	}
}

// - typemap -
void typemap()
{
	{
		nytl::Typemap<std::string> typemap;

		nytl::add<int>(typemap, "int");
		nytl::add<float>(typemap, "float");
		nytl::add<std::string>(typemap, "std::string");

		auto iany = typemap.create("int");
		auto& i = (std::any_cast<int&>(iany) = 7);
		CHECK_EXPECT(i, 7);

		auto sany = typemap.create("std::string");
		auto& s = (std::any_cast<std::string&>(sany) = "ayy");
		CHECK_EXPECT(s, "ayy");

		auto dany = typemap.create("double");
		CHECK_EXPECT(dany.has_value(), false);
		CHECK_ERROR(typemap.id(typeid(double)), std::exception);

		bool found {};
		found = nytl::remove<int>(typemap);
		CHECK_EXPECT(found, true);

		found = typemap.remove("int");
		CHECK_EXPECT(found, false);

		found = typemap.remove("float");
		CHECK_EXPECT(found, true);
		CHECK_ERROR(typemap.id(typeid(int)), std::exception);
		CHECK_EXPECT(typemap.typeInfo("float"), typeid(void));
		CHECK_EXPECT(typemap.typeInfo("std::string"), typeid(std::string));
		CHECK_EXPECT(typemap.exists("void"), false);
	}
}

//  - utf -
void utf()
{
	std::string utf8 = u8"äöüßabêéè"; // some multi-char utf8 string

	CHECK_EXPECT(nytl::charCount(utf8), 9);
	CHECK_EXPECT(std::string(nytl::nth(utf8, 0).data()), std::string(u8"ä"));
	CHECK_EXPECT(nytl::toUtf16(utf8), u"äöüßabêéè");
	CHECK_EXPECT(nytl::toUtf32(utf8), U"äöüßabêéè");
	CHECK_EXPECT(nytl::toUtf8(nytl::toUtf16(utf8)), u8"äöüßabêéè");
	CHECK_EXPECT(nytl::toUtf8(nytl::toUtf32(utf8)), u8"äöüßabêéè");

	std::uint8_t size;
	auto& a = nytl::nth(utf8, 4, size);
	CHECK_EXPECT((unsigned int) size, 1);
	CHECK_EXPECT(std::string(&a, (unsigned int) size), u8"a");
	CHECK_ERROR(nytl::nth(utf8, 10, size), std::out_of_range);
}

// - referenced -
void referenced()
{
	bool deleted {};
	auto deleter = [&](const auto*){ deleted = true; };

	struct MyReferenced : public nytl::Referenced<MyReferenced, decltype(deleter)> {
		using nytl::Referenced<MyReferenced, decltype(deleter)>::Referenced;
	};

	MyReferenced obj(deleter);
	auto ref = nytl::IntrusivePtr<MyReferenced>(obj);
	CHECK_EXPECT(ref->referenceCount(), 1);

	{
		auto copy = ref;
		copy->ref();
		CHECK_EXPECT(ref->referenceCount(), 3);
		copy->unrefNodelete();
		CHECK_EXPECT(ref->referenceCount(), 2);
	}

	auto copy = ref;
	copy.reset();

	CHECK_EXPECT(ref->referenceCount(), 1);

	auto ptr = ref.get();
	ptr->ref();

	ref = {};
	CHECK_EXPECT(ptr->referenceCount(), 1);

	ptr->unref();
	CHECK_EXPECT(ptr->referenceCount(), 0);
	CHECK_EXPECT(deleted, true);
}

int main()
{
	callback();
	span();
	observe();
	typemap();
	utf();
	referenced();

	std::cout << (failed ? std::to_string(failed) : "no") << " tests failed!\n";
}
