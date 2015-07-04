//
// This file is a part of pretty_output project
// See more at https://github.com/shrpnsld/pretty_output
//

#pragma once


#if __cplusplus >= 201103L || _MSC_VER >= 1800
	#define PRETTY_OUTPUT_CPP11
#endif


#include <limits>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include <iterator>
#include <memory>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <ctime>
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
// Public stuff
//


//
// Macros

#if (!defined(NDEBUG) && !defined(PRETTY_OUTPUT_OFF)) || defined(PRETTY_OUTPUT_ON)

	#define $w(...) \
				pretty_output::impl::watch(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, __VA_ARGS__)

	#define $m(pointer, ...) \
				pretty_output::impl::print_memory(PRETTY_OUTPUT_FILENAME_LINE, #pointer, pointer, ##__VA_ARGS__);


	#if defined(PRETTY_OUTPUT_CPP11)

		#define $c(function_name) \
					pretty_output::impl::make_function_call_printer(PRETTY_OUTPUT_FILENAME_LINE, #function_name, function_name)


		#define $cm(object, function_name) \
					pretty_output::impl::make_member_function_call_printer(PRETTY_OUTPUT_FILENAME_LINE, #object, #function_name, pretty_output::impl::reference(object), &std::remove_pointer<decltype(object)>::type::function_name)

	#endif // defined(PRETTY_OUTPUT_CPP11)


	#define $f \
				pretty_output::impl::function_printer PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_f) = pretty_output::impl::make_function_printer(PRETTY_OUTPUT_FILENAME_LINE, PRETTY_OUTPUT_FUNCTION_SIGNATURE);

	#define $return \
				return pretty_output::impl::make_return_printer(PRETTY_OUTPUT_FILENAME_LINE) ,

	#define $if(...) \
				if (pretty_output::impl::block PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_if_block) = pretty_output::impl::if_block(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, (__VA_ARGS__)))

	#define pretty_output_private__for(block_variable_name, ...) \
				if (pretty_output::impl::for_block block_variable_name = pretty_output::impl::make_for_block(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__)) {} else \
					for (__VA_ARGS__) \
						if (pretty_output::impl::block PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_iteration_block) = pretty_output::impl::iteration_block(PRETTY_OUTPUT_FILENAME_LINE, block_variable_name.iteration())) {} else

	#define $for(...) \
				pretty_output_private__for(PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_for_block), ##__VA_ARGS__)

	#define $while(...) \
				if (pretty_output::impl::print_while_header(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__), false) {} else \
					while (pretty_output::impl::block PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_while_block) = pretty_output::impl::while_block(PRETTY_OUTPUT_FILENAME_LINE, #__VA_ARGS__, (__VA_ARGS__)))

	#define $p(format, ...) \
				{ \
					pretty_output::impl::out_stream stream(PRETTY_OUTPUT_FILENAME_LINE); \
					stream.printf(format, ##__VA_ARGS__); \
					stream << pretty_output::impl::ENDLINE; \
				}

	#define $thread(name) \
				pretty_output::impl::set_current_thread_name(#name);

	#define pretty_output_private__time(start_time_variable, end_time_variable, ...) \
				{ \
					uint64_t start_time_variable = pretty_output::impl::time_in_milliseconds(); \
					__VA_ARGS__ \
					uint64_t end_time_variable = pretty_output::impl::time_in_milliseconds(); \
					pretty_output::impl::print_execution_time_in_milliseconds(PRETTY_OUTPUT_FILENAME_LINE, end_time_variable - start_time_variable); \
				}

	#define $time(...) \
				pretty_output_private__time(PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_start_ticks), PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_end_ticks), ##__VA_ARGS__)

	#define pretty_output_private__ticks(start_ticks_variable, end_ticks_variable, ...) \
				{ \
					std::clock_t start_ticks_variable = std::clock(); \
					__VA_ARGS__ \
					std::clock_t end_ticks_variable = std::clock(); \
					std::clock_t execution_time = end_ticks_variable - start_ticks_variable; \
					pretty_output::impl::print_execution_time_in_ticks(PRETTY_OUTPUT_FILENAME_LINE, execution_time, (static_cast<double>(execution_time)) / CLOCKS_PER_SEC * 1000); \
				}

	#define $ticks(...) \
				pretty_output_private__ticks(PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_start_time), PRETTY_OUTPUT_PRIVATE__UNIFY(pretty_output_end_time), ##__VA_ARGS__)

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

	#define $thread(name)

	#define $time(...) \
				__VA_ARGS__

	#define $ticks(...) \
				__VA_ARGS__

#endif


//
// Flags

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

}



//
// Private stuff
//


//
// Macros

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
	#define PRETTY_OUTPUT_REDIRECTION_NAMESPACE pretty_output_to_stdout
#endif


//
// Redirection

namespace PRETTY_OUTPUT_REDIRECTION_NAMESPACE
{

	void print(const char *string);
	void flush();
	size_t width();

}


//
// Implementation

namespace pretty_output
{

	namespace impl
	{

		//
		// Declarations
		//


		//
		// Platform-specific

		extern const char FILE_PATH_COMPONENT_DELIMITER;

		uint64_t current_thread_id();
		size_t printf_string_length(const char *format, va_list arguments);
		size_t printf_to_string(char *buffer, size_t size, const char *format, va_list arguments);


		typedef struct _tlskey *tlskey_t;

		tlskey_t tls_new_key();
		void tls_delete_key(tlskey_t key);
		void *tls_get(tlskey_t key);
		void tls_set(tlskey_t key, void *data);


		typedef struct _mutex *mutex_t;

		mutex_t mutex_new();
		void mutex_delete(mutex_t mutex);
		void mutex_lock(mutex_t mutex);
		void mutex_unlock(mutex_t mutex);

		uint64_t time_in_milliseconds();


		//
		// Common

		void indentation_add();
		void indentation_remove();

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

		template <typename Type_t>
		void crash_on_bad_memory(const Type_t &variable);


		typedef std::ios_base &(*manipulator_t)(std::ios_base &);

		void apply_io_manipulators(std::ostream &stream, va_list manipulators);

		template <typename Type_t>
		const std::string to_string(const Type_t &value, manipulator_t first_manipulator = NULL, ...);


		//
		// Pretties

		template <typename Type_t>
		class pretty
		{
		public:
			pretty(const Type_t &data);
			pretty(const pretty &another);

			const Type_t &get() const;

		private:
			pretty &operator =(const pretty &another); // = delete

#if defined(PRETTY_OUTPUT_CPP11)

			pretty &operator =(pretty &&another); // = delete

#endif // defined(PRETTY_OUTPUT_CPP11)

			const Type_t &_data;
		};


		pretty<const char *> make_pretty(const char *const &value);

		template <typename Type_t>
		pretty<Type_t> make_pretty(const Type_t &value);


#if defined(PRETTY_OUTPUT_CPP11)

		template <typename ...>
		class pretties;


		template <typename Type_t, typename ...RestTypes_t>
		class pretties<Type_t, RestTypes_t...>
		{
		public:
			pretties(const char *delimiter, const Type_t &first, const RestTypes_t &...rest);
			pretties(const pretties &another);

			const char *delimiter() const;
			const pretty<Type_t> &first() const;
			const pretties<RestTypes_t...> &rest() const;

		private:
			pretties &operator =(const pretties &another); // = delete
			pretties &operator =(pretties &&another); // = delete

			const char *_delimiter;
			const pretty<Type_t> _first;
			pretties<RestTypes_t...> _rest;
		};


		template <typename Type_t>
		class pretties<Type_t>
		{
		public:
			pretties(const char *delimiter, const Type_t &first);
			pretties(const pretties &another);

			const char *delimiter() const;
			const pretty<Type_t> &first() const;

		private:
			pretties &operator =(const pretties &another); // = delete
			pretties &operator =(pretties &&another); // = delete

			const char *_delimiter;
			const pretty<Type_t> _first;
		};


		template <>
		class pretties<>
		{
		public:
			pretties(const char *delimiter);
			pretties(const pretties &another);

		private:
			pretties &operator =(const pretties &another); // = delete
			pretties &operator =(pretties &&another); // = delete
		};


		template <typename ...Types_t>
		pretties<Types_t...> make_pretties(const char *delimiter, const Types_t &...values);

#endif // defined(PRETTY_OUTPUT_CPP11)


		template <typename Type_t>
		class pretty_bool
		{
		public:
			pretty_bool(const Type_t &data);
			pretty_bool(const pretty_bool &another);

			const Type_t &get() const;

		private:
			pretty_bool &operator =(const pretty_bool &another); // = delete

#if defined(PRETTY_OUTPUT_CPP11)

			pretty_bool &operator =(pretty_bool &&another); // = delete

#endif // defined(PRETTY_OUTPUT_CPP11)

			const Type_t &_data;
		};


		template <typename Type_t>
		pretty_bool<Type_t> make_pretty_bool(const Type_t &value);


		//
		// Out stream

		class newline_manipulator;
		class endline_manipulator;
		class flush_manipulator;

		extern const newline_manipulator NEWLINE;
		extern const endline_manipulator ENDLINE;
		extern const flush_manipulator FLUSH;


		class out_stream
		{
		public:
			out_stream(const std::string &filename_line);
			out_stream();
			~out_stream();

			out_stream &operator <<(char character);
			out_stream &operator <<(const char *string);
			out_stream &operator <<(const std::string &string);
			out_stream &operator <<(const newline_manipulator &);
			out_stream &operator <<(const endline_manipulator &);
			out_stream &operator <<(const flush_manipulator &);
			size_t width_left() const;
			void printf(const char *format, ...);
			void flush();

			static size_t width();

		private:
			size_t _current_line_length;
		};


		//
		// 'operator <<' overloads

		out_stream &operator <<(out_stream &stream, const pretty<bool> &value);
		out_stream &operator <<(out_stream &stream, const pretty<char> &value);
		out_stream &operator <<(out_stream &stream, const pretty<const char *> &value);
		out_stream &operator <<(out_stream &stream, const pretty<std::string> &value);
		out_stream &operator <<(out_stream &stream, const pretty<short> &value);
		out_stream &operator <<(out_stream &stream, const pretty<unsigned short> &value);
		out_stream &operator <<(out_stream &stream, const pretty<int> &value);
		out_stream &operator <<(out_stream &stream, const pretty<unsigned int> &value);
		out_stream &operator <<(out_stream &stream, const pretty<long> &value);
		out_stream &operator <<(out_stream &stream, const pretty<unsigned long> &value);

#if defined(PRETTY_OUTPUT_CPP11)

		out_stream &operator <<(out_stream &stream, const pretty<long long> &value);
		out_stream &operator <<(out_stream &stream, const pretty<unsigned long long> &value);

#endif // defined(PRETTY_OUTPUT_CPP11)

		out_stream &operator <<(out_stream &stream, const pretty<float> &value);
		out_stream &operator <<(out_stream &stream, const pretty<double> &value);
		out_stream &operator <<(out_stream &stream, const pretty<long double> &value);
		out_stream &operator <<(out_stream &stream, const pretty<const void *> &value);

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<const Type_t *> &value);

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<Type_t *> &value);

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::auto_ptr<Type_t> > &value);

#if defined(PRETTY_OUTPUT_CPP11)

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::shared_ptr<Type_t> > &value);

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::unique_ptr<Type_t> > &value);

#endif // defined(PRETTY_OUTPUT_CPP11)

		template <typename First_t, typename Second_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::pair<First_t, Second_t> > &value);

#if defined(PRETTY_OUTPUT_CPP11)

		template <typename ...Types_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::tuple<Types_t...> > &value);

		template <template <typename ...> class Container, typename ...Parameters_t>
		out_stream &operator <<(out_stream &stream, const pretty<Container<Parameters_t...> > &value);

#endif // defined(PRETTY_OUTPUT_CPP11)


#if defined(PRETTY_OUTPUT_CPP11)

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretties<Type_t> &values);

		template <typename ...Types_t>
		out_stream &operator <<(out_stream &stream, const pretties<Types_t...> &values);

		out_stream &operator <<(out_stream &stream, const pretties<> &values);

#endif // defined(PRETTY_OUTPUT_CPP11)

		out_stream &operator <<(out_stream &stream, const pretty_bool<bool> &value);

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty_bool<Type_t> &value);


		//
		// Watch

#if !defined(PRETTY_OUTPUT_CPP11)

		template <typename Type_t>
		const Type_t &watch(const std::string &filename_line, const char *name, const Type_t &value);

		template <typename Type_t>
		Type_t &watch(const std::string &filename_line, const char *name, Type_t &value);

#else

		template <typename Type_t>
		Type_t &&watch(const std::string &filename_line, const char *name, Type_t &&value);

#endif // !defined(PRETTY_OUTPUT_CPP11)


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

		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 1, true, int8_t, 4, HEX, int8_t, uint8_t);
		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 2, true, int16_t, 6, SDEC, int16_t, uint16_t);
		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 4, true, int32_t, 11, SDEC, int32_t, uint32_t);
		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 8, true, int64_t, 21, SDEC, int64_t, uint64_t);

		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 1, false, uint8_t, 3, HEX, int8_t, uint8_t);
		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 2, false, uint16_t, 5, UDEC, int16_t, uint16_t);
		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 4, false, uint32_t, 10, UDEC, int32_t, uint32_t);
		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_INTEGER, 8, false, uint64_t, 20, UDEC, int64_t, uint64_t);

		// sign + first_digit + point + precision + 'e' + exponent_sign + exponent
		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 4, true, float, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 3, FLT, float, float);
		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 8, true, double, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 4, DBL, double, double);
		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 10, true, long double, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, LDBL, long double, long double);
		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 12, true, long double, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, LDBL, long double, long double);
		PRETTY_OUTPUT__DEFINE_PRINT_TRAITS(TYPE_FAMILY_FLOATING_POINT, 16, true, long double, 1 + 1 + 1 + std::numeric_limits<float>::digits10 + 1 + 1 + 5, LDBL, long double, long double);

#undef PRETTY_OUTPUT__DEFINE_PRINT_TRAITS


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
		class function_call_printer
		{
		public:
			typedef Return_t (*funcptr_t)(Arguments_t...);

			function_call_printer(const std::string &filename_line, const char *function_name, funcptr_t function_pointer);

			template <typename ...CallArguments_t>
			Return_t operator ()(CallArguments_t &&...arguments);

		private:
			std::string _filename_line;
			std::string _function_name;
			funcptr_t _function_pointer;
		};


		template <typename ...Arguments_t>
		class function_call_printer<void, Arguments_t...>
		{
		public:
			typedef void (*funcptr_t)(Arguments_t...);

			function_call_printer(const std::string &filename_line, const char *function_name, funcptr_t function_pointer);

			template <typename ...CallArguments_t>
			void operator ()(CallArguments_t &&...arguments);

		private:
			std::string _filename_line;
			std::string _function_name;
			funcptr_t _function_pointer;
		};


		template <typename Return_t, typename ...Arguments_t>
		function_call_printer<Return_t, Arguments_t...> make_function_call_printer(const std::string &filename_line, const char *function_name, Return_t (*function_pointer)(Arguments_t...));


		//
		// Const member function call

		template <typename Type_t, typename Return_t, typename ...Arguments_t>
		class const_member_function_call_printer
		{
		public:
			typedef Return_t (Type_t::*funcptr_t)(Arguments_t...) const;

			const_member_function_call_printer(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, const Type_t &object, funcptr_t function_pointer);
			const_member_function_call_printer(const const_member_function_call_printer &another);

			template <typename ...CallArguments_t>
			Return_t operator ()(CallArguments_t &&...arguments);

		private:
			const_member_function_call_printer &operator =(const const_member_function_call_printer &another); // = delete
			const_member_function_call_printer &operator =(const_member_function_call_printer &&another); // = delete

			std::string _filename_line;
			std::string _object_name;
			std::string _accessor;
			std::string _function_name;
			const Type_t &_object;
			funcptr_t _function_pointer;
		};


		template <typename Type_t, typename ...Arguments_t>
		class const_member_function_call_printer<Type_t, void, Arguments_t...>
		{
		public:
			typedef void (Type_t::*funcptr_t)(Arguments_t...) const;

			const_member_function_call_printer(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, const Type_t &object, funcptr_t function_pointer);
			const_member_function_call_printer(const const_member_function_call_printer &another);

			template <typename ...CallArguments_t>
			void operator ()(CallArguments_t &&...arguments);

		private:
			const_member_function_call_printer &operator =(const const_member_function_call_printer &another); // = delete
			const_member_function_call_printer &operator =(const_member_function_call_printer &&another); // = delete

			std::string _filename_line;
			std::string _object_name;
			std::string _accessor;
			std::string _function_name;
			const Type_t &_object;
			funcptr_t _function_pointer;
		};


		template <typename Type_t, typename Return_t, typename ...Arguments_t>
		const_member_function_call_printer<Type_t, Return_t, Arguments_t...> make_member_function_call_printer(const std::string &filename_line, const char *object_name, const char *function_name, const Type_t &object, Return_t (Type_t::*function_pointer)(Arguments_t...) const);

		template <typename Type_t, typename Return_t, typename ...Arguments_t>
		const_member_function_call_printer<Type_t, Return_t, Arguments_t...> make_member_function_call_printer(const std::string &filename_line, const char *object_name, const char *function_name, const Type_t *object, Return_t (Type_t::*function_pointer)(Arguments_t...) const);


		//
		// Non-const member function call

		template <typename Type_t, typename Return_t, typename ...Arguments_t>
		class member_function_call_printer
		{
		public:
			typedef Return_t (Type_t::*funcptr_t)(Arguments_t...);

			member_function_call_printer(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, Type_t &object, funcptr_t function_pointer);
			member_function_call_printer(const member_function_call_printer &another);

			template <typename ...CallArguments_t>
			Return_t operator ()(CallArguments_t &&...arguments);

		private:
			member_function_call_printer &operator =(const member_function_call_printer &another); // = delete
			member_function_call_printer &operator =(member_function_call_printer &&another); // = delete

			std::string _filename_line;
			std::string _object_name;
			std::string _accessor;
			std::string _function_name;
			Type_t &_object;
			funcptr_t _function_pointer;
		};


		template <typename Type_t, typename ...Arguments_t>
		class member_function_call_printer<Type_t, void, Arguments_t...>
		{
		public:
			typedef void (Type_t::*funcptr_t)(Arguments_t...);

			member_function_call_printer(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, Type_t &object, funcptr_t function_pointer);
			member_function_call_printer(const member_function_call_printer &another);

			template <typename ...CallArguments_t>
			void operator ()(CallArguments_t &&...arguments);

		private:
			member_function_call_printer &operator =(const member_function_call_printer &another); // = delete
			member_function_call_printer &operator =(member_function_call_printer &&another); // = delete

			std::string _filename_line;
			std::string _object_name;
			std::string _accessor;
			std::string _function_name;
			Type_t &_object;
			funcptr_t _function_pointer;
		};


		template <typename Type_t, typename Return_t, typename ...Arguments_t>
		member_function_call_printer<Type_t, Return_t, Arguments_t...> make_member_function_call_printer(const std::string &filename_line, const char *object_name, const char *function_name, Type_t &object, Return_t (Type_t::*function_pointer)(Arguments_t...));

		template <typename Type_t, typename Return_t, typename ...Arguments_t>
		member_function_call_printer<Type_t, Return_t, Arguments_t...> make_member_function_call_printer(const std::string &filename_line, const char *object_name, const char *function_name, Type_t *object, Return_t (Type_t::*function_pointer)(Arguments_t...));

#endif // defined(PRETTY_OUTPUT_CPP11)


		//
		// Function printer

		class function_printer
		{
		public:
			function_printer(const std::string &filename_line, const char *function_signature);
			~function_printer();

		private:
			std::string _filename_line;
			std::string _function_signature;
		};


		function_printer make_function_printer(const std::string &filename_line, const char *function_signature);


		//
		// Return printer

		class return_printer
		{
		public:
			return_printer(const std::string &filename_line);

			template <typename T>
			const T &operator ,(const T &value);

		private:
			std::string _filename_line;
		};


		return_printer make_return_printer(const std::string &filename_line);


		//
		// Block

		class auto_indentation
		{
		public:
			auto_indentation();
			~auto_indentation();
		};


		class block
		{
		public:
			block(bool value);
			block(const block &another);
			~block();

			operator bool() const;

		private:
			block &operator =(const block &another); // = delete

#if defined(PRETTY_OUTPUT_CPP11)

			block &operator =(block &&another); // = delete

#endif // defined(PRETTY_OUTPUT_CPP11)

			auto_indentation _auto_indentation;
			bool _value;
		};


		template <typename Type_t>
		block if_block(const std::string &filename_line, const char *condition, const Type_t &value);

		template <typename Type_t>
		block while_block(const std::string &filename_line, const char *condition, const Type_t &value);

		block iteration_block(const std::string &filename_line, size_t iteration);


		//
		// For block

		class for_block
		{
		public:
			for_block(const std::string &filename_line, const char *expression);
			~for_block();

			operator bool() const;
			size_t iteration();

		private:
			size_t _iteration_number;
		};


		for_block make_for_block(const std::string &filename_line, const char *expression);


		//
		// While header

		void print_while_header(const std::string &filename_line, const char *condition);


		//
		// Time

		void print_execution_time_in_milliseconds(const std::string &filename_line, uint64_t milliseconds);
		void print_execution_time_in_ticks(const std::string &filename_line, uint64_t ticks, double milliseconds);



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


		template <typename Type_t>
		void crash_on_bad_memory(const Type_t &variable)
		{
			uint8_t buffer[sizeof(variable)];
			std::memcpy(buffer, &variable, sizeof(buffer));
		}


		template <typename Type_t>
		const std::string to_string(const Type_t &value, manipulator_t first_manipulator, ...)
		{
			std::stringstream string_stream;

			if (first_manipulator != NULL)
			{
				va_list rest_manipulators;
				va_start(rest_manipulators, first_manipulator);

				string_stream << first_manipulator;
				apply_io_manipulators(string_stream, rest_manipulators);

				va_end(rest_manipulators);
			}

			string_stream << value;

			return string_stream.str();
		}


		//
		// Pretties

		template <typename Type_t>
		pretty<Type_t>::pretty(const Type_t &data)
			: _data(data)
		{
		}


		template <typename Type_t>
		pretty<Type_t>::pretty(const pretty &another)
			: _data(another._data)
		{
		}


		template <typename Type_t>
		const Type_t &pretty<Type_t>::get() const
		{
			crash_on_bad_memory(_data);

			return _data;
		}


		template <typename Type_t>
		pretty<Type_t> make_pretty(const Type_t &value)
		{
			return pretty<Type_t>(value);
		}


#if defined(PRETTY_OUTPUT_CPP11)

		template <typename Type_t, typename ...RestTypes_t>
		pretties<Type_t, RestTypes_t...>::pretties(const char *delimiter, const Type_t &first, const RestTypes_t &...rest)
			: _delimiter(delimiter), _first(first), _rest(delimiter, rest...)
		{
		}


		template <typename Type_t, typename ...RestTypes_t>
		pretties<Type_t, RestTypes_t...>::pretties(const pretties &another)
			: _delimiter(another._delimiter), _first(another._first), _rest(another._rest)
		{
		}


		template <typename Type_t, typename ...RestTypes_t>
		const char *pretties<Type_t, RestTypes_t...>::delimiter() const
		{
			return _delimiter;
		}


		template <typename Type_t, typename ...RestTypes_t>
		const pretty<Type_t> &pretties<Type_t, RestTypes_t...>::first() const
		{
			return _first;
		}


		template <typename Type_t, typename ...RestTypes_t>
		const pretties<RestTypes_t...> &pretties<Type_t, RestTypes_t...>::rest() const
		{
			return _rest;
		}



		template <typename Type_t>
		pretties<Type_t>::pretties(const char *, const Type_t &first)
			: _delimiter(""), _first(first)
		{
		}


		template <typename Type_t>
		pretties<Type_t>::pretties(const pretties &another)
			: _delimiter(another._delimiter), _first(another._first)
		{
		}


		template <typename Type_t>
		const char *pretties<Type_t>::delimiter() const
		{
			return _delimiter;
		}


		template <typename Type_t>
		const pretty<Type_t> &pretties<Type_t>::first() const
		{
			return _first;
		}



		template <typename ...Types_t>
		pretties<Types_t...> make_pretties(const char *delimiter, const Types_t &...values)
		{
			return pretties<Types_t...>(delimiter, values...);
		}

#endif // defined(PRETTY_OUTPUT_CPP11)


		template <typename Type_t>
		pretty_bool<Type_t>::pretty_bool(const Type_t &data)
			: _data(data)
		{
		}


		template <typename Type_t>
		pretty_bool<Type_t>::pretty_bool(const pretty_bool &another)
			: _data(another._data)
		{
		}


		template <typename Type_t>
		const Type_t &pretty_bool<Type_t>::get() const
		{
			return _data;
		}


		template <typename Type_t>
		pretty_bool<Type_t> make_pretty_bool(const Type_t &value)
		{
			return pretty_bool<Type_t>(value);
		}


		//
		// 'operator <<' overloads

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<const Type_t *> &value)
		{
			stream << FLUSH;
			stream << make_pretty(static_cast<const void *>(value.get())) << " ";
			if (value.get() != NULL)
			{
				stream << FLUSH;
				stream << "-> " << make_pretty(*(value.get()));
			}

			return stream;
		}


		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<Type_t *> &value)
		{
			stream << FLUSH;
			return stream << make_pretty(static_cast<const Type_t *>(value.get()));
		}


		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::auto_ptr<Type_t> > &value)
		{
			stream << FLUSH;
			const std::auto_ptr<Type_t> &pointer = value.get();
			return stream << make_pretty(pointer.get());
		}


#if defined(PRETTY_OUTPUT_CPP11)

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::shared_ptr<Type_t> > &value)
		{
			stream << FLUSH;
			const std::shared_ptr<Type_t> &pointer = value.get();
			stream << make_pretty(static_cast<const void *>(pointer.get())) << ", use_count = " << to_string(pointer.use_count());
			if (pointer.get() != NULL)
			{
				stream << " -> " << FLUSH;
				stream << make_pretty(*(value.get()));
			}

			return stream;
		}


		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::unique_ptr<Type_t> > &value)
		{
			stream << FLUSH;
			const std::unique_ptr<Type_t> &pointer = value.get();
			return stream << make_pretty(pointer.get());
		}

#endif // defined(PRETTY_OUTPUT_CPP11)


		template <typename First_t, typename Second_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::pair<First_t, Second_t> > &value)
		{
			stream << FLUSH;
			const std::pair<First_t, Second_t> &pair = value.get();
			stream << "{";
			stream << make_pretty(pair.first) << ": ";
			stream << make_pretty(pair.second) << "}";
			return stream;
		}


#if defined(PRETTY_OUTPUT_CPP11)

		template <std::size_t Index, typename ...Types_t>
		typename std::enable_if<Index == sizeof...(Types_t) - 1, out_stream &>::type print_tuple(out_stream &stream, const std::tuple<Types_t...> &tuple)
		{
			return stream << make_pretty(std::get<Index>(tuple)) << ")";
		}


		template <std::size_t Index, typename ...Types_t>
		typename std::enable_if<Index < sizeof...(Types_t) - 1, out_stream &>::type print_tuple(out_stream &stream, const std::tuple<Types_t...> &tuple)
		{
			stream << make_pretty(std::get<Index>(tuple)) << ", ";
			return print_tuple<Index + 1>(stream, tuple);
		}


		template <typename ...Types_t>
		out_stream &operator <<(out_stream &stream, const pretty<std::tuple<Types_t...> > &value)
		{
			stream << FLUSH;
			const std::tuple<Types_t...> &tuple = value.get();
			stream << "(";
			return print_tuple<0>(stream, tuple);
		}


		// not sure if all C++11 standard library versions have std::next
		template <typename Type_t>
		Type_t next_itr(Type_t iterator)
		{
			++iterator;
			return iterator;
		}


		template <template <typename ...> class Container, typename ...Parameters_t>
		out_stream &operator <<(out_stream &stream, const pretty<Container<Parameters_t...> > &value)
		{
			stream << FLUSH;
			const auto &container = value.get();

			stream << "[";
			auto iterator = std::begin(container);
			for ( ; next_itr(iterator) != std::end(container); ++iterator)
			{
				stream << make_pretty(*iterator) << ", ";
			}

			stream << make_pretty(*iterator) << "]";

			return stream;
		}

#endif // defined(PRETTY_OUTPUT_CPP11)


#if defined(PRETTY_OUTPUT_CPP11)

		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretties<Type_t> &values)
		{
			stream << FLUSH;
			return stream << make_pretty(values.first().get());
		}


		template <typename ...Types_t>
		out_stream &operator <<(out_stream &stream, const pretties<Types_t...> &values)
		{
			stream << FLUSH;
			return stream << make_pretty(values.first().get()) << values.delimiter() << values.rest();
		}

#endif // defined(PRETTY_OUTPUT_CPP11)


		template <typename Type_t>
		out_stream &operator <<(out_stream &stream, const pretty_bool<Type_t> &value)
		{
			stream << FLUSH;
			stream << (value.get() ? "true" : "false") << " (" << FLUSH;
			stream << make_pretty(value.get()) << ")";
			return stream;
		}


		//
		// Watch

#if !defined(PRETTY_OUTPUT_CPP11)

		template <typename Type_t>
		const Type_t &watch(const std::string &filename_line, const char *name, const Type_t &value)
		{
			out_stream stream(filename_line);
			stream << name << " = " << make_pretty(value) << ENDLINE;
			return value;
		}


		template <typename Type_t>
		Type_t &watch(const std::string &filename_line, const char *name, Type_t &value)
		{
			out_stream stream(filename_line);
			stream << name << " = " << make_pretty(value) << ENDLINE;
			return value;
		}

#else

		template <typename Type_t>
		Type_t &&watch(const std::string &filename_line, const char *name, Type_t &&value)
		{
			out_stream stream(filename_line);
			stream << name << " = " << make_pretty(value) << ENDLINE;
			return std::forward<Type_t>(value);
		}

#endif // !defined(PRETTY_OUTPUT_CPP11)


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

			return to_string(signed_integer);
		}


		template <typename Type_t>
		const std::string bytes_to_unsigned_decimal_string(Type_t value)
		{
			typedef typename print_traits<Type_t>::unsigned_t unsigned_promotion_t;

			unsigned_promotion_t unsigned_value = static_cast<unsigned_promotion_t>(value);
			uint64_t unsigned_integer = static_cast<uint64_t>(unsigned_value);

			return to_string(unsigned_integer);
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

			const Type_t *iterator = pointer;
			size_t length = size / sizeof(Type_t);

			stream << make_pretty(static_cast<const void *>(iterator)) << ":";
			for (std::size_t index = 0; index < length; ++index)
			{
				const std::string string_representation = string_stream.str();
				if (string_representation.length() + column_width + 1 > stream.width_left())
				{
					stream << string_representation;
					string_stream.str("");

					stream << NEWLINE << make_pretty(static_cast<const void *>(&iterator[index])) << ":";
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
				stream << string_representation << NEWLINE;
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
			print_memory_contents(stream, static_cast<const unit_t *>(pointer), size, column_width, bytes_to_string, byte_order);

			indentation_remove();

			stream << ENDLINE;
		}


#if defined(PRETTY_OUTPUT_CPP11)

		//
		// Function call

		template <typename Return_t, typename ...Arguments_t>
		function_call_printer<Return_t, Arguments_t...>::function_call_printer(const std::string &filename_line, const char *function_name, funcptr_t function_pointer)
			: _filename_line(filename_line), _function_name(function_name), _function_pointer(function_pointer)
		{
		}


		template <typename Return_t, typename ...Arguments_t>
		template <typename ...CallArguments_t>
		Return_t function_call_printer<Return_t, Arguments_t...>::operator ()(CallArguments_t &&...arguments)
		{
			{
				out_stream stream(_filename_line);
				stream << _function_name << "(" << make_pretties(", ", arguments...) << ")" << ENDLINE;
			}

			Return_t return_value = _function_pointer(std::forward<CallArguments_t>(arguments)...);

			{
				indentation_add();
				out_stream stream;
				stream << "=> " << make_pretty(return_value) << ENDLINE;
				indentation_remove();
			}

			return return_value;
		}


		template <typename ...Arguments_t>
		function_call_printer<void, Arguments_t...>::function_call_printer(const std::string &filename_line, const char *function_name, funcptr_t function_pointer)
			: _filename_line(filename_line), _function_name(function_name), _function_pointer(function_pointer)
		{
		}


		template <typename ...Arguments_t>
		template <typename ...CallArguments_t>
		void function_call_printer<void, Arguments_t...>::operator ()(CallArguments_t &&...arguments)
		{
			{
				out_stream stream(_filename_line);
				stream << _function_name << "(" << make_pretties(", ", arguments...) << ")" << ENDLINE;
			}

			_function_pointer(std::forward<CallArguments_t>(arguments)...);

			{
				indentation_add();
				out_stream stream;
				stream << "=> " << "(void)" << ENDLINE;
				indentation_remove();
			}
		}


		template <typename Return_t, typename ...Arguments_t>
		function_call_printer<Return_t, Arguments_t...> make_function_call_printer(const std::string &filename_line, const char *function_name, Return_t (*function_pointer)(Arguments_t...))
		{
			return function_call_printer<Return_t, Arguments_t...>(filename_line, function_name, function_pointer);
		}


		//
		// Const member function call

		template <typename Type_t, typename Return_t, typename ...Arguments_t>
		const_member_function_call_printer<Type_t, Return_t, Arguments_t...>::const_member_function_call_printer(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, const Type_t &object, funcptr_t function_pointer)
			: _filename_line(filename_line), _object_name(object_name), _accessor(accessor), _function_name(function_name), _object(object), _function_pointer(function_pointer)
		{
		}


		template <typename Type_t, typename Return_t, typename ...Arguments_t>
		const_member_function_call_printer<Type_t, Return_t, Arguments_t...>::const_member_function_call_printer(const const_member_function_call_printer &another)
			: _filename_line(another._filename_line), _object_name(another._object_name), _accessor(another._accessor), _function_name(another._function_name), _object(another._object), _function_pointer(another._function_pointer)
		{
		}


		template <typename Type_t, typename Return_t, typename ...Arguments_t>
		template <typename ...CallArguments_t>
		Return_t const_member_function_call_printer<Type_t, Return_t, Arguments_t...>::operator ()(CallArguments_t &&...arguments)
		{
			{
				out_stream stream(_filename_line);
				stream << _object_name << _accessor << _function_name << "(" << make_pretties(", ", arguments...) << ")" << ENDLINE;
			}

			Return_t return_value = (_object.*_function_pointer)(std::forward<CallArguments_t>(arguments)...);

			{
				indentation_add();
				out_stream stream;
				stream << "=> " << make_pretty(return_value) << ENDLINE;
				indentation_remove();
			}

			return return_value;
		}


		template <typename Type_t, typename ...Arguments_t>
		const_member_function_call_printer<Type_t, void, Arguments_t...>::const_member_function_call_printer(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, const Type_t &object, funcptr_t function_pointer)
			: _filename_line(filename_line), _object_name(object_name), _accessor(accessor), _function_name(function_name), _object(object), _function_pointer(function_pointer)
		{
		}


		template <typename Type_t, typename ...Arguments_t>
		const_member_function_call_printer<Type_t, void, Arguments_t...>::const_member_function_call_printer(const const_member_function_call_printer &another)
			: _filename_line(another._filename_line), _object_name(another._object_name), _accessor(another._accessor), _function_name(another._function_name), _object(another._object), _function_pointer(another._function_pointer)
		{
		}


		template <typename Type_t, typename ...Arguments_t>
		template <typename ...CallArguments_t>
		void const_member_function_call_printer<Type_t, void, Arguments_t...>::operator ()(CallArguments_t &&...arguments)
		{
			{
				out_stream stream(_filename_line);
				stream << _object_name << _accessor << _function_name << "(" << make_pretties(", ", arguments...) << ")" << ENDLINE;
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
		const_member_function_call_printer<Type_t, Return_t, Arguments_t...> make_member_function_call_printer(const std::string &filename_line, const char *object_name, const char *function_name, const Type_t &object, Return_t (Type_t::*function_pointer)(Arguments_t...) const)
		{
			return const_member_function_call_printer<Type_t, Return_t, Arguments_t...>(filename_line, object_name, ".", function_name, object, function_pointer);
		}


		//
		// Non-const member function call

		template <typename Type_t, typename Return_t, typename ...Arguments_t>
		member_function_call_printer<Type_t, Return_t, Arguments_t...>::member_function_call_printer(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, Type_t &object, funcptr_t function_pointer)
			: _filename_line(filename_line), _object_name(object_name), _accessor(accessor), _function_name(function_name), _object(object), _function_pointer(function_pointer)
		{
		}


		template <typename Type_t, typename Return_t, typename ...Arguments_t>
		member_function_call_printer<Type_t, Return_t, Arguments_t...>::member_function_call_printer(const member_function_call_printer &another)
			: _filename_line(another._filename_line), _object_name(another._object_name), _accessor(another._accessor), _function_name(another._function_name), _object(another._object), _function_pointer(another._function_pointer)
		{
		}


		template <typename Type_t, typename Return_t, typename ...Arguments_t>
		template <typename ...CallArguments_t>
		Return_t member_function_call_printer<Type_t, Return_t, Arguments_t...>::operator ()(CallArguments_t &&...arguments)
		{
			{
				out_stream stream(_filename_line);
				stream << _object_name << _accessor << _function_name << "(" << make_pretties(", ", arguments...) << ")" << ENDLINE;
			}

			Return_t return_value = (_object.*_function_pointer)(std::forward<CallArguments_t>(arguments)...);

			{
				indentation_add();
				out_stream stream;
				stream << "=> " << make_pretty(return_value) << ENDLINE;
				indentation_remove();
			}

			return return_value;
		}


		template <typename Type_t, typename ...Arguments_t>
		member_function_call_printer<Type_t, void, Arguments_t...>::member_function_call_printer(const std::string &filename_line, const char *object_name, const char *accessor, const char *function_name, Type_t &object, funcptr_t function_pointer)
			: _filename_line(filename_line), _object_name(object_name), _accessor(accessor), _function_name(function_name), _object(object), _function_pointer(function_pointer)
		{
		}


		template <typename Type_t, typename ...Arguments_t>
		member_function_call_printer<Type_t, void, Arguments_t...>::member_function_call_printer(const member_function_call_printer &another)
			: _filename_line(another._filename_line), _object_name(another._object_name), _accessor(another._accessor), _function_name(another._function_name), _object(another._object), _function_pointer(another._function_pointer)
		{
		}


		template <typename Type_t, typename ...Arguments_t>
		template <typename ...CallArguments_t>
		void member_function_call_printer<Type_t, void, Arguments_t...>::operator ()(CallArguments_t &&...arguments)
		{
			{
				out_stream stream(_filename_line);
				stream << _object_name << _accessor << _function_name << "(" << make_pretties(", ", arguments...) << ")" << ENDLINE;
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
		member_function_call_printer<Type_t, Return_t, Arguments_t...> make_member_function_call_printer(const std::string &filename_line, const char *object_name, const char *function_name, Type_t &object, Return_t (Type_t::*function_pointer)(Arguments_t...))
		{
			return member_function_call_printer<Type_t, Return_t, Arguments_t...>(filename_line, object_name, ".", function_name, object, function_pointer);
		}

#endif // defined(PRETTY_OUTPUT_CPP11)


		//
		// Return

		template <typename Type_t>
		const Type_t &return_printer::operator ,(const Type_t &value)
		{
			out_stream stream(_filename_line);
			stream << "return " << make_pretty(value) << ENDLINE;
			return value;
		}


		//
		// Block

		template <typename Type_t>
		block if_block(const std::string &filename_line, const char *condition, const Type_t &value)
		{
			out_stream stream(filename_line);
			stream << "if (" << condition << ") => " << FLUSH;
			stream << make_pretty_bool(value) << ENDLINE;

			return block(static_cast<bool>(value));
		}


		template <typename Type_t>
		block while_block(const std::string &filename_line, const char *condition, const Type_t &value)
		{
			{
				auto_indentation auto_indentation;

				out_stream stream(filename_line);
				stream << "// while: " << condition << " => " << FLUSH;
				stream << make_pretty_bool(value) << ENDLINE;
			}

			return block(static_cast<bool>(value));
		}


		//
		// Thread

		void set_current_thread_name(const std::string &name);

	}

}


#if defined(__clang__)

	#pragma clang diagnostic pop

#elif defined(__GNUC__) || defined(__GNUG__)

	#pragma GCC diagnostic pop

#elif defined(_MSC_VER)

	#pragma warning(pop)

#endif

