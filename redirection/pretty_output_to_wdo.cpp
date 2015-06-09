//
// This file is a part of pretty_output project
// See more at https://github.com/shrpnsld/pretty_output
//

#include <windows.h>


namespace pretty_output_to_wdo
{

	void print(const char *string)
	{
		OutputDebugStringA(string);
	}


	void flush()
	{
	}


	size_t width()
	{
#if defined(PRETTY_OUTPUT_WIDTH)

		return PRETTY_OUTPUT_WIDTH;

#else

		return 120;

#endif // defined(PRETTY_OUTPUT_WIDTH)
	}

}

