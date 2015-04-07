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
				pretty_output::impl::watch(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, __VA_ARGS__)


	#define $m(pointer, ...) \
				pretty_output::impl::print_memory(PRETTY_OUTPUT_FILENAME_LINE, #pointer, pointer, ##__VA_ARGS__);


	#if defined(PRETTY_OUTPUT_CPP11)

	#define $c(function_name) \
				pretty_output::impl::function_call(PRETTY_OUTPUT_FILENAME_LINE, #function_name, function_name)


	#define $cm(object, function_name) \
				pretty_output::impl::member_function_call(PRETTY_OUTPUT_FILENAME_LINE, #object, #function_name, pretty_output::impl::reference(object), &std::remove_pointer<decltype(object)>::type::function_name)

	#endif // defined(PRETTY_OUTPUT_CPP11)


	#define $f \
				pretty_output::impl::function_printer_t PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$f) = pretty_output::impl::function_printer(PRETTY_OUTPUT_FILENAME_LINE, PRETTY_OUTPUT_FUNCTION_SIGNATURE);


	#define $return \
				return pretty_output::impl::return_printer(PRETTY_OUTPUT_FILENAME_LINE) ,


	#define $if(...) \
				if (pretty_output::impl::block_t<bool, bool> PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$if_block) = pretty_output::impl::block(PRETTY_OUTPUT_FILENAME_LINE, "if (" #__VA_ARGS__ ") => ", static_cast<bool>((__VA_ARGS__))))


	#define pretty_output_for(block_name, ...) \
				if (pretty_output::impl::for_block_t block_name = pretty_output::impl::for_block_t(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__)) {} else \
					for (__VA_ARGS__) \
						if (pretty_output::impl::block_t<size_t, bool> PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$block) = pretty_output::impl::block("// for: iteration #", block_name.iteration(), false)) {} else


	#define $for(...) \
				pretty_output_for(PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$for_block), __VA_ARGS__)


	#define $while(...) \
				if (pretty_output::impl::print_while_header(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__), false) {} else \
					while (pretty_output::impl::block_t<bool, bool> PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_$while_block) = pretty_output::impl::block("// while: " #__VA_ARGS__ " => ", static_cast<bool>((__VA_ARGS__))))


	#define $p(format, ...) \
				{ \
					pretty_output::impl::out_stream stream(PRETTY_OUTPUT_FILENAME_LINE); \
					stream.printf(format, ##__VA_ARGS__); \
					stream << pretty_output::impl::ENDLINE; \
				}


	#define $t(name) \
				pretty_output::impl::set_current_thread_name(#name);

#elif defined(NDEBUG) || defined(PRETTY_OUTPUT_OFF)

	#define $w(...)


	#define $m(pointer, ...)


	#if defined(PRETTY_OUTPUT_CPP11)

	#define $c(function_name) \
				function_name


	#define $cm(object, function_name) \
				(object.*&std::remove_pointer<decltype(object)>::type::function_name)

	#endif // defined(PRETTY_OUTPUT_CPP11)


	#define $f


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
			(pretty_output::impl::filename_line_field(pretty_output::impl::filename_from_path(__FILE__), __LINE__))


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

	namespace impl
	{

		typedef uint32_t option_t;


		const size_t OPTIONS_START_BASE = 0;
		const size_t OPTIONS_START_BYTE_ORDER = 16;

	}


	const impl::option_t BIN = 0x1 << (impl::OPTIONS_START_BASE + 0);
	const impl::option_t SDEC = 0x1 << (impl::OPTIONS_START_BASE + 1);
	const impl::option_t UDEC = 0x1 << (impl::OPTIONS_START_BASE + 2);
	const impl::option_t HEX = 0x1 << (impl::OPTIONS_START_BASE + 3);
	const impl::option_t FLT = 0x1 << (impl::OPTIONS_START_BASE + 4);
	const impl::option_t DBL = 0x1 << (impl::OPTIONS_START_BASE + 5);
	const impl::option_t LDBL = 0x1 << (impl::OPTIONS_START_BASE + 6);

	const impl::option_t LITTLE = 0x1 << (impl::OPTIONS_START_BYTE_ORDER + 0);
	const impl::option_t BIG = 0x1 << (impl::OPTIONS_START_BYTE_ORDER + 1);


	namespace impl
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

		extern const char *const BASE_NAMES[];
		extern const size_t BASE_NAMES_LENGTH;

		extern const char *const BYTE_ORDER_NAMES[];
		extern const size_t BYTE_ORDER_NAMES_LENGTH;


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
			static const option_t default_base = HEX;
			typedef void signed_t;
			typedef void unsigned_t;
		};

#define PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(family, type_size, is_signed, unit_type, field_width_value, default_base_value, to_signed_type, to_unsigned_type) \
				template <> \
				struct print_traits_details<family, type_size, is_signed> \
				{ \
					typedef unit_type unit_t; \
					static const size_t field_width = field_width_value; \
					static const option_t default_base = default_base_value; \
					typedef to_signed_type signed_t; \
					typedef to_unsigned_type unsigned_t; \
				}

		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 1, true,
											int8_t, 4, HEX, int8_t, uint8_t);

		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 2, true,
											int16_t, 6, SDEC, int16_t, uint16_t);

		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 4, true,
											int32_t, 11, SDEC, int32_t, uint32_t);

		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 8, true,
											int64_t, 21, SDEC, int64_t, uint64_t);


		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 1, false,
											uint8_t, 3, HEX, int8_t, uint8_t);

		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 2, false,
											uint16_t, 5, UDEC, int16_t, uint16_t);

		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 4, false,
											uint32_t, 10, UDEC, int32_t, uint32_t);

		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 8, false,
											uint64_t, 20, UDEC, int64_t, uint64_t);


		// first_digit + point + precision + 'e' + sign + exponent
		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 4, true,
											float, 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 3, FLT, float, float);

		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 8, true,
											double, 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 4, DBL, double, double);

		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 10, true,
											long double, 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, LDBL, long double, long double);

		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 12, true,
											long double, 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, LDBL, long double, long double);

		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 16, true,
											long double, 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, LDBL, long double, long double);


		template <typename Type_t>
		struct print_traits
			: public print_traits_details<type_family<Type_t>::value, sizeof(Type_t), std::numeric_limits<Type_t>::is_signed>
		{
		};


		option_t base_value_from_options(option_t options, option_t default_value);
		option_t byte_order_value_from_options(option_t options, option_t default_value);
		const char *option_name(option_t option, const char *const names[], size_t names_length, const char *default_name);
		const char *byte_to_binary(uint8_t byte);
		const char *byte_to_hexadecimal(uint8_t byte);

		template <typename Type_t>
		size_t field_width(option_t base);

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
		const std::string (*select_conversion(option_t base))(Type_t);

		option_t current_byte_order();
		void reverse_bytes(void *destination, const void *source, size_t size);
		void order_bytes(void *ordered_bytes, const void *unordered_bytes, size_t size, option_t byte_order);

		template <typename Type_t>
		void print_memory(const std::string &filename_line, const char *name, const Type_t *pointer, size_t size = sizeof(Type_t), option_t options = 0);


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
		size_t field_width(option_t base)
		{
			switch (base)
			{
				case BIN:
					return sizeof(typename print_traits<Type_t>::unit_t) * 8;

				case SDEC:
					return print_traits<Type_t>::field_width + (!std::numeric_limits<Type_t>::is_signed ? 1 : 0);

				case UDEC:
					return print_traits<Type_t>::field_width - (std::numeric_limits<Type_t>::is_signed ? 1 : 0);

				case FLT:
				case DBL:
				case LDBL:
					return print_traits<Type_t>::field_width;

				case HEX:
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
		const std::string (*select_conversion(option_t base))(Type_t)
		{
			switch (base)
			{
				case BIN:
					return bytes_to_binary_string<Type_t>;

				case SDEC:
					return bytes_to_signed_decimal_string<Type_t>;

				case UDEC:
					return bytes_to_unsigned_decimal_string<Type_t>;

				case FLT:
				case DBL:
				case LDBL:
					return bytes_to_floating_point_string<Type_t>;

				case HEX:
				default:
					return bytes_to_hexadecimal_string<Type_t>;
			}
		}


		template <typename Type_t>
		void print_memory_contents(out_stream &stream, const Type_t *pointer, size_t size, size_t column_width, const std::string (*bytes_to_string)(Type_t), option_t byte_order)
		{
			std::stringstream string_stream;

			const Type_t *iterator = reinterpret_cast<const Type_t *>(pointer);
			size_t length = size / sizeof(Type_t);

			stream << make_value((void *)iterator) << ":";
			for (std::size_t index = 0; index < length; ++index)
			{
				const std::string string_representation = string_stream.str();
				if (string_representation.length() + column_width + 1 > stream.width_left())
				{
					stream << string_representation.c_str();
					string_stream.str("");

					stream << NEWLINE << make_value((void *)&iterator[index]) << ":";
				}

				string_stream << " ";
				string_stream.fill(' ');
				string_stream.width(column_width);
				string_stream.flags(std::ios::right);

				Type_t ordered_bytes;
				order_bytes(&ordered_bytes, &iterator[index], sizeof(Type_t), byte_order);

				string_stream << bytes_to_string(ordered_bytes);
			}

			const std::string string_representation = string_stream.str();
			if (!string_representation.empty())
			{
				stream << string_representation.c_str() << NEWLINE;
			}
		}


		template <typename Type_t>
		void print_memory(const std::string &filename_line, const char *name, const Type_t *pointer, size_t size, option_t options)
		{
			typedef typename print_traits<Type_t>::unit_t unit_t;

			option_t base = base_value_from_options(options, print_traits<Type_t>::default_base);
			option_t byte_order = byte_order_value_from_options(options, current_byte_order());

			const char *base_name = option_name((base >> OPTIONS_START_BASE), BASE_NAMES, BASE_NAMES_LENGTH, "?");
			const char *byte_order_name = option_name((byte_order >> OPTIONS_START_BYTE_ORDER), BYTE_ORDER_NAMES, BYTE_ORDER_NAMES_LENGTH, "?");

			out_stream stream(filename_line);
			stream << name << " (" << base_name << ", " << byte_order_name << "):";
			indentation_add();
			stream << NEWLINE;

			const std::string (*bytes_to_string)(Type_t) = select_conversion<Type_t>(base);
			size_t column_width = field_width<Type_t>(base);
			print_memory_contents(stream, reinterpret_cast<const unit_t *>(pointer), size, column_width, bytes_to_string, byte_order);

			indentation_remove();

			stream << ENDLINE;
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
				out_stream stream;
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
				out_stream stream;
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
				out_stream stream;
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
				out_stream stream;
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
				out_stream stream;
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
				out_stream stream;
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
			out_stream stream;
			stream << comment << make_value(comment_value) << ENDLINE;
		}


		template <typename Comment_value_t, typename Return_t>
		block_t<Comment_value_t, Return_t>::block_t(const char *comment, const Comment_value_t &comment_value, const Return_t &retval)
			: return_value(retval)
		{
			indentation_add();
			out_stream stream;
			stream << comment << make_value(comment_value) << ENDLINE;
		}


		template <typename Comment_value_t, typename Return_t>
		block_t<Comment_value_t, Return_t>::~block_t()
		{
			indentation_remove();
			out_stream stream;
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

