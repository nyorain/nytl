#include "bugged.hpp"
#include <nytl/observe.hpp>
#include <vector>

struct ObsClass : public nytl::Observable<ObsClass> {};

template<typename T>
struct MyObserver : public nytl::Observer<T> {
	int called {};
	void observeDestruction(T&) override { ++called; }
};

TEST(basic) {
	auto observer = MyObserver<ObsClass> {};
	auto observed = new ObsClass {};

	observed->addObserver(observer);
	observed->addObserver(observer);
	EXPECT(observed->observers().size(), 2u);

	delete observed; // will trigger observer.observeDestruction(*object)

	EXPECT(observer.called, 2);
}

TEST(move) {
	auto observer1 = MyObserver<ObsClass> {};
	auto observer2 = MyObserver<ObsClass> {};
	auto observer3 = MyObserver<ObsClass> {};
	auto observer4 = MyObserver<ObsClass> {};
	auto observer5 = MyObserver<ObsClass> {};

	{
		auto observed = ObsClass {};
		observed.addObserver(observer1);
		observed.addObserver(observer3);
		observed.addObserver(observer4);
		observed.addObserver(observer4);
		observed.addObserver(observer5);
		observed.addObserver(observer5);

		EXPECT(observed.moveObserver(observer1, observer2), true);
		EXPECT(observed.moveObserver(observer1, observer2), false);
		EXPECT(observed.removeObserver(observer3), 1u);
		EXPECT(observed.removeObserver(observer3), 0u);
		EXPECT(observed.removeObserver(observer5), 2u);
		EXPECT(observed.removeObserver(observer5), 0u);

		EXPECT(observed.observers().size(), 3u);
	}

	EXPECT(observer1.called, 0);
	EXPECT(observer2.called, 1);
	EXPECT(observer3.called, 0);
	EXPECT(observer4.called, 2);
	EXPECT(observer5.called, 0);
}

TEST(ptr) {
	auto observed = new ObsClass {};

	auto ptr = nytl::ObservingPtr<ObsClass>(observed);
	auto cpy = ptr;
	auto cpy2 = ptr;
	auto moved = std::move(cpy2);

	EXPECT(ptr, true);
	EXPECT(ptr.get(), observed);
	EXPECT(cpy.get(), observed);
	EXPECT(cpy2.get(), nullptr);
	EXPECT(moved.get(), observed);
	EXPECT(observed->observers().size(), 3u);

	cpy.reset();
	EXPECT(cpy, false);
	EXPECT(cpy.get(), nullptr);
	EXPECT(observed->observers().size(), 2u);

	delete observed;
	EXPECT(ptr, false);
	EXPECT(ptr.get(), nullptr);
	EXPECT(cpy.get(), nullptr);
	EXPECT(cpy2.get(), nullptr);
	EXPECT(moved.get(), nullptr);
}

TEST(wrapper) {
	using ObservedVector = nytl::ObservableWrapper<std::vector<int>>;
	auto observer1 = MyObserver<std::vector<int>> {};
	auto observer2 = MyObserver<std::vector<int>> {};
	auto observer3 = MyObserver<std::vector<int>> {};

	{
		auto observed = ObservedVector {};

		observed.addObserver(observer1);
		observed.addObserver(observer1);
		observed.addObserver(observer2);
		observed.addObserver(observer3);

		EXPECT(observed.moveObserver(observer1, observer3), true);
		EXPECT(observed.removeObserver(observer2), 1u);

		EXPECT(observed.observers().size(), 3u);
	}

	EXPECT(observer1.called, 1);
	EXPECT(observer2.called, 0);
	EXPECT(observer3.called, 2);
}
