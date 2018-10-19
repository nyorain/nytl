#include "test.hpp"
#include <nytl/span.hpp>
#include <nytl/tmpUtil.hpp>
#include <string>
#include <array>

void foo(nytl::Span<std::string> names, int& count) {
	for(auto& name : names) count += name.size();

	if(!names.empty()) {
		*names.begin() = "first name";
		*names.rbegin() = "last name";
	}

	if(names.size() <= 2) return;

	for(auto& name : names.subspan(2)) ++count, nytl::unused(name);
	for(auto& name : names.subspan<0, 2>()) ++count, nytl::unused(name);
}

void bar(nytl::Span<const std::string, 3>) {}
void baz(nytl::Span<const std::string, 5>) {}

TEST(span) {
	int count {};

	std::array<std::string, 3> namesArray {{"foo", "bar", "baz"}};
	foo(namesArray, count);

	EXPECT(namesArray.front(), "first name");
	EXPECT(namesArray.back(), "last name");
	EXPECT(count, 3 * 3 + 1 + 2);

	bar(namesArray);
	// baz(namesArray) // NOTE: should trigger compile time error

	std::vector<std::string> namesVector {"foo", "bar", "baz", "abz", "bla"};
	bar({namesVector.data(), 3});

	auto slice = nytl::span(namesVector).subspan(3);
	EXPECT(slice[0], "abz");
	EXPECT(slice[1], "bla");

	const std::vector<int> cnv {1, 2, 3};
	EXPECT(nytl::span(cnv)[0], 1);

	auto ded1 = nytl::span(namesVector.data(), 2);
	EXPECT(ded1[0], "foo");
	EXPECT(ded1.size(), 2u);

	std::array<int, 2> arr {5, 6};
	auto ded2 = nytl::span(arr);
	EXPECT(ded2.size(), 2u);
	EXPECT(ded2[0], 5);

	auto span4 = nytl::Span<const int, 2>(arr);
	auto ded3 = nytl::span<const int>(span4);
	EXPECT(ded3.size(), 2u);


	count = 0;
	foo(namesVector, count);
	EXPECT(namesVector.front(), "first name");
	EXPECT(namesVector.back(), "last name");
	EXPECT(count, 5 * 3 + 3 + 2);

	baz(namesVector);
	// NOTE: per c++20 this is undefined behavior. So we no longer
	// check it
	// ERROR(bar(namesVector), std::exception);

	count = 0;
	foo({namesVector.data(), 4}, count);
	foo({namesVector.data(), (int) namesVector.size()}, count);
}
