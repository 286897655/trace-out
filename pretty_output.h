#pragma once

#include <limits>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <stdint.h>

#if __cplusplus >= 201103L
	#include <tuple>
#endif


#if defined(__clang__)

	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"
	#pragma clang diagnostic ignored "-Wvariadic-macros"
	#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

#elif defined(__GNUC__) || defined(__GNUG__)

	#pragma gnu diagnostic push

#endif


// public macros:

#if (!defined(NDEBUG) && !defined(PRETTY_OUTPUT_OFF)) || defined(PRETTY_OUTPUT_ON)

	#define $w(...) \
				pretty_output::watch(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, __VA_ARGS__)


	#define $d(pointer, ...) \
				pretty_output::print_dump(PRETTY_OUTPUT_FILENAME_LINE, #pointer, pointer, ##__VA_ARGS__);


	#define $f \
				pretty_output::function_printer_t PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$f) = pretty_output::function_printer(PRETTY_OUTPUT_FILENAME_LINE, PRETTY_OUTPUT_FUNCTION_SIGNATURE);


	#if __cplusplus >= 201103L

	#define $c(function_name) \
				pretty_output::function_call(PRETTY_OUTPUT_FILENAME_LINE, #function_name, function_name)


	#define $m(object, function_name) \
				pretty_output::member_function_call(PRETTY_OUTPUT_FILENAME_LINE, #object, #function_name, object, &std::remove_pointer<decltype(object)>::type::function_name)

	#endif // __cplusplus >= 201103L


	#define $return \
				return pretty_output::return_printer(PRETTY_OUTPUT_FILENAME_LINE) ,


	#define $if(...) \
				if (pretty_output::print_if_block_t PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$if_block) = pretty_output::print_if_block(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, __VA_ARGS__))


	// NOTE: initializing block variable in such way to prevent using of the uniform initialization list and so make it compile with C++03
		#define pretty_output_for(block, ...) \
					if (pretty_output::for_block_t block = pretty_output::for_block_t(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__)) {} else \
						for (__VA_ARGS__) \
							if (block.iteration(), false) {} else

	#define $for(...) \
				pretty_output_for(PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$for_block), __VA_ARGS__)


	#define $while(...) \
				while (pretty_output::print_while_block_t PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$while_block) = pretty_output::print_while_block(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, __VA_ARGS__))


	#define $_ \
				pretty_output::block_t PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$block) = pretty_output::block();


	#define $p(format, ...) \
				pretty_output::out_stream(PRETTY_OUTPUT_FILENAME_LINE).printf(format, ##__VA_ARGS__);


	#define $t(name) \
				pretty_output::set_current_thread_name(#name);

#elif defined(NDEBUG) || defined(PRETTY_OUTPUT_OFF)

	#define $w(...)


	#define $d(pointer, ...)


	#define $f


	#if __cplusplus >= 201103L

	#define $c(function_name) \
				function_name


	#define $m(object, function_name) \
				(object.*&std::remove_pointer<decltype(object)>::type::function_name)

	#endif // __cplusplus >= 201103L


	#define $return \
				return


	#define $if(...) \
				if (__VA_ARGS__)


	#define $for(...) \
				for (__VA_ARGS__)


	#define $while(...) \
				while (__VA_ARGS__)


	#define $_


	#define $p(format, ...)


	#define $t(name)

#endif


// private macros:

#define PRETTY_OUTPUT_PRIVATE__CONCAT_IMPL(a, b) \
			a##b

#define PRETTY_OUTPUT_PRIVATE__CONCAT(a, b) \
			PRETTY_OUTPUT_PRIVATE__CONCAT_IMPL(a, b)


#define PRETTY_OUTPUT_PRIVATE__UNIFY(identifier_base) \
			PRETTY_OUTPUT_PRIVATE__CONCAT(identifier_base, __COUNTER__)


#define PRETTY_OUTPUT_FILENAME_LINE \
			(pretty_output::filename_line_field(pretty_output::filename_from_path(__FILE__), __LINE__))


#define PRETTY_OUTPUT__QUOTIZE_IMPL(something) \
			#something

#define PRETTY_OUTPUT__QUOTIZE(something) \
			PRETTY_OUTPUT__QUOTIZE_IMPL(something)


#if defined(__GNUG__) || defined(__clang__)
	#define PRETTY_OUTPUT_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
	#define PRETTY_OUTPUT_FUNCTION_SIGNATURE __FUNCSIG__
#else
	#error Cannot find function signature macro for current compiler. Try to add one manualy to this block.
#endif


#if defined(PRETTY_OUTPUT_REDIRECTION_H)
	#include PRETTY_OUTPUT__QUOTIZE(PRETTY_OUTPUT_REDIRECTION_H)
#else
	#include <iostream>


	inline void pretty_output_print(const char *string)
	{
		std::cout << string;
	}


	inline void pretty_output_flush()
	{
		std::cout.flush();
	}

#endif


namespace pretty_output
{

	// options

	static const size_t WIDTH =
#if defined(PRETTY_OUTPUT_WIDTH)
		PRETTY_OUTPUT_WIDTH
#else
		79
#endif
	;


	static const char THREAD_HEADER_SEPARATOR =
#if defined(PRETTY_OUTPUT_THREAD_HEADER_SEPARATOR)
		PRETTY_OUTPUT_THREAD_HEADER_SEPARATOR
#else
		'~'
#endif
	;


	static const size_t FILENAME_FIELD_WIDTH =
#if defined(PRETTY_OUTPUT_FILENAME_FIELD_WIDTH)
		PRETTY_OUTPUT_FILENAME_FIELD_WIDTH
#else
		20
#endif
	;


	static const char FILENAME_FIELD_EXCESS_PADDING[] =
#if defined(PRETTY_OUTPUT_FILENAME_FIELD_EXCESS_PADDING)
		PRETTY_OUTPUT_FILENAME_FIELD_EXCESS_PADDING
#else
		"~"
#endif
	;


	static const size_t LINE_FIELD_WIDTH =
#if defined(PRETTY_OUTPUT_LINE_FIELD_WIDTH)
		PRETTY_OUTPUT_LINE_FIELD_WIDTH
#else
		4
#endif
	;


	static const char DELIMITER[] =
#if defined(PRETTY_OUTPUT_DELIMITER)
		PRETTY_OUTPUT_DELIMITER
#else
		" |  "
#endif
	;


	static const char INDENTATION[] =
#if defined(PRETTY_OUTPUT_INDENTATION)
		PRETTY_OUTPUT_INDENTATION
#else
		"    "
#endif
	;


	// declarations

#if defined(_WIN32)
	static const char FILE_PATH_COMPONENT_DELIMITER = '\\';
#else
	static const char FILE_PATH_COMPONENT_DELIMITER = '/';
#endif
	static const size_t DELIMITER_WIDTH = sizeof(DELIMITER) - 1;
	static const size_t INDENTATION_WIDTH = sizeof(INDENTATION) - 1;

	uint64_t current_thread_id();
	const std::string current_thread_name();
	void set_current_thread_name(const std::string &name);
	bool is_running_same_thread();

	void lock_output();
	void unlock_output();

	const std::string &indentation();
	void indentation_add();
	void indentation_remove();

	size_t printf_string_length(const char *format, va_list arguments);
	size_t printf_to_string(char *buffer, size_t size, const char *format, va_list arguments);


	inline const std::string thread_id_field(uint64_t thread_id)
	{
		std::stringstream stream;
		stream << (void*)thread_id;

		return stream.str();
	}


	inline const std::string thread_header(const std::string &thread_id, const std::string &thread_name)
	{
		std::stringstream stream;
		stream.fill(THREAD_HEADER_SEPARATOR);
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


	template <typename T>
	struct value_t
	{
		value_t(const T &value)
			: data(value)
		{
		}


		const T &data;
	};


	inline value_t<const char*> make_value(const char *const &value)
	{
		return value_t<const char*>(value);
	}


	template <typename T>
	inline value_t<T> make_value(const T &value)
	{
		return value_t<T>(value);
	}


#if __cplusplus >= 201103L

	template <typename...>
	struct values_t;


	template <typename T, typename ...R>
	struct values_t<T, R...>
	{
		values_t(const char *delim, const T &first, const R &...rest)
			: delimiter(delim), data(first), values(delim, rest...)
		{
		}


		const char *delimiter;
		const T &data;
		values_t<R...> values;
	};


	template <typename T>
	struct values_t<T>
	{
		values_t(const char *delimiter, const T &value)
			: delimiter(""), data(value)
		{
		}


		const char *delimiter;
		const T &data;
	};


	template <typename ...T>
	inline values_t<T...> make_values(const char *delimiter, const T &...values)
	{
		return values_t<T...>(delimiter, values...);
	}

#endif


	struct endl_t
	{
	};

	extern endl_t endl;


	class out_stream
	{
	public:
		out_stream(const std::string &filename_line)
			: _current_line_length(0)
		{
			lock_output();

			if (!is_running_same_thread())
			{
				std::string thread_id = thread_id_field(current_thread_id());
				const std::string &thread_name = current_thread_name();
				const std::string &header = thread_header(thread_id, thread_name);
				*this << "\n" << header.c_str() << "\n";
			}

			*this << filename_line.c_str() << DELIMITER << indentation().c_str();
		}


		out_stream()
			: _current_line_length(0)
		{
			lock_output();

			std::stringstream stream;
			stream.fill(' ');
			stream.width(FILENAME_FIELD_WIDTH + 1 + LINE_FIELD_WIDTH);
			stream << "";

			*this << stream.str().c_str() << DELIMITER << indentation().c_str();
		}


		~out_stream()
		{
			*this << "\n";

			pretty_output_flush();

			unlock_output();
		}


		out_stream &operator <<(const char *string)
		{
			pretty_output_print(string);
			_current_line_length += std::strlen(string);

			return *this;
		}


		out_stream &operator <<(const endl_t&)
		{
			std::stringstream stream;
			stream.fill(' ');
			stream.width(FILENAME_FIELD_WIDTH + 1 + LINE_FIELD_WIDTH);
			stream << "";

			*this << "\n";
			_current_line_length = 0;
			*this << stream.str().c_str() << DELIMITER << indentation().c_str();

			return *this;
		}


		size_t width_left() const
		{
			return WIDTH - _current_line_length;
		}


		void printf(const char *format, ...)
		{
			va_list arguments;
			va_list arguments_copy;

			va_start(arguments, format);
			va_start(arguments_copy, format);

			size_t size = printf_string_length(format, arguments_copy) + 1;

			char *buffer = (char*)std::malloc(size);
			printf_to_string(buffer, size, format, arguments);
			*this << "// " << buffer;

			va_end(arguments);
			va_end(arguments_copy);
			std::free(buffer);
		}


		void flush()
		{
			pretty_output_flush();
		}

	private:
		size_t _current_line_length;
	};



	inline out_stream &operator <<(out_stream &stream, value_t<const char*> string);

	inline out_stream &operator <<(out_stream &stream, value_t<std::string> value);

	inline out_stream &operator <<(out_stream &stream, value_t<short> value);

	inline out_stream &operator <<(out_stream &stream, value_t<unsigned short> value);

	inline out_stream &operator <<(out_stream &stream, value_t<int> number);

	inline out_stream &operator <<(out_stream &stream, value_t<unsigned int> value);

	inline out_stream &operator <<(out_stream &stream, value_t<long> value);

	inline out_stream &operator <<(out_stream &stream, value_t<unsigned long> value);

#if __cplusplus >= 201103L

	inline out_stream &operator <<(out_stream &stream, value_t<long long> value);

	inline out_stream &operator <<(out_stream &stream, value_t<unsigned long long> value);

#endif // __cpluspluc >= 201103L

	inline out_stream &operator <<(out_stream &stream, value_t<float> value);

	inline out_stream &operator <<(out_stream &stream, value_t<double> value);

	inline out_stream &operator <<(out_stream &stream, value_t<long double> value);

	inline out_stream &operator <<(out_stream &stream, value_t<const void*> value);

	template <typename T>
	inline out_stream &operator <<(out_stream &stream, value_t<const T*> value);

	template <typename T>
	inline out_stream &operator <<(out_stream &stream, value_t<T*> value);

	inline out_stream &operator <<(out_stream &stream, value_t<bool> value);

	inline out_stream &operator <<(out_stream &stream, value_t<char> value);

	template <typename A, typename B>
	inline out_stream &operator <<(out_stream &stream, value_t<std::pair<A, B> > value);

#if __cplusplus >= 201103L

	template <typename T>
	inline out_stream &operator <<(out_stream &stream, values_t<T> values);

	template <typename ...T>
	inline out_stream &operator <<(out_stream &stream, values_t<T...> values);

	template <typename ...T>
	inline out_stream &operator <<(out_stream &stream, value_t<std::tuple<T...> > tuple);

	template <template <typename ...> class Container, typename ...A>
	inline out_stream &operator <<(out_stream &stream, value_t<Container<A...> > value);

#endif // __cplusplus >= 201103L

	template <typename T>
	inline const std::string fundamental_to_string(T value)
	{
		std::stringstream stream;
		stream << value;

		return stream.str();
	}


	out_stream &operator <<(out_stream &stream, value_t<const char*> value)
	{
		return stream << "\"" << value.data << "\"";
	}


	out_stream &operator <<(out_stream &stream, value_t<std::string> value)
	{
		return stream << "\"" << value.data.c_str() << "\"";
	}


	out_stream &operator <<(out_stream &stream, value_t<short> value)
	{
		return stream << fundamental_to_string(value.data).c_str();
	}


	out_stream &operator <<(out_stream &stream, value_t<unsigned short> value)
	{
		return stream << fundamental_to_string(value.data).c_str();
	}


	out_stream &operator <<(out_stream &stream, value_t<int> value)
	{
		return stream << fundamental_to_string(value.data).c_str();
	}


	out_stream &operator <<(out_stream &stream, value_t<unsigned int> value)
	{
		return stream << fundamental_to_string(value.data).c_str();
	}


	out_stream &operator <<(out_stream &stream, value_t<long> value)
	{
		return stream << fundamental_to_string(value.data).c_str();
	}


	out_stream &operator <<(out_stream &stream, value_t<unsigned long> value)
	{
		return stream << fundamental_to_string(value.data).c_str();
	}


#if __cplusplus >= 201103L

	out_stream &operator <<(out_stream &stream, value_t<long long> value)
	{
		return stream << fundamental_to_string(value.data).c_str();
	}


	out_stream &operator <<(out_stream &stream, value_t<unsigned long long> value)
	{
		return stream << fundamental_to_string(value.data).c_str();
	}

#endif // __cplusplus >= 201103L


	out_stream &operator <<(out_stream &stream, value_t<float> value)
	{
		return stream << fundamental_to_string(value.data).c_str();
	}


	out_stream &operator <<(out_stream &stream, value_t<double> value)
	{
		return stream << fundamental_to_string(value.data).c_str();
	}


	out_stream &operator <<(out_stream &stream, value_t<long double> value)
	{
		return stream << fundamental_to_string(value.data).c_str();
	}


	out_stream &operator <<(out_stream &stream, value_t<const void*> value)
	{
		if (value.data == NULL)
		{
			return stream << "(null)";
		}

		std::stringstream string_stream;
		std::size_t numeric_value = (uintptr_t)value.data;
		string_stream << std::hex << std::showbase << numeric_value;

		return stream << string_stream.str().c_str();
	}


	template <typename T>
	out_stream &operator <<(out_stream &stream, value_t<const T*> value)
	{
		stream << make_value((const void*)value.data) << " ";
		if (value.data != NULL)
		{
			stream.flush();
			stream << "-> " << make_value(*(value.data));
		}

		return stream;
	}


	template <typename T>
	out_stream &operator <<(out_stream &stream, value_t<T*> value)
	{
		return stream << make_value((const T*)value.data);
	}


	out_stream &operator <<(out_stream &stream, value_t<bool> value)
	{
		return stream << (value.data ? "true" : "false");
	}


	out_stream &operator <<(out_stream &stream, value_t<char> value)
	{
		std::stringstream string_stream;
		string_stream << "\'" << value.data << "\'";
		return stream << string_stream.str();
	}


	template <typename A, typename B>
	out_stream &operator <<(out_stream &stream, value_t<std::pair<A, B> > value)
	{
		std::stringstream string_stream;
		string_stream << "{" << value.data.first << ": " << value.data.second << "}";
		return stream << string_stream.str();
	}


#if __cplusplus >= 201103L

	template <typename T>
	out_stream &operator <<(out_stream &stream, values_t<T> values)
	{
		return stream << make_value(values.data);
	}


	template <typename ...T>
	out_stream &operator <<(out_stream &stream, values_t<T...> values)
	{
		return stream << make_value(values.data) << values.delimiter << values.values;
	}


	template <std::size_t I, typename ...T>
	typename std::enable_if<I == sizeof...(T), out_stream&>::type print_tuple(out_stream &stream, const std::tuple<T...> &)
	{
		return stream << ")";
	}


	template <std::size_t I, typename ...T>
	typename std::enable_if<I < sizeof...(T), out_stream&>::type print_tuple(out_stream &stream, const std::tuple<T...> &tuple)
	{
		stream << ", " << make_value(std::get<I>(tuple));
		return print_tuple<I + 1>(stream, tuple);
	}


	template <typename ...T>
	out_stream &operator <<(out_stream &stream, value_t<std::tuple<T...> > value)
	{
		stream << "(" << make_value(std::get<0>(value.data));
		return print_tuple<1>(stream, value.data);
	}


	template <template <typename ...> class Container, typename ...A>
	out_stream &operator <<(out_stream &stream, value_t<Container<A...> > value)
	{
		auto iterator = value.data.begin();
		auto &item = *iterator;

		stream << "[" << make_value(item);

		++iterator;
		for ( ; iterator != value.data.end(); ++iterator)
		{
			auto &item = *iterator;
			stream << ", " << make_value(item);
		}

		stream << "]";

		return stream;
	}

#endif // __cplusplus >= 201103L


	// watch

	template <typename T>
	inline const T &watch(const std::string &filename_line, const char *name, const T &value)
	{
		out_stream(filename_line) << name << " = " << make_value(value);
		return value;
	}


	template <typename T>
	inline T &watch(const std::string &filename_line, const char *name, T &value)
	{
		out_stream(filename_line) << name << " = " << make_value(value);
		return value;
	}


	// function

	struct function_printer_t
	{
		inline function_printer_t(const std::string &filename_line, const char *function_signature);

		inline ~function_printer_t();

	private:
		std::string _filename_line;
		std::string _function_signature;
	};


	inline function_printer_t function_printer(const std::string &filename_line, const char *function_signature)
	{
		return function_printer_t(filename_line, function_signature);
	}


	// dump

	enum base_t
	{
		BASE_BIN = 2,
//		BASE_OCT = 8,
		BASE_SDEC = -10,
		BASE_UDEC = 10,
		BASE_HEX = 16
//		BASE_FLT = 17,
//		BASE_DBL = 18,
//		BASE_LDBL = 19
	};


	enum byteorder_t
	{
		BYTE_ORDER_LITTLE_ENDIAN,
		BYTE_ORDER_BIG_ENDIAN
	};


	template <typename T>
	struct print_traits
	{
		typedef uint8_t unit_t;
		static const base_t BASE = BASE_HEX;
	};

#define PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(type, unit_type, base) \
				template <> \
				struct print_traits<type> \
				{ \
					typedef unit_type unit_t; \
					static const base_t BASE = base; \
				}


	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(int8_t, int8_t, BASE_HEX);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(int16_t, int16_t, BASE_SDEC);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(int32_t, int32_t, BASE_SDEC);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(int64_t, int64_t, BASE_SDEC);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(uint8_t, uint8_t, BASE_HEX);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(uint16_t, uint16_t, BASE_UDEC);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(uint32_t, uint32_t, BASE_UDEC);
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(uint64_t, uint64_t, BASE_UDEC);


	template <size_t S>
	struct width_for_numeric_size
	{
		static const size_t VALUE = 0;
	};

#define PRETTY_OUTPUT__DEFINE_WIDTH_FOR_NUMERIC_TYPE_SIZE(size, width) \
			template <> \
			struct width_for_numeric_size<size> \
			{ \
				static const size_t VALUE = width; \
			}


	PRETTY_OUTPUT__DEFINE_WIDTH_FOR_NUMERIC_TYPE_SIZE(1, 3);
	PRETTY_OUTPUT__DEFINE_WIDTH_FOR_NUMERIC_TYPE_SIZE(2, 5);
	PRETTY_OUTPUT__DEFINE_WIDTH_FOR_NUMERIC_TYPE_SIZE(4, 10);
	PRETTY_OUTPUT__DEFINE_WIDTH_FOR_NUMERIC_TYPE_SIZE(8, 20);


	template <typename T>
	struct width_for_type
	{
		static const size_t VALUE = (std::numeric_limits<T>::is_signed ? 1 : 0) + width_for_numeric_size<sizeof(typename print_traits<T>::unit_t)>::VALUE;
	};


	template <typename T>
	struct to_signed
	{
		typedef T type;
	};

#define PRETTY_OUTPUT__DEFINE_SIGNED_PROMOTIONS(source_type, promotion) \
		template <> \
		struct to_signed<source_type> \
		{ \
			typedef promotion type; \
		}


	template <typename T>
	struct to_unsigned
	{
		typedef T type;
	};


#define PRETTY_OUTPUT__DEFINE_UNSIGNED_PROMOTIONS(source_type, promotion) \
		template <> \
		struct to_unsigned<source_type> \
		{ \
			typedef promotion type; \
		}


	PRETTY_OUTPUT__DEFINE_SIGNED_PROMOTIONS(uint8_t, int8_t);
	PRETTY_OUTPUT__DEFINE_SIGNED_PROMOTIONS(uint16_t, int16_t);
	PRETTY_OUTPUT__DEFINE_SIGNED_PROMOTIONS(uint32_t, int32_t);
	PRETTY_OUTPUT__DEFINE_SIGNED_PROMOTIONS(uint64_t, int64_t);

	PRETTY_OUTPUT__DEFINE_UNSIGNED_PROMOTIONS(int8_t, uint8_t);
	PRETTY_OUTPUT__DEFINE_UNSIGNED_PROMOTIONS(int16_t, uint16_t);
	PRETTY_OUTPUT__DEFINE_UNSIGNED_PROMOTIONS(int32_t, uint32_t);
	PRETTY_OUTPUT__DEFINE_UNSIGNED_PROMOTIONS(int64_t, uint64_t);


	inline const char *const byte_to_binary(uint8_t byte);

	inline const char *const byte_to_octal(uint8_t byte);

	inline const char *const byte_to_hexadecimal(uint8_t byte);

	template <typename T>
	inline size_t field_width(base_t base);

	template <typename T>
	inline const std::string bytes_to_binary_string(T value);

	template <typename T>
	inline const std::string bytes_to_signed_decimal_string(T value);

	template <typename T>
	inline const std::string bytes_to_unsigned_decimal_string(T value);

	template <typename T>
	inline const std::string bytes_to_hexadecimal_string(T value);

	template <typename T>
	inline const std::string (*select_conversion(base_t base))(T);

	inline byteorder_t current_byte_order();

	inline void order_bytes(void *ordered_bytes, const void *unordered_bytes, size_t size, byteorder_t byte_order);


	template <typename T>
	inline void print_dump(const std::string &filename_line, const char *name, const T *pointer, size_t size = sizeof(T), base_t base = print_traits<T>::BASE, byteorder_t byte_order = current_byte_order())
	{
		typedef typename print_traits<T>::unit_t unit_t;

		const std::string (*bytes_to_string)(T) = select_conversion<T>(base);

		out_stream stream(filename_line);
		stream << "dump of " << name << ":";
		indentation_add();
		stream << endl;

		std::stringstream string_stream;

		size_t column_width = field_width<T>(base);

		const unit_t *iterator = (const unit_t*)pointer;
		size_t length = size / sizeof(unit_t);

		stream << make_value((void*)iterator) << ":";
		for (std::size_t index = 0; index < length; ++index)
		{
			if (string_stream.str().length() + column_width + 1 > stream.width_left())
			{
				stream << string_stream.str().c_str();
				string_stream.str("");

				stream << endl << make_value((void*)&iterator[index]) << ":";
			}

			string_stream << " ";
			string_stream.fill(' ');
			string_stream.width(column_width);
			string_stream.flags(std::ios::right);

			unit_t ordered_bytes;
			order_bytes(&ordered_bytes, &iterator[index], sizeof(unit_t), byte_order);

			string_stream << bytes_to_string(ordered_bytes);
		}

		if (!string_stream.str().empty())
		{
			stream << string_stream.str().c_str() << endl;
		}

		indentation_remove();
	}


	template <typename T>
	inline void print_dump(const std::string &filename_line, const char *name, const T &variable, base_t base = print_traits<T>::BASE, byteorder_t byte_order = current_byte_order())
	{
		print_dump(filename_line, name, (uint8_t*)&variable, sizeof(T), base, byte_order);
	}


#if __cplusplus >= 201103L

	// function call

	template <typename R, typename ...A>
	struct function_call_printer_t
	{
		typedef R (*funcptr_t)(A...);


		inline function_call_printer_t(const std::string &filename_line, const char *function_name, funcptr_t function_pointer);

		template <typename ...A2>
		inline R operator ()(A2 &&...arguments);

	private:
		std::string _filename_line;
		std::string _function_name;
		funcptr_t _function_pointer;
	};


	template <typename ...A>
	struct function_call_printer_t<void, A...>
	{
		typedef void (*funcptr_t)(A...);


		inline function_call_printer_t(const std::string &filename_line, const char *function_name, funcptr_t function_pointer);

		template <typename ...A2>
		inline void operator ()(A2 &&...arguments);

	private:
		std::string _filename_line;
		std::string _function_name;
		funcptr_t _function_pointer;
	};


	template <typename R, typename ...A>
	inline function_call_printer_t<R, A...> function_call(const std::string &filename_line, const char *function_name, R (*function_pointer)(A...))
	{
		return function_call_printer_t<R, A...>(filename_line, function_name, function_pointer);
	}


	// const member function call

	template <typename T, typename R, typename ...A>
	struct const_member_function_call_printer_t
	{
		typedef R (T::*funcptr_t)(A...) const;


		inline const_member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, const T &object, funcptr_t function_pointer);

		template <typename ...A2>
		inline R operator ()(A2 &&...arguments);

	private:
		std::string _filename_line;
		std::string _object_name;
		std::string _accessor;
		std::string _function_name;
		T &_object;
		funcptr_t _function_pointer;
	};


	template <typename T, typename ...A>
	struct const_member_function_call_printer_t<T, void, A...>
	{
		typedef void (T::*funcptr_t)(A...) const;


		inline const_member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, const T &object, funcptr_t function_pointer);

		template <typename ...A2>
		inline void operator ()(A2 &&...arguments);

	private:
		std::string _filename_line;
		std::string _object_name;
		std::string _accessor;
		std::string _function_name;
		const T &_object;
		funcptr_t _function_pointer;
	};


	template <typename T, typename R, typename ...A>
	inline const_member_function_call_printer_t<T, R, A...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, const T &object, R (T::*function_pointer)(A...) const)
	{
		return const_member_function_call_printer_t<T, R, A...>(filename_line, object_name, ".", function_name, object, function_pointer);
	}


	template <typename T, typename R, typename ...A>
	inline const_member_function_call_printer_t<T, R, A...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, const T *object, R (T::*function_pointer)(A...) const)
	{
		return const_member_function_call_printer_t<T, R, A...>(filename_line, object_name, "->", function_name, *object, function_pointer);
	}



	// non const member function call

	template <typename T, typename R, typename ...A>
	struct member_function_call_printer_t
	{
		typedef R (T::*funcptr_t)(A...);


		inline member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, T &object, funcptr_t function_pointer);

		template <typename ...A2>
		inline R operator ()(A2 &&...arguments);

	private:
		std::string _filename_line;
		std::string _object_name;
		std::string _accessor;
		std::string _function_name;
		T &_object;
		funcptr_t _function_pointer;
	};


	template <typename T, typename ...A>
	struct member_function_call_printer_t<T, void, A...>
	{
		typedef void (T::*funcptr_t)(A...);


		inline member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, T &object, funcptr_t function_pointer);

		template <typename ...A2>
		inline void operator ()(A2 &&...arguments);

	private:
		std::string _filename_line;
		std::string _object_name;
		std::string _accessor;
		std::string _function_name;
		T &_object;
		funcptr_t _function_pointer;
	};


	template <typename T, typename R, typename ...A>
	inline member_function_call_printer_t<T, R, A...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, T &object, R (T::*function_pointer)(A...))
	{
		return member_function_call_printer_t<T, R, A...>(filename_line, object_name, ".", function_name, object, function_pointer);
	}


	template <typename T, typename R, typename ...A>
	inline member_function_call_printer_t<T, R, A...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, T *object, R (T::*function_pointer)(A...))
	{
		return member_function_call_printer_t<T, R, A...>(filename_line, object_name, "->", function_name, *object, function_pointer);
	}

#endif // __cplusplus >= 201103L


	// return

	struct return_printer_t
	{
		inline return_printer_t(const std::string &filename_line);

		template <typename T>
		inline const T &operator ,(const T &value);

	private:
		std::string _filename_line;
	};


	inline return_printer_t return_printer(const std::string &filename_line)
	{
		return return_printer_t(filename_line);
	}


	// if block

	struct print_if_block_t
	{
		template <typename T>
		inline print_if_block_t(const std::string &filename_line, const char *expression, const T &value);

		template <typename T>
		inline print_if_block_t(const std::string &filename_line, const char *expression, T &value);

		inline print_if_block_t(const std::string &filename_line, const char *expression, bool value);

		inline ~print_if_block_t();

		inline operator bool() const;

	private:
		bool _condition;
	};


	template <typename T>
	inline print_if_block_t print_if_block(const std::string &filename_line, const char *expression, const T &value)
	{
		return print_if_block_t(filename_line, expression, value);
	}


	template <typename T>
	inline print_if_block_t print_if_block(const std::string &filename_line, const char *expression, T &value)
	{
		return print_if_block_t(filename_line, expression, value);
	}


	inline print_if_block_t print_if_block(const std::string &filename_line, const char *expression, bool value)
	{
		return print_if_block_t(filename_line, expression, value);
	}


	// for block

	struct for_block_t
	{
		inline for_block_t(const std::string &filename_line, const char *expression);
		inline ~for_block_t();
		inline operator bool() const;
		inline void iteration();

	private:
		std::string _filename_line;
		std::string _expression;
		size_t _iteration_number;
	};


	inline for_block_t for_block(const std::string &filename_line, const char *expression)
	{
		return for_block_t(filename_line, expression);
	}


	// while block

	struct print_while_block_t
	{
		template <typename T>
		inline print_while_block_t(const std::string &filename_line, const char *expression, const T &value);

		template <typename T>
		inline print_while_block_t(const std::string &filename_line, const char *expression, T &value);

		inline print_while_block_t(const std::string &filename_line, const char *expression, bool value);

		inline ~print_while_block_t();

		inline operator bool() const;

	private:
		bool _condition;
	};


	template <typename T>
	inline print_while_block_t print_while_block(const std::string &filename_line, const char *expression, const T &value)
	{
		return print_while_block_t(filename_line, expression, value);
	}

	template <typename T>
	inline print_while_block_t print_while_block(const std::string &filename_line, const char *expression, T &value)
	{
		return print_while_block_t(filename_line, expression, value);
	}

	inline print_while_block_t print_while_block(const std::string &filename_line, const char *expression, bool value)
	{
		return print_while_block_t(filename_line, expression, value);
	}


	// block

	struct block_t
	{
		inline block_t();
		inline ~block_t();
	};


	inline block_t block()
	{
		return block_t();
	}


	// helper stuff

	struct tlskey_t;

	tlskey_t *tls_new_key();
	void tls_delete_key(tlskey_t *key);
	void *tls_get(tlskey_t *key);
	void tls_set(tlskey_t *key, void *data);


	struct mutex_t;

	mutex_t *mutex_new();
	void mutex_delete(mutex_t *mutex);
	void mutex_lock(mutex_t *mutex);
	void mutex_unlock(mutex_t *mutex);


	// definitions

	function_printer_t::function_printer_t(const std::string &filename_line, const char *function_signature)
		: _filename_line(filename_line), _function_signature(function_signature)
	{
		out_stream(_filename_line) << "[call] " << _function_signature.c_str();
		indentation_add();
	}


	function_printer_t::~function_printer_t()
	{
		indentation_remove();
		out_stream(_filename_line) << "[ret]  " << _function_signature.c_str();
	}


	// dump

	const char *const byte_to_binary(uint8_t byte)
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


	const char *const byte_to_octal(uint8_t byte)
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


	const char *const byte_to_hexadecimal(uint8_t byte)
	{
		static const char *const HEXADECIMAL_VALUES[] = {
			"00", "01", "02", "03", "04", "05", "06", "07",
			"08", "09", "0a", "0b", "0c", "0d", "0e", "0f",
			"10", "11", "12", "13", "14", "15", "16", "17",
			"18", "19", "1a", "1b", "1c", "1d", "1e", "1f",
			"20", "21", "22", "23", "24", "25", "26", "27",
			"28", "29", "2a", "2b", "2c", "2d", "2e", "2f",
			"30", "31", "32", "33", "34", "35", "36", "37",
			"38", "39", "3a", "3b", "3c", "3d", "3e", "3f",
			"40", "41", "42", "43", "44", "45", "46", "47",
			"48", "49", "4a", "4b", "4c", "4d", "4e", "4f",
			"50", "51", "52", "53", "54", "55", "56", "57",
			"58", "59", "5a", "5b", "5c", "5d", "5e", "5f",
			"60", "61", "62", "63", "64", "65", "66", "67",
			"68", "69", "6a", "6b", "6c", "6d", "6e", "6f",
			"70", "71", "72", "73", "74", "75", "76", "77",
			"78", "79", "7a", "7b", "7c", "7d", "7e", "7f",
			"80", "81", "82", "83", "84", "85", "86", "87",
			"88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
			"90", "91", "92", "93", "94", "95", "96", "97",
			"98", "99", "9a", "9b", "9c", "9d", "9e", "9f",
			"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
			"a8", "a9", "aa", "ab", "ac", "ad", "ae", "af",
			"b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7",
			"b8", "b9", "ba", "bb", "bc", "bd", "be", "bf",
			"c0", "c1", "c2", "c3", "c4", "c5", "c6", "c7",
			"c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf",
			"d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
			"d8", "d9", "da", "db", "dc", "dd", "de", "df",
			"e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7",
			"e8", "e9", "ea", "eb", "ec", "ed", "ee", "ef",
			"f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",
			"f8", "f9", "fa", "fb", "fc", "fd", "fe", "ff"
		};

		return HEXADECIMAL_VALUES[byte];
	}


	template <typename T>
	size_t field_width(base_t base)
	{
		switch (base)
		{
			case BASE_BIN:
				return sizeof(typename print_traits<T>::unit_t) * 8;

			case BASE_SDEC:
			case BASE_UDEC:
				return width_for_type<T>::VALUE;

			default: // BASE_HEX
				return sizeof(typename print_traits<T>::unit_t) * 2;
		}
	}


	template <typename T>
	const std::string bytes_to_binary_string(T value)
	{
		std::stringstream stream;
		uint8_t *data = (uint8_t*)&value;
		for (size_t index = 0; index < sizeof(value); ++index)
		{
			stream << byte_to_binary(data[index]);
		}

		return stream.str();
	}


	template <typename T>
	const std::string bytes_to_signed_decimal_string(T value)
	{
		typedef typename to_signed<T>::type signed_promotion_t;

		signed_promotion_t signed_value = (signed_promotion_t)value;
		int64_t signed_integer = (int64_t)signed_value;

		std::stringstream stream;
		stream << signed_integer;

		return stream.str();
	}


	template <typename T>
	const std::string bytes_to_unsigned_decimal_string(T value)
	{
		typedef typename to_unsigned<T>::type unsigned_promotion_t;

		unsigned_promotion_t unsigned_value = (unsigned_promotion_t)value;
		uint64_t unsigned_integer = (uint64_t)unsigned_value;

		std::stringstream stream;
		stream << unsigned_integer;

		return stream.str();
	}


	template <typename T>
	const std::string bytes_to_hexadecimal_string(T value)
	{
		std::stringstream stream;
		uint8_t *data = (uint8_t*)&value;
		for (size_t index = 0; index < sizeof(value); ++index)
		{
			stream << byte_to_hexadecimal(data[index]);
		}

		return stream.str();
	}


	template <typename T>
	const std::string (*select_conversion(base_t base))(T)
	{
		switch (base)
		{
			case BASE_BIN:
				return bytes_to_binary_string<T>;

			case BASE_SDEC:
				return bytes_to_signed_decimal_string<T>;

			case BASE_UDEC:
				return bytes_to_unsigned_decimal_string<T>;

			default: // BASE_HEX
				return bytes_to_hexadecimal_string<T>;
		}
	}


	byteorder_t current_byte_order()
	{
		static const uint16_t VALUE = 0x0001;
		static const uint8_t FIRST_BYTE = *(uint8_t*)&VALUE;

		if (FIRST_BYTE == 0x01)
		{
			return BYTE_ORDER_BIG_ENDIAN;
		}
		else
		{
			return BYTE_ORDER_LITTLE_ENDIAN;
		}
	}


	void order_bytes(void *ordered_bytes, const void *unordered_bytes, size_t size, byteorder_t byte_order)
	{
		if (current_byte_order() != byte_order)
		{
			uint8_t *ordered_bytes_iterator = (uint8_t*)ordered_bytes;
			const uint8_t *unordered_bytes_iterator = (const uint8_t*)unordered_bytes + size - 1;
			for (;;)
			{
				if (unordered_bytes_iterator < unordered_bytes)
				{
					break;
				}

				*ordered_bytes_iterator = *unordered_bytes_iterator;
				++ordered_bytes_iterator;
				--unordered_bytes_iterator;
			}
		}
		else
		{
			std::memcpy(ordered_bytes, unordered_bytes, size);
		}
	}


#if __cplusplus >= 201103L

	// function call

	template <typename R, typename ...A>
	function_call_printer_t<R, A...>::function_call_printer_t(const std::string &filename_line, const char *function_name, funcptr_t function_pointer)
		: _filename_line(filename_line), _function_name(function_name), _function_pointer(function_pointer)
	{
	}


	template <typename R, typename ...A>
	template <typename ...A2>
	R function_call_printer_t<R, A...>::operator ()(A2 &&...arguments)
	{
		R return_value = _function_pointer(std::forward<A2>(arguments)...);
		out_stream(_filename_line) << _function_name.c_str() << "(" << make_values(", ", arguments...) << ") => " << make_value(return_value);
		return return_value;
	}


	template <typename ...A>
	function_call_printer_t<void, A...>::function_call_printer_t(const std::string &filename_line, const char *function_name, funcptr_t function_pointer)
		: _filename_line(filename_line), _function_name(function_name), _function_pointer(function_pointer)
	{
	}


	template <typename ...A>
	template <typename ...A2>
	void function_call_printer_t<void, A...>::operator ()(A2 &&...arguments)
	{
		_function_pointer(std::forward<A2>(arguments)...);
		out_stream(_filename_line) << _function_name.c_str() << "(" << make_values(", ", arguments...) << ")";
	}


	// const member function call

	template <typename T, typename R, typename ...A>
	const_member_function_call_printer_t<T, R, A...>::const_member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, const T &object, funcptr_t function_pointer)
		: _filename_line(filename_line), _object_name(object_name), _accessor(accessor), _function_name(function_name), _object(object), _function_pointer(function_pointer)
	{
	}


	template <typename T, typename R, typename ...A>
	template <typename ...A2>
	R const_member_function_call_printer_t<T, R, A...>::operator ()(A2 &&...arguments)
	{
		R return_value = (_object.*_function_pointer)(std::forward<A2>(arguments)...);
		out_stream(_filename_line) << _object_name.c_str() << _accessor.c_str() << _function_name.c_str() << "(" << make_values(", ", arguments...) << ") => " << make_value(return_value);
		return return_value;
	}


	template <typename T, typename ...A>
	const_member_function_call_printer_t<T, void, A...>::const_member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, const T &object, funcptr_t function_pointer)
		: _filename_line(filename_line), _object_name(object_name), _accessor(accessor), _function_name(function_name), _object(object), _function_pointer(function_pointer)
	{
	}


	template <typename T, typename ...A>
	template <typename ...A2>
	void const_member_function_call_printer_t<T, void, A...>::operator ()(A2 &&...arguments)
	{
		(_object.*_function_pointer)(std::forward<A2>(arguments)...);
		out_stream(_filename_line) << _object_name.c_str() << _accessor.c_str() << _function_name.c_str() << "(" << make_values(", ", arguments...) << ")";
	}


	// non const member function call

	template <typename T, typename R, typename ...A>
	member_function_call_printer_t<T, R, A...>::member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, T &object, funcptr_t function_pointer)
		: _filename_line(filename_line), _object_name(object_name), _accessor(accessor), _function_name(function_name), _object(object), _function_pointer(function_pointer)
	{
	}


	template <typename T, typename R, typename ...A>
	template <typename ...A2>
	R member_function_call_printer_t<T, R, A...>::operator ()(A2 &&...arguments)
	{
		R return_value = (_object.*_function_pointer)(std::forward<A2>(arguments)...);
		out_stream(_filename_line) << _object_name.c_str() << _accessor.c_str() << _function_name.c_str() << "(" << make_values(", ", arguments...) << ") => " << make_value(return_value);
		return return_value;
	}


	template <typename T, typename ...A>
	member_function_call_printer_t<T, void, A...>::member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, T &object, funcptr_t function_pointer)
		: _filename_line(filename_line), _object_name(object_name), _accessor(accessor), _function_name(function_name), _object(object), _function_pointer(function_pointer)
	{
	}


	template <typename T, typename ...A>
	template <typename ...A2>
	void member_function_call_printer_t<T, void, A...>::operator ()(A2 &&...arguments)
	{
		(_object.*_function_pointer)(std::forward<A2>(arguments)...);
		out_stream(_filename_line) << _object_name.c_str() << _accessor.c_str() << _function_name.c_str() << "(" << make_values(", ", arguments...) << ")";
	}

#endif // __cpusplus >= 201103L


	// return

	return_printer_t::return_printer_t(const std::string &filename_line)
		: _filename_line(filename_line)
	{
	}


	template <typename T>
	const T &return_printer_t::operator ,(const T &value)
	{
		out_stream(_filename_line) << "return " << make_value(value);
		return value;
	}

	// if

	template <typename T>
	print_if_block_t::print_if_block_t(const std::string &filename_line, const char *expression, const T &value)
		: _condition(value)
	{
		out_stream(filename_line) << "if (" << expression << ") => " << make_value((bool)value) << " (" << make_value(value) << ")";
		indentation_add();
	}


	template <typename T>
	print_if_block_t::print_if_block_t(const std::string &filename_line, const char *expression, T &value)
		: _condition(value)
	{
		out_stream(filename_line) << "if (" << expression << ") => " << make_value((bool)value) << " (" << make_value(value) << ")";
		indentation_add();
	}


	print_if_block_t::print_if_block_t(const std::string &filename_line, const char *expression, bool value)
		: _condition(value)
	{
		out_stream(filename_line) << "if (" << expression << ") => " << make_value(value);
		indentation_add();
	}


	print_if_block_t::~print_if_block_t()
	{
		indentation_remove();
	}


	print_if_block_t::operator bool() const
	{
		return _condition;
	}


	// for

	for_block_t::for_block_t(const std::string &filename_line, const char *expression)
		: _filename_line(filename_line), _expression(expression), _iteration_number(0)
	{
		out_stream(_filename_line) << "for (" << _expression.c_str() << ")";
		indentation_add();
	}


	for_block_t::~for_block_t()
	{
		indentation_remove();
	}


	for_block_t::operator bool() const
	{
		return false;
	}


	void for_block_t::iteration()
	{
		indentation_remove();
		out_stream(_filename_line) << "[iteration #" << make_value(_iteration_number) << "]";
		indentation_add();

		++_iteration_number;
	}


	// while

	template <typename T>
	print_while_block_t::print_while_block_t(const std::string &filename_line, const char *expression, const T &value)
		: _condition(value)
	{
		out_stream(filename_line) << "while (" << expression << ") => " << make_value((bool)value) << " (" << make_value(value) << ")";
		indentation_add();
	}


	template <typename T>
	print_while_block_t::print_while_block_t(const std::string &filename_line, const char *expression, T &value)
		: _condition(value)
	{
		out_stream(filename_line) << "while (" << expression << ") => " << make_value((bool)value) << " (" << make_value(value) << ")";
		indentation_add();
	}


	print_while_block_t::print_while_block_t(const std::string &filename_line, const char *expression, bool value)
		: _condition(value)
	{
		out_stream(filename_line) << "while (" << expression << ") => " << make_value(value);
		indentation_add();
	}


	print_while_block_t::~print_while_block_t()
	{
		indentation_remove();
	}


	print_while_block_t::operator bool() const
	{
		return _condition;
	}


	// block

	block_t::block_t()
	{
		indentation_add();
	}


	block_t::~block_t()
	{
		indentation_remove();
	}

}


#if defined(__clang__)

	#pragma clang diagnostic pop

#elif defined(__GNUC__) || defined(__GNUG__)

	#pragma gnu diagnostic pop

#endif

