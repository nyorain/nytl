#include "test.hpp"
#include <nytl/approx.hpp>
#include <nytl/approxVec.hpp>
#include <nytl/vec.hpp>
#include <nytl/dynVec.hpp>
#include <nytl/vecOps.hpp>

using DynVecf = nytl::Vec<nytl::dynamicSize, float>;

TEST(resize) {
	DynVecf vec;
	ERROR(vec.at(0), std::out_of_range);
	vec.resize(10);
	vec.at(5) = 2.f;
	EXPECT(vec.at(5), 2.f);
	EXPECT(vec[5], 2.f);
}

TEST(ops) {
	auto vec = DynVecf::create(1);
	vec[0] = -10.f;
	EXPECT(nytl::vec::length(vec), nytl::approx(10.f));
	EXPECT(nytl::vec::dot(vec, vec), nytl::approx(100.f));
	EXPECT(vec, vec);
	EXPECT(vec == vec, true);
	EXPECT(vec != vec, false);

	vec.resize(2, 0.f);
	EXPECT(vec, nytl::approx(nytl::Vec2f{-10.f, 0.f}));

	vec = -2 * (vec + vec - nytl::Vec2f{-20.f, 5.f});
	EXPECT(vec, nytl::approx(nytl::Vec2f{0.f, 10.f}));
}
