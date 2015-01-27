//
//  pretty_output.h
//
//  by shrpnsld
//
//


#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <utility>
#include <type_traits>


// public macros:

#define $w(...) \
			pretty_output::watch(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, __VA_ARGS__)


#define $f \
			pretty_output::function_printer PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$f)(PRETTY_OUTPUT_FILENAME_LINE, __PRETTY_FUNCTION__);


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


#define $t(format, ...) \
			pretty_output::trace(PRETTY_OUTPUT_FILENAME_LINE, format, ##__VA_ARGS__);


#define $tn(name) \
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


namespace pretty_output
{

	static const size_t FILENAME_FIELD_WIDTH = 20;
	static const char FILENAME_FIELD_EXCESS_PADDING[] = "~";
	static const size_t LINE_FIELD_WIDTH = 4;
	static const char DELIMITER[] = " |  ";
	static const char INDENTATION[] = "    ";
	static const size_t INDENTATION_SIZE = sizeof(INDENTATION) - 1;


	uint64_t current_thread_id();
	const std::string current_thread_name();
	void set_current_thread_name(const std::string &name);
	bool is_running_same_thread();

	void lock_output();
	void unlock_output();

	const std::string &indentation();
	void indentation_add();
	void indentation_remove();


	inline const std::string filename_from_path(const char *path)
	{
		std::string file_path(path);
		return file_path.substr(file_path.rfind('/') + 1);
	}


	inline const std::string thread_id_field(uint64_t thread_id)
	{
		std::stringstream stream;
		stream.fill(' ');

		stream.width(20);
		stream.flags(std::ios::right);

		stream << thread_id;

		return stream.str();
	}


	inline const std::string filename_line_field(const std::string &file, unsigned int line)
	{
		std::stringstream stream;
		stream.fill(' ');

		stream.width(20);
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
				std::cout << std::endl << "[Thread: " << thread_id << (thread_name != "" ? " " : "") << thread_name << "] -- -- -- -- -- -- -- -- -- -- -- -- -- -- --" << std::endl;
			}

			std::cout << filename_line.c_str() << DELIMITER << indentation();
		}


		out_stream(const std::string &filename_line, const char *format, va_list arguments)
		{
			lock_output();

			std::cout << filename_line.c_str() << DELIMITER << indentation();
			std::vprintf(format, arguments);
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

	inline const std::string to_string(bool value);

	inline const std::string to_string(const char *value);

	inline const std::string to_string(const std::string &value);

	template <typename A, typename B>
	inline const std::string to_string(const std::pair<A, B> &value);


#if __cplusplus >= 201103L

	template <template <typename ...> class Container, typename ...A>
	inline const std::string to_string(const Container<A...> &value);

#endif // __cplusplus >= 201103L

	template <typename T>
	inline const std::string to_string();

	inline const std::string to_string();

#if __cplusplus >= 201103L

	template <typename T, typename ...R>
	const std::string to_string(const T &value, R ...rest);

#endif // __cplusplus >= 201103L


	template <typename T>
	const std::string to_string(const T &value)
	{
		std::stringstream stream;
		stream << value;
		return stream.str();
	}


	inline const std::string to_string(bool value)
	{
		return value ? "true" : "false";
	}


	inline const std::string to_string(const char *value)
	{
		std::stringstream stream;
		stream << "\"" << value << "\"";
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


#if __cplusplus >= 201103L

	template <typename T, typename ...R>
	const std::string to_string(const T &value, R ...rest)
	{
		return to_string<T>(value) + ", " + to_string<R...>(rest...);
	}

#endif // __cplusplus >= 201103L


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
	function_call_printer<R, A...> function_call(const std::string &filename_line, const char *function_name, R (*function_pointer)(A&&...))
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


		size_t iteration_number() const
		{
			return _iteration_number;
		}


		void next_iteration()
		{
			++_iteration_number;
		}

	private:
		size_t _iteration_number;
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


	// trace

	inline void trace(const std::string &filename_line, const char *format, ...)
	{
		va_list arguments;

		va_start(arguments, format);
		out_stream(filename_line, format, arguments);
		va_end(arguments);
	}

}

