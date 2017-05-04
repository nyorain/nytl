#include "test.hpp"
#include <nytl/utf.hpp>

// TODO:
// - test for other charsets (that might take more bytes) e.g. asian

std::string utf8a = u8"äöüßabêéè"; // some multi-char utf8 string

TEST(conversion) {
	EXPECT(nytl::charCount(utf8a), 9u);
	EXPECT(nytl::toUtf16(utf8a), u"äöüßabêéè");
	EXPECT(nytl::toUtf32(utf8a), U"äöüßabêéè");
	EXPECT(nytl::toUtf8(nytl::toUtf16(utf8a)), u8"äöüßabêéè");
	EXPECT(nytl::toUtf8(nytl::toUtf32(utf8a)), utf8a);
}

TEST(nth) {
	std::uint8_t size;
	auto& a = nytl::nth(utf8a, 4, size); // retrieve the char at pos 4 (teh 5th) ('a')
	EXPECT(size, 1u);
	EXPECT(std::string(&a, (unsigned int) size), u8"a");

	auto& b = nytl::nth(utf8a, 1, size);
	EXPECT(size, 2);
	EXPECT(std::string(&b, (unsigned int) size), u8"ö");

	ERROR(nytl::nth(utf8a, 10, size), std::out_of_range);
	EXPECT(std::string(nytl::nth(utf8a, 0).data()), std::string(u8"ä"));
}
