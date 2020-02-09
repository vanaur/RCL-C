# Runtime Core Library

<img src="https://img.shields.io/badge/Made%20in-C-blue.svg?style=for-the-badge"> <img src="https://img.shields.io/github/stars/vanaur/RCL?color=blue&style=for-the-badge">

**Project goal**: A compiler backend library that allows the evaluation, analysis and compilation of programs, through an abstract stack-based virtual machine whose intermediate representation follows the concatenative paradigm; optimized for implicit data vectorization, big numbers and recursive passage styles.

The library provides compilation help features adapted to all types of languages and paradigms.

## Getting Started
The project is not sufficiently developed to be available from a package manager. In the meantime, you can build it from the makefile, which has been written in such a way as to accompany you as best as possible.

## Prerequisites

The dependencies of external dynamic libraries are:

 - [libffi](https://sourceware.org/libffi/)
 - [gmp](https://gmplib.org/)

The programs needed to build the project:

 - makefile
 - gcc
 - (flex)
 - (yacc)

## Installing

At the moment, with `make`:

```
cd RCL/
make ALL
```

Will build the VM as executable one, the library as dynamic linking one and as static linking one, in a file called `All RCL/`.

## Testing

If you're using the library, don't forget use `include` file in your test, with gcc: `-Iinclude` and of course the library that you can link with `-lrcl`. Make sure these files are accessible from the command prompt you're using to build and testing.

The function `rcl__hello_world()` can be used from `Library\rclib.h`, it will display a `Hello, world!` for the RCL.

For the executable, you can try to launch it with a file containing some IR code, try:

**test.clir**
```
3 2 +
```
and
```
./RCL test.clir
```
This should display the result as well as some information.

## Contributing

If you have constructive questions, suggestions for improvements or features to be added, bug or performance reports, then check if a report has not already been raised in [issues](https://github.com/vanaur/RCL/issues), in which case it has probably already been processed or is being maintained, otherwise feel free to do so and/or actively participate in the project by forking it. Amendments will be made if they are relevant.

## License
This project is licensed under the Apache 2.0 License. See the [license](https://github.com/vanaur/RCL/blob/master/LICENSE) file.

