#include "test.hpp"
#include <nytl/referenced.hpp>

// - referenced -
TEST(referenced) {
	bool deleted {};
	auto del = [&](const auto*){ deleted = true; };

	struct MyReferenced : public nytl::Referenced<MyReferenced, decltype(del)> {
		using nytl::Referenced<MyReferenced, decltype(del)>::Referenced;
	};

	MyReferenced obj(del);
	auto ref = nytl::IntrusivePtr<MyReferenced>(obj);
	EXPECT(ref->referenceCount(), 1u);

	{
		auto copy = ref;
		copy->ref();
		EXPECT(ref->referenceCount(), 3u);
		copy->unrefNodelete();
		EXPECT(ref->referenceCount(), 2u);
	}

	auto copy = ref;
	copy.reset();

	EXPECT(ref->referenceCount(), 1u);

	auto ptr = ref.get();
	ptr->ref();

	ref = {};
	EXPECT(ptr->referenceCount(), 1u);

	ptr->unref();
	EXPECT(ptr->referenceCount(), 0u);
	EXPECT(deleted, true);
}
