#include <nytl/functionTraits.hpp>
#include <nytl/tmpUtil.hpp>

#include <functional>
#include <string>
#include <type_traits>

void foo();
int bar(int, char, int);

// utility typedefs to use with validExpression checks
template<typename T> using ArgSize = decltype(T::ArgSize);
template<typename T> using ZerothArg = typename T::template ArgsType<0>;
template<typename T> using FirstArg = typename T::template ArgsType<1>;

int main()
{
	auto lambda = []{};

	static_assert(!nytl::isCallable<int>);
	static_assert(!nytl::isCallable<decltype(false)>);
	static_assert(nytl::isCallable<decltype(foo)>);
	static_assert(nytl::isCallable<decltype(bar)>);
	static_assert(nytl::isCallable<decltype(lambda)>);
	static_assert(nytl::isCallable<std::function<void()>>);
	static_assert(!nytl::isCallable<std::string>);
	static_assert(nytl::isCallable<std::is_enum<int>>);

	using FooTraits = nytl::FunctionTraits<decltype(foo)>;
	static_assert(FooTraits::ArgSize == 0);

	static_assert(!nytl::validExpression<ZerothArg, FooTraits>);
	static_assert(!nytl::validExpression<FirstArg, FooTraits>);
	static_assert(!nytl::validExpression<ArgSize, int>);
}
