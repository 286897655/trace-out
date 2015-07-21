//
// This file is a part of pretty_output project
// See more at https://github.com/shrpnsld/pretty_output
//

#pragma once

#include <cstdarg>
#include <cstring>
#include <string>
#include <sstream>
#include <stdint.h>


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


namespace pretty_output
{

	namespace impl
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

	}

}


namespace pretty_output
{

	namespace impl
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

