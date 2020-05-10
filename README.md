# Runtime Core Library 🧙

<img src="https://img.shields.io/badge/Made%20in-C-blue.svg?style=for-the-badge"> <img src="https://img.shields.io/github/stars/vanaur/RCL?color=blue&style=for-the-badge"> <img src="https://img.shields.io/badge/Version-0.1-brown.svg?style=for-the-badge">

## List of content

 - [TL;DR](#tldr)
 - [The project idea](#the-project-idea)
 - [Getting started](#getting-started)
 - [Installing](#installing)
 - [Testing and examples](#testing-and-examples)
 - [Contributing](#contributing)
 - [Questions & answers](#questions--answers)
 - [Notes on the project](#notes-on-the-project-and-its-stability--construction)
 - [Ressources](#ressources)
 - [Version 2.0 Forecast](#version-20-forecast)
 - [License](#license)

## TL;DR
A compiler frontend and backend library that allows the evaluation, analysis and compilation of programs, through an abstract stack-based virtual machine whose intermediate representation follows the concatenative paradigm; optimized for implicit data vectorization, big numbers and recursive passage styles.

The library provides compilation help features adapted to all types of languages and paradigms.

## The project idea
The Runtime Core Library, abbreviated RCL, is a high-level program compilation, evaluation and diagnostic library.

The library provides everything you need to adapt the compilation to your language constructs and also gives access to a wide range of prefabricated compilation help modules, including full support for imperative paradigm, functional paradigm, object paradigm, logical paradigm, and expression evaluation methods: strict, lazy, and perhaps optimistic (to plan).

The execution machine is based on an abstract and high-level stack-based IR language following the concatenative paradigm. The means of evaluation are interpretation, JIT and AOT compilation. Code generation (to compile into an executable / JIT) is fully optimized for recursion, parallelism (SMID), asynchronous execution, very large numbers computation, memory management, as well as the size of the resulting file.

**The goal** of the RCL is to provide a way to compile, evaluate and diagnose, all in one, high level, theoretical IR from an abstract VM.

## Getting Started
The project is not sufficiently developed to be available from a package manager. In the meantime, you can build it from the makefile, which has been written in such a way as to accompany you as best as possible.

### Prerequisites

The dependencies of external dynamic libraries are:

 - [libffi](https://sourceware.org/libffi/) (used for FFI with C)
 - [gmp](https://gmplib.org/) (used for large number arithmetic)

The programs needed to build the project:

 - Makefile
 - GCC

## Installing

At the moment, with `make`:

```
cd RCL/
make LIB & make DLL & make VM & make clean
```

Will build the VM as executable one, the library as dynamic linking one and as static linking one.

## Testing and examples

<details><summary><b>Use the executable</b></summary>

The generated executable allows you to use the VM (only) through the REPL, but can also run entire files and programs.

Launch the executable with the option **`--repl`** and enter the interactive environment.

| COMMAND 	| Description                                                   	|
|---------	|---------------------------------------------------------------	|
| STACK   	| Displays the current program stack                            	|
| FREE    	| Deletes created variables, structures and functions           	|
| QUIT    	| Exit the program                                              	|
| HELP    	| Display the help menu                                         	|
| TYPEOF  	| Determines the type of the given expression                   	|
| STEP    	| Evaluates the given expression steps by steps                 	|
| EXEC    	| Evaluates the given expression                                	|
| EXECT   	| Evaluates the given expression and display the execution time 	|
| SET     	| Defines a new given option                                    	|
| OOF     	| Determines the algorithmic complexity of the given expression 	|
| OPT     	| /                                                             	|
| SHOW    	| /                                                             	|

You can enter an arithmetic expression with the reverse Polish syntax: `3 7 + 2 * 10 /`. Think of it as instructions on a stack:

```arm
load 3
load 7
add
load 2
mul
load 10
div
```

You can also send an IR file to the executable, it will evaluate it, for example:
```rust
.define
    // An optimized factorial function (with recursion)
    fac_acc =
        dup 1 swap lw quote
            [pop]
            [dup flip * swap -- fac_acc]
        ifte ;
    fac = 1 swap fac_acc ;

.main
    817 x\
    "The factorial of x = " puts
    x fac nvshow puts
```

To evaluate a file, simply pass it as a program argument. You can always use the REPL over it, as well as the functions that have been defined in the program.

</details>

<details><summary><b>Use the library</b></summary>

If you're using the library, make sure that the [header files](include/) are accessible from your compiler. and of course the library that you can link with `-lrcl`. Make sure these files are accessible from the command prompt you're using to build and testing.

All the features you'll need will be found in `RCL\Library\`.

The function `rcl_hello_world()` can be used from `RCL\Library\rclib.h`, it will display a `Hello, world!` for the RCL:

```c
#include <RCL\Library\rclib.h>

int main(int argc, char *argv[])
{
    // Calling the 'Hello, world!' function
    rcl_hello_world();
    return EXIT_SUCCESS;
}
```
If you want something more concrete, you can describe the AST of a program:

```c
#include <RCL\Library\rclib.h>
#include <RCL\Library\Primitive\Expression.h>

int main(int argc, char *argv[])
{
    rcl_expr_t cst_n1 = rcl_expr_cst_int(4);
    rcl_expr_t cst_n2 = rcl_expr_cst_int(8);
    rcl_expr_t my_exp = rcl_expr_add(cst_n1, cst_n2);

    printf("Input: 4 + 8\n");
    printf("RCL display: %s\n", rcl_show_ir_expr(my_exp));
    
    struct IResult res = rcl_evali_expr(my_exp);

    printf("Resulting: %d\n", res.main_returned);
    
    return EXIT_SUCCESS;
}
```

Or, if you don't like variables:

```c
#include <RCL\Library\rclib.h>
#include <RCL\Library\Primitive\Expression.h>

int main(int argc, char *argv[])
{
    rcl_expr_t my_exp =
        rcl_expr__mul(
            rcl_expr__var("4"),
            rcl_expr__add(
                rcl_expr__cst_int(3),
                rcl_expr__div(
                    rcl_expr__var("6"),
                    rcl_expr__cst_int(2))));

    printf("Input: 4 * (3 + (6 / 2))\n");
    printf("RCL display: %s\n", rcl_show_ir_expr(my_exp));
    
    struct IResult res = rcl_evali_expr(my_exp);

    printf("Resulting: %d\n", res.main_returned);
    
    return EXIT_SUCCESS;
}
```

Compile: `gcc -lrcl main.c -o main`.

Don't worry, it sounds barbaric when you put it that way, but what we've done here is really nothing more than the work your compiler will do from AST nodes.

</details>

## Contributing

If you have constructive questions, suggestions for improvements or features to be added, bug or performance reports, then check if a report has not already been raised in [issues](https://github.com/vanaur/RCL/issues), in which case it has probably already been processed or is being maintained, otherwise feel free to do so and/or actively participate in the project by forking it. Amendments will be made if they are relevant.

## Questions & Answers

A list of anticipated questions that I think are legitimate, for those interested in the project...

<details><summary><b>What does RCL bring compared to LLVM or other compilation tool chains?</b></summary>
The project is still very young, so absolutely nothing at the moment. It is rather a "proof of concepts" intended to put forward an abstract and functional intermediate representation. Nevertheless, this supposes, for more advanced versions of the project, that the lib will manage all the technical details (performance, memory, pointers, ...) that it is sometimes more complicated to manage.
</details>

<details><summary><b>Could we consider using RCL for low-level languages?</b></summary>
A priori, this would not be impossible, however the resulting code would not be low-level and would probably not correspond at all to what is expected from a compiler for low-level languages.
</details>

<details><summary><b>Do we need to know the IR specifications to compile to RCL?</b></summary>
It's always useful to have a deeper knowledge of what you're using, but the library will provide layers of abstraction so that you don't have to worry about the resulting code/IR at compile time at all. So, no!
</details>

## Notes on the project and its stability / construction

<details><summary><b>Motivations</b></summary>
There are 3 reasons that pushed me to realize this project:

 - I'm in the process of creating a programming language, I wanted a way to compile, interpret and analyze my executions. There are of course other popular VMs that do a great job, but I wanted to have something of my own, as well as open to proposals and contributions for other projects.
 - The fields of programming languages, their implementation and compilers are part of what I prefer in computer science. In order to better understand their internal workings, I wanted to embark on a "large scale" project in this direction.
 - A lot of VMs or tools to help evaluate and compile languages of all kinds require quite a lot of effort, and most of the time use a machine-like intermediate representation (actually, I don't know of any that don't do otherwise (but it must surely exist)). I wanted to "put aside" this way of doing things a bit, to experiment with a different approach in representation and compilation as well as use.
</details>

<details><summary><b>Stability</b></summary>
The RCL is a relatively "old" project for me, its idea goes back to April 2018, but I didn't have enough programming and computer knowledge to start it. In the meantime the ideas were born, but still no realization. This repository presents the first version of the RCL I wrote in C, its base dates from March 2019.

For me it was therefore a project of (re)learning compilers, language theory, and everything that comes close to that (with of course learning C and project management). So there are some elementary parts of the RCL that are old and would be hard to modify, however this is part of my goals.

In terms of "frondend", few changes should be made on what already exists, it's more about the "backend" (at the time I didn't care about freeing the allocated memory!). In the long term, all these old foundations should be updated properly, however, in the meantime, the project remains fully usable, it is rather bad practice in the [/Core](source/VM/Core).
</details>

<details><summary><b>Usable for "real projects"?</b></summary>
Of course, these are big words compared to the current progress. What I mean by this is that, in the long term, RCL being a potential choice for the compilation backend for languages, in the sense of its capabilities.
</details>

## Ressources

[Inventory of links](.ressources/README.md) to interesting resources on the subject of compilation, intermediate representations and optimizations.

## Version 2.0 Forecast
##### These are ideas of the features I am considering for a future version of the project

The current version is a prototype under construction which aims to be really usable (I'd like in any case 😛). This version is therefore relatively rigid to extensions and customizations as well as limited in terms of code generation backend or optimizations.

The RCL 2.0 version should build on this prototype in order to extend its capabilities to allow you to broaden your quality and personality for compiling and evaluation.

Indeed, version 2.0 provides a very modular and customizable as well as very easily usable and exportable way to extend the RCL's functionalities. Here are the key features that this version will try to achieve:

 - Customization of optimization runs ;
 - Definition of compilation options ;
 - Definition of new compilation and JIT backends ;
 - Definition of customized optimizations on IR ;
 - Definition of customized optimizations on the backend. 

The configurations would be done through the analysis of a Json file.

## License
This project is licensed under the Apache 2.0 License. See the [license](https://github.com/vanaur/RCL/blob/master/LICENSE) file.

