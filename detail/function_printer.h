#pragma once

#include <string>

#include "out_stream.h"


namespace pretty_output { namespace detail
{

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

}
}


namespace pretty_output { namespace detail
{

	template <typename Type_t>
	const Type_t &return_printer::operator ,(const Type_t &value)
	{
		out_stream stream(_filename_line);
		stream << "return " << make_pretty(value) << ENDLINE;
		return value;
	}

}
}

