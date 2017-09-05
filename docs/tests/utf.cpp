#include "test.hpp"
#include <nytl/utf.hpp>
#include <cstring>

std::string utf8a = u8"äöüßabêéè"; // some multi-char utf8 string
std::string utf8b = u8"百川生犬虫"; // some random asian chars

TEST(conversion) {
	EXPECT(nytl::charCount(utf8a), 9u);
	EXPECT(nytl::toUtf16(utf8a), u"äöüßabêéè");
	EXPECT(nytl::toUtf32(utf8a), U"äöüßabêéè");
	EXPECT(nytl::toUtf8(nytl::toUtf16(utf8a)), u8"äöüßabêéè");
	EXPECT(nytl::toUtf8(nytl::toUtf32(utf8a)), utf8a);
}

TEST(asian) {
	EXPECT(nytl::charCount(utf8b), 5u);
	EXPECT(nytl::toUtf16(utf8b), u"百川生犬虫");
	EXPECT(nytl::toUtf32(utf8b), U"百川生犬虫");
	EXPECT(nytl::toUtf8(nytl::toUtf32(utf8b)), utf8b);
	EXPECT(std::string(nytl::nth(utf8b, 0).data()), u8"百");
	EXPECT(std::string(nytl::nth(utf8b, 1).data()), u8"川");
	ERROR(std::string(nytl::nth(utf8b, 5).data()), std::out_of_range);

	// change a char
	std::uint8_t size;
	auto& c2 = nytl::nth(utf8b, 2u, size);
	auto sub = u8"気"; // expect it to have the same byte size
	std::memcpy(&c2, sub, size);

	EXPECT(nytl::charCount(utf8b), 5u);
	EXPECT(std::string(nytl::nth(utf8b, 2u).data()), sub);
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
