#include <nytl/tmpUtil.hpp>
#include <type_traits>
#include <cmath>
#include <string>

void bar();
void foo(std::string);

template<typename... T> using Expression1 = decltype(bar(std::declval<T...>()));
template<typename T> using Expression2 = decltype(foo(std::declval<T>()));
template<typename T> using Expression3 = decltype(std::round(std::declval<T>()));

int main() {
	static_assert(nytl::validExpression<Expression1, int> == false, "tmp:1");
	static_assert(nytl::validExpression<Expression1, float> == false, "tmp:2");
	static_assert(nytl::validExpression<Expression1, void> == false, "tmp:3");

	static_assert(nytl::validExpression<Expression2, int> == false, "tmp:4");
	static_assert(nytl::validExpression<Expression2, float> == false, "tmp:5");
	static_assert(nytl::validExpression<Expression2, std::string> == true, "tmp:6");
	static_assert(nytl::validExpression<Expression2, const char*> == true, "tmp:7");

	static_assert(nytl::validExpression<Expression3, int> == true, "tmp:8");
	static_assert(nytl::validExpression<Expression3, std::string> == false, "tmp:9");
}
