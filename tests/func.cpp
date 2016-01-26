#include <nytl/nytl.hpp>
using namespace nytl;

#include <cassert>

//dummys
int testFunc1(const vec2f&){ return 1; }
void* testFunc2(){ return nullptr; }
double testFunc3(int){ return 1.; }

class testClass1
{
public:
	int func1(void*, float) { return 1; }
};

//check callable
template<typename T, typename = typename std::enable_if<is_callable<T>::value>::type>
void checkCallable(T&&) {};

//main
int main()
{
	testClass1 obj;
	//function traits
	{
		static_assert(std::is_same<function_traits<void(int)>::return_type, void>::value, "");
		static_assert(std::is_same<function_traits<vec2f()>::return_type, vec2f>::value, "");
	
		static_assert(std::is_same<function_traits<decltype(testFunc1)>::return_type, 
				int>::value, "");
		static_assert(std::is_same<function_traits<decltype(&testFunc2)>::return_type, 
			void*>::value, "");
		static_assert(std::is_same<function_traits<decltype(&testClass1::func1)>::return_type, 
			int>::value, "");
	
		static_assert(function_traits<decltype(testFunc1)>::arg_size == 1, "");
		static_assert(function_traits<decltype(testFunc2)>::arg_size == 0, "");
		static_assert(function_traits<decltype(&testClass1::func1)>::arg_size == 2, "");
	
		static_assert(std::is_same<function_traits<decltype(testFunc1)>::arg_tuple, 
			std::tuple<const vec2f&>>::value, "");	
		static_assert(std::is_same<function_traits<decltype(&testFunc2)>::arg_tuple, 
			std::tuple<>>::value, "");	
		static_assert(std::is_same<function_traits<decltype(&testClass1::func1)>::arg_tuple, 
			std::tuple<void*, float>>::value, "");	
	}

	//is_callable
	{
		checkCallable([]{});
		checkCallable(std::function<int(const vec2f&)>(testFunc1));
		checkCallable(&testFunc2);
		checkCallable(testFunc1);
		checkCallable(&testClass1::func1);
	
		static_assert(is_callable<decltype(testFunc1)>::value, "");
		static_assert(is_callable<decltype(&testFunc1)>::value, "");
	}

	//memberCallback
	{
		auto f = memberCallback(&testClass1::func1, obj);
		f(nullptr, 0.f);
	}

	//compatibleFunction
	{
		compatibleFunction<void(void*, double, float)> cp1 = [](float var)
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
		callback<void(float)> cb1;
	
		auto conn1 = cb1.add(&testFunc2);
		auto conn2 = cb1.add(testFunc2);
	
		cb1 = testFunc3;
	
		assert(!conn1.connected());
		assert(!conn2.connected());
	
		auto conn3 = cb1.add([](const connectionRef& ref)
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
