#include <nytl/nytl.hpp>
#include <any>
#include <iostream>

namespace nytl
{

template<typename ID, typename B>
struct ConvertException<nytl::ConnectionRef<ID>, B> : public std::false_type {};

// template<typename B>
// struct ConvertException<B, std::any> : public std::false_type {};
// 
// template<>
// struct ConvertException<std::any, std::any> : public std::true_type {};

}

struct T : public nytl::CloneMovable<T> {};

// void func(int b, const std::any& a)
void func(const std::any& a)
{
	// std::cout << std::any_cast<std::string>(a) << "\n";
	std::cout << std::any_cast<int>(a) << "\n";
	// std::cout << b << "\n";
}

int main()
{
	T t;
	auto& tref = t;
	auto moved = nytl::cloneMove(tref);

	std::any a(std::string("pter"));
	auto f = nytl::Callback<void(int b, const std::any& a, int)>();
	f.add(func);
	f(42, a, 65);
}
