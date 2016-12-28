#include "test.hpp"

#include <nytl/callback.hpp>
#include <nytl/span.hpp>
#include <nytl/observe.hpp>

// TODO: referenced, clone, connection, convert, flags, stringParam, tuple, utf

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
	bool thrown {};

	std::array<std::string, 3> namesArray {"foo", "bar", "baz"};
	foo(namesArray, count);

	CHECK_EXPECT(namesArray.front(), "first name");
	CHECK_EXPECT(namesArray.back(), "last name");
	CHECK_EXPECT(count, 3 * 3 + 1 + 2);

	bar(namesArray);
	try { baz(namesArray); }
	catch(const std::exception&) { thrown = true; }
	CHECK_EXPECT(thrown, true);

	std::vector<std::string> namesVector {"foo", "bar", "baz", "abz", "bla"};

	count = 0;
	foo(namesVector, count);
	CHECK_EXPECT(namesVector.front(), "first name");
	CHECK_EXPECT(namesVector.back(), "last name");
	CHECK_EXPECT(count, 5 * 3 + 3 + 2);

	baz(namesVector);

	thrown = false;
	try { bar(namesVector); }
	catch(const std::exception&) { thrown = true; }
	CHECK_EXPECT(thrown, true);

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
	auto observer = MyObserver {};
	auto object = new ObsClass {};
	object->addObserver(observer);
	delete object; // will trigger observer.observeDestruction(*object)

	CHECK_EXPECT(observer.called, true);
}

int main()
{
	callback();
	span();
	observe();

	std::cout << (failed ? std::to_string(failed) : "no") << " tests failed!\n";
}
