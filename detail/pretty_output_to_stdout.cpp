#if !defined(PRETTY_OUTPUT_REDIRECTION)

#include <cstddef>
#include <iostream>

#include "system_console.h"


namespace pretty_output_to_stdout
{

	void print(const char *string);
	void flush();
	size_t width();

}


namespace pretty_output_to_stdout
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
		int width = pretty_output::detail::console_width();
		if (width == -1)
		{
			return DEFAULT_WIDTH;
		}

		return static_cast<size_t>(width);
	}

}


#endif // !defined(PRETTY_OUTPUT_REDIRECTION)

