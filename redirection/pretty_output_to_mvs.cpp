#include <windows.h>


namespace pretty_output_to_mvs
{

	void print(const char *string)
	{
		OutputDebugStringA(string);
	}


	void flush()
	{
	}

}

