#include "test.hpp"
#include <nytl/clone.hpp>

// Abstract
struct CloneBase : public nytl::AbstractCloneable<CloneBase> {
	virtual int value() const = 0;
};

struct CloneDerived : public nytl::DeriveCloneable<CloneBase, CloneDerived> {
	int value_;
	int value() const override { return value_; }
};


TEST(clone_abstract) {
	auto derived = CloneDerived {};
	derived.value_ = 42;

	auto ptr = static_cast<CloneBase*>(&derived);

	auto copy = nytl::clone(*ptr);
	auto moved = nytl::cloneMove(*ptr);

	EXPECT(copy->value(), 42);
	EXPECT(moved->value(), 42);
}

// Non-Abstract
struct CloneBase2 : public nytl::Cloneable<CloneBase2> {};
struct CloneDerived2 : public nytl::DeriveCloneable<CloneBase2, CloneDerived2> {
	int value2_;
};

struct CloneDerived3 : public nytl::DeriveCloneable<CloneDerived2, CloneDerived3> {
	int value3_;
};

TEST(clone2) {
	// #1
	auto derived2 = CloneDerived2 {};
	derived2.value2_ = 11;
	auto& ref2 = static_cast<CloneBase2&>(derived2);

	auto copy2 = nytl::clone(ref2);
	auto casted2 = dynamic_cast<CloneDerived2*>(copy2.get());
	EXPECT(casted2 == nullptr, false);
	EXPECT(casted2->value2_, 11);

	// #2
	auto derived3 = CloneDerived3 {};
	derived3.value2_ = 21;
	derived3.value3_ = 22;
	auto& ref3 = static_cast<CloneBase2&>(derived3);

	auto copy3 = nytl::clone(ref3);
	auto casted3 = dynamic_cast<CloneDerived3*>(copy3.get());
	EXPECT(casted3 == nullptr, false);
	EXPECT(casted3->value2_, 21);
	EXPECT(casted3->value3_, 22);
}
