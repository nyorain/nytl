#include "test.hpp"
#include <nytl/callback.hpp>
#include <nytl/tmpUtil.hpp>
#include <nytl/observe.hpp>


TEST_METHOD("[callback-1]") {
	nytl::Callback<void()> a;

	int called {};
	auto inc = [&]{ ++called; };
	a += inc;
	a();

	EXPECT(called, 1);
	called = 0;

	a = inc;
	a += inc;
	auto conn1 = a.add(inc);
	a.add([&](nytl::Connection conn){ ++called; conn.disconnect(); });

	a();
	EXPECT(called, 4);
	called = 0;

	conn1.disconnect();
	a();
	EXPECT(called, 2);
	called = 0;

	a = inc;
	a();
	EXPECT(called, 1);
}

TEST_METHOD("[callback-2]") {
	nytl::Callback<unsigned int()> cb;
	cb.add([]{ return 1; });
	cb.add([]{ return 2; });
	cb.add([]{ return 3; });
	cb.add([]{ return 4; });
	cb.add([]{ return 5; });

	auto ret = cb();
	EXPECT(ret.size(), 5u);

	for(auto i = 0u; i < ret.size(); ++i)
		EXPECT(ret[i], i + 1u);
}

TEST_METHOD("[callback-3]") {
	nytl::TrackedConnectionID id;

	{
		nytl::TrackedCallback<void()> cb;
		auto conn = cb.add([]{});
		EXPECT(conn.connected(), true);
		cb.clear();
		EXPECT(conn.connected(), false);

		id = cb.add([]{}).id();
		EXPECT(id.valid(), true);
	}

	EXPECT(id.valid(), false);
}

TEST_METHOD("[connection]") {
	nytl::Callback<void()> cb;

	auto called = 0u;
	nytl::Connection conn1 = cb.add([&]{ ++called; });
	cb();
	EXPECT(called, 1u);

	{
		auto conn2 = nytl::UniqueConnection(cb.add([&]{ ++called; }));
		auto conn3 = cb.add([&]{ ++called; });
		nytl::unused(conn3);

		cb();
		EXPECT(called, 4u);
	}

	cb();
	EXPECT(called, 6u);

	conn1.disconnect();

	cb();
	EXPECT(called, 7u);
}
