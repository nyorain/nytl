#include "test.hpp"
#include <nytl/rect.hpp>
#include <nytl/rectOps.hpp>
#include <string>

// TODO: many more tests needed

TEST(deduction) {
	auto a = nytl::Rect {nytl::Vec{1, 2, 3}, nytl::Vec{4, 5, 6}};
	static_assert(std::is_same_v<decltype(a), nytl::Rect3i>);

	auto b = nytl::Rect {nytl::Vec{1.f, 2.f}, nytl::Vec{1.f, 1.f}};
	static_assert(std::is_same_v<decltype(b), nytl::Rect2f>);

	auto c = nytl::Rect {nytl::Vec{1., 2.}, nytl::Vec{1., 1.}};
	static_assert(std::is_same_v<decltype(c), nytl::Rect2d>);
}

// The interface of difference(rect, rect) does not say anything about
// the order of the returend rectangles. Therefore we have to test all
// valid orders of difference rects. We also don't assume the method
// of splitting the rectangles. We therefore test for a
// 2 dimensional intersection all 4 valid solutions.

// -----------
// |    A    |
// |    -----|------
// |    |    |     |
// -----|-----     |
//      |    B     |
//      ------------
TEST(rect) {
	nytl::Rect2i a {{0, 0}, {100, 100}};
	nytl::Rect2i b {{50, 50}, {100, 100}};

	auto is = nytl::intersection(a, b);
	EXPECT(is.position, (nytl::Vec2i{50, 50}));
	EXPECT(is.size, (nytl::Vec2i{50, 50}));

	// a - b
	auto diffab = nytl::difference(a, b);
	EXPECT(diffab.size(), 2u);

	if(diffab.size() == 2u) {
		nytl::Rect2i da1 {{0, 0}, {50, 100}};
		nytl::Rect2i da2 {{50, 0}, {50, 50}};

		nytl::Rect2i db1 {{0, 0}, {100, 50}};
		nytl::Rect2i db2 {{0, 50}, {50, 50}};

		bool valid = ((diffab[0] == da1) && (diffab[1] == da2));
		valid |= ((diffab[0] == db1) && (diffab[1] == db2));
		valid |= ((diffab[1] == da1) && (diffab[0] == da2));
		valid |= ((diffab[1] == db1) && (diffab[0] == db2));

		EXPECT(valid, true);
	}

	// b - a
	auto diffba = nytl::difference(b, a);
	EXPECT(diffba.size(), 2u);

	if(diffba.size() == 2u) {
		// nytl::print(std::cout, diffba[0]);
		// nytl::print(std::cout, diffba[1]);

		nytl::Rect2i da1 {{50, 100}, {100, 50}};
		nytl::Rect2i da2 {{100, 50}, {50, 50}};

		nytl::Rect2i db1 {{100, 50}, {50, 100}};
		nytl::Rect2i db2 {{50, 100}, {50, 50}};

		bool valid = ((diffba[0] == da1) && (diffba[1] == da2));
		valid |= ((diffba[0] == db1) && (diffba[1] == db2));
		valid |= ((diffba[1] == da1) && (diffba[0] == da2));
		valid |= ((diffba[1] == db1) && (diffba[0] == db2));

		EXPECT(valid, true);
	}
}
