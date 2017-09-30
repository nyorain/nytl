# nytl

A lightweight and generic header-only template library for C++17.
Includes various utility of all kind:

- Extremely lightweight [vector](nytl/vec.hpp) and [matrix](nytl/mat.hpp) templates
	- Basically just std::array with mathematical vector/matrix semantics
	- Additionally various useful operations ([mat](nytl/matOps.hpp) | [vec](nytl/vecOps.hpp))
- Small utf conversion and utf8 parsing helpers: [nytl/utf.hpp](nytl/utf.hpp)
- A [Callback](nytl/callback.hpp) implementation for high-level and fast function callbacks.
- Intrusive reference counted helpers: [nytl/referenced.hpp](nytl/referenced.hpp)
- Observers and observable helpers: [nytl/observe.hpp](nytl/observe.hpp)
- Make classes cloneable: [nytl/clone.hpp](nytl/clone.hpp)
- Pseudo-RAII handling with scope guards: [nytl/scope.hpp](nytl/scope.hpp)
- Lightweight and independent span template: [nytl/span.hpp](nytl/span.hpp)

All headers were written as modular, independent and generic as possible. The only required
dependency is a compiler supporting full C++17 and its stl.
All files are licensed under the __Boost License__ (similar to the MIT license).

Currently in an __alpha__ state, the newest nytl release is
__[version 0.4-alpha](https://github.com/nyorain/nytl/releases)__.

## Contributing

Contributions to library, documentation or examples are always appreciated.
Just start a pull request or an issue.
If you just have any ideas, questions or suggestions, just let us know!

## Using nytl

There are various ways to use nytl. Either install all of its headers on your system and make
sure the install path can be found by the compiler.
If you need just a few header files (or even just a few functions), just copy those files into
your project folder and (if needed) adjust the includes of nytl headers.

Remember that nytl requires a solid C++17 compiler, only development builds of gcc and clang
are supported.
Below some basic code examples for (only a) few nytl features.

### nytl::Callback

Callbacks mirror the signal/slot principle in modern c++ with many useful features.
For the full documentation, see [nytl/callback.hpp](nytl/callback.hpp).

```cpp
auto onEvent = nytl::Callback<void()> {}; // Example callback

auto connection = onEvent.add([]{ std::cout << "called\n"; }); // Adds a callback listener
connection.disconnect(); // unregisters the listener

onEvent = &foo; // sets foo as only listener (i.e. unregisters other listeners)
onEvent += []{}; // same as onEvent.add

// listener functions can also take an additional nytl::Connection argument that
// allows them to unregister themself from within the listener
onEvent += [&](nytl::Connection selfConnection) {
	selfConnection.disconnect(); // will unregister itself on first call
	onEvent += &bar; // one can also add new listeners from inside a listener
};

onEvent(); // calls all registered listener functions
onEvent.call(); // can also be done more explicit
```

### nytl::ScopeGuard

ScopeGuards are another utility concept implemented by nytl. The mirror finally syntax from
other languages and allow safe RAII-like handling of non-RAII resources.
For the full documentation, see [nytl/scope.hpp](nytl/scope.hpp).

```cpp
auto fd = ::open("test.txt"); // open a file descriptor we want to close later on

// create a scope guard that will execute the passed functions as soon as this scope is left,
// no matter in which way. Makes closing the fd description safe and also more maintainable since
// early returns can be added without having to care about the fd.
auto fdGuard = nytl::makeScopeGuard([&]{ ::close(fd); })

// there are also classes that only execute the passed functions if the scope was left normally
// or due to an exception
auto successGuard = nytl::SuccessGuard([&]{ std::cout << "scope left normally\n"; });
auto exceptionGuard = nytl::ExceptionGuard([&]{ std::cout << "exception thrown\n"; });
```

### nytl::Typemap

Another idea implemented by nytl is a type-erased typemap that simulates to associate
identifiers to type. See also [nytl/typemap.hpp](nytl/typemap.hpp).

```cpp
auto typemap = nytl::Typemap<std::string>; // typemap that associates names to types

// add various entries to the typemap
nytl::add<int>(typemap, "int");
nytl::add<float>(typemap, "float");
nytl::add<std::string>(typemap, "std::string");

// objects of the registered types (wrapped e.g. in a std::any) can now dynamically
// be created, or their type_infos retrieved
auto integer = typemap.create("int");
auto stringTypeInfo = typemap.typeInfo("std::string");
```

### Metaprogramming

nytl also implements some useful metaprogramming helpers. See [nytl/tmpUtil.hpp](nytl/tmpUtil.hpp), 
[nytl/functionTraits.hpp](nytl/functionTraits.hpp) or [nytl/tuple.hpp](nytl/tuple.hpp).

```cpp
// constexpr SFINAE-based branches without template specialization
// Some declarations for callables (e.g. functions) foo and bar here

template<typename T> using FooCallbable = decltype(foo(std::declval<T>()));
template<typename T> using BarCallbable = decltype(bar(std::declval<T>()));

template<typename T> auto dispatch(const T& obj)
{
	if constexpr(nytl::validExpression<FooCallable<T>>) return foo(obj);
	else if constexpr(nytl::validExpression<BarCallable<T>>) return bar(obj);
	else return fallback(obj); // otherwise templated static_assert to generate error
}
```

### Mathematical operations

There are also multiple headers for generic maths in nytl. For common structurs, such as
vectors as matrices, nytl strictly separates structural implementation from operations which
makes the operations generic and usable for custom structurs (i.e. you can use your own
vector class as long as it fulfills nytl's Vector concept).
Some interesting ones are [nytl/matOps.hpp](nytl/matOps.hpp),
[nytl/vecOps.hpp](nytl/simplex.hpp) or [nytl/vec.hpp](nytl/vec.hpp).
Everything is kept as generic as simple as possible.

```cpp
nytl::Mat<5, 5, double> a {
	{1, -2, 3, 5, 8},
	{0, -1, -1, 2, 3},
	{2, 4, -1, 3, 1},
	{0, 0, 5, 0, 0},
	{1, 3, 0, 4, -1}
};

// various matrix operations
// note that all these operations are defined generic, they will also work
// for your own Matrix data types if they fulfull the nytl Matrix concept.
auto det = nytl::mat::determinant(a);
auto inverse = nytl::mat::inverse(a);
auto [l, u, p, s] = nytl::mat::luDecomp(a); // lower, upper, permutation, sign (of permutation)
nytl::mat::reducedRowEcholon(a); // reduced row echolon form
nytl::mat::identity(a);
```
