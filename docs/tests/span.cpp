#include "test.hpp"
#include <nytl/span.hpp>
#include <nytl/tmpUtil.hpp>
#include <string>

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

TEST(span) {
	int count {};

	std::array<std::string, 3> namesArray {{"foo", "bar", "baz"}};
	foo(namesArray, count);

	EXPECT(namesArray.front(), "first name");
	EXPECT(namesArray.back(), "last name");
	EXPECT(count, 3 * 3 + 1 + 2);

	bar(namesArray);
	ERROR(baz(namesArray), std::exception);

	std::vector<std::string> namesVector {"foo", "bar", "baz", "abz", "bla"};
	bar({namesVector.data(), 3});

	auto slice = nytl::Span(namesVector).slice(3);
	EXPECT(slice[0], "abz");
	EXPECT(slice[1], "bla");
	ERROR(slice.at(3), std::out_of_range);

	const std::vector<int> cnv {1, 2, 3};
	EXPECT(nytl::Span(cnv)[0], 1);

	auto ded1 = nytl::Span(namesVector.data(), 2);
	EXPECT(ded1[0], "foo");
	EXPECT(ded1.size(), 2u);

	std::array<int, 2> arr {5, 6};
	auto ded2 = nytl::Span(arr);
	EXPECT(ded2.size(), 2u);
	EXPECT(ded2[0], 5);

	auto span4 = nytl::Span<const int, 2>(arr);
	auto ded3 = nytl::Span<const int>(span4);
	EXPECT(ded3.size(), 2u);


	count = 0;
	foo(namesVector, count);
	EXPECT(namesVector.front(), "first name");
	EXPECT(namesVector.back(), "last name");
	EXPECT(count, 5 * 3 + 3 + 2);

	baz(namesVector);
	ERROR(bar(namesVector), std::exception);

	count = 0;
	foo({*namesVector.data(), 4}, count);
	foo({namesVector.data(), namesVector.size()}, count);
}
