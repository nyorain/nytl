#include "bugged.hpp"
#include <nytl/stringParam.hpp>

constexpr void stringParamA(nytl::StringParam) {}
constexpr int stringParamB(nytl::SizedStringParam param) { return param.size(); }

TEST(basic) {
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
