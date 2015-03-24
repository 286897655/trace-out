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

	#pragma GCC diagnostic push
	// TODO: turn off warnings

#endif


// public macros:

#if (!defined(NDEBUG) && !defined(PRETTY_OUTPUT_OFF)) || defined(PRETTY_OUTPUT_ON)

	#define $w(...) \
				pretty_output::watch(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, __VA_ARGS__)


	#define $m(pointer, ...) \
				pretty_output::print_memory(PRETTY_OUTPUT_FILENAME_LINE, #pointer, pointer, ##__VA_ARGS__);


	#define $f \
				pretty_output::function_printer_t PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$f) = pretty_output::function_printer(PRETTY_OUTPUT_FILENAME_LINE, PRETTY_OUTPUT_FUNCTION_SIGNATURE);


	#if __cplusplus >= 201103L

	#define $c(function_name) \
				pretty_output::function_call(PRETTY_OUTPUT_FILENAME_LINE, #function_name, function_name)


	#define $cm(object, function_name) \
				pretty_output::member_function_call(PRETTY_OUTPUT_FILENAME_LINE, #object, #function_name, object, &std::remove_pointer<decltype(object)>::type::function_name)

	#endif // __cplusplus >= 201103L


	#define $return \
				return pretty_output::return_printer(PRETTY_OUTPUT_FILENAME_LINE) ,


	#define $if(...) \
				if (pretty_output::print_if_block_t PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$if_block) = pretty_output::print_if_block(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, __VA_ARGS__))


	#define pretty_output_for(block, ...) \
				if (pretty_output::for_block_t block = pretty_output::for_block_t(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__)) {} else \
					for (__VA_ARGS__) \
						if (block.iteration(), false) {} else


	#define $for(...) \
				pretty_output_for(PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$for_block), __VA_ARGS__)


	#define $while(...) \
				if (pretty_output::print_while_header(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__), false) {} else \
					while (pretty_output::print_while_block_t PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$while_block) = pretty_output::print_while_block(#__VA_ARGS__, __VA_ARGS__))


	#define $_ \
				pretty_output::block_t PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$block) = pretty_output::block();


	#define $p(format, ...) \
				pretty_output::out_stream(PRETTY_OUTPUT_FILENAME_LINE).printf(format, ##__VA_ARGS__);


	#define $t(name) \
				pretty_output::set_current_thread_name(#name);

#elif defined(NDEBUG) || defined(PRETTY_OUTPUT_OFF)

	#define $w(...)


	#define $m(pointer, ...)


	#define $f


	#if __cplusplus >= 201103L

	#define $c(function_name) \
				function_name


	#define $cm(object, function_name) \
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


#if defined(PRETTY_OUTPUT_REDIRECTION)
	#define PRETTY_OUTPUT_REDIRECTION_NAMESPACE PRETTY_OUTPUT_REDIRECTION
#else
	#define PRETTY_OUTPUT_REDIRECTION_NAMESPACE pretty_output
#endif

namespace PRETTY_OUTPUT_REDIRECTION_NAMESPACE
{

	void print(const char *string);
	void flush();

}


namespace pretty_output
{

	// declarations

	extern const size_t WIDTH;
	extern const char INDENTATION[];
	extern const char THREAD_HEADER_SEPARATOR;
	extern const char FILENAME_FIELD_EXCESS_PADDING[];
	extern const size_t FILENAME_FIELD_EXCESS_PADDING_SIZE;
	extern const size_t FILENAME_FIELD_WIDTH;
	extern const size_t LINE_FIELD_WIDTH;
	extern const char DELIMITER[];
	extern const char FILE_PATH_COMPONENT_DELIMITER;
	extern const size_t DELIMITER_WIDTH;
	extern const size_t INDENTATION_WIDTH;


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
			filename = filename.substr(0, FILENAME_FIELD_WIDTH - FILENAME_FIELD_EXCESS_PADDING_SIZE);
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


	template <>
	struct values_t<>
	{
		template <typename ...T>
		values_t(const char *, const T &...)
		{
		}
	};


	template <typename ...T>
	inline values_t<T...> make_values(const char *delimiter, const T &...values)
	{
		return values_t<T...>(delimiter, values...);
	}

#endif

	struct endl_t;
	struct flush_t;

	extern const endl_t ENDL;
	extern const flush_t FLUSH;


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

			flush();

			unlock_output();
		}


		out_stream &operator <<(char character)
		{
			char string[2] = {character, '\0'};
			PRETTY_OUTPUT_REDIRECTION_NAMESPACE::print(string);
			++_current_line_length;

			return *this;
		}


		out_stream &operator <<(const char *string)
		{
			PRETTY_OUTPUT_REDIRECTION_NAMESPACE::print(string);
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


		out_stream &operator <<(const flush_t&)
		{
			flush();
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
			PRETTY_OUTPUT_REDIRECTION_NAMESPACE::flush();
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

	inline out_stream &operator <<(out_stream &stream, values_t<> values);

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
		return stream << "'" << value.data << "'";
	}


	template <typename A, typename B>
	out_stream &operator <<(out_stream &stream, value_t<std::pair<A, B> > value)
	{
		return stream << "{" << make_value(value.data.first) << ": " << make_value(value.data.second) << "}";
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


	out_stream &operator <<(out_stream &stream, values_t<>)
	{
		return stream;
	}


	template <std::size_t I, typename ...T>
	typename std::enable_if<I == sizeof...(T) - 1, out_stream&>::type print_tuple(out_stream &stream, const std::tuple<T...> &tuple)
	{
		return stream << make_value(std::get<I>(tuple)) << ")";
	}


	template <std::size_t I, typename ...T>
	typename std::enable_if<I < sizeof...(T) - 1, out_stream&>::type print_tuple(out_stream &stream, const std::tuple<T...> &tuple)
	{
		stream << ", " << make_value(std::get<I>(tuple)) << ", ";
		return print_tuple<I + 1>(stream, tuple);
	}


	template <typename ...T>
	out_stream &operator <<(out_stream &stream, value_t<std::tuple<T...> > value)
	{
		stream << "(" << make_value(std::get<0>(value.data)) << ", ";
		return print_tuple<1>(stream, value.data);
	}


	// not sure if all C++11 standard library versions have std::next
	template <typename T>
	T next_itr(T iterator)
	{
		++iterator;
		return iterator;
	}


	template <template <typename ...> class Container, typename ...A>
	out_stream &operator <<(out_stream &stream, value_t<Container<A...> > value)
	{
		const auto &container = value.data;

		stream << "[";
		auto iterator = container.cbegin();
		for ( ; next_itr(iterator) != container.cend(); ++iterator)
		{
			stream << make_value(*iterator) << ", ";
		}

		stream << make_value(*iterator) << "]";

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


	// memory

	enum base_t
	{
		BASE_BIN = 2,
		BASE_SDEC = -10,
		BASE_UDEC = 10,
		BASE_HEX = 16,
		BASE_FLT = 17,
		BASE_DBL = 18,
		BASE_LDBL = 19
	};


	enum byteorder_t
	{
		BYTE_ORDER_LITTLE_ENDIAN,
		BYTE_ORDER_BIG_ENDIAN
	};


	enum typefamily_t
	{
		TYPE_FAMILY_INTEGER,
		TYPE_FAMILY_FLOATING_POINT,
		TYPE_FAMILY_OTHER
	};


	template <typename T>
	struct type_family
	{
		static const typefamily_t value = std::numeric_limits<T>::is_integer ? TYPE_FAMILY_INTEGER : (std::numeric_limits<T>::is_specialized ? TYPE_FAMILY_FLOATING_POINT : TYPE_FAMILY_OTHER);
	};


	template <typefamily_t F, size_t S, bool Si>
	struct print_traits_details
	{
		typedef uint8_t unit_t;
		static const size_t field_width = 2;
		static const base_t default_base = BASE_HEX;
		typedef void signed_t;
		typedef void unsigned_t;
	};

#define PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(family, type_size, is_signed, unit_type, field_width_value, default_base_value, to_signed_type, to_unsigned_type) \
			template <> \
			struct print_traits_details<family, type_size, is_signed> \
			{ \
				typedef unit_type unit_t; \
				static const size_t field_width = field_width_value; \
				static const base_t default_base = default_base_value; \
				typedef to_signed_type signed_t; \
				typedef to_unsigned_type unsigned_t; \
			}

	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 1, true,
										int8_t, 2, BASE_HEX, int8_t, uint8_t);

	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 2, true,
										int16_t, 6, BASE_SDEC, int16_t, uint16_t);

	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 4, true,
										int32_t, 11, BASE_SDEC, int32_t, uint32_t);

	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 8, true,
										int64_t, 21, BASE_SDEC, int64_t, uint64_t);


	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 1, false,
										uint8_t, 2, BASE_HEX, int8_t, uint8_t);

	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 2, false,
										uint16_t, 5, BASE_UDEC, int16_t, uint16_t);

	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 4, false,
										uint32_t, 10, BASE_UDEC, int32_t, uint32_t);

	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 8, false,
										uint64_t, 20, BASE_UDEC, int64_t, uint64_t);


	// first_digit + point + precision + 'e' + sign + exponent
	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 4, true,
										float, 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 3, BASE_FLT, float, float);

	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 8, true,
										double, 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 4, BASE_DBL, double, double);

	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 10, true,
										long double, 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, BASE_LDBL, long double, long double);

	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 12, true,
										long double, 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, BASE_LDBL, long double, long double);

	PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 16, true,
										long double, 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, BASE_LDBL, long double, long double);


	template <typename T>
	struct print_traits
		: public print_traits_details<type_family<T>::value, sizeof(T), std::numeric_limits<T>::is_signed>
	{
	};



	inline const char *const byte_to_binary(uint8_t byte);

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
	inline const std::string bytes_to_floating_point_string(T value);

	template <typename T>
	inline const std::string bytes_to_hexadecimal_string(T value);

	template <typename T>
	inline const std::string (*select_conversion(base_t base))(T);

	inline byteorder_t current_byte_order();

	inline void order_bytes(void *ordered_bytes, const void *unordered_bytes, size_t size, byteorder_t byte_order);


	template <typename T>
	inline void print_memory(const std::string &filename_line, const char *name, const T *pointer, size_t size = sizeof(T), base_t base = print_traits<T>::default_base, byteorder_t byte_order = current_byte_order())
	{
		typedef typename print_traits<T>::unit_t unit_t;

		const std::string (*bytes_to_string)(T) = select_conversion<T>(base);

		out_stream stream(filename_line);
		stream << "memory of " << name << ":";
		indentation_add();
		stream << ENDL;

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

				stream << ENDL << make_value((void*)&iterator[index]) << ":";
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
			stream << string_stream.str().c_str() << ENDL;
		}

		indentation_remove();
	}


	template <typename T>
	inline void print_memory(const std::string &filename_line, const char *name, const T &variable, base_t base = print_traits<T>::default_base, byteorder_t byte_order = current_byte_order())
	{
		print_memory(filename_line, name, (uint8_t*)&variable, sizeof(T), base, byte_order);
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
		const T &_object;
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

	inline void print_while_header(const std::string &filename_line, const char *expression);


	struct print_while_block_t
	{
		template <typename T>
		inline print_while_block_t(const char *expression, const T &value);

		template <typename T>
		inline print_while_block_t(const char *expression, T &value);

		inline print_while_block_t(const char *expression, bool value);

		inline ~print_while_block_t();

		inline operator bool() const;

	private:
		bool _condition;
	};


	template <typename T>
	inline print_while_block_t print_while_block(const char *expression, const T &value)
	{
		return print_while_block_t(expression, value);
	}

	template <typename T>
	inline print_while_block_t print_while_block(const char *expression, T &value)
	{
		return print_while_block_t(expression, value);
	}

	inline print_while_block_t print_while_block(const char *expression, bool value)
	{
		return print_while_block_t(expression, value);
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
		out_stream(_filename_line) << _function_signature.c_str() << ENDL << "{";
		indentation_add();
	}


	function_printer_t::~function_printer_t()
	{
		indentation_remove();
		out_stream(_filename_line) << "}    // " << _function_signature.c_str();
	}


	// memory

	extern const char *const BINARY_VALUES[];
	extern const char *const HEXADECIMAL_VALUES[];

	const char *const byte_to_binary(uint8_t byte)
	{
		return BINARY_VALUES[byte];
	}


	const char *const byte_to_hexadecimal(uint8_t byte)
	{
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
				return print_traits<T>::field_width + (!std::numeric_limits<T>::is_signed ? 1 : 0);

			case BASE_UDEC:
				return print_traits<T>::field_width - (std::numeric_limits<T>::is_signed ? 1 : 0);

			case BASE_FLT:
			case BASE_DBL:
			case BASE_LDBL:
				return print_traits<T>::field_width;

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
		typedef typename print_traits<T>::signed_t signed_promotion_t;

		signed_promotion_t signed_value = (signed_promotion_t)value;
		int64_t signed_integer = (int64_t)signed_value;

		std::stringstream stream;
		stream << signed_integer;

		return stream.str();
	}


	template <typename T>
	const std::string bytes_to_unsigned_decimal_string(T value)
	{
		typedef typename print_traits<T>::unsigned_t unsigned_promotion_t;

		unsigned_promotion_t unsigned_value = (unsigned_promotion_t)value;
		uint64_t unsigned_integer = (uint64_t)unsigned_value;

		std::stringstream stream;
		stream << unsigned_integer;

		return stream.str();
	}


	template <typename T>
	inline const std::string bytes_to_floating_point_string(T value)
	{
		std::stringstream stream;
		stream.precision(std::numeric_limits<T>::digits10);
		stream << std::scientific << value;

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

			case BASE_FLT:
			case BASE_DBL:
			case BASE_LDBL:
				return bytes_to_floating_point_string<T>;

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
		out_stream stream(_filename_line);
		stream << _function_name.c_str() << "(" << make_values(", ", arguments...) << ") => " << FLUSH;

		R return_value = _function_pointer(std::forward<A2>(arguments)...);
		stream << make_value(return_value);

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
		out_stream stream(_filename_line);
		stream << _function_name.c_str() << "(" << make_values(", ", arguments...) << ") => " << FLUSH;

		_function_pointer(std::forward<A2>(arguments)...);
		stream << "(void)";
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
		out_stream stream(_filename_line);
		stream << _object_name.c_str() << _accessor.c_str() << _function_name.c_str() << "(" << make_values(", ", arguments...) << ") => " << FLUSH;

		R return_value = (_object.*_function_pointer)(std::forward<A2>(arguments)...);
		stream << make_value(return_value);

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
		out_stream stream(_filename_line);
		stream << _object_name.c_str() << _accessor.c_str() << _function_name.c_str() << "(" << make_values(", ", arguments...) << ") => " << FLUSH;

		(_object.*_function_pointer)(std::forward<A2>(arguments)...);
		stream << "(void)";
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
		out_stream stream(_filename_line);
		stream << _object_name.c_str() << _accessor.c_str() << _function_name.c_str() << "(" << make_values(", ", arguments...) << ") => " << FLUSH;

		R return_value = (_object.*_function_pointer)(std::forward<A2>(arguments)...);
		stream << make_value(return_value);

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
		out_stream stream(_filename_line);
		stream << _object_name.c_str() << _accessor.c_str() << _function_name.c_str() << "(" << make_values(", ", arguments...) << ") => " << FLUSH;

		(_object.*_function_pointer)(std::forward<A2>(arguments)...);
		stream << "(void)";
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
		out_stream();
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
		out_stream();
		indentation_remove();
	}


	for_block_t::operator bool() const
	{
		return false;
	}


	void for_block_t::iteration()
	{
		out_stream stream;
		if (_iteration_number > 0)
		{
			stream << ENDL;
		}

		stream << "// for: #" << make_value(_iteration_number);
		++_iteration_number;
	}


	// while

	void print_while_header(const std::string &filename_line, const char *expression)
	{
		out_stream(filename_line) << "while (" << expression << ")";
	}


	template <typename T>
	print_while_block_t::print_while_block_t(const char *expression, const T &value)
		: _condition(value)
	{
		indentation_add();
		out_stream() << "// while: " << expression << " => " << make_value((bool)value) << " (" << make_value(value) << ")";
	}


	template <typename T>
	print_while_block_t::print_while_block_t(const char *expression, T &value)
		: _condition(value)
	{
		indentation_add();
		out_stream() << "// while: " << expression << " => " << make_value((bool)value) << " (" << make_value(value) << ")";
	}


	print_while_block_t::print_while_block_t(const char *expression, bool value)
		: _condition(value)
	{
		indentation_add();
		out_stream() << "// while: " << expression << " => " << make_value(value);
	}


	print_while_block_t::~print_while_block_t()
	{
		out_stream();
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

	#pragma GCC diagnostic pop

#endif

