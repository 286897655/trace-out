//
// This file is a part of pretty_output project
// See more at https://github.com/shrpnsld/pretty_output
//

#if !defined(PRETTY_OUTPUT_REDIRECTION)

#include <iostream>

#include "console.h"


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
		int width = pretty_output::impl::console_width();
		if (width == -1)
		{
			return DEFAULT_WIDTH;
		}

		return width;
	}

}

#endif // !defined(PRETTY_OUTPUT_REDIRECTION)

