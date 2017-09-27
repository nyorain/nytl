#include "test.hpp"
#include <nytl/callback.hpp>
#include <nytl/tmpUtil.hpp>


// TODO: more testing with custom id type and stuff
// - e.g. throw in constructor

// basic connection and callback functionality
TEST(basic) {
	nytl::Callback<void()> cb;
	auto called = 0u;
	cb();
	EXPECT(called, 0u);

	// add, operator +=
	called = 0u;
	auto inc = [&]{ ++called; };
	cb += inc;
	cb.add(inc);
	cb();
	EXPECT(called, 2u);

	// operator =
	called = 0u;
	cb = inc;
	cb();
	EXPECT(called, 1u);

	// clear
	called = 0u;
	cb.clear();
	cb();
	EXPECT(called, 0u);

	// disconnect
	called = 0u;
	auto conn1 = cb.add(inc);
	cb();
	EXPECT(called, 1u);
	conn1.disconnect();
	cb();
	EXPECT(called, 1u);

	// errors
	cb.clear();
	ERROR(cb.add(std::function<void()>{}), std::invalid_argument);
	ERROR(cb = std::function<void()>{}, std::invalid_argument);
	ERROR(cb += std::function<void()>{}, std::invalid_argument);
}

TEST(exception) {
	// check arguments get passed
	nytl::Callback<void(int)> cb;
	cb.add([&](auto i){ EXPECT(i, 42); });
	cb(42);

	// check exception is propagated
	cb = [&](auto i){ if(i < 5) throw 42; };
	cb(7);
	ERROR(cb(3), int);

	// check no more handlers after exception is called
	auto called = 0u;
	cb.add([&](int){ ++called; });
	cb(42);
	EXPECT(called, 1u);
	ERROR(cb(2), int);
	EXPECT(called, 1u);

	// check callback still works as expected
	cb(69);
	EXPECT(called, 2u);
}

TEST(retval) {
	// basic check that return value is correct
	nytl::Callback<int()> cb;
	cb.add([&]{ return 1; });
	cb.add([&]{ return 2; });
	auto vec = cb();

	EXPECT(vec.size(), 2u);
	EXPECT(vec[0], 1);
	EXPECT(vec[1], 2);
}