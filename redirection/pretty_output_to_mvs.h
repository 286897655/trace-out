#pragma once

#include <windows.h>


inline void pretty_output_print(const char *string)
{
	OutputDebugStringA(string);
}


inline void pretty_output_flush()
{
}

