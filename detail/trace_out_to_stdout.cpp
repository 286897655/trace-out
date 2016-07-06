#if !defined(TRACE_OUT_REDIRECTION)

#include <cstddef>
#include <iostream>

#include "system_console.hpp"


namespace trace_out_to_stdout
{

	void print(const char *string);
	void flush();
	size_t width();

}


namespace trace_out_to_stdout
{

	const size_t DEFAULT_WIDTH = 79;


	void print(const char *string)
	{
		std::cout << string;
	}


	void flush()
	{
		std::cout.flush();
	}


	size_t width()
	{
		int width = trace_out::detail::console_width();
		if (width == -1)
		{
			return DEFAULT_WIDTH;
		}

		return static_cast<size_t>(width);
	}

}


#endif // !defined(TRACE_OUT_REDIRECTION)

