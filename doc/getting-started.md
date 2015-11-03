Getting Started  {#getting-started}
==========

###Building nytl
Since nytl is a header-only library you dont have to compile anything, nytl has __no external dependencies__.
Start by cloning the nytl repository from github:
```
git clone https://github.com/nyorain/nytl
cd nytl
```

Now, you can either just copy all the header files, where you want them to be, or let cmake automatically install them as well as a pkg-config file (__not recommended on windows__).
All the header files are situated in the nytl folder directly at the project root.
cmake:
```
mkdir build
cd build
cmake ..
```

If you now want to configure and install everything just do:
```
make
make install
```

If you just want to make the documentation (remember that you need doxygen installed for that) just type `make doc`.
The documentation will then be located at build/doc/doxygen/html. Start at index.html.


###Windows
If you do not have git installed, or do not know how it works, no problem, you can also just download a zip-package from here: https://github.com/nyorain/nytl
Just extract it, and you should have all the files you need.

Since installing does not work that well and easy on windows, we recommend to just copy the header files where you want them to be.
Then add this path to the include paths of your IDE(or pass it directly to the compiler). 
Since you do not have to link against anything, that's it.