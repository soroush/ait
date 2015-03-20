# Artificial Intelligence Toolkit
AIT is a C++ library aimed to provide a concrete framework for artificial
intelligence research projects. It's also intended to fill the gap between
academy and industry in area of AI by providing high performance implementation
of generic AI concepts and techniques.

# Installation
You may either compile libAIT or download precompiled binaries. Currently a
debian package repository is maintained in
[~soroush/solap](https://launchpad.net/~soroush-r/+archive/solap) ppa. Also
precompiled DLL's for Windows are available in sourceforge home page of the
project.

**WARNING**: There is no support for Microsoft Visual Studio compilers. Please
use latest GNU GCC to compile this project. Earliest MSVC that's able to compile
AIT, will be released in 2015. Current versions do not have capability to
compile modern C++ code.

## Compile

### Dependencies
AIT has a modular architecture by design. Each module has it's own
dependencies.

#### Linux

    ./configure --prefix=/usr/local
    make
    sudo make install

#Usage
## C++
libAIT comes with a comprehensive C++ API. This API is an interface to all features of the library, and it also can be used to extend the library. 

## C
In addition to C++ API, a C wrapper API is implemented which can be used to link this library against standard C code. C API can not be used to extend library.

## Python
AIT-Py is a wrapper implementations for Python in a separate project which can be found at [github.com/soroush/ait-py](https://github.com/soroush/ait-py). As like as C API, this wrapper can not be used to extend library, though you can use all features of AIT library within native Python API.