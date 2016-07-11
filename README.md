# trace-out

printf debugging done the right way.

### Features

* Pretty-prints all the required information about code.
* Uses only C++/C++11, standard and system libraries; does not use any external preprocessors or libraries.
* Supports output redirection.
* Crossplatform. Tested with:
  * Apple LLVM version 7.3.0 (clang-703.0.29)
  * GCC 4.9.2
  * MinGW 4.9.3
  * Microsoft Visual Studio 2010, 2012, 2013, 2015
* Free for all ([MIT license](LICENSE.txt)).


### Example

Code:

```c++
#include "trace-out.hpp"

int main()
{
    $f // pretty-print when function is called and returned

    int answer {42};
    std::string moto {"hellomoto!"};

    $w(answer, moto) // print "answer" and "moto" values

    $if (answer == 42) // print condition value
        $return answer; // print return value

    $return 0; // print return value
}
```

Output:

```
[Thread: 0x7fff77293000]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:5    |  int main()
                          |  {
            main.cpp:10   |      answer = 42
                          |      moto = "hellomoto!"
            main.cpp:12   |      if (answer == 42) => true
            main.cpp:13   |          return 42
                          |          
            main.cpp:5    |  }    // int main()
                          |
```



# Usage

Simply add source code to your project and you're ready to use it.

trace-out's interface is based on macros that pretty-print information about their arguments.


### Macros

`$w(<variable1>, <variable2>, ...)` – print values of listed variables (supported with C++11 and later).

`$e(<expression>)` – print value of passed in expression and return that value (can be used inside other expressions).

`$m(<pointer>, <size>, <base> | <byte-order>)` – print memory under `<pointer>`.

`<pointer>` – address of the memory to be printed. Type of the pointer defines byte grouping and default `<base>`. For example, memory under `unsigned char *` pointer will be grouped by 1 byte and hexadecimal numbers will be used; memory under `int *` will be grouped by 4 (or 8) bytes and signed decimal numbers will be used. For unknown types default grouping is by 1 byte and numerical base is hexadecimal.

`<size>` – size of memory in bytes.

`<base> | <byte-order>` (both optional) – numerical base for value representation and order of the bytes to use when converting bytes to the numeric values.

Numerical base flags (default value is determined as described above):

* `trace_out::BIN` – binary
* `trace_out::SDEC` – signed decimal
* `trace_out::UDEC` – unsigned decimal
* `trace_out::HEX` – hexadecimal (default)
* `trace_out::FLT` – single precision floating point number
* `trace_out::DBL` – double precision floating point number
* `trace_out::LDBL` – floating point number with precision equal to `long double` for current platform

Byte order flags (default value is determined automatically):

* `trace_out::LITTLE` – big-endian
* `trace_out::BIG` – little-endian

`$f` – print function call and return labels. Should be used inside a function and preferably be the first expression in it.

`$return <expression>` – print expression value passed to the `return` statement.

`$if (<condition>)` – print condition value of the `if` statement.

`$for (<statements>)` – print number for each iteration of the `for` loop.

`$while (<condition>)` – print condition value for each iteration of the `while` loop.

`$p(<format>, ...)` – `printf`-like function.

`$thread(<name>)` – set thread name that will be printed in the thread header.

--

Macros `$w`, `$e`, `$return`, `$if` and `$while` support following types:

* all fundamental types, raw pointers, standard smart pointers, `std::string`, `std::pair`, `std::tuple`
* iterable types – for which `std::begin()` and `std::end()` are defined (supported with C++11 and later)
* sturctures and classes with data members `x`, `y`, `z`, `width`, `height`, `origin` and `size` in lowcase, HIGHCASE and Capital

--

Macros `$f`, `$if`, `$for` and `$while` automatically shift indentation of the output inside their blocks.

--

Output is flushed before reading variables and dereferencing pointers that are passed from the outer context, thus it is more clear where things went wrong when memory was corrupted.

Code:

```c++
int bueno {456};
int &no_bueno {*(int *)nullptr};
$w(bueno, no_bueno) // will show "Segmentation fault" when try to read "no_bueno" value
```

Output:

```
[Thread: 0x7fff77293000]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:3    |  int main()
                          |  {
            main.cpp:5    |      bueno = 456
                          |      no_bueno = Segmentation fault: 11
```



### Output Redirection

To make custom redirection implement following functions within some namespace in `.cpp` file (no header required):

* `void print(const char *string)` – print `string`
* `void flush()` – flush output
* `size_t width()` – get width of the output in characters

To use custom redirection you should:

* Add `<redireciton>.cpp` file to a project.
* Define macro `TRACE_OUT_REDIRECTION` with a name of the namespace where redirection functions are defined.

--

There are built-in implementations for redirecting output to Windows debug output and to a file: `trace_out_to_wdo` and `trace_out_to_file` respectively. By defulat `trace_out_to_file` saves output to `trace-out.txt`. To change this define `TRACE_OUT_TO_FILE` with desired file name. No `<redirection>.cpp` files required when using these built-in redirections.

### Notes

trace-out is turned on if `NDEBUG` is not defined or `TRACE_OUT_ON` is defined; turned off if `NDEBUG` or `TRACE_OUT_OFF` is defined.

There is an output synchronization that prevents outputs from different threads to mix up. By default it is turned on. To disable syncronization define macro `TRACE_OUT_NO_OUTPUT_SYNC`.



# Option `#define`s

`TRACE_OUT_ON` – turn trace-out on.

`TRACE_OUT_OFF` – turn trace-out off.

`TRACE_OUT_REDIRECTION` – use redirection functions from namespace that is used to define this macro.

`TRACE_OUT_WIDTH` – width to which output is wrapped (actually only the thread header and memory output are wrapped). This macro overrides value returned by `<redirection_namespace>::width()` function. Default value for standard output is `79`.

`TRACE_OUT_INDENTATION` – string that is used as an indentation for the actual output. Default value is `"    "` (4 spaces).

`TRACE_OUT_NO_OUTPUT_SYNC` – disable output syncronization.



# Troubleshooting

* Passing variable or expression of unknown type to `$w`, `$e`, `$return`, `$if` or `$while` will cause multiple compiler errors like `candidate template ignored: substitution failure [with Type_t = <your-unknown-type>]`.

	*Fix:* try to cast to one of the supported types or try to pass single fields to these macros if type is struct/class/union.

* Using precompiled headers with Visual Studio will cause compiler error.

	*Fix #1:* exclude `trace-out.cpp` from precompilation process.

	*Fix #2:* the precompiled header should be manually included in the `trace-out.cpp` file.