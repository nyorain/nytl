#include "bugged.hpp"
#include <nytl/callback.hpp>
#include <nytl/tmpUtil.hpp>

// basic connection and callback functionality
TEST(basic) {
	nytl::Callback<void()> cb;
	auto called = 0u;

	// #1
	called = 0u;
	auto inc = [&]{ ++called; };
	cb += inc;
	cb.add(inc);
	cb();
	EXPECT(called, 2u);

	// #2
	called = 0u;
	cb = inc;
	cb();
	EXPECT(called, 1u);

	// #3
	called = 0u;
	cb.clear();
	cb();
	EXPECT(called, 0u);

	// #4
	called = 0u;
	cb.add([&](nytl::Connection conn){
		++called;
		EXPECT(conn.connected(), true);
		EXPECT(conn.connectable(), &cb);
		conn.disconnect();
		EXPECT(conn.connected(), false);
	});
	cb();
	EXPECT(called, 1u);

	// #5
	called = 0u;
	cb();
	EXPECT(called, 0u);

	// #6
	cb.clear();
	ERROR(cb.add(std::function<void()>{}), std::logic_error);
	ERROR(cb.add(std::function<void(nytl::Connection)>{}), std::logic_error);
	ERROR(cb = std::function<void()>{}, std::logic_error);
	ERROR(cb += std::function<void(nytl::Connection)>{}, std::logic_error);
}

// tests correct return values,order and exception propagation
TEST(retval) {
	nytl::Callback<unsigned int()> cb;

	// #1
	cb.add([]{ return 0u; });
	cb.add([]{ return 1u; });
	cb.add([]{ return 2u; });
	auto ret = cb();
	EXPECT(ret.size(), 3u);
	EXPECT(ret.at(0), 0u);
	EXPECT(ret.at(1), 1u);
	EXPECT(ret.at(2), 2u);

	// #2
	cb.clear();
	cb.add([]() -> unsigned int { throw 0; });
	cb.add([]() -> unsigned int { throw 1; });
	cb.add([]() -> unsigned int { throw 2; });

	auto caught = false;
	try { cb(); }
	catch(int i) {
		caught = true;
		EXPECT(i, 0);
	}
	EXPECT(caught, true);

	// #3
	cb.clear();
	ret = cb();
	EXPECT(ret.empty(), true);
}

// adds and remove callback during the call
TEST(interfer) {
	nytl::Callback<void()> cb;
	auto called = 0u;

	// #1
	called = 0;
	cb.add([&]{ cb.add([&]{ ++called; }); });
	cb();
	EXPECT(called, 0u);
	cb();
	EXPECT(called, 1u);

	// #2
	called = 0;
	nytl::Connection conn3;
	cb.clear();
	cb.add([&](nytl::Connection conn){ ++called; conn.disconnect(); });
	auto conn2 = cb.add([&]{ ++called; conn3.disconnect(); });
	conn3 = cb.add([&]{ ++called; });
	cb();
	EXPECT(called, 3u);
	conn2.disconnect();
	cb();
	EXPECT(called, 3u);

	// #3
	called = 0;
	cb = [&]{ ++called; };
	cb += [&](nytl::Connection conn){
		++called;
		conn.disconnect();

		cb();
		EXPECT(called, 3u);
	};

	cb();
	EXPECT(called, 3u);
}


TEST(inter_callback) {
	nytl::Callback<void()> cb;
	int called {};

	nytl::Connection c4;

	// more complex example
	// use the call times to calculate the overall called number

	auto c1 = cb.add([&]{ ++called; }); // 1 call
	cb.add([&](nytl::Connection conn){ conn.disconnect(); called++; }); // 1 call
	cb.add([&]{ ++called; c1.disconnect(); }); // 1 + 7 + 1 calls
	cb.add([&]{ if(called < 10) { cb(); c4.disconnect(); } }); // 1 + 7 + 1 calls
	c4 = cb.add([&]{ if(called < 11) cb(); }); // 1 + 7 + 1 calls

	// special case: although the connection is destroyed the first time
	// this will be called 1 + 7 + 1 times as well since it is disconnected
	// in the deepest recall, while the other calls were already started, so
	// they will call the function as well. This behaviour is explicityly wanted
	cb.add([&](nytl::Connection conn){ ++called; conn.disconnect(); });

	cb();
	EXPECT(called, 20);
}


// - old, rather random tests -
// - might break when working on callback -
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
