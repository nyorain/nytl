#include "test.hpp"
#include <nytl/callback.hpp>
#include <nytl/tmpUtil.hpp>

TEST(callback_1) {
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

TEST(callback_2) {
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

TEST(inter_callback) {
	nytl::Callback<void()> cb;
	int called {};

	nytl::Connection c4;

	auto c1 = cb.add([&]{ ++called; });
	cb.add([&](nytl::Connection conn){ conn.disconnect(); called++; });
	cb.add([&]{ ++called; c1.disconnect(); });
	cb.add([&]{ if(called > 3 && called < 10) { cb(); c4.disconnect(); } });
	c4 = cb.add([&]{ if(called < 5) cb(); });
	cb.add([&](nytl::Connection conn){ ++called; conn.disconnect(); });

	cb();
	EXPECT(called, 11);
}

TEST(clusterfuck) {
	nytl::Callback<void()> cb;
	int called {};

	cb.add([&]{ ++called; if(called < 2) cb(); });
	cb.add([&]{ cb.add([&]{ if(called < 3) cb(); }); });
	cb.add([&]{ cb.add([&](nytl::Connection conn){ conn.disconnect(); if(called < 4) cb(); }); });
	cb.add([&](nytl::Connection conn){ conn.disconnect(); cb(); });
	nytl::Connection conn1 = cb.add([&]{ conn1.disconnect(); cb(); });

	auto conn2 = cb.add([&]{ cb.add([&]{ ++called; }); });
	cb.add([&]{ conn2.disconnect(); });

	cb.add([&](nytl::Connection){ cb.clear(); });
	cb.add([&] { cb.clear(); });

	cb();

	EXPECT(called, 4);
	// EXPECT(cb.size(), 0u);
}

TEST(connection) {
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
