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

TEST_METHOD("[span]") {
	int count {};

	std::array<std::string, 3> namesArray {{"foo", "bar", "baz"}};
	foo(namesArray, count);

	EXPECT(namesArray.front(), "first name");
	EXPECT(namesArray.back(), "last name");
	EXPECT(count, 3 * 3 + 1 + 2);

	bar(namesArray);
	EXPECT_ERROR(baz(namesArray), std::exception);

	std::vector<std::string> namesVector {"foo", "bar", "baz", "abz", "bla"};

	count = 0;
	foo(namesVector, count);
	EXPECT(namesVector.front(), "first name");
	EXPECT(namesVector.back(), "last name");
	EXPECT(count, 5 * 3 + 3 + 2);

	baz(namesVector);
	EXPECT_ERROR(bar(namesVector), std::exception);

	bar({namesVector.data(), 3});

	count = 0;
	foo({*namesVector.data(), 4}, count);
	foo({namesVector.data(), namesVector.size()}, count);
}
