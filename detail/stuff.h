#pragma once

#include <cstdint>
#include <cstdarg>
#include <cstring>
#include <string>
#include <ios>
#include <sstream>


#define PRETTY_OUTPUT_PRIVATE__CONCAT_IMPL(a, b) \
			a##b

#define PRETTY_OUTPUT_PRIVATE__CONCAT(a, b) \
			PRETTY_OUTPUT_PRIVATE__CONCAT_IMPL(a, b)

#define PRETTY_OUTPUT_PRIVATE__UNIFY(identifier_base) \
			PRETTY_OUTPUT_PRIVATE__CONCAT(identifier_base, __COUNTER__)

#define PRETTY_OUTPUT_FILENAME_LINE \
			(pretty_output::detail::filename_line_field(pretty_output::detail::filename_from_path(__FILE__), __LINE__))


#if defined(__GNUG__) || defined(__clang__)
	#define PRETTY_OUTPUT_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
	#define PRETTY_OUTPUT_FUNCTION_SIGNATURE __FUNCSIG__
#else
	#error Cannot find function signature macro for current compiler. Try to add one manualy to this block.
#endif


namespace pretty_output { namespace detail
{

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

	std::string first_token(const std::string &tokens);
	std::string rest_tokens(const std::string &tokens);


	template <typename Type_t>
	struct is_fundamental
	{
		enum
		{
			value = false
		};
	};


#define PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(type) \
			template <> \
			struct is_fundamental<type> \
			{ \
				enum \
				{ \
					value = true \
				}; \
			}


	PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(char);
	PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(signed char);
	PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(unsigned char);
	PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(signed short int);
	PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(unsigned short int);
	PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(signed int);
	PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(unsigned int);
	PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(signed long int);
	PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(unsigned long int);

#if defined(PRETTY_OUTPUT_CPP11)

	PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(signed long long);
	PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(unsigned long long);

#endif // defined(PRETTY_OUTPUT_CPP11)

	PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(float);
	PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(double);
	PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL(long double);

#undef PRETTY_OUTPUT__DEFINE_IS_FUNDAMENTAL


	template <typename Type_t>
	struct is_pointer
	{
		enum
		{
			value = false
		};
	};


	template <typename Type_t>
	struct is_pointer<Type_t *>
	{
		enum
		{
			value = true
		};
	};



	template <typename Type_t>
	struct is_primitive
	{
		enum
		{
			value = is_fundamental<Type_t>::value || is_pointer<Type_t>::value
		};
	};



	template <bool Condition, typename True_t, typename False_t>
	struct conditional
	{
		typedef False_t type;
	};


	template <typename True_t, typename False_t>
	struct conditional<true, True_t, False_t>
	{
		typedef True_t type;
	};



	template <bool Condition, typename Type_t>
	struct enable_if;


	template <typename Type_t>
	struct enable_if<true, Type_t>
	{
		typedef Type_t type;
	};

}
}


namespace pretty_output { namespace detail
{

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

}
}

