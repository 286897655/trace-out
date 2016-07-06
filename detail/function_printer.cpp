#include "constants.hpp"

#include "function_printer.hpp"


namespace trace_out { namespace detail
{

	function_printer::function_printer(const std::string &filename_line, const char *function_signature)
		:
		_filename_line(filename_line),
		_function_signature(function_signature)
	{
		out_stream stream(_filename_line);
		stream << _function_signature << NEWLINE << "{" << ENDLINE;
		indentation_add();
	}


	function_printer::~function_printer()
	{
		indentation_remove();
		out_stream stream(_filename_line);
		stream << "}" << INDENTATION << "// " << _function_signature << NEWLINE << ENDLINE;
	}


	function_printer make_function_printer(const std::string &filename_line, const char *function_signature)
	{
		return function_printer(filename_line, function_signature);
	}



	return_printer::return_printer(const std::string &filename_line)
		:
		_filename_line(filename_line)
	{
	}


	return_printer make_return_printer(const std::string &filename_line)
	{
		return return_printer(filename_line);
	}

}
}
