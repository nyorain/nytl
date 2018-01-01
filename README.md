# nytl

A lightweight and generic header-only template library for C++17.
Includes various utility of all kind:

- Extremely lightweight [vector](nytl/vec.hpp) and [matrix](nytl/mat.hpp) templates
	- Basically just std::array with mathematical vector/matrix semantics
	- Additionally various useful operations ([mat](nytl/matOps.hpp) | [vec](nytl/vecOps.hpp))
- Small utf conversion and utf8 parsing helpers: [nytl/utf.hpp](nytl/utf.hpp)
- A [Callback](nytl/callback.hpp) implementation for high-level and fast function callbacks.
	- Also a more functional [RecursiveCallback](nytl/recursiveCallback.hpp)
- Intrusive reference counted helpers: [nytl/referenced.hpp](nytl/referenced.hpp)
- Observers and observable helpers: [nytl/observe.hpp](nytl/observe.hpp)
- Make classes cloneable: [nytl/clone.hpp](nytl/clone.hpp)
- Pseudo-RAII handling with scope guards: [nytl/scope.hpp](nytl/scope.hpp)
- Lightweight and independent span template: [nytl/span.hpp](nytl/span.hpp)

All headers were written as modular, independent and generic as possible. The only required
dependency is a compiler supporting full C++17 and its stl (this means no suppport
for msvc at the moment).
All files are licensed under the Boost License.

The latest nytl release is
__[version 0.5](https://github.com/nyorain/nytl/releases)__.

## Contributing

Contributions to library, tests, documentation, examples as well as
all suggestions and ideas are always appreciated.
Just start a pull request or an issue on [github](https://github.com/nyorain/nytl).

## Using nytl

There are multiple ways to use nytl. Either install all of its headers on your system and make
sure the install path can be found by the compiler.
If you need just a few header files (or even just a few functions), just copy those files into
your project folder and (if needed) adjust the includes of nytl headers.

Remember that nytl requires a solid C++17 compiler, only development builds of gcc and clang
are supported.
Below some basic code examples for (only a) few nytl features to give you an idea.

### nytl::Callback and nytl::RecursiveCallback

Callbacks mirror the signal/slot principle in modern c++ with many useful features.
For the full documentation, see [nytl/callback.hpp](nytl/callback.hpp).
If you want to modify (call/register/disconnect) the callback from
within a handler, see [nytl/recursiveCallback.hpp](nytl/recursiveCallback.hpp).

```cpp
auto onEvent = nytl::RecursiveCallback<void()> {}; // Example callback

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
