About pretty_output
===================

This is a library for pretty printing information about a code. Those who prefer using console output for debugging purposes might consider this library as a more useful alternative to printf/std::cout/whatever.


Features:

* Easy to use, but right now not easy to extend
* Uses only C++/C++11. Does not use any additional preprocessors or libraries, except standard library
* Crossplatform. Tested on Clang 600.0.56, MVS 2010
* Free for all

Usage
=====

`$w(expression)` - print value of `expression` and returns that value, so can be used inside other expression.
The name is an abbreviation of 'watch'.

Code:
```C++
int i = 455;
$w(i + 1);

std::string s = "hellomoto!";
$w(s);

float *pf = new float(789.0f);
$w(pf);

const char *c = NULL;
$w(c);

int r = 123 + $w(*pf);
```

Output:
```
main.cpp:6    |  i + 1 = 456
main.cpp:9    |  s = "hellomoto!"
main.cpp:12   |  pf = 0x7ff8e2404bf0 -> 789
main.cpp:15   |  c = (null)
main.cpp:17   |  *pf = 789
```
---

`$m(pointer, size, base, byte_order)` - print memory under the `pointer`.

`$m(variable, base, byte_order)` - print memory of the `variable`.

`pointer` - address of the memory to be printed. The type of the pointer determines the grouping of bytes and default `base`. For example memory under the `unsigned char*` pointer will be grouped by 1 byte and use hexadecimal numbers; memory under `int*` will be grouped by 4 bytes and use signed decimal numbers. For unknown types default grouping is by 1 byte and base is hexadecimal.

`variable` - variable, memory of which will be printed. Parameter `size` should not be provided. Default grouping is 1 byte, default `base` is hexadecimal.

`size` - size of memory is bytes.

`base` (optional) - numeric base for value representation. Can have following values:
* `pretty_output::BASE_BIN` - binary
* `pretty_output::BASE_SDEC` - signed decimal
* `pretty_output::BASE_UDEC` - unsigned decimal
* `pretty_output::BASE_HEX` - hexadecimal (default)

`byte_order` - order of the bytes to use when converting bytes to the numeric values. Can have following values:
* `pretty_output::BYTE_ORDER_LITTLE_ENDIAN`
* `pretty_output::BYTE_ORDER_BIG_ENDIAN`

Default value is determined automatically.

The name is an abbreviation of 'memory'.

Code:
```C++
struct s_t
{
	int i;
	float f;
	char c;
} s;

s.i = 456;
s.f = 789.123f;
s.c = 'r';

$m(s);
```

Output:
```
main.cpp:17   |  dump of s:
              |      0x7fff54376b10: c8 01 00 00 df 47 45 44 72 02
              |      0x7fff54376b1a: 60 0a
              |
```
---

`$f` - print function or member-function call and return labels. Should be used inside a function or member-function. Automatically shifts indentation of the output.
The name is an abbreviation of 'function'.

Code:
```C++
void func()
{$f
	float f = 789.0f;
	$w(f);
}

int main()
{$f
	int i = 456;
	$w(i);
	func();

	return 0;
}
```

Output:
```
main.cpp:10   |  [call] int main()
main.cpp:12   |      i = 456
main.cpp:4    |      [call] void func()
main.cpp:6    |          f = 789
main.cpp:4    |      [ret]  void func()
main.cpp:10   |  [ret]  int main()
```
---

`$c(function)` - print `function` arguments and return value. Should be used at function call. Automatically shifts indentation of the output.
The name is an abbreviation of 'call'.

Code:
```C++
void func(int i, float f, const std::string &s)
{
}

int i = 456;
$c(func)(i, 789, "hellomoto!");
```

Output:
```
main.cpp:11   |  func(456, 789, hellomoto!)
```
---

`$cm(object, function_name)` - print member-function arguments and return value. Should be used at member-function call. `object` argument can be of a pointer or non-pointer type.
The name is an abbreviation of 'member-function'.

Code:
```C++
class some
{
public:
	void func(int i, float f, const std::string &s)
	{
	}
};

some obj;
int i = 456;
$cm(obj, some::func)(i, 789, "hellomoto!");
```

Output:
```
main.cpp:15   |  obj.func(456, 789, hellomoto!)
```
---

`$return expression` - print value of `epxression` passed to return statement.

Code:
```C++
int add(int a, int b)
{
	$return a + b;
}

add(456, 789);
```

Output:
```
main.cpp:5    |  return 1245
```
---

`$if (condition)` - print value of the if `condition`. Automatically shifts indentation of the output.

Code:
```C++
int i = 0;
$if (i < 2)
{
}
```

Output:
```
main.cpp:7    |  if (i < 2) => true
```
---

`$for (statements)` - print iteration numbers of the for loop. Automatically shifts indentation of the output.

Code:
```C++
$for (int i = 0; i < 3; ++i)
{
	$w(i);
}
```

Output:
```
main.cpp:5    |  for (int i = 0; i < 3; ++i)
main.cpp:5    |  [iteration #0]
main.cpp:7    |      i = 0
main.cpp:5    |  [iteration #1]
main.cpp:7    |      i = 1
main.cpp:5    |  [iteration #2]
main.cpp:7    |      i = 2
```
---

`$while (condition)` - print iteration conditions of the while loop. Automatically shifts indentation of the output.

Code:
```C++
int i = 0;
$while (i < 3)
{
	$w(i);
	++i;
}
```

Output:
```
main.cpp:7    |  while (i < 3) => true
main.cpp:9    |      i = 0
main.cpp:7    |  while (i < 3) => true
main.cpp:9    |      i = 1
main.cpp:7    |  while (i < 3) => true
main.cpp:9    |      i = 2
main.cpp:7    |  while (i < 3) => false
```
---

`$_` - shift indentation in the containing scope.

Code:
```C++
int i = 456;
$w(i);

{$_
	$w(i);
}

$w(i);
```

Output:
```
main.cpp:7    |  i = 456
main.cpp:10   |      i = 456
main.cpp:13   |  i = 456
```
---

`$p(format, ...)` - like printf. The name is an abbreviation of 'printf'.

Code:
```C++
$p("%i %f %s", 456, 789.0f, "hellomoto!")
```

Output:
```
main.cpp:14   |  // 456 789.000000 hellomoto!
```
---

`$t(thread_name)` - set thread name, that will be printed in the thread header. The name is an abbreviation of 'thread'.

Code:
```C++
void func()
{$t(worker)
	int arr[] = {1, 2, 3};
	$for (auto i : arr)
	{
		$w(i);
	}
}

int main()
{$t(main) $f

	std::thread t1(func);
	std::thread t2(func);
	t1.join();
	t2.join();

	$return 0;
}
```

Output:
```
[Thread: 0x7fff71013300 main]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:16   |  [call] int main()

[Thread: 0x108285000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:9    |  for (auto i : arr)

[Thread: 0x108308000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:9    |  for (auto i : arr)

[Thread: 0x108285000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:9    |  [iteration #0]

[Thread: 0x108308000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:9    |  [iteration #0]

[Thread: 0x108285000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:11   |      i = 1

[Thread: 0x108308000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:11   |      i = 1

[Thread: 0x108285000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:9    |  [iteration #1]

[Thread: 0x108308000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:9    |  [iteration #1]

[Thread: 0x108285000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:11   |      i = 2

[Thread: 0x108308000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:11   |      i = 2

[Thread: 0x108285000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:9    |  [iteration #2]

[Thread: 0x108308000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:9    |  [iteration #2]

[Thread: 0x108285000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:11   |      i = 3

[Thread: 0x108308000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:11   |      i = 3

[Thread: 0x7fff71013300 main]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            main.cpp:23   |      return 0
            main.cpp:16   |  [ret]  int main()
```


Options
=======

`PRETTY_OUTPUT_ON` - turn pretty_output on.

`PRETTY_OUTPUT_OFF` - turn pretty_output off.

`PRETTY_OUTPUT_WIDTH` - width, to which output is wrapped (actually wrapping only a thread header and dump output). Default is 79.

`PRETTY_OUTPUT_THREAD_HEADER_SEPARATOR` - char that is used as horizontal separator for a thread header. Default is a `'~'`.

`PRETTY_OUTPUT_FILENAME_FIELD_WIDTH` - width of the file name field of the output. Default is 20.

`PRETTY_OUTPUT_FILENAME_FIELD_EXCESS_PADDING` - string, that is used to indicate wrapped part of the filename. Default is `"~"`.

`PRETTY_OUTPUT_LINE_FIELD_WIDTH` - width of the file line field of the output. Default is 4.

`PRETTY_OUTPUT_DELIMITER` - string, that is used as a delimiter between 'file_name:line' field and the actual output. Default is `" |  "`.

`PRETTY_OUTPUT_INDENTATION` - string, that is used as an indentation for the actual output. Default is `"    "` (4 spaces).

`PRETTY_OUTPUT_NO_OUTPUT_SYNC` - disables output syncronization. Read details in the 'Notes' section.

`PRETTY_OUTPUT_REDIRECTION_H` - header file, which contains overrided printing routines. Read details in the 'Notes' section.


Notes
=====

* If macro `NDEBUG` is not defined or `PRETTY_OUTPUT_ON` is defined then the pretty_output is turned on. If `NDEBUG` or `PRETTY_OUTPUT_OFF` is defined then the pretty_output is turned off.

* Macros `$c` and `$cm` work only with C++11 and later.

* There is an output synchronization that prevents outputs from different threads mixing up. By default this feature is turned on. To disable this synchronization define macro `PRETTY_OUTPUT_NO_OUTPUT_SYNC`.

* Output redirection is done in a little tricky way. You should declare `void pretty_output_print(const char *)` and `void pretty_output_flush()` functions in a separate header file and define macro `PRETTY_OUTPUT_REDIRECTION_H` with a name of that header file. For your convinience there's already files for redirecting output to a file (pretty_output_to_file.{h,cpp}) and for printing to MVS debug output (pretty_output_to_mvs.h). When using pretty_output_to_file, you can define macro `PRETTY_OUTPUT_TO_FILE` with the name of the destination file (default is 'pretty_output_log.txt').
