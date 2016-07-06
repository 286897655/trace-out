#pragma once

#include <string>

#include "platform_defines.hpp"
#include "pretty.hpp"
#include "out_stream.hpp"


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


	template <typename ...Types_t>
	void watch(const std::string &filename_line, const char *names, const Types_t &...arguments)
	{
		out_stream stream(filename_line);
		stream << make_watches(names, arguments...) << ENDLINE;
	}

#endif // !defined(TRACE_OUT_CPP11)

}
}
