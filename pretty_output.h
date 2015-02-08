/* ABOUT *

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

*/


/* HELP *

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


$d(pointer/variable, size) - print memory under the pointer or memory of a
variable. When printing contents under the pointer then the argument 'size'
should be provided. When printing memory of the variable then the argument
'size' should be ommited.
The name is an abbreviation of 'dump'.

	Example:

	 4|
	 5|	std::uint64_t arr[] = {456, 789, 123, 545, 784, 942, 124, 545, 382};
	 6|	$d(arr, sizeof(arr)) // Note, size *is* provided
	 7|

	>
	>	main.cpp:6    |  dump of arr:
	>				  |      0x7fff54376b60: c8 01 00 00 00 00 00 00 15 03
	>				  |      0x7fff54376b6a: 00 00 00 00 00 00 7b 00 00 00
	>				  |      0x7fff54376b74: 00 00 00 00 21 02 00 00 00 00
	>				  |      0x7fff54376b7e: 00 00 10 03 00 00 00 00 00 00
	>				  |      0x7fff54376b88: ae 03 00 00 00 00 00 00 7c 00
	>				  |      0x7fff54376b92: 00 00 00 00 00 00 21 02 00 00
	>				  |      0x7fff54376b9c: 00 00 00 00 7e 01 00 00 00 00
	>				  |      0x7fff54376ba6: 00 00 49 00 00 00 00 00 00 00
	>				  |      0x7fff54376bb0: 47 00 00 00 00 00 00 00 5a 00
	>				  |      0x7fff54376bba: 00 00 00 00 00 00 05 00 00 00
	>				  |      0x7fff54376bc4: 00 00 00 00
	>				  |
	>


	 7|
	 8|	struct s_t
	 9|	{
	10|		int i;
	11|		float f;
	12|		char c;
	13|	} s;
	14|
	15|	s.i = 456;
	16|	s.f = 789.123f;
	17|	s.c = 'r';
	18|
	19|	$d(s); // Note, 'size' *is not* provided
	20|

	>
	>	main.cpp:19   |  dump of s:
	>				  |      0x7fff54376b10: c8 01 00 00 df 47 45 44 72 02
	>				  |      0x7fff54376b1a: 60 0a
	>				  |
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

*/


/* NOTES *

	* Macros $c and $m work only with C++11 and later.

	* There is an output synchronization that prevents outputs from
		different threads mixing up. By default this feture is turned
		on. To disable this synchronization define macro
		PRETTY_OUTPUT_NO_OUTPUT_SYNC.

	* If you want to output your class/struct/whatever, you should
		overload operator <<(std::ostream &, <your_type>)

*/


#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <type_traits>
#include <cstdint>
#include <cstdarg>
#include <cstdio>

#if __cplusplus >= 201103L
#include <tuple>
#endif


// public macros:

#define $w(...) \
			pretty_output::watch(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, __VA_ARGS__)


#define $d(pointer, ...) \
			pretty_output::print_dump(PRETTY_OUTPUT_FILENAME_LINE, #pointer, pointer, ##__VA_ARGS__);


#define $f \
			pretty_output::function_printer PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$f)(PRETTY_OUTPUT_FILENAME_LINE, PRETTY_OUTPUT_FUNCTION_SIGNATURE);


#if __cplusplus >= 201103L

#define $c(function_name) \
			pretty_output::function_call(PRETTY_OUTPUT_FILENAME_LINE, #function_name, function_name)


#define $m(object, function_name) \
			pretty_output::member_function_call(PRETTY_OUTPUT_FILENAME_LINE, #function_name, object, &std::remove_pointer<decltype(object)>::type::function_name)

#endif // __cplusplus >= 201103L


#define $return \
			return pretty_output::return_printer(PRETTY_OUTPUT_FILENAME_LINE) ,


#define $if(...) \
			if (pretty_output::print_if_block PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$if_block) = pretty_output::print_if_block(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, __VA_ARGS__))


// NOTE: initializing block variable in such way to prevent using of the uniform initialization list and so make it compile with C++03
	#define pretty_output_for(block, ...) \
				if (pretty_output::for_block block = pretty_output::for_block(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__)) {} else \
					for (__VA_ARGS__) \
						if (pretty_output::print_for_block(PRETTY_OUTPUT_FILENAME_LINE, block), block.next_iteration(), false) {} else

#define $for(...) \
			pretty_output_for(PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$for_block), __VA_ARGS__)


#define $while(...) \
			while (pretty_output::print_while_block PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$while_block) = pretty_output::print_while_block(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, __VA_ARGS__))


#define $_ \
			pretty_output::block PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$block);


#define $p(format, ...) \
			pretty_output::print(PRETTY_OUTPUT_FILENAME_LINE, format, ##__VA_ARGS__);


#define $t(name) \
			pretty_output::set_current_thread_name(#name);


// private macros:

#define PRETTY_OUTPUT_PRIVATE__CONCAT_IMPL(a, b) \
			a##b

#define PRETTY_OUTPUT_PRIVATE__CONCAT(a, b) \
			PRETTY_OUTPUT_PRIVATE__CONCAT_IMPL(a, b)


#define PRETTY_OUTPUT_PRIVATE__UNIFY(identifier_base) \
			PRETTY_OUTPUT_PRIVATE__CONCAT(identifier_base, __COUNTER__)


#define PRETTY_OUTPUT_FILENAME_LINE \
			(pretty_output::filename_line_field(pretty_output::filename_from_path(__FILE__), __LINE__))


#if defined(__GNUG__) || defined(__clang__)
	#define PRETTY_OUTPUT_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
	#define PRETTY_OUTPUT_FUNCTION_SIGNATURE __FUNCSIG__
#else
	#error Cannot find function signature macro for current compiler. Try to add one manualy to this block.
#endif


namespace pretty_output
{

	static const std::size_t WIDTH = 79;
	static const char THREAD_HEADER_FILL_CHAR = '~';
	static const std::size_t FILENAME_FIELD_WIDTH = 20;
	static const char FILENAME_FIELD_EXCESS_PADDING[] = "~";
	static const std::size_t LINE_FIELD_WIDTH = 4;
#if defined(_WIN32)
	static const char FILE_PATH_COMPONENT_DELIMITER = '\\';
#else
	static const char FILE_PATH_COMPONENT_DELIMITER = '/';
#endif
	static const char DELIMITER[] = " |  ";
	static const std::size_t DELIMITER_WIDTH = sizeof(DELIMITER) - 1;
	static const char INDENTATION[] = "    ";
	static const std::size_t INDENTATION_WIDTH = sizeof(INDENTATION) - 1;


	std::uint64_t current_thread_id();
	const std::string current_thread_name();
	void set_current_thread_name(const std::string &name);
	bool is_running_same_thread();

	void lock_output();
	void unlock_output();

	const std::string &indentation();
	void indentation_add();
	void indentation_remove();


	inline const std::string thread_id_field(std::uint64_t thread_id)
	{
		std::stringstream stream;
		stream << (void*)thread_id;

		return stream.str();
	}


	inline const std::string thread_header(const std::string &thread_id, const std::string &thread_name)
	{
		std::stringstream stream;
		stream.fill(THREAD_HEADER_FILL_CHAR);
		stream.flags(std::ios::left);
		stream.width(WIDTH);
		stream << ("[Thread: " + thread_id + (thread_name != "" ? " " : "") + thread_name + "]");

		return stream.str();
	}


	inline const std::string filename_from_path(const char *path)
	{
		std::string file_path(path);
		return file_path.substr(file_path.rfind(FILE_PATH_COMPONENT_DELIMITER) + 1);
	}


	inline const std::string filename_line_field(const std::string &file, unsigned int line)
	{
		std::stringstream stream;
		stream.fill(' ');

		stream.width(FILENAME_FIELD_WIDTH);
		stream.flags(std::ios::right);
		std::string filename = file;
		if (filename.length() > FILENAME_FIELD_WIDTH)
		{
			filename = filename.substr(0, FILENAME_FIELD_WIDTH - sizeof(FILENAME_FIELD_EXCESS_PADDING));
			filename += FILENAME_FIELD_EXCESS_PADDING;
		}

		stream << filename;

		stream.width(0);
		stream << ":";

		stream.width(LINE_FIELD_WIDTH);
		stream.flags(std::ios::left);
		stream << line;

		return stream.str();
	}


	inline std::size_t output_width_left()
	{
		return WIDTH - (FILENAME_FIELD_WIDTH + 1 + LINE_FIELD_WIDTH + DELIMITER_WIDTH + indentation().length());
	}


	class out_stream
	{
	public:
		out_stream(const std::string &filename_line)
		{
			lock_output();

			if (!is_running_same_thread())
			{
				std::string thread_id = thread_id_field(current_thread_id());
				const std::string &thread_name = current_thread_name();
				const std::string &header = thread_header(thread_id, thread_name);
				std::cout << std::endl << header << std::endl;
			}

			std::cout << filename_line << DELIMITER << indentation();
		}


		out_stream(const std::string &filename_line, const char *format, va_list arguments)
		{
			lock_output();

			std::cout << filename_line << DELIMITER << indentation();
			std::vprintf(format, arguments);
		}


		out_stream()
		{
			lock_output();

			std::cout.fill(' ');
			std::cout.width(FILENAME_FIELD_WIDTH + 1 + LINE_FIELD_WIDTH);
			std::cout << "";

			std::cout << DELIMITER << indentation();
		}


		~out_stream()
		{
			std::cout << std::endl;

			unlock_output();
		}


		template <typename T>
		std::ostream &operator <<(const T &value)
		{
			return std::cout << value;
		}


		template <typename T>
		std::ostream &operator <<(T &value)
		{
			return std::cout << value;
		}
	};



	template <typename T>
	const std::string to_string(const T &value);

	template <typename T>
	const std::string to_string(const T *value);

	template <typename T>
	const std::string to_string(T *value);

	const std::string to_string(const void *value);

	inline const std::string to_string(bool value);

	inline const std::string to_string(char value);

	inline const std::string to_string(const std::string &value);

	template <typename A, typename B>
	inline const std::string to_string(const std::pair<A, B> &value);

#if __cplusplus >= 201103L

	template <typename ...T>
	const std::string to_string(const std::tuple<T...> &tuple);

	template <template <typename ...> class Container, typename ...A>
	inline const std::string to_string(const Container<A...> &value);

	template <typename T, typename ...R>
	const std::string to_string(const T &value, R ...rest);

#endif // __cplusplus >= 201103L

	template <typename T>
	inline const std::string to_string();

	inline const std::string to_string();


	template <typename T>
	const std::string to_string(const T &value)
	{
		std::stringstream stream;
		stream << value;
		return stream.str();
	}


	template <typename T>
	const std::string to_string(const T *value)
	{
		if (value == NULL)
		{
			return "(null)";
		}

		std::stringstream stream;
		std::size_t numeric_value = (std::size_t)value;
		stream << std::hex << std::showbase << numeric_value << " -> " << to_string(*value);
		return stream.str();
	}


	template <typename T>
	const std::string to_string(T *value)
	{
		return to_string((const T*)value);
	}


	inline const std::string to_string(const void *value)
	{
		if (value == NULL)
		{
			return "(null)";
		}

		std::stringstream stream;
		std::size_t numeric_value = (std::size_t)value;
		stream << std::hex << std::showbase << numeric_value;
		return stream.str();
	}


	inline const std::string to_string(bool value)
	{
		return value ? "true" : "false";
	}


	inline const std::string to_string(char value)
	{
		std::stringstream stream;
		stream << "\'" << value << "\'";
		return stream.str();
	}


	inline const std::string to_string(const std::string &value)
	{
		std::stringstream stream;
		stream << "\"" << value << "\"";
		return stream.str();
	}


	template <typename A, typename B>
	inline const std::string to_string(const std::pair<A, B> &value)
	{
		std::stringstream stream;
		stream << "{" << to_string(value.first) << ": " << to_string(value.second) << "}";
		return stream.str();
	}


#if __cplusplus >= 201103L

	template <std::size_t I, typename ...T>
	typename std::enable_if<I == sizeof...(T), std::string>::type tuple_to_string(const std::tuple<T...> &)
	{
		return ")";
	}


	template <std::size_t I, typename ...T>
	typename std::enable_if<I < sizeof...(T), std::string>::type tuple_to_string(const std::tuple<T...> &tuple)
	{
		std::stringstream stream;
		stream << std::get<I>(tuple);
		return ", " + stream.str() + tuple_to_string<I + 1>(tuple);
	}


	template <typename ...T>
	const std::string to_string(const std::tuple<T...> &tuple)
	{
		std::stringstream stream;
		stream << std::get<0>(tuple);
		return "(" + stream.str() + tuple_to_string<1>(tuple);
	}


	template <template <typename ...> class Container, typename ...A>
	inline const std::string to_string(const Container<A...> &container)
	{
		std::stringstream stream;

		auto iterator = container.begin();
		auto &item = *iterator;
		stream << to_string(item);

		++iterator;
		for ( ; iterator != container.end(); ++iterator)
		{
			auto &item = *iterator;
			stream << ", " << to_string(item);
		}

		return stream.str();
	}


	template <typename T, typename ...R>
	const std::string to_string(const T &value, R ...rest)
	{
		return to_string<T>(value) + ", " + to_string<R...>(rest...);
	}

#endif // __cplusplus >= 201103L


	template <typename T>
	inline const std::string to_string()
	{
		return "";
	}


	inline const std::string to_string()
	{
		return "";
	}


	// watch

	template <typename T>
	const T &watch(const std::string &filename_line, const char *name, const T &value)
	{
		out_stream(filename_line) << name << " = " << to_string(value);
		return value;
	}


	template <typename T>
	T &watch(const std::string &filename_line, const char *name, T &value)
	{
		out_stream(filename_line) << name << " = " << to_string(value);
		return value;
	}


	// function

	struct function_printer
	{
		function_printer(const std::string &filename_line, const char *function_signature)
			: _filename_line(filename_line), _function_signature(function_signature)
		{
			out_stream(_filename_line) << "[call] " << _function_signature;
			indentation_add();
		}


		~function_printer()
		{
			indentation_remove();
			out_stream(_filename_line) << "[ret]  " << _function_signature;
		}


	private:
		std::string _filename_line;
		std::string _function_signature;
	};


	// dump

	enum dump_base
	{
		bin = 2,
		oct = 8,
		hex = 16
	};


	inline const char *const byte_to_binary(std::uint8_t byte)
	{
		static const char *const BINARY_VALUES[] = {
			"00000000", "00000001", "00000010", "00000011", "00000100", "00000101", "00000110", "00000111",
			"00001000", "00001001", "00001010", "00001011", "00001100", "00001101", "00001110", "00001111",
			"00010000", "00010001", "00010010", "00010011", "00010100", "00010101", "00010110", "00010111",
			"00011000", "00011001", "00011010", "00011011", "00011100", "00011101", "00011110", "00011111",
			"00100000", "00100001", "00100010", "00100011", "00100100", "00100101", "00100110", "00100111",
			"00101000", "00101001", "00101010", "00101011", "00101100", "00101101", "00101110", "00101111",
			"00110000", "00110001", "00110010", "00110011", "00110100", "00110101", "00110110", "00110111",
			"00111000", "00111001", "00111010", "00111011", "00111100", "00111101", "00111110", "00111111",
			"01000000", "01000001", "01000010", "01000011", "01000100", "01000101", "01000110", "01000111",
			"01001000", "01001001", "01001010", "01001011", "01001100", "01001101", "01001110", "01001111",
			"01010000", "01010001", "01010010", "01010011", "01010100", "01010101", "01010110", "01010111",
			"01011000", "01011001", "01011010", "01011011", "01011100", "01011101", "01011110", "01011111",
			"01100000", "01100001", "01100010", "01100011", "01100100", "01100101", "01100110", "01100111",
			"01101000", "01101001", "01101010", "01101011", "01101100", "01101101", "01101110", "01101111",
			"01110000", "01110001", "01110010", "01110011", "01110100", "01110101", "01110110", "01110111",
			"01111000", "01111001", "01111010", "01111011", "01111100", "01111101", "01111110", "01111111",
			"10000000", "10000001", "10000010", "10000011", "10000100", "10000101", "10000110", "10000111",
			"10001000", "10001001", "10001010", "10001011", "10001100", "10001101", "10001110", "10001111",
			"10010000", "10010001", "10010010", "10010011", "10010100", "10010101", "10010110", "10010111",
			"10011000", "10011001", "10011010", "10011011", "10011100", "10011101", "10011110", "10011111",
			"10100000", "10100001", "10100010", "10100011", "10100100", "10100101", "10100110", "10100111",
			"10101000", "10101001", "10101010", "10101011", "10101100", "10101101", "10101110", "10101111",
			"10110000", "10110001", "10110010", "10110011", "10110100", "10110101", "10110110", "10110111",
			"10111000", "10111001", "10111010", "10111011", "10111100", "10111101", "10111110", "10111111",
			"11000000", "11000001", "11000010", "11000011", "11000100", "11000101", "11000110", "11000111",
			"11001000", "11001001", "11001010", "11001011", "11001100", "11001101", "11001110", "11001111",
			"11010000", "11010001", "11010010", "11010011", "11010100", "11010101", "11010110", "11010111",
			"11011000", "11011001", "11011010", "11011011", "11011100", "11011101", "11011110", "11011111",
			"11100000", "11100001", "11100010", "11100011", "11100100", "11100101", "11100110", "11100111",
			"11101000", "11101001", "11101010", "11101011", "11101100", "11101101", "11101110", "11101111",
			"11110000", "11110001", "11110010", "11110011", "11110100", "11110101", "11110110", "11110111",
			"11111000", "11111001", "11111010", "11111011", "11111100", "11111101", "11111110", "11111111"
		};

		return BINARY_VALUES[byte];
	}


	inline const char *const byte_to_octal(std::uint8_t byte)
	{
		static const char *const OCTAL_VALUES[] = {
			 "0000", "0001", "0002", "0003", "0004", "0005", "0006", "0007",
			 "0010", "0011", "0012", "0013", "0014", "0015", "0016", "0017",
			 "0020", "0021", "0022", "0023", "0024", "0025", "0026", "0027",
			 "0030", "0031", "0032", "0033", "0034", "0035", "0036", "0037",
			 "0040", "0041", "0042", "0043", "0044", "0045", "0046", "0047",
			 "0050", "0051", "0052", "0053", "0054", "0055", "0056", "0057",
			 "0060", "0061", "0062", "0063", "0064", "0065", "0066", "0067",
			 "0070", "0071", "0072", "0073", "0074", "0075", "0076", "0077",
			 "0100", "0101", "0102", "0103", "0104", "0105", "0106", "0107",
			 "0110", "0111", "0112", "0113", "0114", "0115", "0116", "0117",
			 "0120", "0121", "0122", "0123", "0124", "0125", "0126", "0127",
			 "0130", "0131", "0132", "0133", "0134", "0135", "0136", "0137",
			 "0140", "0141", "0142", "0143", "0144", "0145", "0146", "0147",
			 "0150", "0151", "0152", "0153", "0154", "0155", "0156", "0157",
			 "0160", "0161", "0162", "0163", "0164", "0165", "0166", "0167",
			 "0170", "0171", "0172", "0173", "0174", "0175", "0176", "0177",
			 "0200", "0201", "0202", "0203", "0204", "0205", "0206", "0207",
			 "0210", "0211", "0212", "0213", "0214", "0215", "0216", "0217",
			 "0220", "0221", "0222", "0223", "0224", "0225", "0226", "0227",
			 "0230", "0231", "0232", "0233", "0234", "0235", "0236", "0237",
			 "0240", "0241", "0242", "0243", "0244", "0245", "0246", "0247",
			 "0250", "0251", "0252", "0253", "0254", "0255", "0256", "0257",
			 "0260", "0261", "0262", "0263", "0264", "0265", "0266", "0267",
			 "0270", "0271", "0272", "0273", "0274", "0275", "0276", "0277",
			 "0300", "0301", "0302", "0303", "0304", "0305", "0306", "0307",
			 "0310", "0311", "0312", "0313", "0314", "0315", "0316", "0317",
			 "0320", "0321", "0322", "0323", "0324", "0325", "0326", "0327",
			 "0330", "0331", "0332", "0333", "0334", "0335", "0336", "0337",
			 "0340", "0341", "0342", "0343", "0344", "0345", "0346", "0347",
			 "0350", "0351", "0352", "0353", "0354", "0355", "0356", "0357",
			 "0360", "0361", "0362", "0363", "0364", "0365", "0366", "0367",
			 "0370", "0371", "0372", "0373", "0374", "0375", "0376", "0377"
		};

		return OCTAL_VALUES[byte];
	}


	inline const char *const byte_to_hexadecimal(std::uint8_t byte)
	{
		static const char *const HEXADECIMAL_VALUES[] = {
			"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b", "0c", "0d", "0e", "0f",
			"10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1a", "1b", "1c", "1d", "1e", "1f",
			"20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f",
			"30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3a", "3b", "3c", "3d", "3e", "3f",
			"40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4a", "4b", "4c", "4d", "4e", "4f",
			"50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5a", "5b", "5c", "5d", "5e", "5f",
			"60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b", "6c", "6d", "6e", "6f",
			"70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7a", "7b", "7c", "7d", "7e", "7f",
			"80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
			"90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9a", "9b", "9c", "9d", "9e", "9f",
			"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af",
			"b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf",
			"c0", "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf",
			"d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", "d9", "da", "db", "dc", "dd", "de", "df",
			"e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9", "ea", "eb", "ec", "ed", "ee", "ef",
			"f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "fa", "fb", "fc", "fd", "fe", "ff"
		};

		return HEXADECIMAL_VALUES[byte];
	}


	template <typename T>
	inline void print_dump(const std::string &filename_line, const char *name, const T *pointer, std::size_t size = sizeof(T), dump_base base = hex)
	{
		out_stream(filename_line) << "dump of " << name << ":";
		indentation_add();

		const char *const (*convert)(std::uint8_t);
		std::size_t column_width;
		switch (base)
		{
			case bin:
				convert = byte_to_binary;
				column_width = 8;
				break;

			case oct:
				convert = byte_to_octal;
				column_width = 4;
				break;

			default:
				convert = byte_to_hexadecimal;
				column_width = 2;
		}

		std::stringstream stream;

		const std::uint8_t *bytes = (const std::uint8_t*)pointer;
		stream << to_string((void*)bytes) << ":";
		for (std::size_t index = 0; index < size; ++index)
		{
			if (output_width_left() < stream.str().length() + column_width)
			{
				out_stream() << stream.str();
				stream.str("");

				stream << to_string((void*)&bytes[index]) << ":";
			}

			stream << " " << convert(bytes[index]);
		}

		if (!stream.str().empty())
		{
			out_stream() << stream.str();
			out_stream();
		}

		indentation_remove();
	}


	template <typename T>
	inline void print_dump(const std::string &filename_line, const char *name, const T &variable, dump_base base = hex)
	{
		print_dump(filename_line, name, &variable, sizeof(variable));
	}


#if __cplusplus >= 201103L

	// function call

	template <typename R, typename ...A>
	struct function_call_printer
	{
		typedef R (*funcptr_t)(A...);


		function_call_printer(const std::string &filename_line, const char *function_name, funcptr_t function_pointer)
			: _filename_line(filename_line), _function_name(function_name), _function_pointer(function_pointer)
		{
		}


		template <typename ...A2>
		R operator ()(A2 &&...arguments)
		{
			R return_value = _function_pointer(std::forward<A2>(arguments)...);
			out_stream(_filename_line) << _function_name << "(" << to_string(arguments...) << ") => " << to_string(return_value);
			return return_value;
		}


	private:
		std::string _filename_line;
		std::string _function_name;
		funcptr_t _function_pointer;
	};


	template <typename ...A>
	struct function_call_printer<void, A...>
	{
		typedef void (*funcptr_t)(A...);


		function_call_printer(const std::string &filename_line, const char *function_name, funcptr_t function_pointer)
			: _filename_line(filename_line), _function_name(function_name), _function_pointer(function_pointer)
		{
		}


		template <typename ...A2>
		void operator ()(A2 &&...arguments)
		{
			_function_pointer(std::forward<A2>(arguments)...);
			out_stream(_filename_line) << _function_name << "(" << to_string(arguments...) << ")";
		}


	private:
		std::string _filename_line;
		std::string _function_name;
		funcptr_t _function_pointer;
	};


	template <typename R, typename ...A>
	function_call_printer<R, A...> function_call(const std::string &filename_line, const char *function_name, R (*function_pointer)(A...))
	{
		return function_call_printer<R, A...>(filename_line, function_name, function_pointer);
	}


	// const member function call

	template <typename T, typename R, typename ...A>
	struct const_member_function_call_printer
	{
		typedef R (T::*funcptr_t)(A...) const;


		const_member_function_call_printer(const std::string &filename_line, const char *function_name, const T &object, funcptr_t function_pointer)
			: _filename_line(filename_line), _function_name(function_name), _object(object), _function_pointer(function_pointer)
		{
		}


		template <typename ...A2>
		R operator ()(A2 &&...arguments)
		{
			R return_value = (_object.*_function_pointer)(std::forward<A2>(arguments)...);
			out_stream(_filename_line) << _function_name << "(" << to_string(arguments...) << ") => " << to_string(return_value);
			return return_value;
		}


	private:
		std::string _filename_line;
		std::string _function_name;
		T &_object;
		funcptr_t _function_pointer;
	};


	template <typename T, typename ...A>
	struct const_member_function_call_printer<T, void, A...>
	{
		typedef void (T::*funcptr_t)(A...) const;


		const_member_function_call_printer(const std::string &filename_line, const char *function_name, const T &object, funcptr_t function_pointer)
			: _filename_line(filename_line), _function_name(function_name), _object(object), _function_pointer(function_pointer)
		{
		}


		template <typename ...A2>
		void operator ()(A2 &&...arguments)
		{
			(_object.*_function_pointer)(std::forward<A2>(arguments)...);
			out_stream(_filename_line) << _function_name << "(" << to_string(arguments...) << ")";
		}


	private:
		std::string _filename_line;
		std::string _function_name;
		T &_object;
		funcptr_t _function_pointer;
	};


	template <typename T, typename R, typename ...A>
	const_member_function_call_printer<T, R, A...> member_function_call(const std::string &filename_line, const char *function_name, const T &object, R (T::*function_pointer)(A...) const)
	{
		return const_member_function_call_printer<T, R, A...>(filename_line, function_name, object, function_pointer);
	}


	template <typename T, typename R, typename ...A>
	const_member_function_call_printer<T, R, A...> member_function_call(const std::string &filename_line, const char *function_name, const T *object, R (T::*function_pointer)(A...) const)
	{
		return const_member_function_call_printer<T, R, A...>(filename_line, function_name, *object, function_pointer);
	}



	// non const member function call

	template <typename T, typename R, typename ...A>
	struct member_function_call_printer
	{
		typedef R (T::*funcptr_t)(A...);


		member_function_call_printer(const std::string &filename_line, const char *function_name, T &object, funcptr_t function_pointer)
			: _filename_line(filename_line), _function_name(function_name), _object(object), _function_pointer(function_pointer)
		{
		}


		template <typename ...A2>
		R operator ()(A2 &&...arguments)
		{
			R return_value = (_object.*_function_pointer)(std::forward<A2>(arguments)...);
			out_stream(_filename_line) << _function_name << "(" << to_string(arguments...) << ") => " << to_string(return_value);
			return return_value;
		}


	private:
		std::string _filename_line;
		std::string _function_name;
		T &_object;
		funcptr_t _function_pointer;
	};


	template <typename T, typename ...A>
	struct member_function_call_printer<T, void, A...>
	{
		typedef void (T::*funcptr_t)(A...);


		member_function_call_printer(const std::string &filename_line, const char *function_name, T &object, funcptr_t function_pointer)
			: _filename_line(filename_line), _function_name(function_name), _object(object), _function_pointer(function_pointer)
		{
		}


		template <typename ...A2>
		void operator ()(A2 &&...arguments)
		{
			(_object.*_function_pointer)(std::forward<A2>(arguments)...);
			out_stream(_filename_line) << _function_name << "(" << to_string(arguments...) << ")";
		}


	private:
		std::string _filename_line;
		std::string _function_name;
		T &_object;
		funcptr_t _function_pointer;
	};


	template <typename T, typename R, typename ...A>
	member_function_call_printer<T, R, A...> member_function_call(const std::string &filename_line, const char *function_name, T &object, R (T::*function_pointer)(A...))
	{
		return member_function_call_printer<T, R, A...>(filename_line, function_name, object, function_pointer);
	}


	template <typename T, typename R, typename ...A>
	member_function_call_printer<T, R, A...> member_function_call(const std::string &filename_line, const char *function_name, T *object, R (T::*function_pointer)(A...))
	{
		return member_function_call_printer<T, R, A...>(filename_line, function_name, *object, function_pointer);
	}

#endif // __cplusplus >= 201103L


	// return

	struct return_printer
	{
		return_printer(const std::string &filename_line)
			: _filename_line(filename_line)
		{
		}


		template <typename T>
		const T &operator ,(const T &value)
		{
			out_stream(_filename_line) << "return " << to_string(value);
			return value;
		}


	private:
		std::string _filename_line;
	};


	// if block

	struct print_if_block
	{
		template <typename T>
		print_if_block(const std::string &filename_line, const char *expression, const T &value)
			: _condition(value)
		{
			out_stream(filename_line) << "if (" << expression << ") => " << to_string((bool)value) << " (" << to_string(value) << ")";
			indentation_add();
		}


		template <typename T>
		print_if_block(const std::string &filename_line, const char *expression, T &value)
			: _condition(value)
		{
			out_stream(filename_line) << "if (" << expression << ") => " << to_string((bool)value) << " (" << to_string(value) << ")";
			indentation_add();
		}


		print_if_block(const std::string &filename_line, const char *expression, bool value)
			: _condition(value)
		{
			out_stream(filename_line) << "if (" << expression << ") => " << to_string(value);
			indentation_add();
		}


		~print_if_block()
		{
			indentation_remove();
		}


		operator bool() const
		{
			return _condition;
		}


	private:
		bool _condition;
	};


	// for block

	struct for_block
	{
		for_block(const std::string &filename_line, const char *expression)
			: _iteration_number(0)
		{
			out_stream(filename_line) << "for (" << expression << ")";
			indentation_add();
		}


		~for_block()
		{
			indentation_remove();
		}


		operator bool() const
		{
			return false;
		}


		std::size_t iteration_number() const
		{
			return _iteration_number;
		}


		void next_iteration()
		{
			++_iteration_number;
		}

	private:
		std::size_t _iteration_number;
	};


	inline void print_for_block(const std::string &filename_line, const for_block &block)
	{
		indentation_remove();
		out_stream(filename_line) << "[iteration #" << block.iteration_number() << "]";
		indentation_add();
	}


	// while block

	struct print_while_block
	{
		template <typename T>
		print_while_block(const std::string &filename_line, const char *expression, const T &value)
			: _condition(value)
		{
			out_stream(filename_line) << "while (" << expression << ") => " << to_string((bool)value) << " (" << to_string(value) << ")";
			indentation_add();
		}


		template <typename T>
		print_while_block(const std::string &filename_line, const char *expression, T &value)
			: _condition(value)
		{
			out_stream(filename_line) << "while (" << expression << ") => " << to_string((bool)value) << " (" << to_string(value) << ")";
			indentation_add();
		}


		print_while_block(const std::string &filename_line, const char *expression, bool value)
			: _condition(value)
		{
			out_stream(filename_line) << "while (" << expression << ") => " << to_string(value);
			indentation_add();
		}


		~print_while_block()
		{
			indentation_remove();
		}


		operator bool() const
		{
			return _condition;
		}


	private:
		bool _condition;
	};


	// block

	struct block
	{
		block()
		{
			indentation_add();
		}


		~block()
		{
			indentation_remove();
		}
	};


	// print

	inline void print(const std::string &filename_line, const char *format, ...)
	{
		va_list arguments;

		va_start(arguments, format);
		out_stream(filename_line, format, arguments);
		va_end(arguments);
	}


	// helper stuff

	struct _tlskey_t;

	_tlskey_t *_tls_new_key();
	void _tls_delete_key(_tlskey_t *key);
	void *_tls_get(_tlskey_t *key);
	void _tls_set(_tlskey_t *key, void *data);


	struct _mutex_t;

	_mutex_t *_mutex_new();
	void _mutex_delete(_mutex_t *mutex);
	void _mutex_lock(_mutex_t *mutex);
	void _mutex_unlock(_mutex_t *mutex);

}

