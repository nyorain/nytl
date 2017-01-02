# nytl

A lightweight and generic header-only template library for C++17.
Includes various features of all kind:

- Headers for generic [vector](nytl/vecOps.hpp) and [matrix](nytl/matOps.hpp) operations
- Example [vector](nytl/vec.hpp) and [matrix](nytl/mat.hpp) implementation
- [Simplex](nytl/simplex.hpp) implementation and [various operations](nytl/simplexOps.hpp).
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

// alternatively one can use the NYTL_SCOPE_GUARD macro
NYTL_SCOPE_GUARD([&]{ std::cout << "scope left\n"; });

// there are also classes that only execute the passed functions if the scope was left normally
// or due to an exception
auto successGuard = nytl::makeScopeGuard([&]{ std::cout << "scope left normally\n"; });
auto exceptionGuard = nytl::makeExceptionGuard([&]{ std::cout << "exception thrown\n"; });
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

### Mathematical operations

There are also multiple headers for generic maths in nytl.
Some interesting ones are [nytl/matOps.hpp](nytl/matOps.hpp) or
[nytl/simplex.hpp](nytl/simplex.hpp).

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
[auto l, u, p, s] = nytl::mat::luDecomp(a); // lower, upper, permutation, sign (of permutation)
nytl::mat::reducedRowEcholon(a); // reduced row echolon form
nytl::mat::identity(a);

// A simplex is the most trivial but unique object in an n-dimensional space, so e.g.
// a triangle in 2 dimensions or a tetrahedron in 3 dimensions
// There are various useful operations for simplices like computing its barycentric coordinates
// which makes interpolation inside a simplex pretty trivial.
nytl::Simplex<2, double> triangle {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}};
auto size = nytl::size(triangle); // 0.5
auto center = nytl::center(triangle); // nytl::Vec2d(1.0/3, 1.0/3);
auto barycentric = nytl::barycentric(triangle, {0.5, 0.5}); // nytl::Vec3d {0.0, 0.5, 0.5}
```
