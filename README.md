ABOUT

	This is a library for pretty printing information about a code.
	Those who prefer using console output for debugging purposes might
	consider this library as a more useful alternative to
	printf/std::cout/whatever.


	Features:

		* Easy to use and extend

		* Uses only C++/C++11. Does not use any additional
			preprocessors or libraries, except standard library

		* Crossplatform. Tested on Clang 600.0.56, MVS 2010

		* Free for all


	Project page:

		https://github.com/shrpnsld/pretty_output



HELP

$w(epression) - print value of expression and returns that value, so
can be used inside other expression.
The name is an abbreviation of 'watch'.

	Example:

	 4|
	 5|	int i = 455;
	 6|	$w(i + 1);
	 7|

	>
	>	main.cpp:6    |  i + 1 = 456
	>


	 7|
	 8|	std::string s = "hellomoto!";
	 9|	$w(s);
	10|

	>
	>	main.cpp:9    |  s = "hellomoto!"
	>


	10|
	11|	float *pf = new float(789.0f);
	12|	$w(pf);
	13|

	>
	>	main.cpp:12   |  pf = 0x7fd9a8c04bf0 -> 789
	>


	13|
	14|	const char *c = NULL;
	15|	$w(c);
	16|

	>
	>	main.cpp:15   |  c = (null)
	>


	16|
	17|	int r = 123 + $w(*pf);
	18|

	>
	>	main.cpp:9    |  *pf = 789
	>


$f - print function or member-function call and return labels. Should
be used inside a function or member-function. Automatically adds and
removes indentation to the output.
The name is an abbreviation of 'function'.

	Example:

	 2|
	 3|	void func()
	 4|	{$f
	 5|		float f = 789.0f;
	 6|		$w(f);
	 7|	}
	 8|
	 9|	int main()
	10|	{$f
	11|		int i = 456;
	12|		$w(i);
	13|		func();
	14|
	15|		return 0;
	16|	}
	17|

	>
	>	main.cpp:10   |  [call] int main()
	>	main.cpp:12   |      i = 456
	>	main.cpp:4    |      [call] void func()
	>	main.cpp:6    |          f = 789
	>	main.cpp:4    |      [ret]  void func()
	>	main.cpp:10   |  [ret]  int main()
	>


$c(function) - print function arguments and return value. Should be
used at function call. Automatically adds and removes indentation to
the output.
The name is an abbreviation of 'call'.

	Example:

	 2|
	 3|	void func(int i, float f, const std::string &s)
	 4|	{
	 5|	}
	 6|
	...
	 8|
	 9|	int i = 456;
	10|	$c(func)(i, 789, "hellomoto!");
	11|

	>
	>	main.cpp:11   |  func(456, 789, hellomoto!)
	>


$m(object, function_name) - print member-function arguments and return
value.  Should be used at member-function call. 'object' argument can
be of a pointer or non-pointer type.
The name is an abbreviation of 'member-function'.

	Example:

	 2|
	 3|	class some
	 4|	{
	 5|	public:
	 6|		void func(int i, float f, const std::string &s)
	 7|		{
	 8|		}
	 9|	};
	10|
	...
	12|
	13|	some obj;
	14|	int i = 456;
	15|	$m(obj, some::func)(i, 789, "hellomoto!");
	16|

	>
	>	main.cpp:15   |  some::func(456, 789, hellomoto!)
	>


$return expression - print value of epxression passed to return
statement.

	Example:

	 2|
	 3|	int add(int a, int b)
	 4|	{
	 5|		$return a + b;
	 6|	}
	 7|
	...
	 9|
	10|	add(456, 789);
	11|

	>
	>	main.cpp:5    |  return 1245
	>


$if (condition) - print value of the if condition. Automatically adds
and removes indentation to the output.

	Example:

	 5|
	 6|	int i = 0;
	 7|	$if (i < 2)
	 8|	{
	 9|	}
	10|

	>
	>	main.cpp:7    |  if (i < 2) => true
	>


$for (statements) - print iteration numbers of the for loop.
Automatically adds and removes indentation to the output.

	Example:

	 5|
	 6|	$for (int i = 0; i < 3; ++i)
	 7|	{
	 8|		$w(i);
	 9|	}
	10|

	>
	>	main.cpp:6    |  for (int i = 0; i < 3; ++i)
	>	main.cpp:6    |  [iteration #0]
	>	main.cpp:8    |      i = 0
	>	main.cpp:6    |  [iteration #1]
	>	main.cpp:8    |      i = 1
	>	main.cpp:6    |  [iteration #2]
	>	main.cpp:8    |      i = 2
	>


	 5|
	 6|	int arr[] = {4, 5, 6};
	 7|	$for (auto i : arr)
	 8|	{
	 9|		$w(i);
	10|	}
	11|

	>
	>	main.cpp:7    |  for (auto i : arr)
	>	main.cpp:7    |  [iteration #0]
	>	main.cpp:9    |      i = 4
	>	main.cpp:7    |  [iteration #1]
	>	main.cpp:9    |      i = 5
	>	main.cpp:7    |  [iteration #2]
	>	main.cpp:9    |      i = 6
	>


$while (condition) - print iteration conditions of the while loop.
Automatically adds and removes indentation to the output.

	Example:

	 5|
	 6|	int i = 0;
	 7|	$while (i < 3)
	 8|	{
	 9|		$w(i);
	10|		++i;
	11|	}
	12|

	>
	>	main.cpp:7    |  while (i < 3) => true
	>	main.cpp:9    |      i = 0
	>	main.cpp:7    |  while (i < 3) => true
	>	main.cpp:9    |      i = 1
	>	main.cpp:7    |  while (i < 3) => true
	>	main.cpp:9    |      i = 2
	>	main.cpp:7    |  while (i < 3) => false
	>


$_ - Adds and removes indentation in the containing scope.

	Example:

	 5|
	 6|	int i = 456;
	 7|	$w(i);
	 8|
	 9|	{$_
	10|		$w(i);
	11|	}
	12|
	13|	$w(i);
	14|

	>
	>	main.cpp:7    |  i = 456
	>	main.cpp:10   |      i = 456
	>	main.cpp:13   |  i = 456
	>


$p(format, ...) - like printf. The name is an abbreviation of 'printf'.

	Example:

	13|
	14|	$p("%i %f %s", 456, 789.0f, "hellomoto!")
	15|

	>
	>	main.cpp:14   |  456 789.000000 hellomoto!
	>


$t(thread_name) - set thread name, that will be printed in the thread
header.
The name is an abbreviation of 'thread'.

	Example:

	 5|
	 6|	void func()
	 7|	{$t(worker)
	 8|		int arr[] = {1, 2, 3};
	 9|		$for (auto i : arr)
	10|		{
	11|			$w(i);
	12|		}
	13|	}
	14|
	15|	int main()
	16|	{$t(main) $f
	17|
	18|		std::thread t1(func);
	19|		std::thread t2(func);
	20|		t1.join();
	21|		t2.join();
	22|
	23|		$return 0;
	24|	}
	25|

	>
	>	[Thread: 0x7fff71013300 main]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:16   |  [call] int main()
	>
	>	[Thread: 0x108285000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:9    |  for (auto i : arr)
	>
	>	[Thread: 0x108308000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:9    |  for (auto i : arr)
	>
	>	[Thread: 0x108285000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:9    |  [iteration #0]
	>
	>	[Thread: 0x108308000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:9    |  [iteration #0]
	>
	>	[Thread: 0x108285000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:11   |      i = 1
	>
	>	[Thread: 0x108308000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:11   |      i = 1
	>
	>	[Thread: 0x108285000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:9    |  [iteration #1]
	>
	>	[Thread: 0x108308000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:9    |  [iteration #1]
	>
	>	[Thread: 0x108285000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:11   |      i = 2
	>
	>	[Thread: 0x108308000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:11   |      i = 2
	>
	>	[Thread: 0x108285000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:9    |  [iteration #2]
	>
	>	[Thread: 0x108308000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:9    |  [iteration #2]
	>
	>	[Thread: 0x108285000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:11   |      i = 3
	>
	>	[Thread: 0x108308000 worker]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:11   |      i = 3
	>
	>	[Thread: 0x7fff71013300 main]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	>	            main.cpp:23   |      return 0
	>	            main.cpp:16   |  [ret]  int main()
	>



NOTES

	* Macros $c and $m work only with C++11 and later.

	* There is an output synchronization that prevents outputs from
		different threads mixing up. By default this feture is turned
		on. To disable this synchronization define macro
		PRETTY_OUTPUT_NO_OUTPUT_SYNC.

	* If you want to output your class/struct/whatever, you should
		overload operator <<(std::ostream &, <your_type>)

