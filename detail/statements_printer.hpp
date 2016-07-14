#pragma once

#include <string>

#include "platform_defines.hpp"
#include "pretty.hpp"
#include "out_stream.hpp"


namespace trace_out { namespace detail
{

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

#if defined(TRACE_OUT_CPP11)

		block &operator =(block &&another); // = delete

#endif // defined(TRACE_OUT_CPP11)

		auto_indentation _auto_indentation;
		bool _value;
	};


	template <typename Type_t>
	block if_block(const std::string &filename_line, const char *condition, const Type_t &value);

	template <typename Type_t>
	block while_block(const std::string &filename_line, const char *condition, const Type_t &value);

	block iteration_block(const std::string &filename_line, size_t iteration);



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



	void print_while_header(const std::string &filename_line, const char *condition);

}
}


namespace trace_out { namespace detail
{

	template <typename Type_t>
	block if_block(const std::string &filename_line, const char *condition, const Type_t &value)
	{
		out_stream stream(filename_line);
		stream << "if (" << condition << ") => " << FLUSH;
		stream << make_pretty_condition(value) << ENDLINE;

		return block(!!value);
	}


	template <typename Type_t>
	block while_block(const std::string &filename_line, const char *condition, const Type_t &value)
	{
		{
			auto_indentation auto_indentation;

			out_stream stream(filename_line);
			stream << "// while: " << condition << " => " << FLUSH;
			stream << make_pretty_condition(value) << ENDLINE;
		}

		return block(!!value);
	}

}
}
