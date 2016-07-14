#pragma once

#include <string>

#include "platform_defines.hpp"
#include "pretty.hpp"
#include "out_stream.hpp"
#include "stuff.hpp"


namespace trace_out { namespace detail
{

#if !defined(TRACE_OUT_CPP11)

	template <typename Type_t>
	const Type_t &expression(const std::string &filename_line, const char *name, const Type_t &value);

	template <typename Type_t>
	Type_t &expression(const std::string &filename_line, const char *name, Type_t &value);

#else

	template <typename Type_t>
	Type_t &&expression(const std::string &filename_line, const char *name, Type_t &&value);

	template <typename ...Types_t>
	void watch(const std::string &filename_line, const char *names, const Types_t &...arguments);

#endif // !defined(TRACE_OUT_CPP11)

}
}


namespace trace_out { namespace detail
{

#if !defined(TRACE_OUT_CPP11)

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


	template <typename Type_t>
	void print_values(out_stream &stream, const std::string &name, const Type_t &value)
	{
		stream << name << " = " << make_pretty(value) << ENDLINE;
	}


	template <typename FirstType_t, typename ...RestTypes_t>
	void print_values(out_stream &stream, const std::string &names, const FirstType_t &first_value, const RestTypes_t &...rest_values)
	{
		stream << first_token(names) << " = " << make_pretty(first_value) << NEWLINE;
		print_values(stream, rest_tokens(names), rest_values...);
	}

	template <typename ...Types_t>
	void watch(const std::string &filename_line, const char *names, const Types_t &...arguments)
	{
		out_stream stream(filename_line);
		print_values(stream, names, arguments...);
	}

#endif // !defined(TRACE_OUT_CPP11)

}
}
