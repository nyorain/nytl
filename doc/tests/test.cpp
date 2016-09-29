#include <nytl/nytl.hpp>

struct T : public nytl::CloneMovable<T> {};

int main()
{
	T t;
	auto& tref = t;
	auto moved = nytl::cloneMove(tref);
}
