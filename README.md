# pretty_output

This is a library for pretty printing information about a code. Those who prefer using text output for debugging purposes might consider this library as a more useful alternative to printf/std::cout/whatever.



- [Features](#features)
- [Usage](#usage)
- [Macros](#macros)
- [Options](#options)
- [Notes](#notes)
- [Troubleshooting](#troubleshooting)



# Features

* Easy to use, but right now not easy to extend
* Uses C++/C++11 and standard library, does not use any additional preprocessors or libraries
* Supports output redirection
* Crossplatform. Tested on: Clang 602.0.49; MinGW 4.9.2; MVS 2010 (Professional), 2012 (Express), 2013 (Professional)
* No license, free for all



# Usage

To use pretty_output in a project, the "pretty_output" directory should be added to a project (excluding one of the "posix" or "windows" directories) and the header "pretty_output.h" included.

pretty_output is turned on if `NDEBUG` is not defined or `PRETTY_OUTPUT_ON` is defined. pretty_output is turned off if `NDEBUG` or `PRETTY_OUTPUT_OFF` is defined.


## Redirection

To use output redirection the following steps are required:
* The redireciton .cpp file should be added to a project
* Macro `PRETTY_OUTPUT_REDIRECTION` should be defined with a name of the namespace where redirection functions are defined

To implement custom redirection, the following functions should be defined within some namespace (the name of this namespace will be used to define macro `PRETTY_OUTPUT_REDIRECTION`):
* `void print(const char *string)` - print a `string`
* `void flush()` - flush output
* `size_t width()` - get width of the output (in characters)

There are implementations for redirecting output to a file, windows debugger output and via network in the [pretty_output_redirections](https://github.com/shrpnsld/pretty_output_redirections) repository.



# Macros

`$w(<expression>)` - print value of `expression` and return that value (can be used inside other expression).
The name is an abbreviation of "watch".

`$c(<function>)(<arguments>)` - print `function` arguments and return value. Should be used at function call. Automatically shifts indentation of the output.
The name is an abbreviation of "call".

`$cm(<object>, <function_name>)(<arguments>)` - print member-function arguments and return value. Should be used at member-function call. `object` argument can be of a pointer or non-pointer type.
The name is an abbreviation of "call member-function".

`$m(<pointer>, <size>, <options>)` - print memory under the `pointer`.

`pointer` - address of the memory to be printed. The type of the pointer determines the grouping of bytes and default `base`. For example memory under the `unsigned char *` pointer will be grouped by 1 byte and use hexadecimal numbers; memory under `int *` will be grouped by 4 bytes and use signed decimal numbers. For unknown types default grouping is by 1 byte and numerical base is hexadecimal.

`size` - size of memory in bytes.

`options` (optional) - numeric base for value representation and order of the bytes to use when converting bytes to the numeric values.

Numerical base flags:
* `pretty_output::BIN` - binary
* `pretty_output::SDEC` - signed decimal
* `pretty_output::UDEC` - unsigned decimal
* `pretty_output::HEX` - hexadecimal (default)
* `pretty_output::FLT` - single precision floating point number
* `pretty_output::DBL` - double precision floating point number
* `pretty_output::LDBL` - floating point number with precision equal to 'long double' for current platform

Byte order flags (default value is determined automatically):
* `pretty_output::LITTLE` - big-endian byte order
* `pretty_output::BIG` - little-endian byte order

The name is an abbreviation of "memory".

`$f` - print function call and return labels. Should be used inside a function. Automatically shifts indentation of the output.
The name is an abbreviation of "function".

`$return <expression>` - print value of `epxression` passed to the return statement.

`$if (<condition>)` - print value of the if `condition`. Automatically shifts indentation of the output.

`$for (<statements>)` - print iterations' numbers of the `for` loop. Automatically shifts indentation of the output.

`$while (<condition>)` - print iterations' conditions of the `while` loop. Automatically shifts indentation of the output.

`$p(<format>, ...)` - like `printf` function. The name is an abbreviation of "printf".

`$thread(<name>)` - set thread name, that will be printed in the thread header.



# Options

`PRETTY_OUTPUT_ON` - turn pretty_output on.

`PRETTY_OUTPUT_OFF` - turn pretty_output off.

`PRETTY_OUTPUT_WIDTH` - width, to which output is wrapped (actually only a thread header and memory output are wrapped). This macro overrides value returned by `<redirection_namespace>::width()` function. Default value for standard output is 79.

`PRETTY_OUTPUT_INDENTATION` - string, that is used as an indentation for the actual output. Default value is `"    "` (4 spaces).

`PRETTY_OUTPUT_NO_OUTPUT_SYNC` - disables output syncronization. Read details in the [Notes](#notes) section.

`PRETTY_OUTPUT_REDIRECTION` - namespace, that contains overridden redirection routines.



# Notes

* `$w`, `$c`, `$mc`, `$return`, `$if` and `$while` macros support printing of all fundamental types, iterable types (for which `std::begin()` and `std::end()` are defined), `std::pair`, `std::tuple`. Printing values of iterable types and macros `$c`, `$cm` are supported only with C++11 and later.

* Output is flushed before reading variables and dereferencing pointers that are passed from the outer context, thus it is more clear where things went wrong when memory has corrupted.

* There is an output synchronization that prevents outputs from different threads to mix up. By default this feature is turned on. To disable this synchronization the macro `PRETTY_OUTPUT_NO_OUTPUT_SYNC` should be defined.



# Troubleshooting

* Passing expression of type `char []` will cause compiler error. Fix: the expression should be cast to a desired type.

* Passing a pointer to a user-type will cause compiler error. Fix: the pointer should be cast to a `const void *` or `void *`.

* Using macros `$c` and `$cm` with overloaded functions will cause compiler error. Fix: the function name should be explicitly casted to a desired function type.

* Using precompiled headers with Visual Studio will cause compiler error. Fix: the precompiled header should be manually included in all used pretty_output source files; or not use precompiled header.

