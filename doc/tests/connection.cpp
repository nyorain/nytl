#include "bugged.hpp"
#include <nytl/connection.hpp>
#include <nytl/callback.hpp>

TEST(basic) {
	nytl::Connection basic;
	nytl::UniqueConnection unique;

	basic = {};
	unique = {};

	unique = basic;
	// basic = unique; // should NOT compile!

	EXPECT(basic.connected(), false);
	EXPECT(unique.connected(), false);
	EXPECT(basic.connectable(), nullptr);
	EXPECT(unique.connectable(), nullptr);
}

TEST(tracked) {
	nytl::TrackedConnection tracked;
	nytl::TrackedConnection trackedCopy;

	{
		nytl::Callback<void(), nytl::TrackedConnectionID> connectable;
		tracked = connectable.add([]{});
		trackedCopy = tracked;
		EXPECT(tracked.connected(), true);
		EXPECT(tracked.connectable(), &connectable);
		EXPECT(trackedCopy.connected(), true);
	}

	EXPECT(tracked.connected(), false);
	EXPECT(trackedCopy.connected(), false);
}
