About pretty_output
===================
This is a library for pretty printing information about a code. Those who prefer using console output for debugging purposes might consider this library as a more useful alternative to printf/std::cout/whatever.


Features:
* Easy to use and extend
* Uses only C++/C++11. Does not use any additional preprocessors or libraries, except standard library
* Crossplatform. Tested on Clang 600.0.56, MVS 2010
* Free for all

Usage
=====

`$w(epression)` - print value of the `expression` and return that value. Can be used inside other expression. The name is an abbreviation of 'watch'.

Examples:

```C++
int i = 455;
$w(i + 1);
```

```
main.cpp:6    |  i + 1 = 456
```
--

```C++
std::string s = "hellomoto!";
$w(s);
```

```
main.cpp:9    |  s = "hellomoto!"
```
--

```C++
float *pf = new float(789.0f);
$w(pf);
```

```
main.cpp:12   |  pf = 0x7fd9a8c04bf0 -> 789
```
--

```C++
const char *c = NULL;
$w(c);
```

```
main.cpp:15   |  c = (null)
```
--

```C++
int r = 123 + $w(*pf);
```

```
main.cpp:9    |  *pf = 789
```
--

`$f` - print function or member-function 'call' and 'return' labels. Should be used inside a function or member-function. Automatically shifts indentation of the output. The name is an abbreviation of 'function'.

Example:

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

```
main.cpp:10   |  [call] int main()
main.cpp:12   |      i = 456
main.cpp:4    |      [call] void func()
main.cpp:6    |          f = 789
main.cpp:4    |      [ret]  void func()
main.cpp:10   |  [ret]  int main()
```
--

`$c(function)` - print `function` arguments and return value. Should be used at function call. Automatically shifts indentation of the output. The name is an abbreviation of 'call'.

Example:

```C++
void func(int i, float f, const std::string &s)
{
}

...

int i = 456;
$c(func)(i, 789, "hellomoto!");
```

```
main.cpp:11   |  func(456, 789, hellomoto!)
```
--

`$m(object, function_name)` - print member-function arguments and return value. Should be used at member-function call. `object` argument can be of a pointer or non-pointer type. The name is an abbreviation of 'member-function'.

Example:

```C++
class some
{
public:
	void func(int i, float f, const std::string &s)
	{
	}
};

...

some obj;
int i = 456;
$m(obj, some::func)(i, 789, "hellomoto!");
```

```
main.cpp:15   |  some::func(456, 789, hellomoto!)
```
--

`$return` expression - print value of epxression passed to the return statement.

Example:

```C++
int add(int a, int b)
{
	$return a + b;
}

...

add(456, 789);
```

```
main.cpp:5    |  return 1245
```
--

`$if (condition)` - print value of the if `condition`. Automatically shifts indentation of the output.

Example:

```C++
int i = 0;
$if (i < 2)
{
}
```

```
main.cpp:7    |  if (i < 2) => true
```
--

`$for (statements)` - print iteration numbers of the for loop. Automatically shifts indentation of the output.

Example:

```C++
$for (int i = 0; i < 3; ++i)
{
	$w(i);
}
```

```
main.cpp:6    |  for (int i = 0; i < 3; ++i)
main.cpp:6    |  [iteration #0]
main.cpp:8    |      i = 0
main.cpp:6    |  [iteration #1]
main.cpp:8    |      i = 1
main.cpp:6    |  [iteration #2]
main.cpp:8    |      i = 2
```
--

```C++
int arr[] = {4, 5, 6};
$for (auto i : arr)
{
	$w(i);
}
```

```
main.cpp:7    |  for (auto i : arr)
main.cpp:7    |  [iteration #0]
main.cpp:9    |      i = 4
main.cpp:7    |  [iteration #1]
main.cpp:9    |      i = 5
main.cpp:7    |  [iteration #2]
main.cpp:9    |      i = 6
```
--

`$while (condition)` - print iteration conditions of the while loop. Automatically shifts indentation of the output.

Example:

```C++
int i = 0;
$while (i < 3)
{
	$w(i);
	++i;
}
```

```
main.cpp:7    |  while (i < 3) => true
main.cpp:9    |      i = 0
main.cpp:7    |  while (i < 3) => true
main.cpp:9    |      i = 1
main.cpp:7    |  while (i < 3) => true
main.cpp:9    |      i = 2
main.cpp:7    |  while (i < 3) => false
```
--

`$_` - Adds and removes indentation in the containing scope.

Example:

```C++
int i = 456;
$w(i);

{$_
	$w(i);
}

$w(i);
```

```
main.cpp:7    |  i = 456
main.cpp:10   |      i = 456
main.cpp:13   |  i = 456
```
--

`$p(format, ...)` - like `printf`. The name is an abbreviation of 'printf'.

Example:

```C++
$p("%i %f %s", 456, 789.0f, "hellomoto!")
```

```
main.cpp:14   |  456 789.000000 hellomoto!
```
--

`$t(thread_name)` - set thread name, that will be printed in the thread header. The name is an abbreviation of 'thread'.

Example:

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

Noes
=====
* Macros $c and $m work only with C++11 and later.
* There is an output synchronization that prevents outputs from different threads mixing up. By default this feture is turned on. To disable this synchronization define macro `PRETTY_OUTPUT_NO_OUTPUT_SYNC`.
* If you want to output your class/struct/whatever, you should overload `std::ostream &operator <<(std::ostream &, <your_type>)`.

Future possible features
========================
* `$time` and `$cpu_time` macros to measure the time for which the code is executed
* `$d(ptr, size)` for printing memory hex dump
* output redirection

