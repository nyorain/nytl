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
Start by cloning the nytl repository from github:
```
git clone https://github.com/nyorain/nytl.git
cd nytl
```

Now, you can either just copy all the header files, where you want them to be, or let cmake automatically install them as well as a pkg-config file (__not recommended on windows__).
All the header files are situated in the nytl folder directly at the project root.
cmake:
```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr ..
```

If you now want to configure and install everything just do:
```
make install #try it with sudo if needed
```

If you just want to make the documentation (remember that you need doxygen installed for that) just type `make doc`.
The documentation will then be located at build/doc/doxygen/html. Start at index.html.


###Windows
If you do not have git installed, or do not know how it works, no problem, you can also just download a zip-package from this repository (usually on the left).
Just extract it, and you should have all the files you need.

Since installing does not work that well and easy on windows, we recommend to just copy the header files where you want them to be.
Then add this path to the include paths of your IDE(or pass it directly to the compiler). 
Since you do not have to link against anything, that's it.

##Contributing
Contributors to library, documentation or examples are always appreciated. Just fork it and start a pull request, start an issue or send me a mail at nyorain@gmail.com. If you just have any ideas, questions or suggestions, just let me know.
