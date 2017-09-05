#include <nytl/nonCopyable.hpp>
#include <nytl/tmpUtil.hpp>

struct MyNonCopyable : public nytl::NonCopyable {};
struct MyNonMovable : public nytl::NonMovable {};

template<typename T> T&& rref();
template<typename T> const T& cref();

template<typename T> using Copyable = decltype(T{cref<T>()});
template<typename T> using Movable = decltype(T{rref<T>()});

// - noncopyable
int main()
{
	static_assert(nytl::validExpression<Copyable, MyNonCopyable> == false,
		"NonCopyable is copyable");
	static_assert(nytl::validExpression<Movable, MyNonCopyable> == true,
		"NonCopyable is not movable");

	static_assert(nytl::validExpression<Copyable, MyNonMovable> == false,
		"NonMovable is copyable");
	static_assert(nytl::validExpression<Movable, MyNonMovable> == false,
		"NonMovable is movable");
}
