Artificial Intelligence Toolkit
===

AIT is a C++ library aimed to provide a concrete framework for artificial
intelligence research projects. It's also intended to fill the gap between
academy and industry in area of AI by providing high performance implementation
of generic AI concepts and techniques.

Dependencies
=========

AIT has a modular architecture bu design. Each module has it's own
dependencies. 

Installation
======
You may either compile libAIT or download precompiled binaries. Currently a
debian package repository is maintained in
[~soroush/solap](https://launchpad.net/~soroush-r/+archive/solap) ppa. Also
precompiled DLL's for Windows are available in sourceforge home page of the
project.
#Compile
**WARNING**: There is no support for Microsoft Visual Studio compilers. Please
use latest GNU GCC to compile this project. Earliest MSVC that's able to compile
AIT, will be released in 2015. Current versions do not have capability to
compile modern C++ code. 
## Linux
<code>./configure --prefix=/usr/local
make
sudo make install</code>