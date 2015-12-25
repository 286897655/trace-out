#pragma once

#include <string>

#include "platform_defines.h"
#include "pretty.h"
#include "out_stream.h"


namespace pretty_output { namespace detail
{

#if !defined(PRETTY_OUTPUT_CPP11)

	template <typename Type_t>
	const Type_t &expression(const std::string &filename_line, const char *name, const Type_t &value);

	template <typename Type_t>
	Type_t &expression(const std::string &filename_line, const char *name, Type_t &value);

#else

	template <typename Type_t>
	Type_t &&expression(const std::string &filename_line, const char *name, Type_t &&value);

	template <typename ...Types_t>
	void watch(const std::string &filename_line, const char *names, const Types_t &...arguments);

#endif // !defined(PRETTY_OUTPUT_CPP11)

}
}


namespace pretty_output { namespace detail
{

#if !defined(PRETTY_OUTPUT_CPP11)

	template <typename Type_t>
	const Type_t &expression(const std::string &filename_line, const char *name, const Type_t &value)
	{
		out_stream stream(filename_line);
		stream << name << " = " << make_pretty(value) << ENDLINE;
		return value;
	}


	template <typename Type_t>
	Type_t &expression(const std::string &filename_line, const char *name, Type_t &value)
	{
		out_stream stream(filename_line);
		stream << name << " = " << make_pretty(value) << ENDLINE;
		return value;
	}

#else

	template <typename Type_t>
	Type_t &&expression(const std::string &filename_line, const char *name, Type_t &&value)
	{
		out_stream stream(filename_line);
		stream << name << " = " << make_pretty(value) << ENDLINE;
		return std::forward<Type_t>(value);
	}


	template <typename ...Types_t>
	void watch(const std::string &filename_line, const char *names, const Types_t &...arguments)
	{
		out_stream stream(filename_line);
		stream << make_watches(names, arguments...) << ENDLINE;
	}

#endif // !defined(PRETTY_OUTPUT_CPP11)

}
}
