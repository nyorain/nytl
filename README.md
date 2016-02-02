#nytl ![Travis Build](https://travis-ci.org/nyorain/nytl.svg?branch=master)

nytl is a header-only utility library for c++11. It includes math classes and operations like vec, mat or rect, a smaller part about functions and function-conversions with classes like compatibilityFunction, callback or function_traits, and additionally some general utility stuff, like some c++14 stl implementations (if your compiler still just supports c++11, but you often need features like make_unique or integer_sequence) or serializing-helper class and macros.
The name nytl does stand for ny-utility as well as ny-template-library, since it was originally created for the _ny_ gui library.
nytl is really lightweight, yet it offers some pretty useful features for most modern c++ projects.

__nytl is still in a pre-alpha state__ and under heavy-developement, the first alpha version will probably be released soon. Although nothing is guaranteed to be working at the moment and some things may be changed or removed from the library in the future, we try to fix all bugs and problems you have, just use the issue section of this repository (or just fix it and start a pull request).

nytl is licensed under the MIT License, so you can basically do with it whatever you want. If you start an open-source project using nytl, i would really appreciate if you let me know.

__Also have a look at the documentation__ (you can also generate it using cmake/doxygen, WIP): http://nyorain.github.io/nytl/

##Getting Started 
###Building nytl
Since nytl is a header-only library you dont have to compile anything, nytl has __no external dependencies__.
Just clone the repository or download it as zip package if you are not comfortable with using git.
You can then just copy the header files where you need them, or you use cmake to install it wherever you want.

###Usage Examples
This section presents some examples of important nytl classes and functions.
Notice that the classes presented here should just give you an idea of what nytl is and are just a small part of the whole library.
Have also a look at the __doc/examples__ folder.

First Include all nytl headers (precompiled header then useful).
You could also just include the features you need (will speed up compile time).
```````````````
#include <nytl/nytl.hpp>
`````````````

#####nytl::Callback example
Callbacks mirror the signal/slot principle in modern c++11 with many useful features.
Notice how the callback listener signatures differs from the callback signature, callback uses nytl::CompatibleFunction internally.
```````````````
nytl::Callback<void(int, nytl::Vec2f)> onEvent;

auto connection = onEvent.add([]{ std::cout << "called\n" }); //Adds a callback listener and receives connection object
connection.destroy(); //unregisters the connection listener

onEvent += [](int a) -> int { return a * 420; }; //same as .add (adds listener)
onEvent = &someListenerFunction; //Sets the someListenerFunction as ONLY listener

onEvent.call(7, {23.f, 0.3f}); //calls all registered listener functions
```````````````

####nytl::Vec example
The nytl::Vec template class represents a mathematical vector but has many uses since it is basically just an array of objects on which one can easliy perform (especially mathematical/algebraic) operations.
```````````
auto a = nytl::Vec<4, float>(4.f, 3.f, 8.f, 55.f); //aka nytl::Vec4f
auto b = nytl::Vec4i(1, 2, 3, 4); //4D int vec

bool b = all(a < b); //all elements from a are less than the matching element from b
b = any(a == b); //any element(with the same index) from both vecs match
auto c = a * b; //component-wise multiplication (like in glsl)

auto f = dot(a, b); //There are also several useful utility functions available (orientated at glsl vec operations)
f = angle(a, b);
```````````

####Utility classes
There are several small utility template classes in nytl.
Many classes (like nytl::Observeable shown below) are additionally designed threadsafe.

`````````````````
class Base1 : public nytl::NonCopyable, public nytl::Cloneable, public nytl::Observeable {}
class Derived1 : public nytl::DeriveCloneable<Base1, Derived1> {}

//Base1 and Derived1 were made cloneable with the nytl::Cloneable and nytl::DeriveCloneable base classes.
//Note how it was enough to just derive from those classes/templates, no additional function implementation were needed.
Base1* base = new Derived1();
std::unique_ptr<Base1> copy = nytl::clone(base); //clone the derived object.

//Both classes are also Observeable, so one can use custom observer classes to track their lifetime
//There is also a built-in observing smart pointer.
nytl::ObservingPtr<Base1> ptr(base);

std::cout << (ptr) << "\n"; //outputs 1, the pointer holds an object
delete base; //the object is deleted here
std::cout << (ptr) << "\n"; //outputs 0, the pointer holds no object since the one it did hold was destroyed above.
````````````````

##Contributing
Contributors to library, documentation or examples are always appreciated. Just fork it and start a pull request, start an issue or send me a mail at nyorain@gmail.com. If you just have any ideas, questions or suggestions, just let me know.
