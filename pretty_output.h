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

	inline const std::string dump_part(const std::uint8_t *bytes, std::size_t size)
	{
		std::stringstream stream;
		for (std::size_t index = 0; index < size; ++index)
		{
			stream.fill('0');
			stream.width(2);
			stream << std::hex << std::noshowbase << (int)bytes[index];
		}

		return stream.str();
	}


	inline void print_dump(const std::string &filename_line, const char *name, const void *pointer, std::size_t length, std::size_t grouping = 1)
	{
		out_stream(filename_line) << name << ": ";
		indentation_add();

		const std::uint8_t *bytes = (const std::uint8_t*)pointer;
		std::size_t size = length * grouping;
		std::stringstream stream;
		stream << to_string((void*)bytes) << ": ";
		for (std::size_t index = 0; index < size; )
		{
			if (output_width_left() < stream.str().length() + (grouping * 2))
			{
				out_stream() << stream.str();
				stream.str("");

				stream << to_string((void*)(bytes + index)) << ": ";
			}

			stream << dump_part(bytes + index, grouping) << " ";

			index += grouping;
		}

		if (!stream.str().empty())
		{
			out_stream() << stream.str();
			out_stream();
		}

		indentation_remove();
	}


	template <typename T>
	inline void print_dump(const std::string &filename_line, const char *name, const T *pointer, std::size_t length = 1)
	{
		print_dump(filename_line, name, pointer, length, sizeof(T));
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

