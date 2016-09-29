#include <nytl/nytl.hpp>
using namespace nytl;

#include <cassert>

//dummys
int testFunc1(const Vec2f&){ return 1; }
void* testFunc2(){ return nullptr; }
double testFunc3(int){ return 1.; }

class testClass1
{
public:
	int func1(void*, float) { return 1; }
};

//check callable
template<typename T, typename = typename std::enable_if<IsCallable<T>>::type>
void checkCallable(T&&) {}

//main
int main()
{
	testClass1 obj;
	//function traits
	{
		static_assert(std::is_same<FunctionTraits<void(int)>::ReturnType, void>::value, "");
		static_assert(std::is_same<FunctionTraits<Vec2f()>::ReturnType, Vec2f>::value, "");

		static_assert(std::is_same<FunctionTraits<decltype(testFunc1)>::ReturnType,
				int>::value, "");
		static_assert(std::is_same<FunctionTraits<decltype(&testFunc2)>::ReturnType,
			void*>::value, "");
		static_assert(std::is_same<FunctionTraits<decltype(&testClass1::func1)>::ReturnType,
			int>::value, "");

		static_assert(FunctionTraits<decltype(testFunc1)>::ArgSize == 1, "");
		static_assert(FunctionTraits<decltype(testFunc2)>::ArgSize == 0, "");
		static_assert(FunctionTraits<decltype(&testClass1::func1)>::ArgSize == 2, "");

		static_assert(std::is_same<FunctionTraits<decltype(testFunc1)>::ArgTuple,
			std::tuple<const Vec2f&>>::value, "");
		static_assert(std::is_same<FunctionTraits<decltype(&testFunc2)>::ArgTuple,
			std::tuple<>>::value, "");
		static_assert(std::is_same<FunctionTraits<decltype(&testClass1::func1)>::ArgTuple,
			std::tuple<void*, float>>::value, "");
	}

	//is_callable
	{
		checkCallable([]{});
		checkCallable(std::function<int(const Vec2f&)>(testFunc1));
		checkCallable(&testFunc2);
		checkCallable(testFunc1);
		checkCallable(&testClass1::func1);

		static_assert(IsCallable<decltype(testFunc1)>, "");
		static_assert(IsCallable<decltype(&testFunc1)>, "");
	}

	//memberCallback
	{
		auto f = memberCallback(&testClass1::func1, obj);
		f(nullptr, 0.f);
	}

	//compatibleFunction
	{
		CompFunc<void(void*, double, float)> cp1 = [](float var)
		{
			assert(var == 8.f);
		};

		cp1(nullptr, 8., 4.f);
		cp1 = testFunc2;
		cp1 = &testFunc3;

		cp1.set(memberCallback(&testClass1::func1, &obj));
		assert(cp1.function());
	}


	//callback
	{
		Callback<void(float)> cb1;

		auto conn1 = cb1.add(&testFunc2);
		auto conn2 = cb1.add(testFunc2);

		cb1 = testFunc3;

		assert(!conn1.connected());
		assert(!conn2.connected());

		auto conn3 = cb1.add([](const CbConnRef& ref)
				{
					auto ref2 = ref;
					ref2.destroy();

					assert(!ref.connected());
				});

		auto conn4 = cb1.add(testFunc3);

		cb1(5.f);

		assert(!conn3.connected());
		assert(conn4.connected());
	}

	return EXIT_SUCCESS;
}
