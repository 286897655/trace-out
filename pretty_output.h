#pragma once


#if __cplusplus >= 201103L || _MSC_VER >= 1800
	#define PRETTY_OUTPUT_CPP11
#endif


#include <limits>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <stdint.h>

#if defined(PRETTY_OUTPUT_CPP11)
	#include <tuple>
#endif


#if defined(__clang__)

	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"
	#pragma clang diagnostic ignored "-Wvariadic-macros"
	#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

#elif defined(__GNUC__) || defined(__GNUG__)

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wvariadic-macros" // doesn't work with MinGW and probably also with GCC

#elif defined(_MSC_VER)

	#pragma warning(push)
	#pragma warning(disable:4296) // expression is always false

#endif


//
// Public macros

#if (!defined(NDEBUG) && !defined(PRETTY_OUTPUT_OFF)) || defined(PRETTY_OUTPUT_ON)

	#define $w(...) \
				pretty_output::watch(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, __VA_ARGS__)


	#define $m(pointer, ...) \
				pretty_output::print_memory(PRETTY_OUTPUT_FILENAME_LINE, #pointer, pointer, ##__VA_ARGS__);


	#define $f \
				pretty_output::function_printer_t PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$f) = pretty_output::function_printer(PRETTY_OUTPUT_FILENAME_LINE, PRETTY_OUTPUT_FUNCTION_SIGNATURE);


	#if defined(PRETTY_OUTPUT_CPP11)

	#define $c(function_name) \
				pretty_output::function_call(PRETTY_OUTPUT_FILENAME_LINE, #function_name, function_name)


	#define $cm(object, function_name) \
				pretty_output::member_function_call(PRETTY_OUTPUT_FILENAME_LINE, #object, #function_name, pretty_output::reference(object), &std::remove_pointer<decltype(object)>::type::function_name)

	#endif // defined(PRETTY_OUTPUT_CPP11)


	#define $return \
				return pretty_output::return_printer(PRETTY_OUTPUT_FILENAME_LINE) ,


	#define $if(...) \
				if (pretty_output::block_t<bool, bool> PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$if_block) = pretty_output::block(PRETTY_OUTPUT_FILENAME_LINE, "if (" #__VA_ARGS__ ") => ", static_cast<bool>((__VA_ARGS__))))


	#define pretty_output_for(block_name, ...) \
				if (pretty_output::for_block_t block_name = pretty_output::for_block_t(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__)) {} else \
					for (__VA_ARGS__) \
						if (pretty_output::block_t<size_t, bool> PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$block) = pretty_output::block("// for: iteration #", block_name.iteration(), false)) {} else


	#define $for(...) \
				pretty_output_for(PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$for_block), __VA_ARGS__)


	#define $while(...) \
				if ((pretty_output::out_stream(PRETTY_OUTPUT_FILENAME_LINE) << "while (" << #__VA_ARGS__ << ")"), false) {} else \
					while (pretty_output::block_t<bool, bool> PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$while_block) = pretty_output::block("// while: " #__VA_ARGS__ " => ", static_cast<bool>((__VA_ARGS__))))


	#define $_ \
				pretty_output::block_t<void, void> PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$block) = pretty_output::block_t<void, void>();


	#define $p(format, ...) \
				pretty_output::out_stream(PRETTY_OUTPUT_FILENAME_LINE).printf(format, ##__VA_ARGS__);


	#define $t(name) \
				pretty_output::set_current_thread_name(#name);

#elif defined(NDEBUG) || defined(PRETTY_OUTPUT_OFF)

	#define $w(...)


	#define $m(pointer, ...)


	#define $f


	#if defined(PRETTY_OUTPUT_CPP11)

	#define $c(function_name) \
				function_name


	#define $cm(object, function_name) \
				(object.*&std::remove_pointer<decltype(object)>::type::function_name)

	#endif // defined(PRETTY_OUTPUT_CPP11)


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


//
// Private macros

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



#if !defined(PRETTY_OUTPUT_REDIRECTION)
	#define PRETTY_OUTPUT_REDIRECTION pretty_output
#endif

namespace PRETTY_OUTPUT_REDIRECTION
{

	void print(const char *string);
	void flush();

}


namespace pretty_output
{

	//
	// Declarations
	//


	//
	// Common

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

	const std::string thread_id_field(uint64_t thread_id);
	const std::string thread_header(const std::string &thread_id, const std::string &thread_name);
	const std::string filename_from_path(const char *path);
	const std::string filename_line_field(const std::string &file, unsigned int line);

	template <typename T>
	T &reference(T &object);

	template <typename T>
	const T &reference(const T &object);

	template <typename T>
	T &reference(T *object);

	template <typename T>
	const T &reference(const T *object);


	//
	// Values

	template <typename T>
	struct value_t
	{
		value_t(const T &value);


		const T &data;

	private:
		value_t &operator = (const value_t&);
	};


	inline value_t<const char*> make_value(const char *const &value);

	template <typename T>
	value_t<T> make_value(const T &value);


#if defined(PRETTY_OUTPUT_CPP11)

	template <typename...>
	struct values_t;


	template <typename T, typename ...R>
	struct values_t<T, R...>
	{
		values_t(const char *delimiter_value, const T &first, const R &...rest);


		const char *delimiter;
		const T &data;
		values_t<R...> values;
	};


	template <typename T>
	struct values_t<T>
	{
		values_t(const char *, const T &value);


		const char *delimiter;
		const T &data;
	};


	template <>
	struct values_t<>
	{
		template <typename ...T>
		values_t(const char *, const T &...);
	};


	template <typename ...T>
	inline values_t<T...> make_values(const char *delimiter, const T &...values);

#endif // defined(PRETTY_OUTPUT_CPP11)


	//
	// Out stream

	struct endl_t;
	struct flush_t;

	extern const endl_t ENDL;
	extern const flush_t FLUSH;


	class out_stream
	{
	public:
		out_stream(const std::string &filename_line);
		out_stream();
		~out_stream();
		out_stream &operator <<(char character);
		out_stream &operator <<(const char *string);
		out_stream &operator <<(const endl_t&);
		out_stream &operator <<(const flush_t&);
		size_t width_left() const;
		void printf(const char *format, ...);
		void flush();

	private:
		size_t _current_line_length;
	};


	//
	// 'operator <<' overloads

	inline out_stream &operator <<(out_stream &stream, value_t<const char*> string);

	inline out_stream &operator <<(out_stream &stream, value_t<std::string> value);

	inline out_stream &operator <<(out_stream &stream, value_t<short> value);

	inline out_stream &operator <<(out_stream &stream, value_t<unsigned short> value);

	inline out_stream &operator <<(out_stream &stream, value_t<int> number);

	inline out_stream &operator <<(out_stream &stream, value_t<unsigned int> value);

	inline out_stream &operator <<(out_stream &stream, value_t<long> value);

	inline out_stream &operator <<(out_stream &stream, value_t<unsigned long> value);

#if defined(PRETTY_OUTPUT_CPP11)

	inline out_stream &operator <<(out_stream &stream, value_t<long long> value);

	inline out_stream &operator <<(out_stream &stream, value_t<unsigned long long> value);

#endif // defined(PRETTY_OUTPUT_CPP11)

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

#if defined(PRETTY_OUTPUT_CPP11)

	template <typename T>
	inline out_stream &operator <<(out_stream &stream, values_t<T> values);

	template <typename ...T>
	inline out_stream &operator <<(out_stream &stream, values_t<T...> values);

	inline out_stream &operator <<(out_stream &stream, values_t<> values);

	template <typename ...T>
	inline out_stream &operator <<(out_stream &stream, value_t<std::tuple<T...> > tuple);

	template <template <typename ...> class Container, typename ...A>
	inline out_stream &operator <<(out_stream &stream, value_t<Container<A...> > value);

#endif // defined(PRETTY_OUTPUT_CPP11)


	//
	// Watch

	template <typename T>
	const T &watch(const std::string &filename_line, const char *name, const T &value);

	template <typename T>
	T &watch(const std::string &filename_line, const char *name, T &value);


	//
	// Memory

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



	const char *byte_to_binary(uint8_t byte);

	const char *byte_to_hexadecimal(uint8_t byte);

	template <typename T>
	size_t field_width(base_t base);

	template <typename T>
	const std::string bytes_to_binary_string(T value);

	template <typename T>
	const std::string bytes_to_signed_decimal_string(T value);

	template <typename T>
	const std::string bytes_to_unsigned_decimal_string(T value);

	template <typename T>
	const std::string bytes_to_floating_point_string(T value);

	template <typename T>
	const std::string bytes_to_hexadecimal_string(T value);

	template <typename T>
	const std::string (*select_conversion(base_t base))(T);

	byteorder_t current_byte_order();

	void reverse_bytes(void *destination, const void *source, size_t size);

	void order_bytes(void *ordered_bytes, const void *unordered_bytes, size_t size, byteorder_t byte_order);

	template <typename T>
	void print_memory(const std::string &filename_line, const char *name, const T *pointer, size_t size = sizeof(T), base_t base = print_traits<T>::default_base, byteorder_t byte_order = current_byte_order());

	template <typename T>
	void print_memory(const std::string &filename_line, const char *name, const T &variable, base_t base = print_traits<T>::default_base, byteorder_t byte_order = current_byte_order());


#if defined(PRETTY_OUTPUT_CPP11)

	//
	// Function call

	template <typename R, typename ...A>
	struct function_call_printer_t
	{
		typedef R (*funcptr_t)(A...);


		inline function_call_printer_t(const std::string &filename_line, const char *function_name, funcptr_t function_pointer);

		template <typename ...A2>
		R operator ()(A2 &&...arguments);

	private:
		std::string _filename_line;
		std::string _function_name;
		funcptr_t _function_pointer;
	};


	template <typename ...A>
	struct function_call_printer_t<void, A...>
	{
		typedef void (*funcptr_t)(A...);


		function_call_printer_t(const std::string &filename_line, const char *function_name, funcptr_t function_pointer);

		template <typename ...A2>
		void operator ()(A2 &&...arguments);

	private:
		std::string _filename_line;
		std::string _function_name;
		funcptr_t _function_pointer;
	};


	template <typename R, typename ...A>
	function_call_printer_t<R, A...> function_call(const std::string &filename_line, const char *function_name, R (*function_pointer)(A...));


	//
	// Const member function call

	template <typename T, typename R, typename ...A>
	struct const_member_function_call_printer_t
	{
		typedef R (T::*funcptr_t)(A...) const;


		const_member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, const T &object, funcptr_t function_pointer);

		template <typename ...A2>
		R operator ()(A2 &&...arguments);

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


		const_member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, const T &object, funcptr_t function_pointer);

		template <typename ...A2>
		void operator ()(A2 &&...arguments);

	private:
		std::string _filename_line;
		std::string _object_name;
		std::string _accessor;
		std::string _function_name;
		const T &_object;
		funcptr_t _function_pointer;
	};


	template <typename T, typename R, typename ...A>
	const_member_function_call_printer_t<T, R, A...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, const T &object, R (T::*function_pointer)(A...) const);


	//
	// Non const member function call

	template <typename T, typename R, typename ...A>
	struct member_function_call_printer_t
	{
		typedef R (T::*funcptr_t)(A...);


		member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, T &object, funcptr_t function_pointer);

		template <typename ...A2>
		R operator ()(A2 &&...arguments);

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


		member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, T &object, funcptr_t function_pointer);

		template <typename ...A2>
		void operator ()(A2 &&...arguments);

	private:
		std::string _filename_line;
		std::string _object_name;
		std::string _accessor;
		std::string _function_name;
		T &_object;
		funcptr_t _function_pointer;
	};


	template <typename T, typename R, typename ...A>
	member_function_call_printer_t<T, R, A...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, T &object, R (T::*function_pointer)(A...));

#endif // defined(PRETTY_OUTPUT_CPP11)


	//
	// Function printer

	struct function_printer_t
	{
		function_printer_t(const std::string &filename_line, const char *function_signature);

		~function_printer_t();

	private:
		std::string _filename_line;
		std::string _function_signature;
	};


	function_printer_t function_printer(const std::string &filename_line, const char *function_signature);


	//
	// Return printer

	struct return_printer_t
	{
		return_printer_t(const std::string &filename_line);

		template <typename T>
		const T &operator ,(const T &value);

	private:
		std::string _filename_line;
	};


	return_printer_t return_printer(const std::string &filename_line);


	//
	// Block
	// NOTE: god-entity, but still better than prevous solution

	template <typename C, typename R>
	struct block_t
	{
		block_t(const std::string &filename_line, const char *comment, const C &comment_value);
		block_t(const char *comment, const C &comment_value);
		block_t(const char *comment, const C &comment_value, const R &retval);
		~block_t();
		operator const R &();


		const R &return_value;
	};


	template <>
	struct block_t<void, void>
	{
		block_t();
		~block_t();
	};


	template <typename C>
	block_t<C, C> block(const std::string &filename_line, const char *comment, const C &comment_value);

	template <typename C>
	block_t<C, C> block(const char *comment, const C &comment_value);

	template <typename C, typename R>
	block_t<C, R> block(const char *comment, const C &comment_value, const R &return_value);


	//
	// For block

	struct for_block_t
	{
		for_block_t(const std::string &filename_line, const char *expression);
		~for_block_t();
		operator bool() const;
		size_t iteration();

	private:
		size_t _iteration_number;
	};


	for_block_t for_block(const std::string &filename_line, const char *expression);


	//
	// Helper stuff

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



	//
	// Definitions
	//


	//
	// Common

	template <typename T>
	T &reference(T &object)
	{
		return object;
	}


	template <typename T>
	const T &reference(const T &object)
	{
		return object;
	}


	template <typename T>
	T &reference(T *object)
	{
		return *object;
	}


	template <typename T>
	const T &reference(const T *object)
	{
		return *object;
	}


	//
	// Values

	template <typename T>
	value_t<T>::value_t(const T &value)
		: data(value)
	{
	}


	template <typename T>
	value_t<T> &value_t<T>::operator =(const value_t&)
	{
		return *this;
	}


	value_t<const char*> make_value(const char *const &value)
	{
		return value_t<const char*>(value);
	}


	template <typename T>
	inline value_t<T> make_value(const T &value)
	{
		return value_t<T>(value);
	}


#if defined(PRETTY_OUTPUT_CPP11)

	template <typename T, typename ...R>
	values_t<T, R...>::values_t(const char *delimiter_value, const T &first, const R &...rest)
		: delimiter(delimiter_value), data(first), values(delimiter_value, rest...)
	{
	}


	template <typename T>
	values_t<T>::values_t(const char *, const T &value)
		: delimiter(""), data(value)
	{
	}


	template <typename ...T>
	values_t<>::values_t(const char *, const T &...)
	{
	}


	template <typename ...T>
	inline values_t<T...> make_values(const char *delimiter, const T &...values)
	{
		return values_t<T...>(delimiter, values...);
	}

#endif // defined(PRETTY_OUTPUT_CPP11)


	//
	// 'operator <<' overloads

	template <typename T>
	const std::string fundamental_to_string(T value)
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


#if defined(PRETTY_OUTPUT_CPP11)

	out_stream &operator <<(out_stream &stream, value_t<long long> value)
	{
		return stream << fundamental_to_string(value.data).c_str();
	}


	out_stream &operator <<(out_stream &stream, value_t<unsigned long long> value)
	{
		return stream << fundamental_to_string(value.data).c_str();
	}

#endif // defined(PRETTY_OUTPUT_CPP11)


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
		std::size_t numeric_value = reinterpret_cast<uintptr_t>(value.data);
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


#if defined(PRETTY_OUTPUT_CPP11)

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

#endif // defined(PRETTY_OUTPUT_CPP11)


	//
	// Watch

	template <typename T>
	const T &watch(const std::string &filename_line, const char *name, const T &value)
	{
		out_stream(filename_line) << name << " = " << make_value(value);
		return value;
	}


	template <typename T>
	T &watch(const std::string &filename_line, const char *name, T &value)
	{
		out_stream(filename_line) << name << " = " << make_value(value);
		return value;
	}


	//
	// Memory

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

			case BASE_HEX:
			default:
				return sizeof(typename print_traits<T>::unit_t) * 2;
		}
	}


	template <typename T>
	const std::string bytes_to_binary_string(T value)
	{
		std::stringstream stream;
		uint8_t *data = reinterpret_cast<uint8_t*>(&value);
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

		signed_promotion_t signed_value = static_cast<signed_promotion_t>(value);
		int64_t signed_integer = static_cast<int64_t>(signed_value);

		std::stringstream stream;
		stream << signed_integer;

		return stream.str();
	}


	template <typename T>
	const std::string bytes_to_unsigned_decimal_string(T value)
	{
		typedef typename print_traits<T>::unsigned_t unsigned_promotion_t;

		unsigned_promotion_t unsigned_value = static_cast<unsigned_promotion_t>(value);
		uint64_t unsigned_integer = static_cast<uint64_t>(unsigned_value);

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
		uint8_t *data = reinterpret_cast<uint8_t*>(&value);
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

			case BASE_HEX:
			default:
				return bytes_to_hexadecimal_string<T>;
		}
	}


	template <typename T>
	void print_memory(const std::string &filename_line, const char *name, const T *pointer, size_t size, base_t base, byteorder_t byte_order)
	{
		typedef typename print_traits<T>::unit_t unit_t;

		const std::string (*bytes_to_string)(T) = select_conversion<T>(base);

		out_stream stream(filename_line);
		stream << "memory of " << name << ":";
		indentation_add();
		stream << ENDL;

		std::stringstream string_stream;

		size_t column_width = field_width<T>(base);

		const unit_t *iterator = reinterpret_cast<const unit_t*>(pointer);
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
	void print_memory(const std::string &filename_line, const char *name, const T &variable, base_t base, byteorder_t byte_order)
	{
		print_memory(filename_line, name, reinterpret_cast<const uint8_t*>(&variable), sizeof(T), base, byte_order);
	}


#if defined(PRETTY_OUTPUT_CPP11)

	//
	// Function call

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


	template <typename R, typename ...A>
	function_call_printer_t<R, A...> function_call(const std::string &filename_line, const char *function_name, R (*function_pointer)(A...))
	{
		return function_call_printer_t<R, A...>(filename_line, function_name, function_pointer);
	}


	//
	// Const member function call

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


	template <typename T, typename R, typename ...A>
	const_member_function_call_printer_t<T, R, A...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, const T &object, R (T::*function_pointer)(A...) const)
	{
		return const_member_function_call_printer_t<T, R, A...>(filename_line, object_name, ".", function_name, object, function_pointer);
	}


	template <typename T, typename R, typename ...A>
	const_member_function_call_printer_t<T, R, A...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, const T *object, R (T::*function_pointer)(A...) const)
	{
		return const_member_function_call_printer_t<T, R, A...>(filename_line, object_name, "->", function_name, *object, function_pointer);
	}


	//
	// Non-const member function call

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


	template <typename T, typename R, typename ...A>
	member_function_call_printer_t<T, R, A...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, T &object, R (T::*function_pointer)(A...))
	{
		return member_function_call_printer_t<T, R, A...>(filename_line, object_name, ".", function_name, object, function_pointer);
	}


	template <typename T, typename R, typename ...A>
	member_function_call_printer_t<T, R, A...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, T *object, R (T::*function_pointer)(A...))
	{
		return member_function_call_printer_t<T, R, A...>(filename_line, object_name, "->", function_name, *object, function_pointer);
	}

#endif // defined(PRETTY_OUTPUT_CPP11)


	//
	// Return

	template <typename T>
	const T &return_printer_t::operator ,(const T &value)
	{
		out_stream(_filename_line) << "return " << make_value(value);
		return value;
	}


	//
	// Block

	template <typename C, typename R>
	block_t<C, R>::block_t(const std::string &filename_line, const char *comment, const C &comment_value)
		: return_value(comment_value)
	{
		out_stream(filename_line) << comment << make_value(comment_value);
		indentation_add();
	}


	template <typename C, typename R>
	block_t<C, R>::block_t(const char *comment, const C &comment_value)
		: return_value(comment_value)
	{
		indentation_add();
		out_stream() << comment << make_value(comment_value);
	}


	template <typename C, typename R>
	block_t<C, R>::block_t(const char *comment, const C &comment_value, const R &retval)
		: return_value(retval)
	{
		indentation_add();
		out_stream() << comment << make_value(comment_value);
	}


	template <typename C, typename R>
	block_t<C, R>::~block_t()
	{
		indentation_remove();
		out_stream();
	}


	template <typename C, typename R>
	block_t<C, R>::operator const R&()
	{
		return return_value;
	}


	template <typename C>
	block_t<C, C> block(const std::string &filename_line, const char *comment, const C &comment_value)
	{
		return block_t<C, C>(filename_line, comment, comment_value);
	}


	template <typename C>
	block_t<C, C> block(const char *comment, const C &comment_value)
	{
		return block_t<C, C>(comment, comment_value);
	}


	template <typename C, typename R>
	block_t<C, R> block(const char *comment, const C &comment_value, const R &return_value)
	{
		return block_t<C, R>(comment, comment_value, return_value);
	}

}


#undef PRETTY_OUTPUT_CPP11
#undef PRETTY_OUTPUT__DEFINE_PRINT_TRAITS


#if defined(__clang__)

	#pragma clang diagnostic pop

#elif defined(__GNUC__) || defined(__GNUG__)

	#pragma GCC diagnostic pop

#elif defined(_MSC_VER)

	#pragma warning(pop)

#endif

