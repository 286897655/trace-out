#pragma once

#include <cstdint>
#include <string>


namespace pretty_output { namespace detail
{

	void print_execution_time_in_milliseconds(const std::string &filename_line, uint64_t milliseconds);
	void print_execution_time_in_ticks(const std::string &filename_line, uint64_t ticks, double milliseconds);

}
}

