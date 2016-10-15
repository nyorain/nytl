#include <nytl/nytl.hpp>
#include <any>
#include <iostream>

struct T : public nytl::CloneMovable<T> {};

void func(const std::any& a)
{
	std::cout << std::any_cast<int>(a) << "\n";
}

int main()
{
	T t;
	auto& tref = t;
	auto moved = nytl::cloneMove(tref);

	std::any a(std::string("pter"));
	auto f = nytl::Callback<void(int b, const std::any& a, int)>();
	f.add(func);
	int c = 345;
	f(std::move(c), a, 65);
}
