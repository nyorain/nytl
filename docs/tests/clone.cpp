#include "test.hpp"
#include <nytl/clone.hpp>

// Abstract
struct CloneBase : public nytl::AbstractCloneable<CloneBase> {
	virtual int value() const = 0;
};

struct CloneDerived : public nytl::DeriveCloneable<CloneDerived, CloneBase> {
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
struct CloneDerived2 : public nytl::DeriveCloneable<CloneDerived2, CloneBase2> {
	int value2_;
};

struct CloneDerived3 : public nytl::DeriveCloneable<CloneDerived3, CloneDerived2> {
	int value3_;
};

struct CloneDerived4 : public nytl::DeriveCloneable<CloneDerived4, CloneBase, CloneDerived3> {
	int value_;
	int value() const override { return value_; }
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

	// #3
	auto o4 = CloneDerived4 {};
	o4.value_ = 23;
	auto o5c = nytl::clone(o4);
	EXPECT(o5c->value_, o4.value_);
}

// constructors
struct Base2 : public nytl::Cloneable<Base2> {
	Base2(int o) : val1(o) {}
	int val1;
};

struct Base3 : public nytl::AbstractCloneable<Base3> {
	Base3(float o) : val2(o) {}
	virtual void dosth() = 0;
	float val2;
};

// just using one base class has pros and cons:
// - pro: we can use their constructors (through nytl::DeriveCloneable)
// - con: we have to manually select the doClone/doCloneMove functions
//   - it doesn't matter which one we choose, but we have to also add
//     the friend declaration.
// Basically don't do it this way if you don't really need
// the base class constructors. You could also just manually override
// doClone() and doCloneMove() the same way the nytl helper classes do in
// this case.
struct Derived1 : 
		public nytl::DeriveCloneable<Derived1, Base2>,
		public nytl::DeriveCloneable<Derived1, Base3> {
	Derived1(int a, float b) : 
		nytl::DeriveCloneable<Derived1, Base2>(a),
		nytl::DeriveCloneable<Derived1, Base3>(b) {}

	void dosth() override {};

private:
	using nytl::DeriveCloneable<Derived1, Base2>::doClone;
	using nytl::DeriveCloneable<Derived1, Base2>::doCloneMove;

	template<typename O> friend std::unique_ptr<O> nytl::cloneMove(O&);
	template<typename O> friend std::unique_ptr<O> nytl::clone(const O&);
};

TEST(clone3) {
	Derived1 a(1, 2.f);
	EXPECT(a.val1, 1);
	EXPECT(a.val2, 2.f);

	auto b = nytl::clone(a);
	EXPECT(b->val1, 1);
	EXPECT(b->val2, 2.f);
}

// test that cloneMove really moves
struct Base4 : public nytl::AbstractCloneMovable<Base4> {
	virtual const std::vector<int>& get() const = 0;
};

struct Derived4 : public nytl::DeriveCloneMovable<Derived4, Base4> {
	std::vector<int> vals {};
	const std::vector<int>& get() const override { return vals; }
};

TEST(clone4) {
	Derived4 a {};
	a.vals = {1, 2, 3, 4, 5};
	EXPECT(a.vals.size(), 5u);

	auto b = nytl::cloneMove(a);
	EXPECT(b->get().size(), 5u);
	EXPECT(a.vals.empty(), true);
}