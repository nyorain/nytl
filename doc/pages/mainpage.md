nytl documentation                      {#mainpage}
==================

Welcome to the nytl documentation. nytl is a __header-only__ utility library for __c++11__. It includes __math__ classes and operations like nytl::vec, nytl::mat or nytl::rect, a smaller part about __functions__
and function-conversions with classes like nytl::compatibilityFunction, nytl::callback or nytl::function_traits, and additionally some general __utility__ stuff, like some c++14 implementation (if your compiler still just
supports c++11, but you often need features like make_unique, integer_sequence or apply) or serialzer class and macros.

At the moment ny has about 5000 lines of pure code, so it is still pretty lightweight. If you use nearly all fils in you project, it may seriously speed-up your build process if you just include nytl.hpp
everywhere (all nytl header files) and use a pre-compiled header, they can work really well. But if you just need a few files, it is much easier to just include them, and will not really slow down your build process.
The name nytl does stand for ny-util(or ny-utility) as well as ny-template-library, since it was originally created for the __ny__ gui library.
Besides the doxygen-generated documentation, it includes stuff like tutorials or code examples.

At the moment, this documentation is, as the library itself, in a pre-alpha phase and nothing is guaranteed to be working or correct.
So, contributions or corrections to the documentation, as well as to the library itself, are always appreciated.
Just have a look at the github page: https://github.com/nyorain/nytl

##Categories
* @ref getting-started
* @ref nytl-math
* @ref nytl-utility
* @ref nytl-function
* @ref examples
* @ref about

If you have an idea for a new page, or want to add one here, just let us know.

