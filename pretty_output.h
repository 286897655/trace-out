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
				if (pretty_output::print_while_header(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__), false) {} else \
					while (pretty_output::block_t<bool, bool> PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$while_block) = pretty_output::block("// while: " #__VA_ARGS__ " => ", static_cast<bool>((__VA_ARGS__))))


	#define $_ \
				pretty_output::block_t<void, void> PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$block) = pretty_output::block_t<void, void>();


	#define $p(format, ...) \
				{ \
					pretty_output::out_stream stream(PRETTY_OUTPUT_FILENAME_LINE); \
					stream.printf(format, ##__VA_ARGS__); \
					stream << pretty_output::ENDLINE; \
				}


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

	template <typename Type_t>
	Type_t &reference(Type_t &object);

	template <typename Type_t>
	const Type_t &reference(const Type_t &object);

	template <typename Type_t>
	Type_t &reference(Type_t *object);

	template <typename Type_t>
	const Type_t &reference(const Type_t *object);


	//
	// Values

	template <typename Type_t>
	struct value_t
	{
		value_t(const Type_t &value);


		const Type_t &data;

	private:
		value_t &operator = (const value_t &);
	};


	inline value_t<const char *> make_value(const char *const &value);

	template <typename Type_t>
	value_t<Type_t> make_value(const Type_t &value);


#if defined(PRETTY_OUTPUT_CPP11)

	template <typename...>
	struct values_t;


	template <typename Type_t, typename ...RestTypes_t>
	struct values_t<Type_t, RestTypes_t...>
	{
		values_t(const char *delimiter_value, const Type_t &first, const RestTypes_t &...rest);


		const char *delimiter;
		const Type_t &data;
		values_t<RestTypes_t...> values;
	};


	template <typename Type_t>
	struct values_t<Type_t>
	{
		values_t(const char *, const Type_t &value);


		const char *delimiter;
		const Type_t &data;
	};


	template <>
	struct values_t<>
	{
		template <typename ...Whatever_t>
		values_t(const char *, const Whatever_t &...);
	};


	template <typename ...Types_t>
	values_t<Types_t...> make_values(const char *delimiter, const Types_t &...values);

#endif // defined(PRETTY_OUTPUT_CPP11)


	//
	// Out stream

	struct newline_t;
	struct endline_t;
	struct flush_t;

	extern const newline_t NEWLINE;
	extern const endline_t ENDLINE;
	extern const flush_t FLUSH;


	class out_stream
	{
	public:
		out_stream(const std::string &filename_line);
		out_stream(const newline_t &);
		out_stream();
		~out_stream();
		out_stream &operator <<(char character);
		out_stream &operator <<(const char *string);
		out_stream &operator <<(const newline_t &);
		out_stream &operator <<(const endline_t &);
		out_stream &operator <<(const flush_t &);
		size_t width_left() const;
		void printf(const char *format, ...);
		void flush();

	private:
		size_t _current_line_length;
	};


	//
	// 'operator <<' overloads

	inline out_stream &operator <<(out_stream &stream, value_t<const char *> string);

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

	inline out_stream &operator <<(out_stream &stream, value_t<const void *> value);

	template <typename Type_t>
	inline out_stream &operator <<(out_stream &stream, value_t<const Type_t *> value);

	template <typename Type_t>
	inline out_stream &operator <<(out_stream &stream, value_t<Type_t *> value);

	inline out_stream &operator <<(out_stream &stream, value_t<bool> value);

	inline out_stream &operator <<(out_stream &stream, value_t<char> value);

	template <typename First_t, typename Second_t>
	inline out_stream &operator <<(out_stream &stream, value_t<std::pair<First_t, Second_t> > value);

#if defined(PRETTY_OUTPUT_CPP11)

	template <typename Type_t>
	inline out_stream &operator <<(out_stream &stream, values_t<Type_t> values);

	template <typename ...Types_t>
	inline out_stream &operator <<(out_stream &stream, values_t<Types_t...> values);

	inline out_stream &operator <<(out_stream &stream, values_t<> values);

	template <typename ...Types_t>
	inline out_stream &operator <<(out_stream &stream, value_t<std::tuple<Types_t...> > tuple);

	template <template <typename ...> class Container, typename ...Parameters_t>
	inline out_stream &operator <<(out_stream &stream, value_t<Container<Parameters_t...> > value);

#endif // defined(PRETTY_OUTPUT_CPP11)


	//
	// Watch

	template <typename Types_t>
	const Types_t &watch(const std::string &filename_line, const char *name, const Types_t &value);

	template <typename Types_t>
	Types_t &watch(const std::string &filename_line, const char *name, Types_t &value);


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


	template <typename Type_t>
	struct type_family
	{
		static const typefamily_t value = std::numeric_limits<Type_t>::is_integer ? TYPE_FAMILY_INTEGER : (std::numeric_limits<Type_t>::is_specialized ? TYPE_FAMILY_FLOATING_POINT : TYPE_FAMILY_OTHER);
	};


	template <typefamily_t Family, size_t Size, bool IsSigned>
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


	template <typename Type_t>
	struct print_traits
		: public print_traits_details<type_family<Type_t>::value, sizeof(Type_t), std::numeric_limits<Type_t>::is_signed>
	{
	};



	const char *byte_to_binary(uint8_t byte);

	const char *byte_to_hexadecimal(uint8_t byte);

	template <typename Type_t>
	size_t field_width(base_t base);

	template <typename Type_t>
	const std::string bytes_to_binary_string(Type_t value);

	template <typename Type_t>
	const std::string bytes_to_signed_decimal_string(Type_t value);

	template <typename Type_t>
	const std::string bytes_to_unsigned_decimal_string(Type_t value);

	template <typename Type_t>
	const std::string bytes_to_floating_point_string(Type_t value);

	template <typename Type_t>
	const std::string bytes_to_hexadecimal_string(Type_t value);

	template <typename Type_t>
	const std::string (*select_conversion(base_t base))(Type_t);

	byteorder_t current_byte_order();

	void reverse_bytes(void *destination, const void *source, size_t size);

	void order_bytes(void *ordered_bytes, const void *unordered_bytes, size_t size, byteorder_t byte_order);

	// NOTE: Visual Studio crashes at passing 'Type_t' to 'print_traits' in function declaration, so assume the following commented code as a function declarations

	// template <typename Type_t>
	// void print_memory(const std::string &filename_line, const char *name, const Type_t *pointer, size_t size = sizeof(Type_t), base_t base = print_traits<Type_t>::default_base, byteorder_t byte_order = current_byte_order());

	// template <typename Type_t>
	// void print_memory(const std::string &filename_line, const char *name, const Type_t &variable, base_t base = print_traits<Type_t>::default_base, byteorder_t byte_order = current_byte_order());


#if defined(PRETTY_OUTPUT_CPP11)

	//
	// Function call

	template <typename Return_t, typename ...Arguments_t>
	struct function_call_printer_t
	{
		typedef Return_t (*funcptr_t)(Arguments_t...);


		function_call_printer_t(const std::string &filename_line, const char *function_name, funcptr_t function_pointer);

		template <typename ...CallArguments_t>
		Return_t operator ()(CallArguments_t &&...arguments);

	private:
		std::string _filename_line;
		std::string _function_name;
		funcptr_t _function_pointer;
	};


	template <typename ...Arguments_t>
	struct function_call_printer_t<void, Arguments_t...>
	{
		typedef void (*funcptr_t)(Arguments_t...);


		function_call_printer_t(const std::string &filename_line, const char *function_name, funcptr_t function_pointer);

		template <typename ...CallArguments_t>
		void operator ()(CallArguments_t &&...arguments);

	private:
		std::string _filename_line;
		std::string _function_name;
		funcptr_t _function_pointer;
	};


	template <typename Return_t, typename ...Arguments_t>
	function_call_printer_t<Return_t, Arguments_t...> function_call(const std::string &filename_line, const char *function_name, Return_t (*function_pointer)(Arguments_t...));


	//
	// Const member function call

	template <typename Type_t, typename Return_t, typename ...Arguments_t>
	struct const_member_function_call_printer_t
	{
		typedef Return_t (Type_t::*funcptr_t)(Arguments_t...) const;


		const_member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, const Type_t &object, funcptr_t function_pointer);

		template <typename ...CallArguments_t>
		Return_t operator ()(CallArguments_t &&...arguments);

	private:
		std::string _filename_line;
		std::string _object_name;
		std::string _accessor;
		std::string _function_name;
		const Type_t &_object;
		funcptr_t _function_pointer;
	};


	template <typename Type_t, typename ...Arguments_t>
	struct const_member_function_call_printer_t<Type_t, void, Arguments_t...>
	{
		typedef void (Type_t::*funcptr_t)(Arguments_t...) const;


		const_member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, const Type_t &object, funcptr_t function_pointer);

		template <typename ...CallArguments_t>
		void operator ()(CallArguments_t &&...arguments);

	private:
		std::string _filename_line;
		std::string _object_name;
		std::string _accessor;
		std::string _function_name;
		const Type_t &_object;
		funcptr_t _function_pointer;
	};


	template <typename Type_t, typename Return_t, typename ...Arguments_t>
	const_member_function_call_printer_t<Type_t, Return_t, Arguments_t...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, const Type_t &object, Return_t (Type_t::*function_pointer)(Arguments_t...) const);


	//
	// Non const member function call

	template <typename Type_t, typename Return_t, typename ...Arguments_t>
	struct member_function_call_printer_t
	{
		typedef Return_t (Type_t::*funcptr_t)(Arguments_t...);


		member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, Type_t &object, funcptr_t function_pointer);

		template <typename ...CallArguments_t>
		Return_t operator ()(CallArguments_t &&...arguments);

	private:
		std::string _filename_line;
		std::string _object_name;
		std::string _accessor;
		std::string _function_name;
		Type_t &_object;
		funcptr_t _function_pointer;
	};


	template <typename Type_t, typename ...Arguments_t>
	struct member_function_call_printer_t<Type_t, void, Arguments_t...>
	{
		typedef void (Type_t::*funcptr_t)(Arguments_t...);


		member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, Type_t &object, funcptr_t function_pointer);

		template <typename ...CallArguments_t>
		void operator ()(CallArguments_t &&...arguments);

	private:
		std::string _filename_line;
		std::string _object_name;
		std::string _accessor;
		std::string _function_name;
		Type_t &_object;
		funcptr_t _function_pointer;
	};


	template <typename Type_t, typename Return_t, typename ...Arguments_t>
	member_function_call_printer_t<Type_t, Return_t, Arguments_t...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, Type_t &object, Return_t (Type_t::*function_pointer)(Arguments_t...));

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

	template <typename Comment_value_t, typename Return_t>
	struct block_t
	{
		block_t(const std::string &filename_line, const char *comment, const Comment_value_t &comment_value);
		block_t(const char *comment, const Comment_value_t &comment_value);
		block_t(const char *comment, const Comment_value_t &comment_value, const Return_t &retval);
		~block_t();
		operator const Return_t &();


		const Return_t &return_value;
	};


	template <>
	struct block_t<void, void>
	{
		block_t();
		~block_t();
	};


	template <typename Comment_value_t>
	block_t<Comment_value_t, Comment_value_t> block(const std::string &filename_line, const char *comment, const Comment_value_t &comment_value);

	template <typename Comment_value_t>
	block_t<Comment_value_t, Comment_value_t> block(const char *comment, const Comment_value_t &comment_value);

	template <typename Comment_value_t, typename Return_t>
	block_t<Comment_value_t, Return_t> block(const char *comment, const Comment_value_t &comment_value, const Return_t &return_value);


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
	// While header

	void print_while_header(const std::string &filename_line, const char *condition);


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

	template <typename Type_t>
	Type_t &reference(Type_t &object)
	{
		return object;
	}


	template <typename Type_t>
	const Type_t &reference(const Type_t &object)
	{
		return object;
	}


	template <typename Type_t>
	Type_t &reference(Type_t *object)
	{
		return *object;
	}


	template <typename Type_t>
	const Type_t &reference(const Type_t *object)
	{
		return *object;
	}


	//
	// Values

	template <typename Type_t>
	value_t<Type_t>::value_t(const Type_t &value)
		: data(value)
	{
	}


	template <typename Type_t>
	value_t<Type_t> &value_t<Type_t>::operator =(const value_t &)
	{
		return *this;
	}


	value_t<const char *> make_value(const char *const &value)
	{
		return value_t<const char *>(value);
	}


	template <typename Type_t>
	value_t<Type_t> make_value(const Type_t &value)
	{
		return value_t<Type_t>(value);
	}


#if defined(PRETTY_OUTPUT_CPP11)

	template <typename Type_t, typename ...RestTypes_t>
	values_t<Type_t, RestTypes_t...>::values_t(const char *delimiter_value, const Type_t &first, const RestTypes_t &...rest)
		: delimiter(delimiter_value), data(first), values(delimiter_value, rest...)
	{
	}


	template <typename Type_t>
	values_t<Type_t>::values_t(const char *, const Type_t &value)
		: delimiter(""), data(value)
	{
	}


	template <typename ...Whatever_t>
	values_t<>::values_t(const char *, const Whatever_t &...)
	{
	}


	template <typename ...Types_t>
	values_t<Types_t...> make_values(const char *delimiter, const Types_t &...values)
	{
		return values_t<Types_t...>(delimiter, values...);
	}

#endif // defined(PRETTY_OUTPUT_CPP11)


	//
	// 'operator <<' overloads

	template <typename Type_t>
	const std::string fundamental_to_string(Type_t value)
	{
		std::stringstream stream;
		stream << value;

		return stream.str();
	}


	out_stream &operator <<(out_stream &stream, value_t<const char *> value)
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


	out_stream &operator <<(out_stream &stream, value_t<const void *> value)
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


	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, value_t<const Type_t *> value)
	{
		stream << make_value((const void *)value.data) << " ";
		if (value.data != NULL)
		{
			stream.flush();
			stream << "-> " << make_value(*(value.data));
		}

		return stream;
	}


	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, value_t<Type_t *> value)
	{
		return stream << make_value((const Type_t *)value.data);
	}


	out_stream &operator <<(out_stream &stream, value_t<bool> value)
	{
		return stream << (value.data ? "true" : "false");
	}


	out_stream &operator <<(out_stream &stream, value_t<char> value)
	{
		return stream << "'" << value.data << "'";
	}


	template <typename First_t, typename Second_t>
	out_stream &operator <<(out_stream &stream, value_t<std::pair<First_t, Second_t> > value)
	{
		return stream << "{" << make_value(value.data.first) << ": " << make_value(value.data.second) << "}";
	}


#if defined(PRETTY_OUTPUT_CPP11)

	template <typename Type_t>
	out_stream &operator <<(out_stream &stream, values_t<Type_t> values)
	{
		return stream << make_value(values.data);
	}


	template <typename ...Types_t>
	out_stream &operator <<(out_stream &stream, values_t<Types_t...> values)
	{
		return stream << make_value(values.data) << values.delimiter << values.values;
	}


	out_stream &operator <<(out_stream &stream, values_t<>)
	{
		return stream;
	}


	template <std::size_t Index, typename ...Types_t>
	typename std::enable_if<Index == sizeof...(Types_t) - 1, out_stream &>::type print_tuple(out_stream &stream, const std::tuple<Types_t...> &tuple)
	{
		return stream << make_value(std::get<Index>(tuple)) << ")";
	}


	template <std::size_t Index, typename ...Types_t>
	typename std::enable_if<Index < sizeof...(Types_t) - 1, out_stream &>::type print_tuple(out_stream &stream, const std::tuple<Types_t...> &tuple)
	{
		stream << ", " << make_value(std::get<Index>(tuple)) << ", ";
		return print_tuple<Index + 1>(stream, tuple);
	}


	template <typename ...Types_t>
	out_stream &operator <<(out_stream &stream, value_t<std::tuple<Types_t...> > value)
	{
		stream << "(" << make_value(std::get<0>(value.data)) << ", ";
		return print_tuple<1>(stream, value.data);
	}


	// not sure if all C++11 standard library versions have std::next
	template <typename Types_t>
	Types_t next_itr(Types_t iterator)
	{
		++iterator;
		return iterator;
	}


	template <template <typename ...> class Container, typename ...Parameters_t>
	out_stream &operator <<(out_stream &stream, value_t<Container<Parameters_t...> > value)
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

	template <typename Types_t>
	const Types_t &watch(const std::string &filename_line, const char *name, const Types_t &value)
	{
		out_stream stream(filename_line);
		stream << name << " = " << make_value(value) << ENDLINE;
		return value;
	}


	template <typename Types_t>
	Types_t &watch(const std::string &filename_line, const char *name, Types_t &value)
	{
		out_stream stream(filename_line);
		stream << name << " = " << make_value(value) << ENDLINE;
		return value;
	}


	//
	// Memory

	template <typename Type_t>
	size_t field_width(base_t base)
	{
		switch (base)
		{
			case BASE_BIN:
				return sizeof(typename print_traits<Type_t>::unit_t) * 8;

			case BASE_SDEC:
				return print_traits<Type_t>::field_width + (!std::numeric_limits<Type_t>::is_signed ? 1 : 0);

			case BASE_UDEC:
				return print_traits<Type_t>::field_width - (std::numeric_limits<Type_t>::is_signed ? 1 : 0);

			case BASE_FLT:
			case BASE_DBL:
			case BASE_LDBL:
				return print_traits<Type_t>::field_width;

			case BASE_HEX:
			default:
				return sizeof(typename print_traits<Type_t>::unit_t) * 2;
		}
	}


	template <typename Type_t>
	const std::string bytes_to_binary_string(Type_t value)
	{
		std::stringstream stream;
		uint8_t *data = reinterpret_cast<uint8_t *>(&value);
		for (size_t index = 0; index < sizeof(value); ++index)
		{
			stream << byte_to_binary(data[index]);
		}

		return stream.str();
	}


	template <typename Type_t>
	const std::string bytes_to_signed_decimal_string(Type_t value)
	{
		typedef typename print_traits<Type_t>::signed_t signed_promotion_t;

		signed_promotion_t signed_value = static_cast<signed_promotion_t>(value);
		int64_t signed_integer = static_cast<int64_t>(signed_value);

		std::stringstream stream;
		stream << signed_integer;

		return stream.str();
	}


	template <typename Type_t>
	const std::string bytes_to_unsigned_decimal_string(Type_t value)
	{
		typedef typename print_traits<Type_t>::unsigned_t unsigned_promotion_t;

		unsigned_promotion_t unsigned_value = static_cast<unsigned_promotion_t>(value);
		uint64_t unsigned_integer = static_cast<uint64_t>(unsigned_value);

		std::stringstream stream;
		stream << unsigned_integer;

		return stream.str();
	}


	template <typename Type_t>
	const std::string bytes_to_floating_point_string(Type_t value)
	{
		std::stringstream stream;
		stream.precision(std::numeric_limits<Type_t>::digits10);
		stream << std::scientific << value;

		return stream.str();
	}


	template <typename Type_t>
	const std::string bytes_to_hexadecimal_string(Type_t value)
	{
		std::stringstream stream;
		uint8_t *data = reinterpret_cast<uint8_t *>(&value);
		for (size_t index = 0; index < sizeof(value); ++index)
		{
			stream << byte_to_hexadecimal(data[index]);
		}

		return stream.str();
	}


	template <typename Type_t>
	const std::string (*select_conversion(base_t base))(Type_t)
	{
		switch (base)
		{
			case BASE_BIN:
				return bytes_to_binary_string<Type_t>;

			case BASE_SDEC:
				return bytes_to_signed_decimal_string<Type_t>;

			case BASE_UDEC:
				return bytes_to_unsigned_decimal_string<Type_t>;

			case BASE_FLT:
			case BASE_DBL:
			case BASE_LDBL:
				return bytes_to_floating_point_string<Type_t>;

			case BASE_HEX:
			default:
				return bytes_to_hexadecimal_string<Type_t>;
		}
	}


	template <typename Type_t>
	void print_memory(const std::string &filename_line, const char *name, const Type_t *pointer, size_t size = sizeof(Type_t), base_t base = print_traits<Type_t>::default_base, byteorder_t byte_order = current_byte_order())
	{
		typedef typename print_traits<Type_t>::unit_t unit_t;

		const std::string (*bytes_to_string)(Type_t) = select_conversion<Type_t>(base);

		out_stream stream(filename_line);
		stream << "memory of " << name << ":";
		indentation_add();
		stream << NEWLINE;

		std::stringstream string_stream;

		size_t column_width = field_width<Type_t>(base);

		const unit_t *iterator = reinterpret_cast<const unit_t *>(pointer);
		size_t length = size / sizeof(unit_t);

		stream << make_value((void *)iterator) << ":";
		for (std::size_t index = 0; index < length; ++index)
		{
			if (string_stream.str().length() + column_width + 1 > stream.width_left())
			{
				stream << string_stream.str().c_str();
				string_stream.str("");

				stream << NEWLINE << make_value((void *)&iterator[index]) << ":";
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
			stream << string_stream.str().c_str() << NEWLINE;
		}

		indentation_remove();

		stream << ENDLINE;
	}


	template <typename Type_t>
	void print_memory(const std::string &filename_line, const char *name, const Type_t &variable, base_t base = print_traits<Type_t>::default_base, byteorder_t byte_order = current_byte_order())
	{
		print_memory(filename_line, name, reinterpret_cast<const uint8_t *>(&variable), sizeof(Type_t), base, byte_order);
	}


#if defined(PRETTY_OUTPUT_CPP11)

	//
	// Function call

	template <typename Return_t, typename ...Arguments_t>
	function_call_printer_t<Return_t, Arguments_t...>::function_call_printer_t(const std::string &filename_line, const char *function_name, funcptr_t function_pointer)
		: _filename_line(filename_line), _function_name(function_name), _function_pointer(function_pointer)
	{
	}


	template <typename Return_t, typename ...Arguments_t>
	template <typename ...CallArguments_t>
	Return_t function_call_printer_t<Return_t, Arguments_t...>::operator ()(CallArguments_t &&...arguments)
	{
		{
			out_stream stream(_filename_line);
			stream << _function_name.c_str() << "(" << make_values(", ", arguments...) << ")" << ENDLINE;
		}

		Return_t return_value = _function_pointer(std::forward<CallArguments_t>(arguments)...);

		{
			indentation_add();
			out_stream stream(NEWLINE);
			stream << "=> " << make_value(return_value) << ENDLINE;
			indentation_remove();
		}

		return return_value;
	}


	template <typename ...Arguments_t>
	function_call_printer_t<void, Arguments_t...>::function_call_printer_t(const std::string &filename_line, const char *function_name, funcptr_t function_pointer)
		: _filename_line(filename_line), _function_name(function_name), _function_pointer(function_pointer)
	{
	}


	template <typename ...Arguments_t>
	template <typename ...CallArguments_t>
	void function_call_printer_t<void, Arguments_t...>::operator ()(CallArguments_t &&...arguments)
	{
		{
			out_stream stream(_filename_line);
			stream << _function_name.c_str() << "(" << make_values(", ", arguments...) << ")" << ENDLINE;
		}

		_function_pointer(std::forward<CallArguments_t>(arguments)...);

		{
			indentation_add();
			out_stream stream(NEWLINE);
			stream << "=>" << "(void)" << ENDLINE;
			indentation_remove();
		}
	}


	template <typename Return_t, typename ...Arguments_t>
	function_call_printer_t<Return_t, Arguments_t...> function_call(const std::string &filename_line, const char *function_name, Return_t (*function_pointer)(Arguments_t...))
	{
		return function_call_printer_t<Return_t, Arguments_t...>(filename_line, function_name, function_pointer);
	}


	//
	// Const member function call

	template <typename Type_t, typename Return_t, typename ...Arguments_t>
	const_member_function_call_printer_t<Type_t, Return_t, Arguments_t...>::const_member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, const Type_t &object, funcptr_t function_pointer)
		: _filename_line(filename_line), _object_name(object_name), _accessor(accessor), _function_name(function_name), _object(object), _function_pointer(function_pointer)
	{
	}


	template <typename Type_t, typename Return_t, typename ...Arguments_t>
	template <typename ...CallArguments_t>
	Return_t const_member_function_call_printer_t<Type_t, Return_t, Arguments_t...>::operator ()(CallArguments_t &&...arguments)
	{
		{
			out_stream stream(_filename_line);
			stream << _object_name.c_str() << _accessor.c_str() << _function_name.c_str() << "(" << make_values(", ", arguments...) << ")" << ENDLINE;
		}

		Return_t return_value = (_object.*_function_pointer)(std::forward<CallArguments_t>(arguments)...);

		{
			indentation_add();
			out_stream stream(NEWLINE);
			stream << "=> " << make_value(return_value) << ENDLINE;
			indentation_remove();
		}

		return return_value;
	}


	template <typename Type_t, typename ...Arguments_t>
	const_member_function_call_printer_t<Type_t, void, Arguments_t...>::const_member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, const Type_t &object, funcptr_t function_pointer)
		: _filename_line(filename_line), _object_name(object_name), _accessor(accessor), _function_name(function_name), _object(object), _function_pointer(function_pointer)
	{
	}


	template <typename Type_t, typename ...Arguments_t>
	template <typename ...CallArguments_t>
	void const_member_function_call_printer_t<Type_t, void, Arguments_t...>::operator ()(CallArguments_t &&...arguments)
	{
		{
			out_stream stream(_filename_line);
			stream << _object_name.c_str() << _accessor.c_str() << _function_name.c_str() << "(" << make_values(", ", arguments...) << ")" << ENDLINE;
		}

		(_object.*_function_pointer)(std::forward<CallArguments_t>(arguments)...);

		{
			indentation_add();
			out_stream stream(NEWLINE);
			stream << "=> " << "(void)";
			indentation_remove();
		}
	}


	template <typename Type_t, typename Return_t, typename ...Arguments_t>
	const_member_function_call_printer_t<Type_t, Return_t, Arguments_t...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, const Type_t &object, Return_t (Type_t::*function_pointer)(Arguments_t...) const)
	{
		return const_member_function_call_printer_t<Type_t, Return_t, Arguments_t...>(filename_line, object_name, ".", function_name, object, function_pointer);
	}


	template <typename Type_t, typename Return_t, typename ...Arguments_t>
	const_member_function_call_printer_t<Type_t, Return_t, Arguments_t...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, const Type_t *object, Return_t (Type_t::*function_pointer)(Arguments_t...) const)
	{
		return const_member_function_call_printer_t<Type_t, Return_t, Arguments_t...>(filename_line, object_name, "->", function_name, *object, function_pointer);
	}


	//
	// Non-const member function call

	template <typename Type_t, typename Return_t, typename ...Arguments_t>
	member_function_call_printer_t<Type_t, Return_t, Arguments_t...>::member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, Type_t &object, funcptr_t function_pointer)
		: _filename_line(filename_line), _object_name(object_name), _accessor(accessor), _function_name(function_name), _object(object), _function_pointer(function_pointer)
	{
	}


	template <typename Type_t, typename Return_t, typename ...Arguments_t>
	template <typename ...CallArguments_t>
	Return_t member_function_call_printer_t<Type_t, Return_t, Arguments_t...>::operator ()(CallArguments_t &&...arguments)
	{
		{
			out_stream stream(_filename_line);
			stream << _object_name.c_str() << _accessor.c_str() << _function_name.c_str() << "(" << make_values(", ", arguments...) << ")" << ENDLINE;
		}

		Return_t return_value = (_object.*_function_pointer)(std::forward<CallArguments_t>(arguments)...);

		{
			indentation_add();
			out_stream stream(NEWLINE);
			stream << "=> " << make_value(return_value) << ENDLINE;
			indentation_remove();
		}

		return return_value;
	}


	template <typename Type_t, typename ...Arguments_t>
	member_function_call_printer_t<Type_t, void, Arguments_t...>::member_function_call_printer_t(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, Type_t &object, funcptr_t function_pointer)
		: _filename_line(filename_line), _object_name(object_name), _accessor(accessor), _function_name(function_name), _object(object), _function_pointer(function_pointer)
	{
	}


	template <typename Type_t, typename ...Arguments_t>
	template <typename ...CallArguments_t>
	void member_function_call_printer_t<Type_t, void, Arguments_t...>::operator ()(CallArguments_t &&...arguments)
	{
		{
			out_stream stream(_filename_line);
			stream << _object_name.c_str() << _accessor.c_str() << _function_name.c_str() << "(" << make_values(", ", arguments...) << ")" << ENDLINE;
		}

		(_object.*_function_pointer)(std::forward<CallArguments_t>(arguments)...);

		{
			indentation_add();
			out_stream stream(NEWLINE);
			stream << "=> " << "(void)" << ENDLINE;
			indentation_remove();
		}
	}


	template <typename Type_t, typename Return_t, typename ...Arguments_t>
	member_function_call_printer_t<Type_t, Return_t, Arguments_t...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, Type_t &object, Return_t (Type_t::*function_pointer)(Arguments_t...))
	{
		return member_function_call_printer_t<Type_t, Return_t, Arguments_t...>(filename_line, object_name, ".", function_name, object, function_pointer);
	}


	template <typename Type_t, typename Return_t, typename ...Arguments_t>
	member_function_call_printer_t<Type_t, Return_t, Arguments_t...> member_function_call(const std::string &filename_line, const char *object_name, const char *function_name, Type_t *object, Return_t (Type_t::*function_pointer)(Arguments_t...))
	{
		return member_function_call_printer_t<Type_t, Return_t, Arguments_t...>(filename_line, object_name, "->", function_name, *object, function_pointer);
	}

#endif // defined(PRETTY_OUTPUT_CPP11)


	//
	// Return

	template <typename Type_t>
	const Type_t &return_printer_t::operator ,(const Type_t &value)
	{
		out_stream stream(_filename_line);
		stream << "return " << make_value(value) << ENDLINE;
		return value;
	}


	//
	// Block

	template <typename Comment_value_t, typename Return_t>
	block_t<Comment_value_t, Return_t>::block_t(const std::string &filename_line, const char *comment, const Comment_value_t &comment_value)
		: return_value(comment_value)
	{
		out_stream stream(filename_line);
		stream << comment << make_value(comment_value) << ENDLINE;
		indentation_add();
	}


	template <typename Comment_value_t, typename Return_t>
	block_t<Comment_value_t, Return_t>::block_t(const char *comment, const Comment_value_t &comment_value)
		: return_value(comment_value)
	{
		indentation_add();
		out_stream stream(NEWLINE);
		stream << comment << make_value(comment_value) << ENDLINE;
	}


	template <typename Comment_value_t, typename Return_t>
	block_t<Comment_value_t, Return_t>::block_t(const char *comment, const Comment_value_t &comment_value, const Return_t &retval)
		: return_value(retval)
	{
		indentation_add();
		out_stream stream(NEWLINE);
		stream << comment << make_value(comment_value) << ENDLINE;
	}


	template <typename Comment_value_t, typename Return_t>
	block_t<Comment_value_t, Return_t>::~block_t()
	{
		indentation_remove();
		out_stream stream(NEWLINE);
		stream << ENDLINE;
	}


	template <typename Comment_value_t, typename Return_t>
	block_t<Comment_value_t, Return_t>::operator const Return_t&()
	{
		return return_value;
	}


	template <typename Comment_value_t>
	block_t<Comment_value_t, Comment_value_t> block(const std::string &filename_line, const char *comment, const Comment_value_t &comment_value)
	{
		return block_t<Comment_value_t, Comment_value_t>(filename_line, comment, comment_value);
	}


	template <typename Comment_value_t>
	block_t<Comment_value_t, Comment_value_t> block(const char *comment, const Comment_value_t &comment_value)
	{
		return block_t<Comment_value_t, Comment_value_t>(comment, comment_value);
	}


	template <typename Comment_value_t, typename Return_t>
	block_t<Comment_value_t, Return_t> block(const char *comment, const Comment_value_t &comment_value, const Return_t &return_value)
	{
		return block_t<Comment_value_t, Return_t>(comment, comment_value, return_value);
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

