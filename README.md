#nytl [![Build Status](https://travis-ci.org/nyorain/nytl.svg?branch=master)](https://travis-ci.org/nyorain/nytl)

nytl is a generic header-only template library for c++14. It provides math classes with useful operations like e.g. [nytl::Vec], [nytl::Mat] or [nytl::Rect].
Then there is a smaller part about functions and function-conversions with classes like [nytl::CompatibleFunction], [nytl::Callback] or [nytl::FunctionTraits]. 
Additionally nytl provides some generic utility stuff, like [nytl::Referenced] and [nytl::IntrusivePtr] \(Base class for thread-safe reference counter classes and a intrusive smart pointer using this built-in reference count), [nytl::Cloneable]/[nytl::AbstractCloneable] and [nytl::DeriveCloneable] which make it really easy to implement cloneable classes or [nytl::Observer]/[nytl::Observable] and [nytl::ObservingPtr] for observing the lifetime of an object and making use of it in a smart pointer.
The name nytl does stand for "ny template library", since it was originally created for the _[ny]_ gui library.
nytl is really lightweight, yet it offers some pretty useful features for most modern c++ projects.

nytl is licensed under the MIT License, so you can freely use it in your projects. If you start an open-source project using nytl, i would really appreciate if you let me know.

nytl is currently in an __alpha__ state, its newest release is __[version 0.2-alpha]__.

__Also have a look at the documentation__ (you can also generate it using cmake/doxygen, WIP): http://nyorain.github.io/nytl/

##Contributing
If you find any bug or mistake or would like some new features or additional documentation or examples, just contact me.
Contributors to library, documentation or examples are always appreciated. Just fork it and start a pull request, start an issue or send me a mail at nyorain@gmail.com. If you just have any ideas, questions or suggestions, just let me know.
Everything is usually answered in a few days.

##Getting Started 
###Building nytl
Since nytl is a header-only library you dont have to compile anything, nytl has __no external dependencies__.
Just clone the repository or download it as zip package if you are not comfortable with using git.
You can then just copy the header files where you need them, or you use __cmake__ to install it wherever you want.

###Usage Examples
This section presents some examples of important nytl classes and functions.
Notice that the classes presented here should just give you an idea of what nytl is and are just a small part of the whole library.
Have also a look at the __[doc/examples]__ folder.

First Include all nytl headers (precompiled header then useful).
You could also just include the features you need (will speed up compile time).
```````````````c++
#include <nytl/nytl.hpp>
`````````````

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

bool b = all(a < b); //all elements from a are less than the matching element from b
b = any(a == b); //any element(with the same index) from both vecs match
auto c = a * b; //component-wise multiplication (like in glsl)

//There are also several useful utility functions available, also orientated at glsl
auto f = dot(a, b);
f = angle(a, b);
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

[nytl::Mat]: http://nyorain.github.io/nytl/classnytl_1_1Mat.html
[nytl::Vec]: http://nyorain.github.io/nytl/classnytl_1_1Vec.html
[nytl::Rect]: http://nyorain.github.io/nytl/classnytl_1_1Rect.html
[nytl::CompatibleFunction]: http://nyorain.github.io/nytl/classnytl_1_1CompatibleFunction_3_01R_07A_8_8_8_08_4.html
[nytl::FunctionTraits]: http://nyorain.github.io/nytl/classnytl_1_1FunctionTraits.html
[nytl::Callback]: http://nyorain.github.io/nytl/classnytl_1_1Callback_3_01Ret_07Args_8_8_8_08_4.html
[nytl::Referenced]: http://nyorain.github.io/nytl/classnytl_1_1Referenced.html
[nytl::IntrusivePtr]: http://nyorain.github.io/nytl/classnytl_1_1IntrusivePtr.html
[nytl::Observable]: http://nyorain.github.io/nytl/classnytl_1_1Observable.html
[nytl::Observer]: http://nyorain.github.io/nytl/classnytl_1_1Observer.html
[nytl::ObservingPtr]: http://nyorain.github.io/nytl/classnytl_1_1ObservingPtr.html
[nytl::Cloneable]: http://nyorain.github.io/nytl/classnytl_1_1Cloneable.html
[nytl::DeriveCloneable]: http://nyorain.github.io/nytl/classnytl_1_1DeriveCloneable.html
[nytl::AbstractCloneable]: http://nyorain.github.io/nytl/classnytl_1_1AbstractCloneable.html
[doc/examples]: doc/examples
[doc/examples/callback.cpp]: doc/examples/callback.cpp
[doc/examples/serialize.cpp]: doc/examples/serialize.cpp
[version 0.2-alpha]: https://github.com/nyorain/nytl/releases
[ny]: https://github.com/nyorain/ny
