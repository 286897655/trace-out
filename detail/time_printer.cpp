#include "stuff.hpp"
#include "out_stream.hpp"

#include "time_printer.hpp"


namespace trace_out { namespace detail
{

	void print_execution_time_in_milliseconds(const std::string &filename_line, uint64_t milliseconds)
	{
		out_stream stream(filename_line);
		stream << "// execution time: " << to_string(milliseconds) << " ms" << ENDLINE;
	}


	void print_execution_time_in_ticks(const std::string &filename_line, std::clock_t ticks, double milliseconds)
	{
		out_stream stream(filename_line);
		stream << "// execution time: " << to_string(ticks) << " ticks (" << to_string(milliseconds) << " ms)" << ENDLINE;
	}

}
}

