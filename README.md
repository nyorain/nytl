#nytl

A generic header-only template library for C++14/c++17. It provides math classes like
nytl::Vec, nytl::Mat or nytl::Rect, utility classes like nytl:::NonCopyable, nytl::Observeable,
nytl::Referenced and functional classes like nytl::Callback, nytl::CompatibleFunction or
nytl::FunctionTraits.
The name nytl does stand for "ny template library", since it was originally created for the _[ny]_ gui library.

It is really lightweight yet it offers some pretty useful features for most modern C++ projects.
Since there is no good tool for extracting template-heavy C++ code at the moment, all documentation
is directly inside the header files and there is currently no good way of extracting it.

Currently in an __alpha__ state, its newest release is __[version 0.2-alpha]__.

##Contributing
If you find any bug or mistakes or would like some new features, additional documentation or examples, just contact me.
Contributors to library, documentation or examples are always appreciated. Just fork it and start a pull request or start an issue. If you just have any ideas, questions or suggestions, just let me know.

##Getting Started
###Building nytl
Since nytl is a header-only library you dont have to compile or install anything, nytl has
__no external dependencies__.
Just clone the repository or download it as zip package if you are not comfortable with using git.
You can then just copy the header files where you need them, or you use __cmake__ to install
them wherever you want. Note that using cmake is the preferred method since it will also install
a pkg-config module (and in future a cmake config file).
Note that some nytl header files depend on other ones and address them as <nytl/[file].hpp> and
therefore you either have to change the includes to relative ones or make sure that
all required files are in a nytl folder in a location where the compiler will find it.

###Usage Examples
This section presents some examples of important nytl classes and functions.
Notice that the classes presented here should just give you an idea of what nytl is and are just a small part of the whole library.
Have also a look at the __[doc/examples]__ folder.

To use ny, you can either include all header files (which may be useful when using many of the
features and using a pre-compiled header) with ```#include <nytl/nytl.hpp>``` or you just
include the seperate features you need like ```#include <nytl/vec.hpp>```.
Note that ny also provides forward-declarations for its classes, so if they are enough for
a file using a nytl object, just ```#include <nytl/fwd.hpp>``` which should cause no real
compiler overhead.

#####nytl::Callback example
Callbacks mirror the signal/slot principle in modern c++ with many useful features.
Notice how the callback listener signatures differs from the callback signature, callback uses nytl::CompatibleFunction internally. More information at __[doc/examples/callback.cpp]__.
```````````````c++
nytl::Callback<void(int, nytl::Vec2f)> onEvent;

auto connection = onEvent.add([]{ std::cout << "called\n"; }); //Adds a callback listener
connection.destroy(); //unregisters the connection listener from received connection object

onEvent += [](int a) -> int { return a * 420; }; //same as .add (adds listener)
onEvent = &someListenerFunction; //Sets the someListenerFunction as ONLY listener

onEvent.call(7, {23.f, 0.3f}); //calls all registered listener functions
```````````````

####nytl::Vec example
The nytl::Vec template class represents a mathematical vector but has many uses since it is basically just an array of objects on which one can easliy perform (especially mathematical/algebraic) operations.
```````````c++
auto a = nytl::Vec<4, float>(4.f, 3.f, 8.f, 55.f); //aka nytl::Vec4f
auto b = nytl::Vec4i(1, 2, 3, 4); //4D int vec

bool b = nytl::all_of(a < b); //all elements from a are less than the matching element from b
b = nytl::any_of(a == b); //any element(with the same index) from both vecs match
auto c = a * b; //component-wise multiplication (like in glsl)

//There are also several useful utility functions available, also orientated at glsl
auto f = nytl::dot(a, b);
f = nytl::angle(a, b);
```````````

####Utility classes
There are several small utility template classes in nytl.

`````````````````c++
class Base1 : public nytl::NonCopyable, public nytl::Cloneable, public nytl::Observeable {}
class Derived1 : public nytl::DeriveCloneable<Base1, Derived1> {}

//Base1 and Derived1 were made cloneable with the Cloneable/DeriveCloneable base classes.
//Note how it was enough to just derive from those classes/templates,
//no additional function implementation were needed.
Base1* base = new Derived1();
std::unique_ptr<Base1> copy = nytl::clone(base); //clone the derived object.

//Both classes are also Observeable, so one can use custom observer classes to track their lifetime
//There is also a built-in observing smart pointer.
nytl::ObservingPtr<Base1> ptr(base);

std::cout << (ptr) << "\n"; //outputs 1, the pointer holds an object
delete base; //the object is deleted here
std::cout << (ptr) << "\n"; //outputs 0, since holds no valid object
````````````````

[doc/examples]: doc/examples
[doc/examples/callback.cpp]: doc/examples/callback.cpp
[doc/examples/serialize.cpp]: doc/examples/serialize.cpp
[version 0.2-alpha]: https://github.com/nyorain/nytl/releases
[ny]: https://github.com/nyorain/ny
